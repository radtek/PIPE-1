
// ------------------------------------------------------------
// PanCIM_Oracle.cpp
// ------------------------------------------------------------
//  Production Information PanCIM Exporter
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  InfoPlus.21 - TSK_PIPE
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//  PanCIM interface to Oracle and other relational databases
//  written: February 20, 2006
//  author : Bill Cosmen
// ------------------------------------------------------------

#include "stdafx.h"
#include "OracleOrgInfo.h"
#include "PanCIM_Oracle.h"
#include "PanCIM_WIP_V.h"
#include "PanCIM_WIP.h"
#include "Logger.h"
#include "OracleMAP.h"
#include "JobPerformance.h"
#include "JobHistory.h"
#include "PanCIMMessage.h"
#include "PCOItems.h"
#include "ProdOrders.h"
#include "PCO_UpdateQty.H"
#include "MoldMachPerf.h"
#include "BaggerPerf.h"
#include "InserterPerf.h"
#include "LoaderPerf.h"
#include "SystemComment.h"
#include "MoldRepair.h"
#include "BaggerRepair.h"
#include "JobRefID.h"
#include "OracleSeqNum1.h"
#include "OracleTransactionInfo.h"
#include "IssueData.h"
#include "TextFilter.h"
#include "DateFormat.h"
#include "InventoryLocation.h"
#include "ReactivateMsg.h"
#include "oracl.h"
#include "OracleUTCoffset.h"
#include "ConvertTimeZone.h"
#include "time.h"
#include "WorkorderRecord.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OracleOrgInfo OracleOrganizationData;
OracleMAP OracleFieldMap;
CJobMgrStatus JobMgrStatus;
CLogger Log;

CWinApp theApp;
using namespace std;

/* ----------------------------------------------------------------------------
    Initialize IP21
------------------------------------------------------------------------------- */  
void InitIP21(int WC_Records)
{
  short NumChars = 0,
        NumOK    = 0,
        StopTask = 0,
        j;
  long  ft[MAXFIELDS],
        id[MAXRECORDS],
        ftBuf = 0,
        idBuf = 0;
  char  *pBuf = 0;
  CString LogMsgTxt;
  long  TskRecid = 0,                     // external task record ID
        ActRecid = 0,                     // activated record ID
        ActField = 0;                     // field tag of activation field
  short ActCode  = 0,                     // record activation code
        Priority = 0;                     // activation priority
  char  COSFlag  = 0;                     // COS flag
  CTextFilter TxtFilter;
  ERRBLOCK IP21error;
  ERRARRAY IP21message;
  COracleUTCoffset Oracle_UTC_Offset;
  DWORD CurrentTZ;
  TIME_ZONE_INFORMATION tzInfo;

  _tzset();
  LogMsgTxt.Empty();

  if (!OStartup())      // initialize the Oracle library
  {
    LogMsgTxt="PanCIM - Oracle Interface startup error: unable to start the Oracle library!";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);    
    exit(0);
  } 
  if (!INISETC())       // connect to IP21 database
  {
    LogMsgTxt="PanCIM - Oracle Interface startup error: unable to connect to InfoPlus.21 database";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);    
    exit(0);
  }
  EXTSKINI(&TskRecid, &IP21error);
  for (j=0; j<MAXRECORDS; j++)        // decode all required record IDs
  {
    pBuf = IP21RecordNames[j];
    NumChars = strlen(pBuf);
    DECODNAM(pBuf, NumChars, &idBuf, &IP21error);
    id[j] = idBuf;
  }
  WorkorderDefID = id[0];
  OracleJobDefID = id[1];
  PCMItemNameID  = id[2];
  MessageCheckID = id[3];
  WOCleanupID    = id[4];
  DefaultMsgID   = id[5];
  PanCIMMsgDefID = id[6];
  
  if (OracleJobDefID < 1)
  {
    LogMsgTxt = "TSK_PIPE initialization failure: OracleJobDef decode error";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    StopTask = 1;
  }  
  if (PanCIMMsgDefID < 1)
  {
    LogMsgTxt = "TSK_PIPE initialization failure: PanCIMMsgDef decode error";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    StopTask = 1;
  }  
  if (DefaultMsgID < 1)
  {
    LogMsgTxt = "TSK_PIPE initialization failure: d-PanCIMMessage decode error";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    StopTask = 1;
  }  
  if (MessageCheckID < 1)
  {
    LogMsgTxt = "TSK_PIPE initialization failure: PanCIMMsgCheck decode error";
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    StopTask = 1;
  }  
  if (WC_Records) // WC specific records
  {
    if (WorkorderDefID < 1)
    {
      LogMsgTxt = "TSK_PIPE initialization failure: WorkorderDef decode error";
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      StopTask = 1;
    }  
    if (PCMItemNameID < 1)
    {
      LogMsgTxt = "TSK_PIPE initialization failure: PCM_ItemName decode error";
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      StopTask = 1;
    }  
    if (WOCleanupID < 1)
    {
      LogMsgTxt = "TSK_PIPE initialization failure: WOCleanup decode error";
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      StopTask = 1;
    }  
  }
  if (StopTask) // something was missing - do not continue
  {  
   ENDSETC();
   exit(0);
  }
  for (j=0; j<MAXFIELDS; j++)         // decode all required field tags.
  {
    pBuf = IP21FieldName[j];
    NumChars = strlen(pBuf);
    DECODFT(pBuf, NumChars, &ftBuf, &IP21error);
    ft[j] = ftBuf;
    if (ftBuf < 1)
    {
      pBuf[NumChars] = '\0';
      LogMsgTxt.Format("Production Information PanCIM Exporter initialization failed: %s decode field tag error", pBuf);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      ENDSETC();
      exit(0);
    }
  }
  NAME_FT                  = ft[0];
  WONUM_FT                 = ft[1];
  DATECREATED_FT           = ft[2];
  DATEDUE_FT               = ft[3];
  WOPRIORITY_FT            = ft[4];
  PLANQTY_FT               = ft[5];
  MACHNUM_FT               = ft[6];
  PARTNAME_FT              = ft[7];
  PACKCODE_FT              = ft[8];
  MATERIAL_FT              = ft[9];
  COLOR_FT                 = ft[10];
  DCINUMBER_FT             = ft[11];
  MOLDNAME_FT              = ft[12];
  NUMCAVITIES_FT           = ft[13];
  STANDARDRATE_FT          = ft[14];
  WOSTATUS_FT              = ft[15];
  WIPQTY_FT                = ft[16];
  DONEQTY_FT               = ft[17];
  REJQTY_FT                = ft[18];
  WOCYCLES_F               = ft[19];
  RUNTIME_FT               = ft[20];
  DATEDONE_FT              = ft[21];
  WORKORDER_FT             = ft[22];
  PART_FT                  = ft[23];
  ORGANIZATIONCODE_FT      = ft[24];
  ORGANIZATIONID_FT        = ft[25];
  LAST_UPDATE_FT           = ft[26];
  CURRENT_STATUS_FT        = ft[27];
  ACTIVATE_TYPE_FT         = ft[28];
  RECORDS_ADDED_FT         = ft[29];
  RECORDS_MODIFIED_FT      = ft[30];
  RECORDS_REMOVED_FT       = ft[31];
  NUM_INTEGER_VALUES_FT    = ft[32];
  NUM_REAL_VALUES_FT       = ft[33];
  NUM_TIME_STAMP_VALUES_FT = ft[34];
  NUM_CHARACTER_STRINGS_FT = ft[35];
  STATUS_FT                = ft[36];
  JOB_FT                   = ft[37];
  CELL_FT                  = ft[38];
  MEASURED_QTY_FT          = ft[39];
  ERROR_TEXT_FT            = ft[40];
  INTEGER_LABEL_FT         = ft[41];
  INTEGER_VALUE_FT         = ft[42];
  INTEGER_MODIFIED_FT      = ft[43];
  REAL_LABEL_FT            = ft[44];
  REAL_VALUE_FT            = ft[45];
  REAL_MODIFIED_FT         = ft[46];
  TIMESTAMP_LABEL_FT       = ft[47];
  TIME_STAMP_VALUE_FT      = ft[48];
  TIMESTAMP_MODIFIED_FT    = ft[49];
  CHARACTER_LABEL_FT       = ft[50];
  CHARACTER_STRING_FT      = ft[51];
  CHARACTER_MODIFIED_FT    = ft[52];
  QUERY_RECORD_FT          = ft[53];
  INTEGERCOUNT_FT          = ft[54];
  CHARCOUNT_FT             = ft[55];
  REALCOUNT_FT             = ft[56];
  TIMESTAMPCOUNT_FT        = ft[57];
  TRANSCODE_FT             = ft[58];
  INT_FIELD_FT             = ft[59];
  CHAR_FIELD_FT            = ft[60];
  REAL_FIELD_FT            = ft[61];
  TIME_FIELD_FT            = ft[62];
  SOURCE_FT                = ft[63];
  DB_NAME_FT               = ft[64];
  DB_USER_FT               = ft[65];
  DB_PWD_FT                = ft[66];
  WIP_ID_FT                = ft[67];
  NUM_DEPARTMENTS_FT       = ft[68];
  DEPARTMENT_ID_FT         = ft[69];
  NUM_SELECTIONS_FT        = ft[70];
  SELECT_DESCRIPTION_FT    = ft[71];
  
  if (WC_Records)
  {
    // store the PanCIM material types
    DB2SHRT(PCMItemNameID, NUM_SELECTIONS_FT, &TotalMaterialTypes, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("InitIP21:read PCM_ItemName error %.*s)", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      ENDSETC();
      exit(0);
    }
    if (TotalMaterialTypes > MAXMTLTYPES)
      TotalMaterialTypes = MAXMTLTYPES;
    RDBOCCS(PCMItemNameID, SELECT_DESCRIPTION_FT, 1, TotalMaterialTypes, MATERIALTYPELEN,
            MaterialType, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("InitIP21:material types read error (ID=%d) %.*s)",
                        PCMItemNameID, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      ENDSETC();
      exit(0);
    }
    // Set maximum length to MATERIALLEN characters.
    for (j=0; j<TotalMaterialTypes; j++)
    {
      MaterialType[j][MATERIALLEN] = '\0';
      strtok(MaterialType[j], " ");
    }
  }
  OracleFieldMap.ReadOracleMAP();
  
  // DEBUG  ---------------------------------------------------------------------------------------------------
  // print the Oracle Field Map data used
  // for (j=0; j<OracleFieldMap.TotalMapFields; j++)
  // {
  //   LogMsgTxt.Format("OracleField=|%s|  IP21Field=%0X  IP21OccNum=%d  IP21DataType=%d  IP21FieldLabel=|%s|",
  //                     OracleFieldMap.OracleField[j],
  //                     OracleFieldMap.IP21Field[j],
  //                     OracleFieldMap.IP21OccNum[j],
  //                     OracleFieldMap.IP21DataType[j],
  //                     OracleFieldMap.IP21FieldLabel[j]);
  //   Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
  // }
  // ----------------------------------------------------------------------------------------------------------
  
  if (OracleFieldMap.rtnStatus == 0)
  {  
    /* NOTE: -----------------------------------------------------
             We need to get the division and connection info now
             in case a PanCIMMessageDef record activates first.
       ----------------------------------------------------------- */
    
    // get the division info from the OracleJobMgr record
    OracleOrganizationData.GetOrganizationID(ORGANIZATIONID_FT,NUM_DEPARTMENTS_FT,DEPARTMENT_ID_FT);
    // get the Oracle database connection information
    TxtFilter.MaxLen = ORACLEDBINFOLEN;
    OracleOrganizationData.GetOracleConnInfo(DB_NAME_FT);
    TxtFilter.Strip(OracleOrganizationData.ConnInfo);
    memcpy(ORACLE_DATABASE, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
    strtok(ORACLE_DATABASE, " ");
    OracleOrganizationData.GetOracleConnInfo(DB_USER_FT);
    TxtFilter.Strip(OracleOrganizationData.ConnInfo);
    memcpy(ORACLE_USER, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
    strtok(ORACLE_USER, " ");
    OracleOrganizationData.GetOracleConnInfo(DB_PWD_FT);
    TxtFilter.Strip(OracleOrganizationData.ConnInfo);
    memcpy(ORACLE_PASSWORD, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
    strtok(ORACLE_PASSWORD, " ");
    Log.DivisionName = OracleOrganizationData.OrgName;  
    
    // read and save local and Oracle server UTC time zone offsets
    Oracle_UTC_Offset.dbNam = ORACLE_DATABASE;
    Oracle_UTC_Offset.dbUsr = ORACLE_USER;
    Oracle_UTC_Offset.dbPwd = ORACLE_PASSWORD;
    OracleTimeZoneOffset    = Oracle_UTC_Offset.GetUTCoffset(); 
    LocalTimeZoneOffset     = timezone;
    LocalDST_Flag           = daylight;  

    // determine if DST is in effect
    CurrentTZ = GetTimeZoneInformation(&tzInfo);
    if (CurrentTZ == TIME_ZONE_ID_DAYLIGHT)  // adjust offset for DST
      LocalTimeZoneOffset = (LocalTimeZoneOffset - SECONDS_IN_HOUR);
    
    LocalTimeZoneOffset = LocalTimeZoneOffset * (-1);
    if (WC_Records)
      LogMsgTxt.Format("%s Production Information PanCIM Exporter version %s.%s startup (WC)",
                      Log.DivisionName, Log.AppVersion, Log.BuildNumber);
    else
      LogMsgTxt.Format("%s Production Information PanCIM Exporter version %s.%s startup",
                      Log.DivisionName, Log.AppVersion, Log.BuildNumber);  
    Log.LogMessage(2, (LPCTSTR) LogMsgTxt); 
    
    if (CurrentTZ == TIME_ZONE_ID_DAYLIGHT)
    {
      LogMsgTxt.Format("*** Oracle TZ Offset = %d  Local TZ Offset = %d  DST Adjustment = True ***",
                            OracleTimeZoneOffset, LocalTimeZoneOffset);
    }
    else
    {
      LogMsgTxt.Format("*** Oracle TZ Offset = %d  Local TZ Offset = %d  DST Adjustment = False ***",
                             OracleTimeZoneOffset, LocalTimeZoneOffset);
    }  
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt); 
    OracleTimeZoneOffset = OracleTimeZoneOffset * (-1);
    LocalTimeZoneOffset  = LocalTimeZoneOffset * (-1); 
        
    // now wait for IP21 record activations
    while (IP21error.ERRCODE == SUCCESS)
    {
      EXTASKCHK(&ActRecid, &ActField, &Priority, &ActCode, &COSFlag, &IP21error);
      if (!ActRecid)
      {
        EXTSKWAI();
      }
      else
      {
        if (COSFlag == 0)
          IP21ActivationHandler(ActRecid, ActCode);  // Send to IP21 activation handler    
      }
    } 
    ERRMESS(&IP21error, IP21message, &NumChars);
    LogMsgTxt.Format("%s Production Information PanCIM Exporter version %s.%s stopped \n         ___  %.*s ___\n",
                      OracleOrganizationData.OrgName,Log.AppVersion,Log.BuildNumber,NumChars,IP21message);
    Log.LogMessage(2, (LPCTSTR) LogMsgTxt);
  }
  ENDSETC();
  OShutdown();
}
/* --------------------------------------------------------------------------
    Process activated records
   -------------------------------------------------------------------------- */
void IP21ActivationHandler(long RecID, short ActCode)
{
  int rtnval  = 0;
  CString TimeBuf,
          LogMsgTxt;
  
  switch(ActCode)
  {
    case ORACLEJOBMGR:  // OracleJobDef record activations
      rtnval = OracleJobMgr(RecID);
      break;

    case PANCIMMESSAGE: // PanCIMMessageDef record activations
      rtnval = PanCIMMessageHandler(RecID);
      break;
    
    default:  // not defined
      LogMsgTxt.Format("IP21MsgHandler:Unknown activation code (%d)", ActCode);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      break;
  }
}
/* ----------------------------------------------------------------------------
    Process Oracle Job Manager record activations
------------------------------------------------------------------------------- */  
int OracleJobMgr(long RecID)
{
  CString dbConn,
          strSQL,
          strTmp,
          LogMsgTxt;
  short   NumOK = 0,
          j;
  int rtnval         = 0;
  long WOCreateStat  = 0,
       WOUpdateStat  = 0,
       WOCloseStat   = 0,
       JobCreateStat = 0,
       JobUpdateStat = 0,
       JobCloseStat  = 0,
       DeptID[MAXDEP];
  char OrganizationName[ORGCODELEN];
  CDatabase db;
  CTextFilter TxtFilter;

  long  OrgTags[6]  = {DB_NAME_FT, DB_USER_FT, DB_PWD_FT,
                       ORGANIZATIONCODE_FT, ORGANIZATIONID_FT,NUM_DEPARTMENTS_FT};
  short OrgTypes[6] = {ORACLEDBINFOLEN,ORACLEDBINFOLEN,ORACLEDBINFOLEN,
                       ORGCODELEN,DTYPLONG,DTYPLONG};
  struct { char dbName[ORACLEDBINFOLEN],
                dbUser[ORACLEDBINFOLEN],
                dbPwd[ORACLEDBINFOLEN],
                OrgCode[ORGCODELEN];
           long OrgID,
                NumDepts;
         } OrgData;
  long ent_ID = 0,
       org_ID = 0; 

  // reset counters
  JobMgrStatus.RecordsAdded    = 0,
  JobMgrStatus.RecordsModified = 0,
  JobMgrStatus.RecordsDeleted  = 0,
  JobMgrStatus.Status = JOBMGRSTAT_PROCESSING;
  UpdateJobManagerStatus(RecID);
  JobMgrStatus.Status = JOBMGRSTAT_OK;

  // read the organization and connection data from the activated record
  RDBVALS(RecID,6, OrgTags, OrgTypes, &OrgData, &NumOK, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("OracleJobMgr:read organization data error %.*s)", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    return 1;
  }
  // validate organization data
  TxtFilter.MaxLen = ORGCODELEN;
  TxtFilter.Strip(OrgData.OrgCode);
  memcpy(OrganizationName, (LPCTSTR) TxtFilter.TextOut, ORGCODELEN);
  strtok(OrganizationName, " ");
  if(OracleOrganizationData.OrgCheck(OrgData.OrgID, OrganizationName) != 1)
  {
    LogMsgTxt.Format("OracleJobMgr:organization data match error (ID=%d  code=%s)",OrgData.OrgID, OrganizationName);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    JobMgrStatus.Status = JOBMGRSTAT_ORGERROR;
    UpdateJobManagerStatus(RecID);
    return 1;
  }
  // get the department IDs
  if (OrgData.NumDepts > 0)
  {
    if (OrgData.NumDepts > MAXDEP)
      OrgData.NumDepts = MAXDEP;
   
    // read and store the department IDs
    RDBOCCS(RecID, DEPARTMENT_ID_FT, 1, OrgData.NumDepts, DTYPLONG, DeptID, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("OracleJobMgr: department ID read error: %.*s\n", ErrorLength, IP21message);
      IP21error.ERRCODE = SUCCESS;
      OrgData.NumDepts = 0;
    }
  }
  // cleanup and save the Oracle connection information
  TxtFilter.MaxLen = ORACLEDBINFOLEN;
  TxtFilter.Strip(OrgData.dbName);
  memcpy(ORACLE_DATABASE, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
  strtok(ORACLE_DATABASE, " ");
  TxtFilter.Strip(OrgData.dbUser);
  memcpy(ORACLE_USER, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
  strtok(ORACLE_USER, " ");
  TxtFilter.Strip(OrgData.dbPwd);
  memcpy(ORACLE_PASSWORD, (LPCTSTR) TxtFilter.TextOut, ORACLEDBINFOLEN);
  strtok(ORACLE_PASSWORD, " ");

  // build Oracle query string  
  
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // IMPORTANT!
    //   DO NOT add, remove or re-arrange field names in following string without making the appropriate
    //   changes to the field index values in the CreateOracleJobRecord function and OracleMap class.      
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
  strSQL =  "SELECT ORGANIZATION_ID,WIP_ENTITY_ID,CREATION_DATE,CREATED_BY,LAST_UPDATE_DATE,LAST_UPDATE_BY,";
  strSQL += "WIP_STATUS_CODE,WIP_STATUS_FLAG,PANCIM_STATUS,DEPARTMENT_ID,MACHINE_ID,MACHINE_RESOURCE_FLAG,START_QTY,";
  strSQL += "START_QTY_FLAG,COMPLETED_QTY,COMPLETED_QTY_FLAG,DATE_COMPLETED,DATE_COMPLETED_FLAG,DATE_CLOSED,";
  strSQL += "DATE_CLOSED_FLAG,ORGANIZATION_CODE,WIP_ENTITY_NAME,ITEM_CODE,ITEM_UOM,COLOR,MATERIAL_CODE,MATERIAL_UOM,";
  strSQL += "DEPARTMENT_CODE,PRIORITY,QUANTITY_SCRAPPED,SCHED_START_DATE,SCHED_COMPLETION_DATE,PRINT_DATE,STANDARD_COST,";
  strSQL += "RESOURCE_RATE,MATERIAL_RATE,MATERIAL_OVERHEAD_RATE,OVERHEAD_RATE,OUTSIDE_PROCESS_RATE,EMPLOYEE_ID,";
  strSQL += "WIP_JOB_STATUS,MOLD_NAME,MOLD_BASE,MACHINE_NAME, ACTUAL_CAVITIES, DESIGN_CAVITIES, MOLD_ID_FLAG ";
  strSQL += "FROM APPS.PANDUIT_WIP_PANCIM_JOB_V ";
  strSQL += "WHERE (MACHINE_NAME <> 'OFFLINE') AND (WIP_STATUS_FLAG='Y' OR MACHINE_RESOURCE_FLAG='Y' OR START_QTY_FLAG='Y' OR ";
  strSQL += "COMPLETED_QTY_FLAG='Y' OR DATE_COMPLETED_FLAG='Y' OR DATE_CLOSED_FLAG='Y' OR MOLD_ID_FLAG='Y')"; 
  
  if (OrgData.NumDepts > 0)
  {
    strSQL += " AND (";
    for (j=0; j<OrgData.NumDepts; j++)
    {
      strTmp.Format("DEPARTMENT_ID=%d", DeptID[j]);    
      strSQL += strTmp;    
      if (j+1 == OrgData.NumDepts)
      {
        strSQL += ")";
      }
      else
      {
        strSQL += " OR ";
      }
    }
  }
  strTmp.Format(" AND (ORGANIZATION_ID=%d)", OrgData.OrgID);  
  strSQL += strTmp;   
  dbConn = DSN_WIP_V; // connect to the Oracle PANDUIT_WIP_PANCIM_JOB_V table view
  try
  {
    PanCIM_WIP_V rsWIP_V(&db);
    db.SetLoginTimeout(30);
    if (db.OpenEx(dbConn,CDatabase::openReadOnly | CDatabase::noOdbcDialog))
    {     
      if (rsWIP_V.Open(CRecordset::snapshot, strSQL))
      {       
        if ((!rsWIP_V.IsBOF()) && (!rsWIP_V.IsEOF()))
        {     
          /* NOTE ---------------------------------------------------- 
                use of case statements for each division to handle
                any differences and provide for future flexability.
             --------------------------------------------------------- */          
     
          switch(OrgData.OrgID)
          {
            case ORGID_WC:  // Wiring Components Division   
              while (!rsWIP_V.IsEOF())
              {            
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")  // check if new Oracle job record
                {             
                  WOCreateStat = CreateWorkorderRecord(&rsWIP_V); // create a new IP21 workorder record
                  if (WOCreateStat == 0)  // workorder record was created successfully
                  {       
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID); // reset all Oracle flag fields  
                    if ((WOCreateStat == 0) && (rtnval == 0))  
                    {                   
                      JobMgrStatus.RecordsAdded++;
                      rtnval = UpdateJobHistory(&rsWIP_V);  // create or update job history record for this order
                      if (rtnval == 1)
                      {
                        // try one more time if error creating new record
                        Sleep(WAITTIME);
                        rtnval = UpdateJobHistory(&rsWIP_V);
                      }
                    }
                  }
                  if (WOCreateStat > 1) // there was a problem creating the workorder record
                  { 
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(WOCreateStat); // remove all traces of the incomplete record
                  }        
                }
                else  // workorder record should already exist in IP21 - just update
                {
                  WOUpdateStat = ModifyWorkorderRecord(&rsWIP_V); // update the workorder record data
                  if (WOUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    rtnval = UpdateJobHistory(&rsWIP_V); // post all changes to the job history db
                  }
                }
                rsWIP_V.MoveNext();
              } 
              break;

            case ORGID_SGP: // Singapore Division
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {
                  WOCreateStat = CreateWorkorderRecord(&rsWIP_V);
                  
                  if (WOCreateStat == 0)  // workorder record was created successfully
                  {       
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID); // reset all Oracle flag fields  
                    if ((WOCreateStat == 0) && (rtnval == 0))  
                    {                   
                      JobMgrStatus.RecordsAdded++;
                      rtnval = UpdateJobHistory(&rsWIP_V);  // create or update job history record for this order
                      if (rtnval == 1)
                      {
                        // try one more time if error creating new record
                        Sleep(WAITTIME);
                        rtnval = UpdateJobHistory(&rsWIP_V);
                      }
                    }
                  }
                  if (WOCreateStat > 1) // there was a problem creating the workorder record
                  { 
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(WOCreateStat); // remove all traces of the incomplete record
                  }        
                }
                else  // workorder record should already exist in IP21 - just update
                {
                  WOUpdateStat = ModifyWorkorderRecord(&rsWIP_V); // update the workorder record data
                  if (WOUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;  
                    rtnval = UpdateJobHistory(&rsWIP_V); // post all changes to the job history db
                  }
                  if (rtnval == 1)
                  {
                    // try one more time if error creating new record
                    Sleep(WAITTIME);
                    rtnval = UpdateJobHistory(&rsWIP_V);
                  }               
                }
                rsWIP_V.MoveNext();
              } 
              break;
   
            case ORGID_CR:  // Costa Rica
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")  // check if new Oracle job record
                {             
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);  // create a new IP21 job record
                  if (JobCreateStat > 1)  // there was a problem creating the IP21 job record
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);  // remove all traces of the incomplete IP21 record
                  }
                  else  
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);  // update the job history db
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }              
                  }
                }
                else  // the Oracle job record should already exist in IP21 - just update it
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);  // update the job record data
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V); // post all changes to the job history db
                  }
                }
                rsWIP_V.MoveNext();
              } 
              break;
            
            case ORGID_OP:  // Orland Park
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")  // check if new Oracle job record
                {
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);  // create a new IP21 job record
                  if (JobCreateStat > 1)  // there was a problem creating the IP21 job record
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);  // remove all traces of the incomplete IP21 record
                  }
                  else  
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);  // update the job history db
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else  // the Oracle job record should already exist in IP21 - just update it
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);  // update the job record data
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V); // post all changes to the job history db
                  }
                }
                rsWIP_V.MoveNext();
              } 
              break;
            
            case ORGID_NL:  // New Lenox
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {         
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);
                  if (JobCreateStat > 1)
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);
                  }
                  else
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);             
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V);
                  }
                }      
                rsWIP_V.MoveNext();
              } 
              break;

            case ORGID_WA:  // Wiring Accessories
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);
                  if (JobCreateStat > 1)
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);
                  }
                  else
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V);
                  }
                }      
                rsWIP_V.MoveNext();
              } 
              break;

            case ORGID_ID:  // ID Division
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);
                  if (JobCreateStat > 1)
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);
                  }
                  else
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V);
                  }
                }      
                rsWIP_V.MoveNext();
              } 
              break;

            case ORGID_LP:  // Lockport
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);
                  if (JobCreateStat > 1)
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);
                  }
                  else
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V);
                  }
                }      
                rsWIP_V.MoveNext();
              } 
              break;

            case ORGID_BR:  // Burr Ridge
              while (!rsWIP_V.IsEOF())
              {
                if (rsWIP_V.m_PANCIM_STATUS == "NEW")
                {
                  JobCreateStat = CreateOracleJobRecord(&rsWIP_V);
                  if (JobCreateStat > 1)
                  {
                    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
                    DeleteIP21Record(JobCreateStat);
                  }
                  else
                  {
                    JobMgrStatus.RecordsAdded++;
                    rtnval = UpdateOracleFlag(OrgData.OrgID,rsWIP_V.m_WIP_ENTITY_ID);
                    if (rtnval == 0)
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    if (rtnval == 1)
                    {
                      // try one more time if error creating new record
                      Sleep(WAITTIME);
                      rtnval = UpdateJobHistory(&rsWIP_V);
                    }             
                  }
                }
                else
                {
                  JobUpdateStat = ModifyOracleJobRecord(&rsWIP_V);
                  if (JobUpdateStat == 0)
                  {
                    JobMgrStatus.RecordsModified++;
                    UpdateJobHistory(&rsWIP_V);
                  }
                }      
                rsWIP_V.MoveNext();
              } 
              break;

            default:
              LogMsgTxt.Format("OracleJobMgr: unknown organization ID (%d)", OrgData.OrgID);
              Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
              break;
          }
        }
        rsWIP_V.Close();
      } 
      db.Close();
      UpdateJobManagerStatus(RecID);
    } 
  }
  catch( CDBException* e )
  {
    LogMsgTxt = "OracleJobMgr Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    JobMgrStatus.Status = JOBMGRSTAT_ERROR;
    UpdateJobManagerStatus(RecID);
    rtnval=1;
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Create a new WorkorderDef record
------------------------------------------------------------------------------- */  
long CreateWorkorderRecord(PanCIM_WIP_V *WOData)
{
  CString WORecName,
          LogMsgTxt,
          MachNameBuf,
          PackCode,
          strOriginalTimeStamp,
          strLocalTimeStamp,
          strCreateDate,
          strDateDue;
  long    rtnval      = 0,
          lCreateDate = 0,
          lDateDue    = 0,
          OracleTS_offset = 0,
          WorkorderTags[13],
          WORecID  = MINRECID;
    int   DateLen  = 0;
  short   WorkorderTypes[13],
          WrapFlag = FALSE,
          SizeBuff = 18,
          NumOK    = 0;
  char    tmError  = 0;
  CTime lts;
  struct  { char WONum[WONUMLEN],
                 PartName[PARTNAMELEN],
                 PackCode[PACKCODELEN],
                 Material[MATERIALLEN],
                 Color[COLORLEN],
                 MoldName[MOLDNAMELEN];
            long DateCreated,
                 DateDue,
                 PlanQty,
                 MachNum,
                 NumCavities,
                 WIP_ID;
            int  WOPriority;} WorkorderData; 
  CTextFilter TxtFilter;
  COracleUTCoffset Oracle_UTC_Offset;
  CConvertTimeZone ConvertTimeZone;
  
  Oracle_UTC_Offset.dbNam = ORACLE_DATABASE;
  Oracle_UTC_Offset.dbUsr = ORACLE_USER;
  Oracle_UTC_Offset.dbPwd = ORACLE_PASSWORD;
  OracleTS_offset = Oracle_UTC_Offset.GetUTCoffset(); 
  
  WorkorderTags[0]  = WONUM_FT; 
  WorkorderTags[1]  = PARTNAME_FT;
  WorkorderTags[2]  = PACKCODE_FT;
  WorkorderTags[3]  = MATERIAL_FT;
  WorkorderTags[4]  = COLOR_FT;
  WorkorderTags[5]  = MOLDNAME_FT;
  WorkorderTags[6]  = DATECREATED_FT;
  WorkorderTags[7]  = DATEDUE_FT;
  WorkorderTags[8]  = PLANQTY_FT;
  WorkorderTags[9]  = MACHNUM_FT;
  WorkorderTags[10] = NUMCAVITIES_FT;
  WorkorderTags[11] = WIP_ID_FT;
  WorkorderTags[12] = WOPRIORITY_FT;
   
  WorkorderTypes[0]  = WONUMLEN;
  WorkorderTypes[1]  = PARTNAMELEN;
  WorkorderTypes[2]  = PACKCODELEN;
  WorkorderTypes[3]  = MATERIALLEN;
  WorkorderTypes[4]  = COLORLEN;
  WorkorderTypes[5]  = MOLDNAMELEN;
  WorkorderTypes[6]  = DTYPTIME;
  WorkorderTypes[7]  = DTYPTIME;
  WorkorderTypes[8]  = DTYPLONG;
  WorkorderTypes[9]  = DTYPLONG;
  WorkorderTypes[10] = DTYPLONG;
  WorkorderTypes[11] = DTYPLONG;
  WorkorderTypes[12] = DTYPSHRT;
                
  TxtFilter.MaxLen = NAMELEN;
  TxtFilter.Strip(WOData->m_WIP_ENTITY_NAME);
  WORecName = TxtFilter.TextOut;
  if (WOData->m_MACHINE_NAME.IsEmpty())
    WorkorderData.MachNum = 0;
  else
    WorkorderData.MachNum = atol((LPCTSTR) WOData->m_MACHINE_NAME);      
 
  // only create record if machine number > 0
  if (WorkorderData.MachNum < 1)
  {
    LogMsgTxt.Format("CreateWorkorder:workorder %s not created for machine %d",
                      WOData->m_WIP_ENTITY_NAME, WorkorderData.MachNum);
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
    return 1;  // prevent job history update 
  }
  try
  {
    if (WOData->m_CREATION_DATE == NULL) // use current time if not supplied
    {
      lCreateDate = GETDBTIM();    
    }
    else                                
    {     
      // convert Oracle timestamp to local time zone
      lts = ConvertTimeZone.ConvertTS(WOData->m_CREATION_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
      strCreateDate = lts.Format("%b-%d-%y %H:%M:%S");  // convert Oracle's time for IP21
      ASCII2TMST(((LPCTSTR) strCreateDate), SizeBuff, &lCreateDate, &tmError);  

      // debug !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //  strOriginalTimeStamp = WOData->m_CREATION_DATE.Format("%b-%d-%y %H:%M:%S");
      //  LogMsgTxt.Format("Create Date - Oracle TS:%s  Local TS:%s",
      //                    strOriginalTimeStamp,strCreateDate);
      //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        
    } 
    if (tmError != 0)
    { 
      lCreateDate = GETDBTIM();
      LogMsgTxt.Format("Error converting creation date |%s| for job %s - setting to current", 
        WOData->m_CREATION_DATE.Format("%b-%d-%y %H:%M:%S"), WOData->m_WIP_ENTITY_NAME);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    }
    if (WOData->m_SCHED_COMPLETION_DATE == NULL) // use current time if not supplied
    {
      lDateDue = GETDBTIM();    
    }
    else                                
    {  
      // convert Oracle timestamp to local time zone
      lts = ConvertTimeZone.ConvertTS(WOData->m_SCHED_COMPLETION_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
      strDateDue = lts.Format("%b-%d-%y %H:%M:%S");  // convert Oracle's time for IP21
      ASCII2TMST(((LPCTSTR) strDateDue), SizeBuff, &lDateDue, &tmError);

      // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //  strOriginalTimeStamp = WOData->m_SCHED_COMPLETION_DATE.Format("%b-%d-%y %H:%M:%S");
      //  LogMsgTxt.Format("Completion Date - Oracle TS:%s  Local TS:%s",
      //                    strOriginalTimeStamp,strDateDue);
      //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  
    }
    if (tmError != 0)
    { 
      lDateDue = GETDBTIM();
      LogMsgTxt.Format("Error converting due date |%s| for job %s - setting to current", 
        WOData->m_SCHED_COMPLETION_DATE.Format("%b-%d-%y %H:%M:%S"), WOData->m_WIP_ENTITY_NAME);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    }
    if ((WOData->m_PRIORITY == NULL) || (WOData->m_PRIORITY < 0))
      WorkorderData.WOPriority = 0;
    else
      WorkorderData.WOPriority = (int) WOData->m_PRIORITY;
       
    if ((WOData->m_START_QTY == NULL) || (WOData->m_START_QTY < 0))
      WorkorderData.PlanQty = 0;
    else
      WorkorderData.PlanQty = (long) WOData->m_START_QTY;  
     
    PackCode = ExtractPackCode(WOData->m_ITEM_CODE);
    TxtFilter.MaxLen = WONUMLEN;
    TxtFilter.Strip(WOData->m_WIP_ENTITY_NAME);
    memcpy(WorkorderData.WONum, (LPCTSTR) TxtFilter.TextOut, WONUMLEN);
    TxtFilter.MaxLen = PARTNAMELEN;
    TxtFilter.Strip(WOData->m_ITEM_CODE);
    memcpy(WorkorderData.PartName, (LPCTSTR) TxtFilter.TextOut, PARTNAMELEN);
    TxtFilter.MaxLen = PACKCODELEN;
    TxtFilter.Strip(PackCode);
    memcpy(WorkorderData.PackCode, (LPCTSTR) TxtFilter.TextOut, PACKCODELEN);
    TxtFilter.MaxLen = MATERIALLEN;
    TxtFilter.Strip(WOData->m_MATERIAL_CODE);
    memcpy(WorkorderData.Material, (LPCTSTR) TxtFilter.TextOut, MATERIALLEN);
    TxtFilter.MaxLen = COLORLEN;
    TxtFilter.Strip(WOData->m_COLOR);
    memcpy(WorkorderData.Color, (LPCTSTR) TxtFilter.TextOut, COLORLEN);
    TxtFilter.MaxLen = MOLDNAMELEN;
    TxtFilter.Strip(WOData->m_MOLD_NAME);
    memcpy(WorkorderData.MoldName, (LPCTSTR) TxtFilter.TextOut, MOLDNAMELEN);
    WorkorderData.DateCreated = lCreateDate;
    WorkorderData.DateDue = lDateDue; 
    WorkorderData.NumCavities = (long) WOData->m_ACTUAL_CAVITIES;
    WorkorderData.WIP_ID = (long) WOData->m_WIP_ENTITY_ID;
    NXTFREE(WrapFlag, &WORecID); // get the first free record ID
    if (WORecID > MINRECID)
    {
      CREATEREC(WORecID, WorkorderDefID, (LPCTSTR) WORecName, WONAMELEN, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateWorkorder:create record error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return WORecID; // return incomplete record ID number        
      }
      WDBVALS(WORecID, 13, WorkorderTags, WorkorderTypes, &WorkorderData, &NumOK, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateWorkorder:data write error (id=%d): %.*s (NumOK=%d", WORecID, ErrorLength, IP21message, NumOK);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return WORecID;        
      }
      MAKUSABL(WORecID, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateWorkorder:make record usable error (id=%d): %.*s", WORecID, ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return WORecID;        
      }
   
      // now activate the workorder program
      SHRT2DB(WORecID, WOSTATUS_FT, WOSTAT_PRINTED, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateWorkorder:write status error (id=%d): %.*s", WORecID, ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return WORecID;        
      }
      LogMsgTxt.Format("CreateWorkorder:New workorder record created for job %s", WOData->m_WIP_ENTITY_NAME);
      Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      LogMsgTxt.Format("CreateWorkorder:no free IDs available to create record for %s", 
                        WOData->m_WIP_ENTITY_NAME);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      rtnval = 1; // no free record IDs
    }
  }
  
  catch(CException *e)
  {
    TCHAR   szCause[255];
    e->GetErrorMessage(szCause, 255);  
    LogMsgTxt.Format("CreateWorkorderRecord Exception: %s", szCause);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Modify a WorkorderDef record
      (updates various fields in a WorkorderDef record)
------------------------------------------------------------------------------- */  
int ModifyWorkorderRecord(PanCIM_WIP_V *WOData)
{
  int   NumChars       = 0,
        CloseHistStat  = 0,
        CloseWOStat    = 0,
        IP21Stat       = 0,
        UpdStat        = 0,
        FlagReset      = 0,
        WIPFlagReset   = 0,
        rtnval         = 0;        
  long  WORecID        = 0,
        WOStat         = 0;
   CString LogMsgTxt,
          FlagField;
  
  // only modify record if machine number > 0
  if ((WOData->m_MACHINE_NAME.IsEmpty()) || (atol((LPCTSTR) WOData->m_MACHINE_NAME) < 1))      
  {
    rtnval = UpdateOracleFlag((long) OracleOrganizationData.OrgNum, WOData->m_WIP_ENTITY_ID); // reset all Oracle flag fields  
    return 0;
  }
  // find the workorder record to update
  WOData->m_WIP_ENTITY_NAME.TrimRight();
  NumChars = WOData->m_WIP_ENTITY_NAME.GetLength();
  DECODNAM((LPCTSTR) WOData->m_WIP_ENTITY_NAME, NumChars, &WORecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    if (IP21error.ERRCODE == NOREC)
    {
      // the IP21 record no longer exists - just update the job history 
      LogMsgTxt.Format("ModifyWorkorderRecord: %s (decode error) Updating JobHistory Only!", 
                        WOData->m_WIP_ENTITY_NAME );
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      // reset all Oracle flags
      rtnval = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID);  
      IP21error.ERRCODE = SUCCESS;
      return 0;
    }
    else
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("ModifyWorkorderRecord: %s decode error: %.*s", 
                        WOData->m_WIP_ENTITY_NAME, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      return 1;
    }  
  }
  if (WORecID > MINRECID)
  {  
    // check Oracle flags
    if (WOData->m_MACHINE_RESOURCE_FLAG == 'Y')
    {
      // update the machine number in the workorder record if not scheduled or running
      IP21Stat = UpdateWorkorderField(WORecID, MACHNUM_FT, atol((LPCTSTR) WOData->m_MACHINE_NAME), 1);
      if (IP21Stat == 0)
      {
        // reset the Oracle flag field
        FlagField = "MACHINE_RESOURCE_FLAG";
        FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);   
        if (FlagReset == 0)
        {
          LogMsgTxt.Format("ModifyWorkorderRecord: job %s machine number changed to %d",
                            WOData->m_WIP_ENTITY_NAME, (long) WOData->m_MACHINE_ID);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        }
      }
      else
      {  
        LogMsgTxt.Format("ModifyWorkorderRecord: Errors detected - job %s machine number NOT changed to %d",
                          WOData->m_WIP_ENTITY_NAME, atol((LPCTSTR) WOData->m_MACHINE_NAME));
        Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
      }    
    }
    if (WOData->m_START_QTY_FLAG == 'Y')
    {
      // update the planned quantity in the workorder record
      IP21Stat = UpdateWorkorderField(WORecID, PLANQTY_FT,(long) WOData->m_START_QTY, 0);
      if (IP21Stat == 0)
      {
        // reset the Oracle flag field
        FlagField = "START_QTY_FLAG";
        FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);
        if (FlagReset == 0)
        {
          LogMsgTxt.Format("ModifyWorkorderRecord: job %s start quantity changed to %d",
                            WOData->m_WIP_ENTITY_NAME, WOData->m_START_QTY);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        }
      }
    }
    if (WOData->m_COMPLETED_QTY_FLAG == 'Y')
    {
      // update the done quantity in the workorder record
      IP21Stat = UpdateWorkorderField(WORecID, DONEQTY_FT,(long) WOData->m_COMPLETED_QTY, 0);
      if (IP21Stat == 0)
      {
        // reset the Oracle flag field
        FlagField = "COMPLETED_QTY_FLAG";
        FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);    
        if (FlagReset == 0)
        {
          LogMsgTxt.Format("ModifyWorkorderRecord: job %s done quantity changed to %d",
                            WOData->m_WIP_ENTITY_NAME, (long) WOData->m_COMPLETED_QTY);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        }
      }  
    }
    if (WOData->m_MOLD_ID_FLAG == 'Y')
    {
      // update the mold name in the workorder record
      IP21Stat = UpdateWorkorderField(WORecID, MOLDNAME_FT, WOData->m_MOLD_NAME, MOLDNAMELEN);      
      if (IP21Stat == 0)
      {
        // now update the number of cavities in case it has changed
        UpdStat = UpdateWorkorderField(WORecID, NUMCAVITIES_FT, (long) WOData->m_ACTUAL_CAVITIES, 1);
        if (UpdStat == 0)
        {
          // reset the Oracle flag field
          FlagField = "MOLD_ID_FLAG";
          FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);    
          if (FlagReset == 0)
          {
            LogMsgTxt.Format("ModifyWorkorderRecord: job %s mold name changed to %s",
                              WOData->m_WIP_ENTITY_NAME, WOData->m_MOLD_NAME);
            Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          }
        }
      }  
    } 
    if (WOData->m_DATE_COMPLETED_FLAG == 'Y')
    {
      CTime   lts;
      CString strOriginalTimeStamp,
              strLocalTimeStamp;
      CConvertTimeZone ConvertTimeZone; 

      // convert Oracle timestamp to local time zone
      lts = ConvertTimeZone.ConvertTS(WOData->m_DATE_COMPLETED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
      
      // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //  strOriginalTimeStamp = WOData->m_DATE_COMPLETED.Format("%b-%d-%y %H:%M:%S");
      //  strLocalTimeStamp = lts.Format("%b-%d-%y %H:%M:%S");
      //  LogMsgTxt.Format("Completion Date - Oracle TS:%s  Local TS:%s",
      //                    strOriginalTimeStamp,strLocalTimeStamp);
      //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
       
      // update the date completed in the workorder record
      IP21Stat = UpdateWorkorderField(WORecID, DATEDONE_FT, lts);
      if (IP21Stat == 0)
      {
        // reset the Oracle flag field
        FlagField = "DATE_COMPLETED_FLAG";
        FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField); 
        if (FlagReset == 0)
        {
          LogMsgTxt.Format("ModifyWorkorderRecord: job %s date done changed", WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        }
      }
    }
    if (WOData->m_DATE_CLOSED_FLAG == 'Y')
    {
      FlagField = "DATE_CLOSED_FLAG";
      FlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);
      if (FlagReset == 0)
      {
        LogMsgTxt.Format("ModifyWorkorderRecord: job %s date closed changed", WOData->m_WIP_ENTITY_NAME);
        Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
      }
    }
    // WIP status changes
    if (WOData->m_WIP_STATUS_FLAG == 'Y')
    {
      bool ResetStatusFlag = true;
      switch ((long) WOData->m_WIP_STATUS_CODE)
      {    
        case ORACLE_WIP_UNRELEASED: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to UNRELEASED",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_RELEASED: // reopen the workorder record
          IP21Stat = UpdateWorkorderField(WORecID, WOSTATUS_FT, WOSTAT_PRINTED, 1);
          if (IP21Stat == 0)
          {
            LogMsgTxt.Format("%s WIP_STATUS_CODE changed to RELEASED",WOData->m_WIP_ENTITY_NAME);
            Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          }
          else
          {   
            LogMsgTxt.Format("WARNING: Job %s RELEASED by Oracle - Scheduled or Running in PanCIM", 
                                       WOData->m_WIP_ENTITY_NAME);
            Log.LogMessage(1, (LPCTSTR) LogMsgTxt);       
            ResetStatusFlag = false;
          }
          break;
        
        case ORACLE_WIP_COMPLETE: // production quantity has been reached
          CloseWOStat = CloseWorkorderRecord(WORecID, WOData, 1);
          if (CloseWOStat == 0)
          {            
            // set the workorder status to CLOSe
            IP21Stat = UpdateWorkorderField(WORecID, WOSTATUS_FT, WOSTAT_CLOSE, 1);
            
            /* NOTE: TSK_WORK will update the IP21 workorder history and send another
               PanCIM message for the JobHistory update. It will also set the WOStatus
               to REMOVE to remove it from the IP21 summary screens.
            */
  
            if (IP21Stat == 0)
            {
              // write the PanCIM status back to Oracle
              CloseHistStat = PanCIMHistoryClose(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID);  
            }
            else
            {
              ResetStatusFlag = false;
            }
          }
          else
          {
            ResetStatusFlag = false;
          }            
          if (ResetStatusFlag == true)
             LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to COMPLETE",WOData->m_WIP_ENTITY_NAME);
           else  
              LogMsgTxt.Format("ModifyWorkorderRecord: %s Error(s) detected when changing WIP_STATUS_CODE to COMPLETE",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;
                
        case ORACLE_WIP_CLOSED: 
          CloseWOStat = CloseWorkorderRecord(WORecID, WOData, 2);
          if (CloseWOStat == 0)
          {            
            // set the workorder status to CLOSe
            IP21Stat = UpdateWorkorderField(WORecID, WOSTATUS_FT, WOSTAT_CLOSE, 1);
            
            /*
               NOTE: TSK_WORK will update the IP21 workorder history and send another
               PanCIM message for the JobHistory update. It will also set the WOStatus
               to REMOVE to remove it from the IP21 summary screens.
            */
   
            if (IP21Stat == 0)
            {
              // write the PanCIM status back to Oracle
              CloseHistStat = PanCIMHistoryClose(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID);  
            }
            else
            {
              ResetStatusFlag = false;
            }
          }
          else
          {
            ResetStatusFlag = false;
          }                      
          if (ResetStatusFlag == true)
             LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to CLOSED",WOData->m_WIP_ENTITY_NAME);
           else  
              LogMsgTxt.Format("ModifyWorkorderRecord: %s Error(s) detected when changing WIP_STATUS_CODE to CLOSED",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_NO_CHARGE:  // workorder is complete
          // IP21Stat = UpdateWorkorderField(WORecID, WOSTATUS_FT, WOSTAT_DONE, 1);
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to COMPLETE - NO CHARGES",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_HOLD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to ON HOLD",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_CANCELLED: // hide the workorder record in PanCIM
          IP21Stat = UpdateWorkorderField(WORecID, WOSTATUS_FT, WOSTAT_REMOVE, 1);
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to CANCELLED",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_BILL_LOAD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to PENDING BILL LOAD",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_BILL_LOAD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to FAILED BILL LOAD",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_ROUTE_LOAD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to PENDING ROUTING LOAD",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_ROUTE_LOAD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to FAILED ROUTING LOAD",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_MASS_LOAD: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to PENDING - MASS LOADED",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_CLOSE: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to PENDING CLOSE",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_CLOSED: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to FAILED CLOSE",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_SCHEDULE: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to PENDING SCHEDULING",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_DRAFT: // don't care - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s WIP_STATUS_CODE changed to DRAFT",WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        default: // unknown code - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: %s Unknown Oracle WIP_STATUS_CODE (%d)",
                            WOData->m_WIP_ENTITY_NAME, WOData->m_WIP_STATUS_CODE);
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          break;
      }
      if (ResetStatusFlag == true)
      {
        FlagField = "WIP_STATUS_FLAG"; 
        WIPFlagReset = UpdateOracleFlag(WOData->m_ORGANIZATION_ID, WOData->m_WIP_ENTITY_ID, FlagField);
      }
    }  
  }
  else
  {
    IP21Stat = 1;
    LogMsgTxt.Format("ModifyWorkorderRecord: IP21 record for workorder %s not found",WOData->m_WIP_ENTITY_NAME);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Update a workorder record field
      (updates long values in workorder records) 
------------------------------------------------------------------------------- */  
int UpdateWorkorderField(long WORecID, long FieldTag, long UpdateVal, short StatusFilter)
{
  int  AllowUpdate = 0,
       IP21Stat    = 0;
  long WOStat      = 0;
  CString LogMsgTxt;

  if (StatusFilter) // restrict update based on current workorder status  
  {
    // read current workorder status 
    DB2LONG(WORecID, WOSTATUS_FT, &WOStat, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateWorkorderField (ID=%d): WOStatus read error: %.*s", WORecID, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      IP21Stat = 1;
    }
    else
    {
      if ((WOStat != WOSTAT_SCHEDULED) && (WOStat != WOSTAT_RUNNING))
      {
        AllowUpdate = 1;  // workorder status is OK for update
      }      
      else
      {
        AllowUpdate = 0;
        IP21Stat = 1;
      }
    }
  }
  else
  {
    AllowUpdate = 1;  // no filter = allow any update
  }
  if (AllowUpdate)
  {
    LONG2DB(WORecID, FieldTag, UpdateVal, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateWorkorderField (ID=%d): write error: %.*s", WORecID, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      IP21Stat = 1;
    }
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
   Update a workorder record field
    (updates timestamp values in workorder records) 
------------------------------------------------------------------------------- */  
int UpdateWorkorderField(long WORecID, long FieldTag, CTime UpdateVal)
{
  int   IP21Stat    = 0;
  long  IP21ts      = 0;
  char  tsErr       = 0;
  CString strDate,
          LogMsgTxt;
  
  try
  {
    strDate = UpdateVal.Format("%b-%d-%y %H:%M:%S");
    ASCII2TMST(((LPCTSTR) strDate), 18, &IP21ts, &tsErr);  
        
    if (tsErr != 0)
    {
      LogMsgTxt.Format("UpdateWorkorderField (ID=%d): time conversion error (%s)", WORecID, strDate);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21Stat = 1;
    }
    else        
    {  
      TIME2DB(WORecID, FieldTag, IP21ts, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("UpdateWorkorderField (ID=%d): Timestamp write error: %.*s",
                           WORecID, ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        IP21Stat = 1;
      }  
    }
  }  
  catch(CException *e)
  {
    TCHAR   szCause[255];
    e->GetErrorMessage(szCause, 255);  
    LogMsgTxt.Format("UpdateWorkorderField Exception: %s", szCause);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Update a workorder record field
      (updates character strings in workorder records) 
------------------------------------------------------------------------------- */  
int UpdateWorkorderField(long WORecID, long FieldTag, CString UpdateVal, long strSize)
{
  int  IP21Stat = 0;
  CString LogMsgTxt;
  
  CHBF2DB(WORecID, FieldTag, (LPCTSTR) UpdateVal, strSize, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("UpdateWorkorderField (ID=%d): write error: %.*s", WORecID, ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    IP21Stat = 1;
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Close a WorkorderDef record (WC and SP only)
      (updates quanties in workorder records)
------------------------------------------------------------------------------- */  
int CloseWorkorderRecord(long WORecID, PanCIM_WIP_V *WOData, short OraStat)
{
  long WOStat   = 0;
  int  NumChars = 0,
       IP21Stat = 0;
  short NumOK   = 0;
  CString LogMsgTxt;
  CString OracleStatus[3];
  long IP21Tags[2] = {DONEQTY_FT, REJQTY_FT};
  static short IP21Types[2] = {DTYPLONG, DTYPLONG};
  struct IP21Fields {long GoodQty,BadQty;} IP21Data;
 
  OracleStatus[0] = "?????????";
  OracleStatus[1] = "COMPLETED";
  OracleStatus[2] = "CLOSED";
  DB2LONG(WORecID, WOSTATUS_FT, &WOStat, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("CloseWorkorderRecord: WOStatus read error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    IP21Stat = 1;
  }
  else
  {
    // update the done and scrap quantities if not scheduled or running
    if ((WOStat != WOSTAT_SCHEDULED) && (WOStat != WOSTAT_RUNNING))
    {
      IP21Data.GoodQty = (long) WOData->m_COMPLETED_QTY;
      IP21Data.BadQty  = (long) WOData->m_QUANTITY_SCRAPPED;
      WDBVALS(WORecID, 2, IP21Tags, IP21Types, &IP21Data, &NumOK, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {      
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CloseWorkorderRecord: %s close write error: %.*s",
                          WOData->m_WIP_ENTITY_NAME, ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        IP21Stat = 1;
      }    
    }
    else
    {
      CString state;
      state = "???";
      if (WOStat == WOSTAT_SCHEDULED)
        state = "SCHEDULED";
      if (WOStat == WOSTAT_RUNNING)
        state = "RUNNING";
      if (OraStat > 2)
        OraStat = 0;

      LogMsgTxt.Format("WARNING: Job %s %s by Oracle - %s in PanCIM", 
                      WOData->m_WIP_ENTITY_NAME, OracleStatus[OraStat], state);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21Stat = 1;
    }
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Reopen a WorkorderDef record
      (returns a WorkorderDef record back on summary in PRINTED status)
------------------------------------------------------------------------------- */  
int ReopenWorkorderRecord(PanCIM_WIP_V *WOData)
{
  long WORecID  = 0,
       WOStat   = 0;
  int  NumChars = 0,
       IP21Stat = 0;
  short NumOK   = 0;
  CString LogMsgTxt;
 
  // find the workorder record
  NumChars = WOData->m_WIP_ENTITY_NAME.GetLength();
  DECODNAM((LPCTSTR) WOData->m_WIP_ENTITY_NAME, NumChars, &WORecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("ReopenWorkorderRecord: %s decode error: %.*s", 
                      WOData->m_WIP_ENTITY_NAME, ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    IP21Stat = 1;        
  }
  if (WORecID > MINRECID)
  {
    DB2LONG(WORecID, WOSTATUS_FT, &WOStat, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("ReopenWorkorderRecord: WOStatus read error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      IP21Stat = 1;
    }
    else
    {
      // reset the status to printed if not scheduled or running
      if ((WOStat != WOSTAT_SCHEDULED) || (WOStat != WOSTAT_RUNNING))
      {
        LONG2DB(WORecID, WOSTATUS_FT, WOSTAT_PRINTED, &IP21error);
        if (IP21error.ERRCODE == SUCCESS)
        {      
          LogMsgTxt.Format("%s Reopened by Oracle", WOData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        }
        else
        {
          ERRMESS(&IP21error, IP21message, &ErrorLength);
          LogMsgTxt.Format("ReopenWorkorderRecord: %s status write error: %.*s",
                            WOData->m_WIP_ENTITY_NAME, ErrorLength, IP21message);
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          IP21error.ERRCODE = SUCCESS;
          IP21Stat = 1;
        }    
      }
      else
      {
        CString state;
        state = "???";
        if (WOStat == WOSTAT_SCHEDULED)
          state = "SCHEDULED";
        if (WOStat == WOSTAT_RUNNING)
          state = "RUNNING";
        LogMsgTxt.Format("WARNING: Job %s REOPENED by Oracle - %s in PanCIM", WOData->m_WIP_ENTITY_NAME, state);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      }
    }
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Create a new OracleJob record
------------------------------------------------------------------------------- */  
long CreateOracleJobRecord(PanCIM_WIP_V *rsOracleJobData)
{
  int     BufLen = 0,
          rtnval = 0;
  CString LogMsgTxt,
          PackCode, 
          strCreateDate,
          strDateDue,
          tmpCharBuffer,
          JobRecName,
          strOriginalTimeStamp,
          strLocalTimeStamp;
  long    tmp_Label_FT = 0,
          dbTime       = 0,
          tmpTimeStamp = 0,
          lCreateDate  = 0,
          lDateDue     = 0,
          JobRecID     = MINRECID,
          OracleTS_offset = 0,
          j, k;
  short   WrapFlag = FALSE,
          NumOK  = 0,
          iOcc   = 0,
          rOcc   = 0,
          tOcc   = 0,
          cOcc   = 0,
          IP21FieldType = 0;
  char    LabelBuffer[LABELLEN],
          tmError = 0;
  CTextFilter TxtFilter;
  CTime lts;
  CConvertTimeZone ConvertTimeZone;
  COracleUTCoffset Oracle_UTC_Offset;
 
  Oracle_UTC_Offset.dbNam = ORACLE_DATABASE;
  Oracle_UTC_Offset.dbUsr = ORACLE_USER;
  Oracle_UTC_Offset.dbPwd = ORACLE_PASSWORD;
  OracleTS_offset = Oracle_UTC_Offset.GetUTCoffset(); 
  
  long FixedTags[8] = {JOB_FT, PARTNAME_FT,  QUERY_RECORD_FT,DATECREATED_FT, NUM_INTEGER_VALUES_FT,
                       NUM_REAL_VALUES_FT, NUM_TIME_STAMP_VALUES_FT, NUM_CHARACTER_STRINGS_FT };
  static short FixedTypes[8] = {JOBLEN, PARTNAMELEN,  DTYPREID, DTYPTIME, DTYPLONG,
                                DTYPLONG, DTYPLONG, DTYPLONG};
  struct FixFields{    char job[JOBLEN],
                            part[PARTNAMELEN];
                       long queryrec,
                            create,
                            iVals,
                            rVals,
                            tVals,
                            cVals;
                   } FixedData;
 
  dbTime=GETDBTIM();
  try
  {
    TxtFilter.MaxLen = NAMELEN;
    TxtFilter.Strip(rsOracleJobData->m_WIP_ENTITY_NAME);
    JobRecName = "JOB" + TxtFilter.TextOut;
    NXTFREE(WrapFlag, &JobRecID); // Get the first free record ID
    if (JobRecID > MINRECID)
    {
      CREATEREC(JobRecID, OracleJobDefID, (LPCTSTR) JobRecName , NAMELEN, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateOracleJobRecord: create record error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return 1; // no valid record ID so return 1   
      }
      // make new record usable
      MAKUSABL(JobRecID, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      { 
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateOracleJobRecord: make record usable error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return JobRecID;        
      }     
      // convert Oracle timestamp to local time zone
      lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_CREATION_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
      strCreateDate = lts.Format("%b-%d-%y %H:%M:%S"); // format the date string for IP21
      ASCII2TMST(((LPCTSTR) strCreateDate), 18, &lCreateDate, &tmError);
      
      if (tmError != 0)
      { 
        lCreateDate = GETDBTIM();
        LogMsgTxt.Format("Error converting creation date |%s| for job %s - setting to current", 
          rsOracleJobData->m_CREATION_DATE.Format("%b-%d-%y %H:%M:%S"), rsOracleJobData->m_WIP_ENTITY_NAME);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      }

      // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //  strOriginalTimeStamp = rsOracleJobData->m_CREATION_DATE.Format("%b-%d-%y %H:%M:%S");
      //  LogMsgTxt.Format("Create Date - Oracle TS:%s  Local TS:%s",
      //                    strOriginalTimeStamp,strCreateDate);
      //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
              
      // convert Oracle timestamp to local time zone
      lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_SCHED_COMPLETION_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
      strDateDue = lts.Format("%b-%d-%y %H:%M:%S"); // format the date string for IP21
      ASCII2TMST(((LPCTSTR) strDateDue), 18, &lDateDue, &tmError);

      if (tmError != 0)
      { 
        lDateDue = GETDBTIM();
        LogMsgTxt.Format("Error converting date due |%s| for job %s - setting to current", 
          rsOracleJobData->m_SCHED_COMPLETION_DATE.Format("%b-%d-%y %H:%M:%S"), rsOracleJobData->m_WIP_ENTITY_NAME);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      }
      
      // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!      
      //  strOriginalTimeStamp = rsOracleJobData->m_SCHED_COMPLETION_DATE.Format("%b-%d-%y %H:%M:%S");
      //  LogMsgTxt.Format("Completion Date - Oracle TS:%s  Local TS:%s",
      //                    strOriginalTimeStamp,strDateDue);
      //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!    
      
      // TODO: check for conversion error and set default 
      
          
      // clean up strings and write fixed data first
      TxtFilter.MaxLen = JOBLEN;
      TxtFilter.Strip(rsOracleJobData->m_WIP_ENTITY_NAME);
      memcpy(FixedData.job, (LPCTSTR) TxtFilter.TextOut, JOBLEN);
      TxtFilter.MaxLen = PARTNAMELEN;
      TxtFilter.Strip(rsOracleJobData->m_ITEM_CODE);
      memcpy(FixedData.part, (LPCTSTR) TxtFilter.TextOut, PARTNAMELEN);
      FixedData.create   = lCreateDate;
      FixedData.queryrec = OracleFieldMap.QueryRecordID;
      FixedData.iVals    = OracleFieldMap.MAXiOcc ;
      FixedData.rVals    = OracleFieldMap.MAXrOcc;
      FixedData.tVals    = OracleFieldMap.MAXtOcc;
      FixedData.cVals    = OracleFieldMap.MAXcOcc;
      WDBVALS(JobRecID, 8, FixedTags, FixedTypes, &FixedData, &NumOK, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateOracleJobRecord: write fixed data error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return JobRecID;       
      }
      // write the labels
      
      // debug -----------------------------------------------------------------------------------------------
        LogMsgTxt.Format("Fixed tags write success! ");
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      // ------------------------------------------------------------------------------------------------------
      
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)
      {
        if (OracleFieldMap.IP21OccNum[j] > 0) // this is a repeat area value 
        {
          strcpy(LabelBuffer, (LPCTSTR) OracleFieldMap.IP21FieldLabel[j]); 
          BufLen = OracleFieldMap.IP21FieldLabel[j].GetLength(); 
          if (BufLen < LABELLEN)
          {
            for (k=(BufLen-1); k<(LABELLEN+1); k++) 
              LabelBuffer[k] = ' ';
          }        
          switch (OracleFieldMap.IP21DataType[j])
          {
            case MAPDATATYPE_INTEGER:
              tmp_Label_FT  = INTEGER_LABEL_FT | OracleFieldMap.IP21OccNum[j];
              IP21FieldType = MAPDATATYPE_INTEGER;
              break;
          
            case MAPDATATYPE_REAL:
              tmp_Label_FT  = REAL_LABEL_FT | OracleFieldMap.IP21OccNum[j];
              IP21FieldType = MAPDATATYPE_REAL;
              break;

            case MAPDATATYPE_CHARACTER:
              tmp_Label_FT  = CHARACTER_LABEL_FT | OracleFieldMap.IP21OccNum[j];
              IP21FieldType = MAPDATATYPE_CHARACTER;
              break;

            case MAPDATATYPE_TIMESTAMP:
              tmp_Label_FT  = TIMESTAMP_LABEL_FT | OracleFieldMap.IP21OccNum[j];
              IP21FieldType = MAPDATATYPE_TIMESTAMP;
              break;

            default:
              tmp_Label_FT  = 0;
              IP21FieldType = 0;
              break;
          }
          if (tmp_Label_FT)
          {  
            CHBF2DB(JobRecID, tmp_Label_FT, LabelBuffer, LABELLEN, &IP21error);
            if (IP21error.ERRCODE != SUCCESS)
            {
              ERRMESS(&IP21error, IP21message, &ErrorLength);
              LogMsgTxt.Format("CreateOracleJobRecord: label data occurrence write error: %.*s",ErrorLength, IP21message);
              Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
              IP21error.ERRCODE = SUCCESS;
              return JobRecID;        
            }
            /* NOTE: the following method for determining which Oracle field values
                     to write is not very efficient and depends on hard coded field
                     name strings. It should be rewritten when time permits.
                                                                                   */
            // now add the value associated with this label 
            
            if (IP21FieldType == MAPDATATYPE_INTEGER)
            { 
              if (OracleFieldMap.JobFields_I[0] == OracleFieldMap.OracleField[j])  // MACHINE_ID
              {
                
                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Machine ID ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------
              
                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_MACHINE_ID ,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: MACHINE ID integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_MACHINE_ID, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[1] == OracleFieldMap.OracleField[j])  // ORGANIZATION_ID
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Organization ID ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_ORGANIZATION_ID,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: ORGANIZATION ID integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_ORGANIZATION_ID, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[2] == OracleFieldMap.OracleField[j])  // WIP_ENTITY_ID
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... WIP Entity ID ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_WIP_ENTITY_ID,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: WIP ENTITY ID integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_WIP_ENTITY_ID, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[3] == OracleFieldMap.OracleField[j])  // CREATED_BY
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Created By ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_CREATED_BY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: CREATED BY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_CREATED_BY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[4] == OracleFieldMap.OracleField[j])  // LAST_UPDATE_BY
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Lastupdate By ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_LAST_UPDATE_BY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: LAST UPDATE BY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_LAST_UPDATE_BY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[5] == OracleFieldMap.OracleField[j])  // WIP_STATUS_CODE
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... WIP Status Code ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_WIP_STATUS_CODE,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: WIP STATUS CODE integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_WIP_STATUS_CODE, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[6] == OracleFieldMap.OracleField[j])  // START_QTY
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Start Qty ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_START_QTY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: START QTY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_START_QTY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[7] == OracleFieldMap.OracleField[j])  // COMPLETED_QTY
              {
                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Completed Qty ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_COMPLETED_QTY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: COMPLETED QTY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_COMPLETED_QTY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[8] == OracleFieldMap.OracleField[j])  // MEASURED_QTY
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Measured Qty ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_MEASURED_QTY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: MEASURED QTY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_MEASURED_QTY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[9] == OracleFieldMap.OracleField[j])  // PRIORITY
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Priority ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_PRIORITY,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: PRIORITY integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_PRIORITY, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[10] == OracleFieldMap.OracleField[j])  // QUANTITY_SCRAPPED
              {
                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Quantity Scrapped ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_QUANTITY_SCRAPPED,&IP21error);  
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: QUANTITY SCRAPPED integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_QUANTITY_SCRAPPED, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[11] == OracleFieldMap.OracleField[j])  // DESIGN_CAVITIES
              {
               
                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Design Cavities ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_DESIGN_CAVITIES,&IP21error);      
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: DESIGN CAVITIES integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_DESIGN_CAVITIES, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[12] == OracleFieldMap.OracleField[j])  // ACTUAL_CAVITIES
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Actual Cavities ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_ACTUAL_CAVITIES,&IP21error);      
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: ACTUAL CAVITIES integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_ACTUAL_CAVITIES, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (OracleFieldMap.JobFields_I[13] == OracleFieldMap.OracleField[j])  // DEPARTMENT_ID
              {

                // debug -----------------------------------------------------------------------------------------------
                  LogMsgTxt.Format("Writing... Department ID ");
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                // ------------------------------------------------------------------------------------------------------

                LONG2DB(JobRecID,(INTEGER_VALUE_FT | OracleFieldMap.IP21OccNum[j]),(long) rsOracleJobData->m_DEPARTMENT_ID,&IP21error);      
                if (IP21error.ERRCODE != SUCCESS)
                {
                  ERRMESS(&IP21error, IP21message, &ErrorLength);
                  LogMsgTxt.Format("CreateOracleJobRecord: DEPARTMENT ID integer (%d) value data occurrence write error: %.*s",
                                    (long) rsOracleJobData->m_DEPARTMENT_ID, ErrorLength, IP21message);
                  Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                  IP21error.ERRCODE = SUCCESS;
                  return JobRecID;                     
                }
              }
              if (IP21error.ERRCODE != SUCCESS)
              {
                ERRMESS(&IP21error, IP21message, &ErrorLength);
                LogMsgTxt.Format("CreateOracleJobRecord: integer value data occurrence write error: %.*s",
                                  ErrorLength, IP21message);
                Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                IP21error.ERRCODE = SUCCESS;
                return JobRecID;                     
              }
            }
            if (IP21FieldType == MAPDATATYPE_REAL)  // real data types
            { 
              if (OracleFieldMap.JobFields_R[0] == OracleFieldMap.OracleField[j])  // STANDARD_COST
              {
                if (rsOracleJobData->m_STANDARD_COST != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_STANDARD_COST,&IP21error);  
                }
                else
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);  
                }
              }
              if (OracleFieldMap.JobFields_R[1] == OracleFieldMap.OracleField[j])  // RESOURCE_RATE
              {
                if (rsOracleJobData->m_RESOURCE_RATE != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_RESOURCE_RATE,&IP21error);  
                }
                else
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);  
                }
              }
              if (OracleFieldMap.JobFields_R[2] == OracleFieldMap.OracleField[j])  // MATERIAL_RATE
              {
                if (rsOracleJobData->m_MATERIAL_RATE != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_MATERIAL_RATE,&IP21error);  
                }
                else
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);  
                }
              }
              if (OracleFieldMap.JobFields_R[3] == OracleFieldMap.OracleField[j])  // MATERIAL_OVERHEAD_RATE
              {
                if (rsOracleJobData->m_MATERIAL_OVERHEAD_RATE != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_MATERIAL_OVERHEAD_RATE,&IP21error);  
                }
                else
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);  
                }
              }
              if (OracleFieldMap.JobFields_R[4] == OracleFieldMap.OracleField[j])  // OVERHEAD_RATE
              {
                if (rsOracleJobData->m_OVERHEAD_RATE != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_OVERHEAD_RATE,&IP21error);  
                }
                else
                {  
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);              
                }
              }
              if (OracleFieldMap.JobFields_R[5] == OracleFieldMap.OracleField[j])  // OUTSIDE_PROCESS_RATE
              {
                if (rsOracleJobData->m_OUTSIDE_PROCESS_RATE != NULL)
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),rsOracleJobData->m_OUTSIDE_PROCESS_RATE,&IP21error);  
                }
                else
                {
                  REAL2DB(JobRecID,(REAL_VALUE_FT | OracleFieldMap.IP21OccNum[j]),0,&IP21error);  
                }
              }
              if (IP21error.ERRCODE != SUCCESS)
              {
                ERRMESS(&IP21error, IP21message, &ErrorLength);
                LogMsgTxt.Format("CreateOracleJobRecord: real value data occurrence write error: %.*s",
                                  ErrorLength, IP21message);
                Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                IP21error.ERRCODE = SUCCESS;
                return JobRecID;                     
              }
            }
            if (IP21FieldType == MAPDATATYPE_CHARACTER) // character data types
            {
              tmpCharBuffer.Empty();
              TxtFilter.MaxLen = CHARSTRINGLEN;
              if (OracleFieldMap.JobFields_C[0] == OracleFieldMap.OracleField[j])  // PANCIM_STATUS
              {
                TxtFilter.Strip(rsOracleJobData->m_PANCIM_STATUS);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[1] == OracleFieldMap.OracleField[j])  // ORGANIZATION_CODE
              {
                TxtFilter.Strip(rsOracleJobData->m_ORGANIZATION_CODE);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[2] == OracleFieldMap.OracleField[j])  // WIP_ENTITY_NAME
              {
                TxtFilter.Strip(rsOracleJobData->m_WIP_ENTITY_NAME);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[3] == OracleFieldMap.OracleField[j])  // ITEM_CODE
              {
                TxtFilter.Strip(rsOracleJobData->m_ITEM_CODE);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[4] == OracleFieldMap.OracleField[j])  // ITEM_UOM
              {
                TxtFilter.Strip(rsOracleJobData->m_ITEM_UOM);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[5] == OracleFieldMap.OracleField[j])  // COLOR
              {
                TxtFilter.Strip(rsOracleJobData->m_COLOR);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[6] == OracleFieldMap.OracleField[j])  // MATERIAL_CODE
              {
                TxtFilter.Strip(rsOracleJobData->m_MATERIAL_CODE);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[7] == OracleFieldMap.OracleField[j])  // MATERIAL_UOM
              {
                TxtFilter.Strip(rsOracleJobData->m_MATERIAL_UOM);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[8] == OracleFieldMap.OracleField[j])  // EMPLOYEE_ID
              {
                TxtFilter.Strip(rsOracleJobData->m_EMPLOYEE_ID);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[9] == OracleFieldMap.OracleField[j])  // WIP_JOB_STATUS
              {
                TxtFilter.Strip(rsOracleJobData->m_WIP_JOB_STATUS);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[10] == OracleFieldMap.OracleField[j])  // MOLD_NAME
              {
                TxtFilter.Strip(rsOracleJobData->m_MOLD_NAME);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[11] == OracleFieldMap.OracleField[j])  // MOLD_BASE
              {
                TxtFilter.Strip(rsOracleJobData->m_MOLD_BASE);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[12] == OracleFieldMap.OracleField[j])  // MACHINE_NAME
              {
                TxtFilter.Strip(rsOracleJobData->m_MACHINE_NAME);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (OracleFieldMap.JobFields_C[13] == OracleFieldMap.OracleField[j])  // DEPARTMENT_CODE
              {
                TxtFilter.Strip(rsOracleJobData->m_DEPARTMENT_CODE);
                tmpCharBuffer = TxtFilter.TextOut;
              }
              if (tmpCharBuffer.IsEmpty() == 0)  
              {
                CHBF2DB(JobRecID,(CHARACTER_STRING_FT | OracleFieldMap.IP21OccNum[j]),(LPCTSTR) tmpCharBuffer,CHARSTRINGLEN,&IP21error);  
              }      
              if (IP21error.ERRCODE != SUCCESS)
              {
                ERRMESS(&IP21error, IP21message, &ErrorLength);
                LogMsgTxt.Format("CreateOracleJobRecord: character string data occurrence write error: %.*s",
                                  ErrorLength, IP21message);
                Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
                IP21error.ERRCODE = SUCCESS;
                return JobRecID;                     
              }
            }
            if (IP21FieldType == MAPDATATYPE_TIMESTAMP) // timestamp data types
            {
              tmpCharBuffer.Empty();
              tmpTimeStamp = 0;
              if (OracleFieldMap.JobFields_T[0] == OracleFieldMap.OracleField[j])  // CREATION_DATE
              {
                tmpCharBuffer = rsOracleJobData->m_CREATION_DATE.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[1] == OracleFieldMap.OracleField[j])  // LAST_UPDATE_DATE
              {
                tmpCharBuffer = rsOracleJobData->m_LAST_UPDATE_DATE.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[2] == OracleFieldMap.OracleField[j])  // DATE_COMPLETE
              {
                tmpCharBuffer = rsOracleJobData->m_DATE_COMPLETED.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[3] == OracleFieldMap.OracleField[j])  // DATE_CLOSED
              {
                tmpCharBuffer = rsOracleJobData->m_DATE_CLOSED.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[4] == OracleFieldMap.OracleField[j])  // SCHED_START_DATE
              {
                tmpCharBuffer = rsOracleJobData->m_SCHED_START_DATE.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[5] == OracleFieldMap.OracleField[j])  // SCHED_COMPLETION_DATE
              {
                tmpCharBuffer = rsOracleJobData->m_SCHED_COMPLETION_DATE.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (OracleFieldMap.JobFields_T[6] == OracleFieldMap.OracleField[j])  // PRINT_DATE
              {
                tmpCharBuffer = rsOracleJobData->m_PRINT_DATE.Format("%b-%d-%y %H:%M:%S");
                ASCII2TMST((LPCTSTR) tmpCharBuffer, 18 , &tmpTimeStamp, &tmError);
              }
              if (tmpTimeStamp > 0)
              {
                TIME2DB(JobRecID,(TIME_STAMP_VALUE_FT | OracleFieldMap.IP21OccNum[j]),tmpTimeStamp,&IP21error);  
              }
            }
          }
        }
      } 
      // activate the OracleJob record    
      LONG2DB(JobRecID, ACTIVATE_TYPE_FT, ORACLEJOB_ACT_NEW, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("CreateOracleJobRecord: Activate type write error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        return JobRecID;        
      }
    }
    else
    {
      LogMsgTxt.Format("CreateOracleJobRecord:no free IDs available to create record for %s", rsOracleJobData->m_WIP_ENTITY_NAME);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      rtnval = 1; // no free record IDs
    }      
  }
  catch( CException* e )
  {
    TCHAR   szCause[255];
    e->GetErrorMessage(szCause, 255);  
    LogMsgTxt.Format("CreateOracleJobRecord Exception: %s", szCause);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    rtnval=1;
  }  
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Modify an OracleJobDef record
      (updates various fields in OracleJobDef records)
------------------------------------------------------------------------------- */  
int ModifyOracleJobRecord(PanCIM_WIP_V *rsOracleJobData)
{
  int   NumChars       = 0,
        IP21Stat       = 0,
        FlagReset      = 0, 
        UpdateStat     = 0,
        RecActStat     = 0,
        CloseHistStat  = 0,
        UpdateRequired = 0,
        j;
  long JobRecID        = 0,
       JobStatus       = 0;
  CString JobRecName,
          LogMsgTxt,
          FlagField,
          tmBuf1,
          tmBuf2;
 
  // find the OracleJob record to update
  JobRecName = "JOB" + rsOracleJobData->m_WIP_ENTITY_NAME;
  NumChars = JobRecName.GetLength();
  DECODNAM(JobRecName, NumChars, &JobRecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    if (IP21error.ERRCODE == NOREC)
    {
      // the IP21 record no longer exists - just update the job history
      LogMsgTxt.Format("ModifyOracleJobRecord: JOB%s (decode error) updating Job History only!", 
                        rsOracleJobData->m_WIP_ENTITY_NAME);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      // reset all Oracle flags
      FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID);  
      IP21error.ERRCODE = SUCCESS;
      return 0;
    }
    else
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("ModifyOracleJobRecord: JOB%s decode error: %.*s", 
                        rsOracleJobData->m_WIP_ENTITY_NAME, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      IP21Stat = 1;        
    }
  }
  if (JobRecID > MINRECID)
  {
    if (rsOracleJobData->m_MACHINE_RESOURCE_FLAG == "Y")
    {
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "MACHINE_NAME")
        {
          UpdateRequired = 1; // field data requires updating

          /* replaced with string value update per ACB 09/04/07
             -------------------------------------------------- 
                UpdateStat = UpdateOracleJobRecord(JobRecID, (atol((LPCTSTR) rsOracleJobData->m_MACHINE_NAME)),
                            OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          */
          
          UpdateStat = UpdateOracleJobRecord(JobRecID, rsOracleJobData->m_MACHINE_NAME,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);   
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {  
              // reset the Oracle flag field
              FlagField = "MACHINE_RESOURCE_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "MACHINE_RESOURCE_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }  
    if (rsOracleJobData->m_START_QTY_FLAG == "Y")
    {
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "START_QTY")
        {
          UpdateRequired = 1; // field data requires updating
          UpdateStat = UpdateOracleJobRecord(JobRecID, (long) rsOracleJobData->m_START_QTY,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {   
              // reset the Oracle flag field
              FlagField = "START_QTY_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "START_QTY_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }
    if (rsOracleJobData->m_COMPLETED_QTY_FLAG == "Y")
    {
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "COMPLETED_QTY")
        {
          UpdateRequired = 1; // field data requires updating
          UpdateStat = UpdateOracleJobRecord(JobRecID, (long) rsOracleJobData->m_COMPLETED_QTY,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {   
              // reset the Oracle flag field
              FlagField = "COMPLETED_QTY_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "COMPLETED_QTY_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }
    if (rsOracleJobData->m_DATE_COMPLETED_FLAG == "Y")
    {
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "DATE_COMPLETED")
        {
          // convert Oracle timestamp to local time zone
          CTime lts;
          CString strOriginalTimeStamp,
                  strLocalTimeStamp;
          CConvertTimeZone ConvertTimeZone; 
          lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_DATE_COMPLETED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
            
          // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                     
          //   strOriginalTimeStamp = rsOracleJobData->m_DATE_COMPLETED.Format("%b-%d-%y %H:%M:%S");
          //   strLocalTimeStamp = lts.Format("%b-%d-%y %H:%M:%S");
          //   LogMsgTxt.Format("Complete Date - Oracle TS:%s  Local TS:%s",
          //                     strOriginalTimeStamp,strLocalTimeStamp);
          //   Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
          // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          
          UpdateRequired = 1; // field data requires updating
          UpdateStat = UpdateOracleJobRecord(JobRecID, lts,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {   
              // reset the Oracle flag field
              FlagField = "DATE_COMPLETED_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "DATE_COMPLETED_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }
    if (rsOracleJobData->m_DATE_CLOSED_FLAG == "Y")
    {    
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "DATE_CLOSED")
        {
          // convert Oracle timestamp to local time zone
          CTime lts;
          CString strOriginalTimeStamp,
          strLocalTimeStamp;
          CConvertTimeZone ConvertTimeZone; 
          lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_DATE_CLOSED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
            
          // DEBUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                     
          //  strOriginalTimeStamp = rsOracleJobData->m_DATE_CLOSED.Format("%b-%d-%y %H:%M:%S");
          //  strLocalTimeStamp = lts.Format("%b-%d-%y %H:%M:%S");
          //  LogMsgTxt.Format("Close Date - Oracle TS:%s  Local TS:%s",
          //                    strOriginalTimeStamp,strLocalTimeStamp);
          //  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
          // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!      
            
          UpdateRequired = 1; // field data requires updating
          UpdateStat = UpdateOracleJobRecord(JobRecID, lts,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {   
              // reset the Oracle flag field
              FlagField = "DATE_CLOSED_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "DATE_CLOSED_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }  
    if (rsOracleJobData->m_MOLD_ID_FLAG == "Y")
    {
      UpdateRequired = 0; // reset update required flag
      for (j=0; j<OracleFieldMap.TotalMapFields; j++)  // check if an OracleJob record field needs to be updated
      {
        if (OracleFieldMap.OracleField[j] == "MOLD_NAME")
        {
          UpdateRequired = 1; // field data requires updating
          UpdateStat = UpdateOracleJobRecord(JobRecID, rsOracleJobData->m_MOLD_NAME ,
                       OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_MODIFIED);
            if (RecActStat == 0) 
            {   
              // reset the Oracle flag field
              FlagField = "MOLD_ID_FLAG";
              FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
            }
          }
        }
      }
      if (UpdateRequired == 0)  // just update the Oracle flag field
      {
        FlagField = "MOLD_ID_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }  
    if (rsOracleJobData->m_WIP_STATUS_FLAG == "Y")
    {
      UpdateStat = 0;
      switch ((long) rsOracleJobData->m_WIP_STATUS_CODE)
      {    
        case ORACLE_WIP_UNRELEASED: // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to UNRELEASED",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_RELEASED: // job reopened
          RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_REOPEN);
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to RELEASED",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_COMPLETE: // job complete
          RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_COMPLETE);
                    LogMsgTxt.Format("%s WIP_STATUS_CODE changed to COMPLETE",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;
        
        case ORACLE_WIP_NO_CHARGE:  // complete - no charge
          RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_NO_CHARGE);
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to COMPLETE - NO CHARGES",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_HOLD: // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to ON HOLD",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_CANCELLED:  // job cancelled
          RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_CANCEL);
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to CANCELLED",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_BILL_LOAD:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to PENDING BILL LOAD",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_BILL_LOAD: // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to FAILED BILL LOAD",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_ROUTE_LOAD: // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to PENDING ROUTING LOAD",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_ROUTE_LOAD:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to FAILED ROUTING LOAD",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_CLOSED: // job closed in Oracle
          UpdateStat = CloseOracleJobRecord(JobRecID, rsOracleJobData);
          if (UpdateStat == 0)
          {
            RecActStat = ActivateOracleJobRecord(JobRecID, ORACLEJOB_ACT_CLOSE);
            if (RecActStat == 0)
              CloseHistStat = PanCIMHistoryClose(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID);
          }
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to CLOSED",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);       
          break;

        case ORACLE_WIP_PENDING_MASS_LOAD:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to PENDING - MASS LOADED",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_CLOSE:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to PENDING CLOSE",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_FAILED_CLOSED:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to FAILED CLOSE",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_PENDING_SCHEDULE: // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to PENDING SCHEDULING",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        case ORACLE_WIP_DRAFT:  // don't care - just log event
          LogMsgTxt.Format("%s WIP_STATUS_CODE changed to DRAFT",rsOracleJobData->m_WIP_ENTITY_NAME);
          Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
          break;

        default:  // unknown code - just log event
          LogMsgTxt.Format("ModifyWorkorderRecord: Unknown Oracle WIP_STATUS_CODE (%d)",rsOracleJobData->m_WIP_STATUS_CODE);
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          break;
      }
      if (UpdateStat == 0)
      {
        // reset the Oracle flag field
        FlagField = "WIP_STATUS_FLAG";
        FlagReset = UpdateOracleFlag(rsOracleJobData->m_ORGANIZATION_ID, rsOracleJobData->m_WIP_ENTITY_ID, FlagField);   
      }
    }
  }
  else
  {
    // invalid record ID
    LogMsgTxt.Format("ModifyOracleJobRecord: unable to find record %s", rsOracleJobData->m_WIP_ENTITY_NAME);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Close an OracleJobDef record
      (update quantities in OracleJobDef records)
------------------------------------------------------------------------------- */  
int CloseOracleJobRecord(long JobRecID, PanCIM_WIP_V *rsOracleJobData)
{
  long JobStat   = 0;
  int  NumChars  = 0,
       IP21Stat  = 0,
       j;
  short NumOK    = 0;
  CString JobRecName,
          LogMsgTxt;
   
  DB2LONG(JobRecID, STATUS_FT, &JobStat, &IP21error); // read the current job status in IP21 database
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("CloseOracleJobRecord: Status read error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;        
    IP21Stat = 1;
  }
  else
  {
    if (JobStat == ORACLEJOB_STAT_PRODRECINUSE)
    {
      IP21Stat = 1;
      LogMsgTxt.Format("CloseOracleJobRecord: Oracle WIP close update to in use job %s failed", JobRecName);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      // update the done and scrap quantities if mapped     
      for (j=0; j<OracleFieldMap.TotalMapFields; j++);
      {
        if (OracleFieldMap.OracleField[j] == "COMPLETED_QTY")
        {
          IP21Stat = UpdateOracleJobRecord(JobRecID, (long) rsOracleJobData->m_COMPLETED_QTY,
                     OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
        }
        if (OracleFieldMap.OracleField[j] == "QUANTITY_SCRAPPED")
        {
          IP21Stat = UpdateOracleJobRecord(JobRecID, (long) rsOracleJobData->m_QUANTITY_SCRAPPED,
                     OracleFieldMap.IP21OccNum[j], OracleFieldMap.IP21Field[j]);
        }
      }
    }
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Update an OracleJob record 
      (updates long values in OracleJobDef records) 
------------------------------------------------------------------------------- */  
int UpdateOracleJobRecord(long rec, long val, long occ, long tag)
{
  int rtnval = 0;
  CString LogMsgTxt;

  if (occ > 0)  // repeat area field update
  {
    LONG2DB(rec, (INTEGER_VALUE_FT | occ), val, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: integer data repeat area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }
    else
    {
      // now update the modified flag
      LONG2DB(rec, (INTEGER_MODIFIED_FT | occ), 1, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("UpdateOracleJobRecord: integer modified flag repeat area write error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        rtnval = 1;
      }
    }
  }
  else  // fixed area field update
  {    
    LONG2DB(rec, tag, val, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: integer data fixed area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }  
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Update an OracleJob record 
      (Updates timestamp values in OracleJobDef records) 
------------------------------------------------------------------------------- */  
int UpdateOracleJobRecord(long rec, CTime val, long occ, long tag)
{
  int rtnval   = 0;
  long IP21ts  = 0;
  char tmError = 0;
  CString tsBuff,
          LogMsgTxt;

  // convert to IP21 timestamp
  tsBuff = val.Format("%b-%d-%y %H:%M:%S");
  ASCII2TMST((LPCTSTR) tsBuff, 18, &IP21ts, &tmError);
  if (occ > 0)  // repeat area field update
  {
    TIME2DB(rec, (TIME_STAMP_VALUE_FT | occ), IP21ts, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: timestamp data repeat area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }
    else
    {
      // now update the modified flag
      LONG2DB(rec, (TIMESTAMP_MODIFIED_FT | occ), 1, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("UpdateOracleJobRecord: timestamp modified flag repeat area write error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        rtnval = 1;
      }
    }
  }
  else  // fixed area field update
  {
    TIME2DB(rec, tag, IP21ts, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: timestamp data fixed area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }  
  }
  return rtnval;  
}
/* ----------------------------------------------------------------------------
    Update an OracleJob record 
      (updates double values in OracleJobDef records) 
------------------------------------------------------------------------------- */  
int UpdateOracleJobRecord(long rec, double val, long occ, long tag)
{
  int rtnval = 0;
  CString LogMsgTxt;

  if (occ > 0)  // repeat area field update
  {
    REAL2DB(rec, (REAL_VALUE_FT | occ), val, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: real data repeat area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }
    else
    {
      // now update the modified flag
      LONG2DB(rec, (REAL_MODIFIED_FT | occ), 1, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("UpdateOracleJobRecord: real modified flag repeat area write error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        rtnval = 1;
      }
    }
  }
  else  // fixed area field update
  {
    REAL2DB(rec, tag, val, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: real data fixed area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }  
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Update an OracleJob record 
      (updates string values in OracleJobDef records) 
------------------------------------------------------------------------------- */  
int UpdateOracleJobRecord(long rec, CString val, long occ, long tag)
{
  int valLen = 0,
      rtnval = 0;
  CString LogMsgTxt;
  
  valLen = val.GetLength();
  if (occ > 0)  // repeat area field update
  {
    CHBF2DB(rec, (CHARACTER_STRING_FT | occ), (LPCTSTR) val, valLen, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: character data repeat area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }
    else
    {
      // now update the modified flag
      LONG2DB(rec, (CHARACTER_MODIFIED_FT | occ), 1, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        LogMsgTxt.Format("UpdateOracleJobRecord: character modified flag repeat area write error: %.*s", ErrorLength, IP21message);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        IP21error.ERRCODE = SUCCESS;
        rtnval = 1;
      }
    }
  }
  else  // fixed area field update
  {
    CHBF2DB(rec, tag, (LPCTSTR) val, valLen, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateOracleJobRecord: character data fixed area write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;
    }  
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Activate an OracleJobDef record
------------------------------------------------------------------------------- */  
int ActivateOracleJobRecord(long JobRecID, long ActCode)
{
  int IP21Stat = 0;
  CString LogMsgTxt;

  LONG2DB(JobRecID, ACTIVATE_TYPE_FT, ActCode, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("ActivateOracleJobRecord: Activate type write error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;        
    IP21Stat = 1;
  }
  return IP21Stat;
}
/* ----------------------------------------------------------------------------
    Update a job history record
      (updates various data in JobHistory table)
------------------------------------------------------------------------------- */  
int UpdateJobHistory(PanCIM_WIP_V *rsOracleJobData)
{
  int rtnval     = 0;
  double DblVal1 = 0,
         DblVal2 = 0,
         DblVal3 = 0,
         DblVal4 = 0;
  long RefID     = 0;
  CString strSQL,
          strTS1,
          strTS2,
          strFields,
          strValues,
          LogMsgTxt,
          PackCode,
          dbConn;
  CDatabase db;
  CTime lts;
  CConvertTimeZone ConvertTimeZone;
  dbConn = DSN_PRODJOBHIST;
  CJobRefID rsJobRef(&db);
  try
  {
    if (db.OpenEx(dbConn))
    { 
      strSQL.Format("SELECT RefID FROM JobHistory WHERE JobID = '%s'", rsOracleJobData->m_WIP_ENTITY_NAME);
      rsJobRef.Open(CRecordset::snapshot, strSQL);
      if (rsJobRef.m_RefID)
        RefID = rsJobRef.m_RefID;  // save the reference ID   
      if (rsJobRef.IsOpen())
        rsJobRef.Close();
      if (db.IsOpen())
        db.Close();
      if (RefID)  // record already exists - just update
      {
        // convert to local time zone
        lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_DATE_COMPLETED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
        strTS1 = lts.Format("%c");
        if (rsOracleJobData->m_DATE_CLOSED == NULL)
        { 
          strSQL.Format("UPDATE JobHistory SET Status='%s',Machine=%d,PlanQty=%f,CompleteQty=%f,ProdDoneDate='%s', MoldName='%s', ModFlag=1 WHERE RefID=%d",  
          rsOracleJobData->m_WIP_JOB_STATUS, atol((LPCTSTR) rsOracleJobData->m_MACHINE_NAME),rsOracleJobData->m_START_QTY,rsOracleJobData->m_COMPLETED_QTY, strTS1, rsOracleJobData->m_MOLD_NAME, RefID);
        }
        else
        {
          // convert to local time zone
          lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_DATE_CLOSED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
          strTS2 = lts.Format("%c"); 
          strSQL.Format("UPDATE JobHistory SET Status='%s',Machine=%d,PlanQty=%f,CompleteQty=%f,ProdDoneDate='%s', ClosedDate='%s', MoldName='%s', ModFlag=1 WHERE RefID=%d",
          rsOracleJobData->m_WIP_JOB_STATUS, atol((LPCTSTR) rsOracleJobData->m_MACHINE_NAME),rsOracleJobData->m_START_QTY,rsOracleJobData->m_COMPLETED_QTY,
          strTS1, strTS2,rsOracleJobData->m_MOLD_NAME,RefID);
        }
        LogMsgTxt.Format("UpdateJobHistory: Updating RefID %d for job %s", RefID,rsOracleJobData->m_WIP_ENTITY_NAME);  
        Log.LogMessage(0, (LPCTSTR) LogMsgTxt);      
      }
      else  // create a new record
      {
        if (rsOracleJobData->m_RESOURCE_RATE == NULL)
          DblVal1 = 0;
        else
          DblVal1 = rsOracleJobData->m_RESOURCE_RATE;
        if (rsOracleJobData->m_STANDARD_COST == NULL)
          DblVal2 = 0;
        else
          DblVal2 = rsOracleJobData->m_STANDARD_COST;
        if (rsOracleJobData->m_OVERHEAD_RATE == NULL)
          DblVal3 = 0;
        else
          DblVal3 = rsOracleJobData->m_OVERHEAD_RATE;
        if (rsOracleJobData->m_MATERIAL_RATE == NULL)
          DblVal4 = 0;
        else
          DblVal4 = rsOracleJobData->m_MATERIAL_RATE;

        // convert to local time zone
        lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_PRINT_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
        strTS1 = lts.Format("%c");
        lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_SCHED_COMPLETION_DATE,OracleTimeZoneOffset,LocalTimeZoneOffset); 
        strTS2 = lts.Format("%c");
        strFields = "JobID,PrintDate,Priority,DueDate,PlanQty,CellID,Machine,PartName,PackCode,";
        strFields += "Material,Color,MoldName,ResourceRate,StandardCost,OverHeadRate,MaterialCost,Status, ModFlag";
        PackCode = ExtractPackCode(rsOracleJobData->m_ITEM_CODE);
        strValues.Format("'%s','%s',%f,'%s',%f,'%s',%d,'%s','%s','%s','%s','%s',%f,%f,%f,%f,'%s',1",
          rsOracleJobData->m_WIP_ENTITY_NAME,
          strTS1,
          rsOracleJobData->m_PRIORITY,
          strTS2,
          rsOracleJobData->m_START_QTY,
          rsOracleJobData->m_DEPARTMENT_CODE,
          atol((LPCTSTR) rsOracleJobData->m_MACHINE_NAME),
          rsOracleJobData->m_ITEM_CODE,
          PackCode,
          rsOracleJobData->m_MATERIAL_CODE,
          rsOracleJobData->m_COLOR,
          rsOracleJobData->m_MOLD_NAME,
          DblVal1,
          DblVal2,
          DblVal3,
          DblVal4,
          rsOracleJobData->m_WIP_JOB_STATUS); 
        strSQL.Format("INSERT INTO JobHistory (%s) VALUES (%s)", strFields, strValues);
        LogMsgTxt.Format("UpdateJobHistory: Adding new record for job %s", rsOracleJobData->m_WIP_ENTITY_NAME);  
        Log.LogMessage(0, (LPCTSTR) LogMsgTxt);     
      }        
      JobHistory rsJobHistory(&db);
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        db.ExecuteSQL((LPCTSTR)strSQL);
      }
    }
  }
  catch( CDBException* e )
  {
    LogMsgTxt = "UpdateJobHistory Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    rtnval=1;
  }
  if (db.IsOpen())
    db.Close();
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Close a job history record.
      (updates close data in JobHistory table)
------------------------------------------------------------------------------- */  
int CloseJobHistory(PanCIM_WIP_V *rsOracleJobData)
{
  int rtnval = 0;
  CString strSQL,
          strTS1,
          recFilter,
          LogMsgTxt,
          dbConn;
  CTime lts;
  CDatabase db;
  CConvertTimeZone ConvertTimeZone;

  dbConn = DSN_PRODJOBHIST;
  
  // format the date
  lts = ConvertTimeZone.ConvertTS(rsOracleJobData->m_DATE_CLOSED,OracleTimeZoneOffset,LocalTimeZoneOffset); 
  strTS1 = lts.Format("%b-%d-%y %H:%M:%S");
  // build SQL string
  strSQL.Format("Update JobHistory SET MeasuredQty=%d,CompletedQty=%d,ScrapQty=%d,EmployeeID='%s',CloseDate='%s',Status='%s' WHERE JobID='%s'",
      rsOracleJobData->m_MEASURED_QTY,rsOracleJobData->m_COMPLETED_QTY,rsOracleJobData->m_QUANTITY_SCRAPPED,rsOracleJobData->m_EMPLOYEE_ID,
      strTS1,rsOracleJobData->m_WIP_JOB_STATUS,rsOracleJobData->m_WIP_ENTITY_NAME);
  try
  {   
    if (db.OpenEx(dbConn,CDatabase::useCursorLib))
    { 
      db.ExecuteSQL((LPCTSTR)strSQL);
    }
  }
  catch( CDBException* e )
  {
    LogMsgTxt = "CloseJobHistory Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    rtnval=1;
  }
  if (db.IsOpen())
    db.Close();
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Update one of the flag fields in the Oracle PANDUIT_WIP_PANCIM_JOB record
------------------------------------------------------------------------------- */  
int UpdateOracleFlag(long org_id, long ent_id, CString fld_id)
{
  int rtnval = 0;
  CString dbConn,
          strSQL,
          LogMsgTxt;
  CDatabase db;
  dbConn = DSN_WIP;

  strSQL.Format("UPDATE APPS.PANDUIT_WIP_PANCIM_JOB SET %s = 'N' WHERE ORGANIZATION_ID=%d AND WIP_ENTITY_ID=%d",
                 (LPCTSTR) fld_id, org_id, ent_id); 
  try
  { 
    if (db.OpenEx(dbConn))
    {
      db.ExecuteSQL((LPCTSTR) strSQL);
    }
  }
  catch( CDBException* e )
  {
    LogMsgTxt = "UpdateOracleFlag Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    rtnval=1;
  }
  if (db.IsOpen())
  {
    db.Close();
  }
  return rtnval;
}
/* --------------------------------------------------------------------------------------
    Update all flag fields and PanCIM status in the Oracle PANDUIT_WIP_PANCIM_JOB record
----------------------------------------------------------------------------------------- */  
int UpdateOracleFlag(long org_id, long ent_id)
{
  int rtnval = 0;
  CString dbConn,
          strSQL,
          strFilter,
          strUpdate,
          LogMsgTxt;
  CDatabase db;
  dbConn = DSN_WIP;

  strSQL = "UPDATE APPS.PANDUIT_WIP_PANCIM_JOB SET WIP_STATUS_FLAG='N',PANCIM_STATUS='OK',";
  strSQL+= "MACHINE_RESOURCE_FLAG='N',START_QTY_FLAG='N',COMPLETED_QTY_FLAG='N',";
  strSQL+= "DATE_COMPLETED_FLAG='N',DATE_CLOSED_FLAG='N', MOLD_ID_FLAG='N' ";
  strFilter.Format("WHERE ORGANIZATION_ID=%d AND WIP_ENTITY_ID=%d",org_id, ent_id); 
  strUpdate = strSQL + strFilter;  
  try
  { 
    if (db.OpenEx(dbConn))
    {
      db.ExecuteSQL((LPCTSTR) strUpdate);
    }
  }
  catch( CDBException* e )
  {
    LogMsgTxt = "UpdateOracleFlag Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    rtnval=1;
  }
  if (db.IsOpen())
  {
    db.Close();
  }  
  return rtnval;
}
/* ------------------------------------------------------------------------------
    Update production quantities in Oracle from workorder records
--------------------------------------------------------------------------------- */  
int UpdateOracleWODone(long rec)
{
  int UpdateStat = 0;
  CString dbConn,
          strSQL,
          LogMsgTxt;
  CPanCIMMessage IP21Msg;
  CDatabase db;
  dbConn = DSN_WIP;
  
  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_WODONE_ICOUNT) && (IP21Msg.cCount == MSG_WODONE_CCOUNT) &&
        (IP21Msg.rCount == MSG_WODONE_RCOUNT) && (IP21Msg.tCount == MSG_WODONE_TCOUNT))
    {
      strSQL.Format("UPDATE APPS.PANDUIT_WIP_PANCIM_JOB SET MEASURED_QTY=%d, PANCIM_STATUS='DONE' WHERE ORGANIZATION_ID=%d AND WIP_ENTITY_ID=%d",
                     IP21Msg.IntVals[0], IP21Msg.MsgSource,  IP21Msg.IntVals[2]); 
      try
      { 
        if (db.OpenEx(dbConn))
        {
          db.ExecuteSQL((LPCTSTR) strSQL);  
        }
      }
      catch( CDBException* e )
      {
        LogMsgTxt = "UpdateOracleWODone Exception:";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateStat = 1;
      }
      if (db.IsOpen())
      {
        db.Close();
      }
    }
    else
    {
      LogMsgTxt.Format("UpdateOracleWODone: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }
  else
  {
    LogMsgTxt.Format("UpdateOracleWODone: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }
  if (UpdateStat == 0)
  {
    IP21Msg.CharVals[0][WONUMLEN] = '\0';
    LogMsgTxt.Format("UpdateOracleWODone.... job:%s  measured qty:%d",
                      IP21Msg.CharVals[0],IP21Msg.IntVals[0] );
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* ------------------------------------------------------------------------------
    Update production quantities in the JobHistory table from workorder records 
--------------------------------------------------------------------------------- */  
int UpdateHistWODone(long rec)
{
  int UpdateStat = 0;
  CString strSQL,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  CDatabase db;
  dbConn = DSN_PRODJOBHIST;

  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_WODONE_ICOUNT) && (IP21Msg.cCount == MSG_WODONE_CCOUNT) &&
        (IP21Msg.rCount == MSG_WODONE_RCOUNT) && (IP21Msg.tCount == MSG_WODONE_TCOUNT))
    {
      // build SQL string
      strSQL.Format("Update JobHistory SET MeasuredQty=%d,CycleCount=%d,RunTime=%f,ProdTime=%f WHERE JobID='%s'",
                     IP21Msg.IntVals[0],IP21Msg.IntVals[1],IP21Msg.RealVals[0],IP21Msg.RealVals[1],IP21Msg.CharVals[0]);
      try
      {   
        if (db.OpenEx(dbConn,CDatabase::useCursorLib))
        { 
          db.ExecuteSQL((LPCTSTR)strSQL);
        }    
        if (db.IsOpen())
          db.Close();
      }
      catch( CDBException* e )
      {
        LogMsgTxt = "UpdateHistWODone Exception:";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateStat = 1;
      }
    }
    else
    {
      LogMsgTxt.Format("UpdateHistWODone: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }
  else
  {
    LogMsgTxt.Format("UpdateHistWODone: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }
  if (UpdateStat == 0)
  {  
    IP21Msg.CharVals[0][WONUMLEN] = '\0';
    LogMsgTxt.Format("UpdateHistWODone.... job:%s  measured qty:%d",
                      IP21Msg.CharVals[0],IP21Msg.IntVals[0] );
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* ------------------------------------------------------------------------------
    Update production data in the JobHistory table from workorder records 
--------------------------------------------------------------------------------- */  
int UpdateHistWOClose(long rec)
{
  int UpdateStat = 0;
  CString strSQL,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  CDatabase db;
  dbConn = DSN_PRODJOBHIST;

  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {    
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_WOCLOSE_ICOUNT) && (IP21Msg.cCount == MSG_WOCLOSE_CCOUNT) &&
        (IP21Msg.rCount == MSG_WOCLOSE_RCOUNT) && (IP21Msg.tCount == MSG_WOCLOSE_TCOUNT))
    {
      // build SQL string
      strSQL.Format("Update JobHistory SET MeasuredQty=%d,CycleCount=%d,RunTime=%f,ProdTime=%f WHERE JobID='%s'",
                     IP21Msg.IntVals[1],IP21Msg.IntVals[3],IP21Msg.RealVals[0],IP21Msg.RealVals[1],IP21Msg.CharVals[0]);
      try
      {   
        if (db.OpenEx(dbConn,CDatabase::useCursorLib))
        { 
          db.ExecuteSQL((LPCTSTR)strSQL);
        }    
        if (db.IsOpen())
          db.Close();
      }
      catch( CDBException* e )
      {
        LogMsgTxt = "UpdateHistWOClose Exception:";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateStat = 1;
      }
    }
    else
    {
      LogMsgTxt.Format("UpdateHistWOClose: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }
  else
  {
    LogMsgTxt.Format("UpdateHistWOClose: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }
  if (UpdateStat == 0)
  {  
    IP21Msg.CharVals[0][WONUMLEN] = '\0';
    LogMsgTxt.Format("UpdateHistWOClose.... job:%s  measured qty:%d  actual qty:%d",
                      IP21Msg.CharVals[0],IP21Msg.IntVals[1], IP21Msg.IntVals[0]);
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* ------------------------------------------------------------------------------
    Update production quantities in Oracle from OracleJob records
--------------------------------------------------------------------------------- */  
int UpdateOracleJobDone(long rec)
{
  int UpdateStat = 0;
  CString dbConn,
          strSQL,
          LogMsgTxt;
  CPanCIMMessage IP21Msg;
  CDatabase db;
  dbConn = DSN_WIP;
  
  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_JOBDONE_ICOUNT) && (IP21Msg.cCount == MSG_JOBDONE_CCOUNT) &&
        (IP21Msg.rCount == MSG_JOBDONE_RCOUNT) && (IP21Msg.tCount == MSG_JOBDONE_TCOUNT))
    {
      strSQL.Format("UPDATE APPS.PANDUIT_WIP_PANCIM_JOB SET MEASURED_QTY=%d, PANCIM_STATUS='DONE' WHERE ORGANIZATION_ID=%d AND WIP_ENTITY_ID=%d",
                     IP21Msg.IntVals[0], IP21Msg.MsgSource,  IP21Msg.IntVals[2]);      
      try
      { 
        if (db.OpenEx(dbConn))
        {
          db.ExecuteSQL((LPCTSTR) strSQL);  
        }
        else
        {
          LogMsgTxt = "UpdateOracleJobDone: unable to open Oracle database connection";
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          UpdateStat = 1;
        }
      }
      catch( CDBException* e )
      {
        LogMsgTxt = "UpdateOracleJobDone Exception:";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateStat = 1;
      }
      if (db.IsOpen())
        db.Close();
    }
    else
    {
      LogMsgTxt.Format("UpdateOracleJobDone: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }
  else
  {
    LogMsgTxt.Format("UpdateOracleJobDone: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }
  if (UpdateStat == 0)
  {  
    IP21Msg.CharVals[0][WONUMLEN] = '\0';
    LogMsgTxt.Format("UpdateOracleJobDone.... job:%s  measured qty:%d",
                      IP21Msg.CharVals[0],IP21Msg.IntVals[0]);
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* ------------------------------------------------------------------------------
    Update production quantities in the JobHistory table from OracleJob records 
--------------------------------------------------------------------------------- */  
int UpdateHistJobDone(long rec)
{
  int UpdateStat = 0;
  CString strSQL,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  CDatabase db;

  dbConn = DSN_PRODJOBHIST;
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_JOBDONE_ICOUNT) && (IP21Msg.cCount == MSG_JOBDONE_CCOUNT) &&
        (IP21Msg.rCount == MSG_JOBDONE_RCOUNT) && (IP21Msg.tCount == MSG_JOBDONE_TCOUNT))
    {
      // build SQL string
      strSQL.Format("Update JobHistory SET MeasuredQty=%d,CycleCount=%d,RunTime=%f,ProdTime=%f,DownTime=%f WHERE JobID='%s'",
                     IP21Msg.IntVals[0],IP21Msg.IntVals[1],IP21Msg.RealVals[0],IP21Msg.RealVals[1],IP21Msg.RealVals[2],
                     IP21Msg.CharVals[0]);      
      try
      {
        if (db.OpenEx(dbConn,CDatabase::useCursorLib))
        {
          db.ExecuteSQL((LPCTSTR) strSQL);
        }    
        if (db.IsOpen())
          db.Close();
      }
      catch( CDBException* e )
      {
        LogMsgTxt = "UpdateHistJobDone Exception:";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateStat = 1;
      }
    }
    else
    {
      LogMsgTxt.Format("UpdateHistJobDone: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }
  else
  {
    LogMsgTxt.Format("UpdateHistJobDone: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }
  /*
  if (UpdateStat == 0)
  {  
    IP21Msg.CharVals[0][WONUMLEN] = '\0';
    LogMsgTxt.Format("UpdateHistJobDone: job name=%s  WIP ID=%d  measured qty=%d",
                      IP21Msg.CharVals[0],IP21Msg.IntVals[2],IP21Msg.IntVals[0] );
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  */
  return UpdateStat;
}
/* ----------------------------------------------------------------------------
    PanCIM History Close
      (updates Oracle PanCIM status field to HISTORY CLOSE)
------------------------------------------------------------------------------- */  
int PanCIMHistoryClose(long org_id, long ent_id)
{
  int UpdateStat = 0;
  CString dbConn,
          strSQL,
          LogMsgTxt;
  CDatabase db;
  dbConn = DSN_WIP;
  
  // build the query string
  strSQL.Format("UPDATE APPS.PANDUIT_WIP_PANCIM_JOB SET PANCIM_STATUS='CLOSE' WHERE ORGANIZATION_ID=%d AND WIP_ENTITY_ID=%d",
                  org_id, ent_id); 
  try
  { 
    if (db.OpenEx(dbConn))
    {
      db.ExecuteSQL((LPCTSTR) strSQL);  
    }
  } 
  catch( CDBException* e )
  {
    LogMsgTxt = "PanCIMHistoryClose Exception:";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
    UpdateStat = 1;
  }
  if (db.IsOpen())
  {
    db.Close();
  }
  return UpdateStat;
}
/* ----------------------------------------------------------------------------
    Process PanCIM message record activations
------------------------------------------------------------------------------- */  
int PanCIMMessageHandler(long RecID)
{
  int MessageStat        = 0,
      OracleUpdateStat   = 0,
      HistUpdateStat     = 0,
      ProdCtrlUpdateStat = 0;
  long TransCode = 0;
  CString LogMsgTxt;
  CReactivateMsg ReactivateMsg;
  CPanCIMMessage IP21Msg;
  IP21Msg.RecID = RecID;
  IP21Msg.TransCodeFT      = TRANSCODE_FT;
  IP21Msg.SourceFT         = SOURCE_FT;
  IP21Msg.IntegerCountFT   = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT      = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT       = INT_FIELD_FT;
  IP21Msg.CharFieldFT      = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT      = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT      = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
 
  if (IP21Msg.status == 0)
  {    
    switch (IP21Msg.TransCode)
    {
      case PANCIM_MSGTYPE_WO_DONE:  // workorder record done state
        OracleUpdateStat = UpdateOracleWODone(RecID);     
        if (OracleUpdateStat == 0)  // Oracle was updated with no errors
        {  
          // update the history db
          HistUpdateStat = UpdateHistWODone(RecID);
          if (HistUpdateStat == 0)  // history db updated with no errors  
          {
            MessageStat = DeleteIP21Record(RecID);
          }
        }        
        break;
       
      case PANCIM_MSGTYPE_JOB_DONE: // OracleJob record done state           
        OracleUpdateStat = UpdateOracleJobDone(RecID);             
        if (OracleUpdateStat == 0)  // Oracle was updated with no errors
        {  
          // update the history db
          HistUpdateStat = UpdateHistJobDone(RecID);          
          if (HistUpdateStat == 0)  // history db updated with no errors  
          {
            MessageStat = DeleteIP21Record(RecID);
          }
        }        
        break;
        
      case PANCIM_MSGTYPE_WO_CLOSE:  // workorder record close message
        // update the history db
        HistUpdateStat = UpdateHistWOClose(RecID);
        if (HistUpdateStat == 0)  // history db updated with no errors  
        {
          MessageStat = DeleteIP21Record(RecID);
        }        
        break;
 
      case PANCIM_MSGTYPE_MDS_ISSUE:  // MDS machine issue
        OracleUpdateStat = UpdateOracleMDSIssue(RecID);     
        if (OracleUpdateStat == 0)  // Oracle was updated with no errors
        {  
            MessageStat = DeleteIP21Record(RecID);
        }        
        break;
        
      case PANCIM_MSGTYPE_MDS_INVEN:  // inventory transfer (bin filling)
        OracleUpdateStat = UpdateOracleMDSInventory(RecID);     
        if (OracleUpdateStat == 0)  // Oracle was updated with no errors
        {  
          MessageStat = DeleteIP21Record(RecID);
        }        
        break;
       
      case PANCIM_MSGTYPE_JOB_PERF:  // job performance data update
        HistUpdateStat = UpdateJobPerformance(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;

      case PANCIM_MSGTYPE_BAGGER_PERF:  // bagger performance data update
        HistUpdateStat = UpdateBaggerPerf(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;
      
      case PANCIM_MSGTYPE_INSERTER_PERF:  // inserter performance data update
        HistUpdateStat = UpdateInserterPerf(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;

      case PANCIM_MSGTYPE_LOADER_PERF:  // loader performance data update
        HistUpdateStat = UpdateLoaderPerf(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;

      case PANCIM_MSGTYPE_MOLD_REPAIR:  // mold repair history update
        HistUpdateStat = UpdateMoldRepair(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;

      case PANCIM_MSGTYPE_BAGGER_REPAIR:  // bagger repair history update
        HistUpdateStat = UpdateBaggerRepair(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;
  
      case PANCIM_MSGTYPE_SYSTEM_COMMENT: // system comment data update
        HistUpdateStat = UpdateSysComment(RecID);
        if (HistUpdateStat == 0)
        {
          MessageStat = DeleteIP21Record(RecID);
        }
        break;

      case PANCIM_MSGTYPE_REACTIVATE: // re-activated messages
        ReactivateMsg.DefinitionRecordID = PanCIMMsgDefID;
        ReactivateMsg.DefaultRecordID    = DefaultMsgID ;
        ReactivateMsg.MessageCheckerID   = MessageCheckID;
        ReactivateMsg.ReactivateRecords();
        LogMsgTxt.Format("Message re-Activation...  %d records activated", ReactivateMsg.ActRecordCount);
        Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
        break;
      
      case PANCIM_MSGTYPE_WOCLEANUP:  // workorder cleanup
        WOCleanup();
        break;
     
      default:  // unknown transcode
        LogMsgTxt.Format("PanCIMMessageHandler: Unknown TransCode %d in record %d", IP21Msg.TransCode, RecID);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        MessageStat = 1;
        break;
    }
  }
  if ((OracleUpdateStat) || (HistUpdateStat))
    MessageStat = 1;
  return MessageStat;
}
/* ----------------------------------------------------------------------------
    Delete an IP21 record
------------------------------------------------------------------------------- */  
int DeleteIP21Record(long RecID)
{
  int rtnval = 0;
  CString LogMsgTxt;

  MAKUNUSA(RecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    // remove after testing --------------------------------------------------------------
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("DeleteIP21Record: MAKUNUSA error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    // -----------------------------------------------------------------------------------

    IP21error.ERRCODE = SUCCESS;  // disregard errors here - record may already be unusable
  }

  DELETREC(RecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("DeleteIP21Record: delete IP21 record error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    rtnval = RecID;        
  }
  return rtnval;
}
/* ------------------------------------------------------------------------------
    MDS material issue Oracle update
--------------------------------------------------------------------------------- */  
int UpdateOracleMDSIssue(long rec)
{
  int UpdateStat       = 0;
  double  SeqNum       = 0,
          LocatorID    = 0,
          TransLocator = 0,
          TransAmt     = 0;
  CString strSQL,
          strLotNum,
          strFields,
          strValues,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  COracleSeqNum OracleSeqNum;
  COracleTransactionInfo OracleTrans;
  CInventoryLocation InvenLoc;
  CIssueData IssueData;
  CDatabase db;
  CDateFormat dt;
  CTextFilter TxtFilter;

  dt.GetOracleTime();
  OracleSeqNum.dbNam       = ORACLE_DATABASE;
  OracleSeqNum.dbUsr       = ORACLE_USER;
  OracleSeqNum.dbPwd       = ORACLE_PASSWORD;
  OracleTrans.dbNam        = ORACLE_DATABASE;
  OracleTrans.dbUsr        = ORACLE_USER;
  OracleTrans.dbPwd        = ORACLE_PASSWORD;
  IssueData.dbNam          = ORACLE_DATABASE;
  IssueData.dbUsr          = ORACLE_USER;
  IssueData.dbPwd          = ORACLE_PASSWORD;
  InvenLoc.dbNam           = ORACLE_DATABASE;
  InvenLoc.dbUsr           = ORACLE_USER;
  InvenLoc.dbPwd           = ORACLE_PASSWORD;
  dbConn                   = DSN_MTL_ISSUE;
  IP21Msg.RecID            = rec;
  IP21Msg.TransCodeFT      = TRANSCODE_FT;
  IP21Msg.SourceFT         = SOURCE_FT;
  IP21Msg.IntegerCountFT   = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT      = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT       = INT_FIELD_FT;
  IP21Msg.CharFieldFT      = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT      = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT      = TIME_FIELD_FT;
  IP21Msg.GetMsgData();

  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_MSDISSUE_ICOUNT) && (IP21Msg.cCount == MSG_MDSISSUE_CCOUNT) &&
        (IP21Msg.rCount == MSG_MDSISSUE_RCOUNT) && (IP21Msg.tCount == MSG_MDSISSUE_TCOUNT))
    {         
      strtok(IP21Msg.CharVals[0], " ");
      TransAmt = IP21Msg.IntVals[4] * -1; // make negative
      OracleTrans.TransName = "WIP COMPONENT ISSUE";
      OracleTrans.GetOracleTransactionCodes();
      if (OracleTrans.TransTypeID > 0)
      {
        // Get additional data from APPS.PANDUIT_WIP_PANCIM_JOB_V required for this transaction.
        OracleOrganizationData.GetOrganizationID(ORGANIZATIONID_FT,NUM_DEPARTMENTS_FT,DEPARTMENT_ID_FT);   
        IssueData.OrgIdentity = OracleOrganizationData.OrgNum;   
        IssueData.WIPEntityName = IP21Msg.CharVals[0];
        IssueData.GetIssueData();
        if (IssueData.dbErrors == FALSE)
        {
          try
          {
            SeqNum = OracleSeqNum.GetSeqNum();
            // InvenLoc.Seg1  = "M";
            InvenLoc.OrgID = OracleOrganizationData.OrgNum;   
            InvenLoc.Seg1.Format("S%d", IP21Msg.IntVals[5]);  // silo ID 
            InvenLoc.SubInvenCode = "MATERIAL";
            InvenLoc.GetInventoryLocationID();
            LocatorID = InvenLoc.InvenLocationID;
            TransLocator = InvenLoc.InvenLocationID;
            strLotNum.Format("BAT%d", IP21Msg.IntVals[1]);            
            strFields =  "TRANSACTION_INTERFACE_ID,LAST_UPDATE_DATE,LAST_UPDATED_BY,CREATION_DATE,";
            strFields += "CREATED_BY,LOT_NUMBER,TRANSACTION_QUANTITY,PRIMARY_QUANTITY";
            strValues.Format("%f,Sysdate,-1,Sysdate,-1,'%s',%f,%f",
                              SeqNum,strLotNum,TransAmt,TransAmt);
            strSQL.Format("INSERT INTO INV.MTL_TRANSACTION_LOTS_INTERFACE (%s) VALUES (%s)", strFields, strValues);
            
            if (db.OpenEx(dbConn))
            {
              db.ExecuteSQL((LPCTSTR) strSQL);
              strFields =  "TRANSACTION_INTERFACE_ID, SOURCE_CODE,SOURCE_LINE_ID, SOURCE_HEADER_ID,PROCESS_FLAG,";
              strFields += "VALIDATION_REQUIRED, TRANSACTION_MODE, TRANSACTION_DATE, LAST_UPDATE_DATE, LAST_UPDATED_BY,";
              strFields += "CREATION_DATE, CREATED_BY, INVENTORY_ITEM_ID, ITEM_SEGMENT1, ORGANIZATION_ID, TRANSACTION_QUANTITY,";
              strFields += "TRANSACTION_UOM, TRANSACTION_TYPE_ID, TRANSACTION_SOURCE_TYPE_ID, TRANSACTION_ACTION_ID,";
              strFields += "TRANSACTION_SOURCE_NAME, TRANSACTION_SOURCE_ID, SUBINVENTORY_CODE, LOCATOR_ID, ";
              strFields += "SCHEDULED_FLAG, WIP_ENTITY_TYPE, OPERATION_SEQ_NUM";
              
              strValues.Format("%f, 'PANCIM', 1, 1, 1, 2, 3, Sysdate, Sysdate, -1, Sysdate, -1, %f, '%s', %f, %f, 'LB', %f, %f, %f, 'WIP', %f, 'MATERIAL', %f, 2, %f, %f",
                SeqNum, IssueData.MaterialID, IssueData.MaterialCode, OracleOrganizationData.OrgNum, TransAmt, OracleTrans.TransTypeID, OracleTrans.TransSourceID,
                OracleTrans.TransActionID, IssueData.WIPEntityID, LocatorID, IssueData.JobType, IssueData.FirstOpSeqNum);
              strSQL.Format("INSERT INTO INV.MTL_TRANSACTIONS_INTERFACE (%s) VALUES (%s)", strFields, strValues);                     
              db.ExecuteSQL((LPCTSTR) strSQL);
              if (db.IsOpen())
                db.Close();
            }  // end db open
          }  // end try  
          catch( CDBException* e )
          {
            LogMsgTxt = "UpdateOracleMDSIssue Exception:";
            LogMsgTxt += (LPCSTR) e->m_strError;
            Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
            e->Delete();
            UpdateStat = 1;
          }  // end catch    
        }  
        else
        {
          LogMsgTxt = "UpdateOracleMDSIssue: errors detected retrieving Oracle data for transaction";
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          UpdateStat = 1;
        } // end issue data errors
      }
      else
      {
        LogMsgTxt = "UpdateOracleMDSIssue: invalid Oracle trans_type_id for transaction";
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        UpdateStat = 1;
      } // trans type id check     
    }
    else
    {
      LogMsgTxt.Format("UpdateOracleMDSIssue: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    } // end IP21 message format
  }
  else
  {
    LogMsgTxt.Format("UpdateOracleMDSIssue: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  } // end IP21 message status
  if (UpdateStat == 0)
  {
    // log transaction info
    LogMsgTxt.Format("MDS Issue... Job:%s  Mtl:%s  Wgt:%0d", 
                      IP21Msg.CharVals[0],MaterialType[IP21Msg.IntVals[0]],IP21Msg.IntVals[4]);
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* ------------------------------------------------------------------------------
    MDS material inventory Oracle update
--------------------------------------------------------------------------------- */  
int UpdateOracleMDSInventory(long rec) 
{
  int UpdateStat        = 0;
  double  SeqNum        = 0,
          FromLocatorID = 0,
          ToLocatorID   = 0,
          TransLocator  = 0,
          TransAmt      = 0;
  CString strSQL,
          OracleMtlCode,
          strBatchNum,
          strBinNum,
          strFields,
          strValues,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  COracleSeqNum OracleSeqNum;
  COracleTransactionInfo OracleTrans;
  CInventoryLocation InvenLoc;
  CDatabase db;
  CDateFormat dt;

  dt.GetOracleTime();
  OracleSeqNum.dbNam = ORACLE_DATABASE;
  OracleSeqNum.dbUsr = ORACLE_USER;
  OracleSeqNum.dbPwd = ORACLE_PASSWORD;
  OracleTrans.dbNam  = ORACLE_DATABASE;
  OracleTrans.dbUsr  = ORACLE_USER;
  OracleTrans.dbPwd  = ORACLE_PASSWORD;
  InvenLoc.dbNam     = ORACLE_DATABASE;
  InvenLoc.dbUsr     = ORACLE_USER;
  InvenLoc.dbPwd     = ORACLE_PASSWORD;

  dbConn = DSN_MTL_INVEN;
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData();
  if (IP21Msg.status == SUCCESS)
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_MSDINVEN_ICOUNT) && (IP21Msg.cCount == MSG_MDSINVEN_CCOUNT) &&
        (IP21Msg.rCount == MSG_MDSINVEN_RCOUNT) && (IP21Msg.tCount == MSG_MDSINVEN_TCOUNT))
    {      
      if (IP21Msg.IntVals[4] < (TotalMaterialTypes+1))
        OracleMtlCode.Format("%s", MaterialType[IP21Msg.IntVals[4]]);
      else
        OracleMtlCode = "?";
      TransAmt = IP21Msg.IntVals[5] * -1; // make negative
      OracleTrans.TransName = "SUBINVENTORY TRANSFER";
      OracleTrans.GetOracleTransactionCodes();    
      if (OracleTrans.TransTypeID > 0)
      {
        OracleOrganizationData.GetOrganizationID(ORGANIZATIONID_FT,NUM_DEPARTMENTS_FT,DEPARTMENT_ID_FT);   
        try
        { 
          SeqNum = OracleSeqNum.GetSeqNum();              // get the next Oracle sequence number
          InvenLoc.OrgID = OracleOrganizationData.OrgNum; // organization ID
          InvenLoc.SubInvenCode = "MATERIAL";             // hard coded sub-inventory code
          
          // get the Oracle FROM locator ID
          InvenLoc.Seg1.Format("S%d",IP21Msg.IntVals[2]); // silo ID         
          InvenLoc.GetInventoryLocationID();              // get the Oracle location ID
          FromLocatorID = InvenLoc.InvenLocationID;       // Oracle ID for silo location 
        
          // determine bin prefix based on silo number
          
          if (IP21Msg.IntVals[2] > 4)                        // black material
            InvenLoc.Seg1 = "BB";                            // black bin location
          else                                               // natural material
            InvenLoc.Seg1 = "NB";                            // natural bin location
            
          InvenLoc.GetInventoryLocationID();                 // get the Oracle location ID
          ToLocatorID = InvenLoc.InvenLocationID;            // Oracle ID for bin location
          strBatchNum.Format("BAT%d", IP21Msg.IntVals[0]);   // batch number
          strFields =  "TRANSACTION_INTERFACE_ID, LAST_UPDATE_DATE, LAST_UPDATED_BY, CREATION_DATE,";
          strFields += "CREATED_BY, LOT_NUMBER, TRANSACTION_QUANTITY, PRIMARY_QUANTITY";
          strValues.Format("%f,Sysdate,-1,Sysdate,-1,'%s',%f,%f", SeqNum,strBatchNum,TransAmt,TransAmt);
          strSQL.Format("INSERT INTO INV.MTL_TRANSACTION_LOTS_INTERFACE (%s) VALUES (%s)", strFields, strValues);
          if (db.OpenEx(dbConn))
          {                           
            db.ExecuteSQL((LPCTSTR) strSQL);
            strFields =  "TRANSACTION_INTERFACE_ID, SOURCE_CODE, SOURCE_LINE_ID, SOURCE_HEADER_ID, PROCESS_FLAG,";
            strFields += "VALIDATION_REQUIRED, TRANSACTION_MODE, TRANSACTION_DATE, LAST_UPDATE_DATE, LAST_UPDATED_BY,";
            strFields += "CREATION_DATE, CREATED_BY, ITEM_SEGMENT1, ORGANIZATION_ID, TRANSACTION_QUANTITY,";
            strFields += "TRANSACTION_UOM, TRANSACTION_TYPE_ID, TRANSACTION_SOURCE_TYPE_ID, TRANSACTION_ACTION_ID,";
            strFields += "SUBINVENTORY_CODE, LOCATOR_ID, TRANSFER_ORGANIZATION, TRANSFER_SUBINVENTORY, TRANSFER_LOCATOR, SCHEDULED_FLAG";           
            strValues.Format("%f, 'PANCIM', 1, 1, 1, 2, 3, Sysdate, Sysdate, -1,Sysdate, -1, '%s', %f, %f, 'LB', %f, %f, %f, 'MATERIAL', %f, %f, 'MATERIAL', %f, 2",
              SeqNum, OracleMtlCode, OracleOrganizationData.OrgNum, TransAmt, OracleTrans.TransTypeID, OracleTrans.TransSourceID,
              OracleTrans.TransActionID, FromLocatorID, OracleOrganizationData.OrgNum, ToLocatorID);

            strSQL.Format("INSERT INTO INV.MTL_TRANSACTIONS_INTERFACE (%s) VALUES (%s)", strFields, strValues);            
            db.ExecuteSQL((LPCTSTR) strSQL);
            if (db.IsOpen())
              db.Close();
          }
          else
          {
            LogMsgTxt.Format("UpdateOracleMDSInven: errors detected retrieving Oracle data for transaction", rec);
            Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
            UpdateStat = 1;
          }
        }  
        catch( CDBException* e )
        {
          LogMsgTxt = "UpdateOracleMDSInven Exception:";
          LogMsgTxt += (LPCSTR) e->m_strError;
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          e->Delete();
          UpdateStat = 1;
        } 
      }
      else
      {
        LogMsgTxt.Format("UpdateOracleMDSInven: invalid Oracle trans_type_id for transaction", rec);
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        UpdateStat = 1;
      }
    }  
    else
    {
      LogMsgTxt.Format("UpdateOracleMDSInven: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }        
  else
  {
    LogMsgTxt.Format("UpdateOracleMDSInven: PanCIMMessage (ID= %d) read error", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;  // reset the IP21 error code
    UpdateStat = 1;
  }  
  if (UpdateStat == 0)
  {
    // log transaction info
    LogMsgTxt.Format("MDS Inven Trans... Batch:%d  Container:%d  Wgt:%0d", 
                      IP21Msg.IntVals[0],IP21Msg.IntVals[1],IP21Msg.IntVals[5]);
    Log.LogMessage(0, (LPCTSTR) LogMsgTxt);
  }
  return UpdateStat;
}
/* -----------------------------------------------------------------------------
    Update production control order done quantity and add record into ProdOrders
      [Database: ProdSched  Tables: WOPerfData, PCOMaster, ProdOrders, PCOItem]
   ----------------------------------------------------------------------------- */
int UpdateProdCtrlOrder(long rec) 
{
  int     idx        = 0,
          stat       = 0,
          BufPos     = 0,
          UpdateErr  = 0,
          UpdateStat = 0;
  long    PCOkey     = 0,
          PCOid      = 0,
          DoneQty    = 0;
  short   MsgType    = 0;
  CTime   tsBuffer;
  CString WorkorderBuf,
          MoldNameBuf,
          PartNameBuf,
          PackCodeBuf,
          tmpBuf,     
          strSQL,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;
  CDatabase db;

  dbConn = DSN_PRODSCHED;
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // Read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateProdCtrlOrder: error reading PCMIfaceMsg record %d (code:%d)", rec, IP21Msg.status);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    return(IP21Msg.status);
  }
  else
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_PRODCTRL_ICOUNT) && (IP21Msg.cCount == MSG_PRODCTRL_CCOUNT) &&
        (IP21Msg.rCount == MSG_PRODCTRL_RCOUNT) && (IP21Msg.tCount == MSG_PRODCTRL_TCOUNT))
    {
      tsBuffer = CTime::GetCurrentTime();
      WorkorderBuf.Empty();
      MoldNameBuf.Empty();
      PartNameBuf.Empty();
      PackCodeBuf.Empty();
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if ((isprint(IP21Msg.CharVals[0][BufPos]) != 0) && (IP21Msg.CharVals[0][BufPos] != ' '))
        {
          WorkorderBuf += IP21Msg.CharVals[0][idx];
          idx++;
        }
      }
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if ((isprint(IP21Msg.CharVals[3][BufPos]) != 0) && (IP21Msg.CharVals[3][BufPos] != ' '))
        {
          MoldNameBuf += IP21Msg.CharVals[3][idx];
          idx++;
        }
      }
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if ((isprint(IP21Msg.CharVals[1][BufPos]) != 0) && (IP21Msg.CharVals[1][BufPos] != ' '))
        {
          PartNameBuf += IP21Msg.CharVals[1][idx];
          idx++;
        }
      }
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if ((isprint(IP21Msg.CharVals[2][BufPos]) != 0) && (IP21Msg.CharVals[2][BufPos] != ' '))
        {
          PackCodeBuf += IP21Msg.CharVals[2][idx];
          idx++;
        }
      }
      dbConn = DSN_PRODSCHED;            // Connect to the ProdSched database
      CPCOitems rsPCOitems(&db);
      CProdOrders rsProdOrder(&db);    
      try
      {
        if (db.OpenEx(dbConn,CDatabase::useCursorLib))
        {
          strSQL.Empty();
          LogMsgTxt.Empty();
          tmpBuf.Empty();
          PCOid   = 0;
          PCOkey  = 0;
          DoneQty = 0;          
          strSQL.Format("SELECT PCOmaster.PCOnum, PCOitem.PCOref, PCOitem.PCOkey, PCOitem.QtyDone FROM PCOmaster INNER JOIN PCOitem ON PCOmaster.PCOnum=PCOitem.PCOref WHERE ((PCOmaster.MoldName) = '%s') AND ((PCOitem.PackCode) = '%s') AND ((PCOmaster.Closed)=0) AND ((PCOitem.Complete)=0)",
                         MoldNameBuf,PackCodeBuf);
          rsPCOitems.Open(CRecordset::snapshot, strSQL );
          if ((!rsPCOitems.IsBOF()) && (!rsPCOitems.IsEOF()))
          {
            // We found a matching PCO record 
            PCOkey  = rsPCOitems.m_PCOkey;  // save the pco key value for the update
            PCOid   = rsPCOitems.m_PCOnum;  // save the pco id value for the ProdOrders record   
            DoneQty = rsPCOitems.m_QtyDone + IP21Msg.IntVals[4];
            if (PCOkey)
            {
              CPCO_UpdateQty rsPCO_UpdateQty(&db);
              rsPCO_UpdateQty.m_strFilter = "PCOkey= ";
              tmpBuf.Format("%d", PCOkey);
              rsPCO_UpdateQty.m_strFilter += tmpBuf;
              rsPCO_UpdateQty.Open(CRecordset::snapshot);
              if ((!rsPCO_UpdateQty.IsBOF()) && (!rsPCO_UpdateQty.IsEOF()))
              {
                // Update the done quantity in the PCO item table
                rsPCO_UpdateQty.Edit();
                rsPCO_UpdateQty.m_QtyDone = DoneQty;
                if ((!rsPCO_UpdateQty.Update()) && (DoneQty > 0))
                {
                  UpdateErr = 2;  // PCOItem quantity update failure
                }
              }
              if (rsPCO_UpdateQty.IsOpen())
              rsPCO_UpdateQty.Close();     
            }
          }
          else
          {
            // no PCO record found
            PCOid   = 0;  
          }
          // Add the record to the ProdOrders table
          if (rsProdOrder.Open(CRecordset::snapshot))
          {
            rsProdOrder.AddNew();
            rsProdOrder.m_PCOnum       = PCOid;
            rsProdOrder.m_MachNum      = IP21Msg.IntVals[11];
            rsProdOrder.m_Workorder    = (LPCTSTR) WorkorderBuf;
            rsProdOrder.m_PartName     = (LPCTSTR) PartNameBuf;
            rsProdOrder.m_MoldName     = (LPCTSTR) MoldNameBuf;
            rsProdOrder.m_PackCode     = (LPCTSTR) PackCodeBuf;
            rsProdOrder.m_ProdQty      = IP21Msg.IntVals[4];
            rsProdOrder.m_ScrapQty     = IP21Msg.IntVals[6];
            rsProdOrder.m_EstQty       = IP21Msg.IntVals[5];
            rsProdOrder.m_RunHrs       = IP21Msg.IntVals[9] * .01;
            rsProdOrder.m_CycleCount   = IP21Msg.IntVals[8];
            rsProdOrder.m_EmpNum       = IP21Msg.IntVals[12];
            rsProdOrder.m_StandardRate = IP21Msg.RealVals[0];
            rsProdOrder.m_UpdateTime   = tsBuffer;
            rsProdOrder.m_ShiftNum     = 0; // no shift number available
            if (!rsProdOrder.Update())
            {
              UpdateErr = 3;  // ProdOrders table update failure
            }
          }
        }
        else
        {
          UpdateErr = 1;  //  WCProdSched database open failed
        }
      }
      catch( CDBException* e )
      {
        UpdateErr = 4;  // undetermined exception code
        LogMsgTxt += " ** ";
        LogMsgTxt += (LPCSTR) e->m_strError;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
        e->Delete();
        UpdateErr = 1;
        stat = 1;
      }
      if (UpdateErr)
      {
        CString dbErrorMsg;
    
        switch(UpdateErr)
        {
          case 1:
            dbErrorMsg = "UpdateProdCtrlOrder:WCProdSched database open failed";
            break;
          case 2:
            dbErrorMsg = "UpdateProdCtrlOrder:PCOItem quantity update failure";
            break;
          case 3:
            dbErrorMsg = "UpdateProdCtrlOrder:ProdOrders table update failure";
            break;
          case 4:
            dbErrorMsg = "UpdateProdCtrlOrder:Undetermined exception code";
            break;
          default:
            dbErrorMsg = "UpdateProdCtrlOrder:Undefined UpdateErr code";
            break;
        }     
        LogMsgTxt += dbErrorMsg;
        Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      }
      if (rsPCOitems.IsOpen())
        rsPCOitems.Close();     
      if (rsProdOrder.IsOpen())
        rsProdOrder.Close();
      if (db.IsOpen())
        db.Close();
      return UpdateErr;
    }
    else
    {
      LogMsgTxt.Format("UpdateProdCtrlOrder: invalid PanCIMMessage (ID=%d) format", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }  
  }
  return UpdateStat;
} 
/* --------------------------------------------------------------------------
    Update molding job performance data
      Database: ProductionHist  Table: JobPerformance
   -------------------------------------------------------------------------- */
int UpdateJobPerformance(long rec) 
{
  int   UpdateStat = 0;
  short MsgType    = 0;
  long  RefID      = 0;
  CDatabase db;
  CString WoNumBuf,
          IntDurBuf,
          strSQL,
          LogMsgTxt,
          strFields,
          strValues,
          dbConn;
  CPanCIMMessage IP21Msg;
  CTextFilter TxtFilter;

  WoNumBuf.Empty();
  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {   
    LogMsgTxt.Format("UpdateJobPerformance: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    return(IP21Msg.status);
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_JOBPERF_ICOUNT) && (IP21Msg.cCount == MSG_JOBPERF_CCOUNT) &&
      (IP21Msg.rCount == MSG_JOBPERF_RCOUNT) && (IP21Msg.tCount == MSG_JOBPERF_TCOUNT))
  {    
    TxtFilter.MaxLen= WONUMLEN;
    TxtFilter.Strip(IP21Msg.CharVals[0]);
    WoNumBuf = TxtFilter.TextOut;
    
    TxtFilter.MaxLen= STRTIMELEN;
    TxtFilter.Strip(IP21Msg.CharVals[1]);
    IntDurBuf = TxtFilter.TextOut;
  
    if (WoNumBuf.IsEmpty())
      WoNumBuf = "????????";
  
    if (IntDurBuf.IsEmpty())
      IntDurBuf = "??????.??.??";
  
    dbConn = DSN_PRODJOBHIST;
    JobPerformance rsJobPerf(&db);
 
    try
    {
      if (db.OpenEx(dbConn))
      {           
        strSQL.Format("SELECT RefID FROM JobPerformance WHERE JobID = '%s'", WoNumBuf);
        rsJobPerf.Open(CRecordset::snapshot, strSQL);    
        if (rsJobPerf.m_RefID)
          RefID = rsJobPerf.m_RefID;  // save the reference ID    
        
        if (rsJobPerf.IsOpen())
          rsJobPerf.Close();
        
        if (RefID)  // record already exists - just update
        {
          strSQL.Format("UPDATE JobPerformance SET ProductionTime=%f,DesignCycleTime=%f,ActualCycleTime=%f,InterruptCount=%d,InterruptDuration='%s',SetPointChanges=%d,PurgeWeight=%f,PurgeDistance=%f,PurgeTime=%f,EventMessages=%d,EventWarnings=%d,EventFaults=%d WHERE RefID=%d",  
            IP21Msg.RealVals[0],IP21Msg.RealVals[1],IP21Msg.RealVals[2],IP21Msg.IntVals[1],IntDurBuf,IP21Msg.IntVals[2],IP21Msg.RealVals[3],IP21Msg.RealVals[4],
            IP21Msg.RealVals[5],IP21Msg.IntVals[3],IP21Msg.IntVals[4],IP21Msg.IntVals[5],RefID);
        }
        else  // add a new record
        {
          strFields = "Machine,JobID,ProductionTime,DesignCycleTime,ActualCycleTime,InterruptCount,";
          strFields += "InterruptDuration,SetPointChanges,PurgeWeight,PurgeDistance,PurgeTime,";
          strFields += "EventMessages,EventWarnings,EventFaults";
          strValues.Format("%d,'%s',%f,%f,%f,%d,'%s',%d,%f,%f,%f,%d,%d,%d",
          IP21Msg.IntVals[0],WoNumBuf,IP21Msg.RealVals[0],IP21Msg.RealVals[1],IP21Msg.RealVals[2],
          IP21Msg.IntVals[1],IntDurBuf,IP21Msg.IntVals[2],IP21Msg.RealVals[3],IP21Msg.RealVals[4],
          IP21Msg.RealVals[5],IP21Msg.IntVals[3],IP21Msg.IntVals[4],IP21Msg.IntVals[5]);  
          strSQL.Format("INSERT INTO JobPerformance (%s) VALUES (%s)",strFields, strValues);     
        }                    
        db.ExecuteSQL((LPCTSTR)strSQL);       
        if (db.IsOpen())
          db.Close();
      }            
    }        
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("Machine %d job performance data Update errors detected", IP21Msg.IntVals[0]);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt); 
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Machine %d job performance data Updated successfully", IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt); 
    }
  }  
  else
  {
    LogMsgTxt.Format("UpdateJobPerformance: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update system comments data
      [Database: EquipmentPerf  Table: SystemComments]
   -------------------------------------------------------------------------- */
int UpdateSysComment(long rec) 
{
  SYSTEMTIME systime;
  int   BufPos     = 0,
        idx        = 0,
        UpdateStat = 0;
  short MsgType    = 0;
  CDatabase db;
  CString WoNumBuf,
          SysCommentBuf,
          CommentLine1Buf,
          CommentLine2Buf,
          CommentLine3Buf,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;

  LogMsgTxt.Empty();
  SysCommentBuf.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateSysComment: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_SYSCOMMENT_ICOUNT) && (IP21Msg.cCount == MSG_SYSCOMMENT_CCOUNT) &&
      (IP21Msg.rCount == MSG_SYSCOMMENT_RCOUNT) && (IP21Msg.tCount == MSG_SYSCOMMENT_TCOUNT))
  {
    WoNumBuf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if ((isprint(IP21Msg.CharVals[0][BufPos]) != 0) && (IP21Msg.CharVals[0][BufPos] != ' '))
      {
        WoNumBuf += IP21Msg.CharVals[0][idx];
        idx++;
      }
    }
    CommentLine1Buf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[1][BufPos]) != 0)
      {
        CommentLine1Buf += IP21Msg.CharVals[1][idx];
        idx++;
      }
    }
    CommentLine2Buf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[2][BufPos]) != 0)
      {
        CommentLine2Buf += IP21Msg.CharVals[2][idx];
        idx++;
      }
    }
    CommentLine3Buf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[3][BufPos]) != 0)
      {
        CommentLine3Buf += IP21Msg.CharVals[3][idx];
        idx++;
      }
    }
    // Remove excess spaces and concatenant strings
    CommentLine1Buf.TrimRight();
    CommentLine2Buf.TrimRight();
    CommentLine3Buf.TrimRight();
    if (CommentLine1Buf.GetLength() < LEN_CHAR_OCCS)
      CommentLine1Buf += " ";
    if (CommentLine2Buf.GetLength() < LEN_CHAR_OCCS)
      CommentLine2Buf += " ";
    SysCommentBuf = CommentLine1Buf + CommentLine2Buf + CommentLine3Buf;
    if (WoNumBuf.IsEmpty())
      WoNumBuf = "????????";
    if (SysCommentBuf.IsEmpty())
      SysCommentBuf = " ";
    dbConn = DSN_EQPTPERFHIST;
    CSystemComment rst(&db);
    try
    {
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        if (rst.Open(CRecordset::snapshot))
        {
          GetLocalTime(&systime);
          rst.AddNew();
          rst.m_LogTime   = CTime(systime, -1); 
          rst.m_Workorder = (LPCTSTR) WoNumBuf;
          rst.m_Comment   = (LPCTSTR) SysCommentBuf;
          rst.m_Machine   = IP21Msg.IntVals[0];
          rst.m_MsgCode   = IP21Msg.IntVals[1];
          rst.m_Employee  = IP21Msg.IntVals[2];
          if (!rst.Update())
          {
            UpdateStat = 1;
          }
        }
        else
        {
          UpdateStat = 1;
        }
      }
    }
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("UdateSystemComment: Machine %d system comment Update error(s) detected",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Machine %d system comment data Updated successfully", IP21Msg.IntVals[0]);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt); 
    }    
    if (rst.IsOpen())
      rst.Close();
    if (db.IsOpen())
      db.Close();
  }
  else
  {
    LogMsgTxt.Format("UpdateSysComment: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update bagger performance data
      [Database: EquipmentPerf Table: BaggerPerf]
   -------------------------------------------------------------------------- */
int UpdateBaggerPerf(long rec) 
{
  SYSTEMTIME systime;
  int   BufPos     = 0,
        idx        = 0,
        UpdateStat = 0;
  short MsgType    = 0;
  CDatabase db;
  CString WoNumBuf,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;

  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateBaggerPerf: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_BAGGERPERF_ICOUNT) && (IP21Msg.cCount == MSG_BAGGERPERF_CCOUNT) &&
      (IP21Msg.rCount == MSG_BAGGERPERF_RCOUNT) && (IP21Msg.tCount == MSG_BAGGERPERF_TCOUNT))
  {
    WoNumBuf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if ((isprint(IP21Msg.CharVals[0][BufPos]) != 0) && (IP21Msg.CharVals[0][BufPos] != ' '))
      {
        WoNumBuf += IP21Msg.CharVals[0][idx];
        idx++;
      }
    }
    if (WoNumBuf.IsEmpty())
      WoNumBuf = "????????";
    dbConn = DSN_EQPTPERFHIST;
    CBaggerPerf rst(&db);
    try
    {
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        if (rst.Open(CRecordset::snapshot))
        {
          GetLocalTime(&systime);
          rst.AddNew();
          rst.m_LogTime      = CTime(systime, -1); 
          rst.m_Workorder    = (LPCTSTR) WoNumBuf;
          rst.m_EqptNum      = IP21Msg.IntVals[0];
          rst.m_BagCount     = IP21Msg.IntVals[1];
          rst.m_DivertCount  = IP21Msg.IntVals[2];
          rst.m_MessageCount = IP21Msg.IntVals[3];
          rst.m_WarningCount = IP21Msg.IntVals[4];
          rst.m_FaultCount   = IP21Msg.IntVals[5];
          if (!rst.Update())
            UpdateStat = 1;
        }
        else
        {
          UpdateStat = 1;
        }
      }
    }
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("UpdateBaggerPerf: Bagger %d performance data update error(s) detected",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Bagger %d performance data updated successfully",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    if (rst.IsOpen())
      rst.Close();
    if (db.IsOpen())
      db.Close();
  }
  else
  {
    LogMsgTxt.Format("UpdateBaggerPerf: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update inserter performance data
      [Database: EquipmentPerf Table: InserterPerf]
   -------------------------------------------------------------------------- */
int UpdateInserterPerf(long rec) 
{
  SYSTEMTIME systime;
  int   BufPos    = 0,
        idx       = 0,
       UpdateStat = 0;
  short MsgType   = 0;
  CDatabase db;
  CString WoNumBuf,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;

  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateInserterPerf: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_INSERTERPERF_ICOUNT) && (IP21Msg.cCount == MSG_INSERTERPERF_CCOUNT) &&
      (IP21Msg.rCount == MSG_INSERTERPERF_RCOUNT) && (IP21Msg.tCount == MSG_INSERTERPERF_TCOUNT))
  {
    WoNumBuf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if ((isprint(IP21Msg.CharVals[0][BufPos]) != 0) && (IP21Msg.CharVals[0][BufPos] != ' '))
      {
        WoNumBuf += IP21Msg.CharVals[0][idx];
        idx++;
      }
    }
    if (WoNumBuf.IsEmpty())
      WoNumBuf = "????????";
    dbConn = DSN_EQPTPERFHIST;
    CInserterPerf rst(&db);
    try
    {
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        if (rst.Open(CRecordset::snapshot))
        {
          GetLocalTime(&systime);
          rst.AddNew();
          rst.m_LogTime      = CTime(systime, -1); 
          rst.m_Workorder    = (LPCTSTR) WoNumBuf;
          rst.m_EqptNum      = IP21Msg.IntVals[0];
          rst.m_CycleCount   = IP21Msg.IntVals[1];
          rst.m_MessageCount = IP21Msg.IntVals[2];
          rst.m_WarningCount = IP21Msg.IntVals[3];
          rst.m_FaultCount   = IP21Msg.IntVals[4];
          if (!rst.Update())
            UpdateStat = 1;
        }
        else
        {
          UpdateStat = 1;
        }
      }
    }
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("UpdateInserterPerf: Inserter %d performance data update error(s) detected",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Inserter %d performance data updated successfully",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    if (rst.IsOpen())
      rst.Close();
    if (db.IsOpen())
      db.Close();
  }
  else
  {
    LogMsgTxt.Format("UpdateInserterPerf: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update loader performance data
      [Database: EquipmentPerf Table: LoaderPerf]
   -------------------------------------------------------------------------- */
int UpdateLoaderPerf(long rec) 
{
  SYSTEMTIME systime;
  int  BufPos     = 0,
       idx,
       UpdateStat = 0;
  short MsgType   = 0;
  CDatabase db;
  CString WoNumBuf,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;

  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateLoaderPerf: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_LOADERPERF_ICOUNT) && (IP21Msg.cCount == MSG_LOADERPERF_CCOUNT) &&
      (IP21Msg.rCount == MSG_LOADERPERF_RCOUNT) && (IP21Msg.tCount == MSG_LOADERPERF_TCOUNT))
  {
    WoNumBuf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if ((isprint(IP21Msg.CharVals[0][BufPos]) != 0) && (IP21Msg.CharVals[0][BufPos] != ' '))
      {
        WoNumBuf += IP21Msg.CharVals[0][idx];
        idx++;
      }
    }
    if (WoNumBuf.IsEmpty())
      WoNumBuf = "????????";
    dbConn = DSN_EQPTPERFHIST;
    CLoaderPerf rst(&db);
    try
    {
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        if (rst.Open(CRecordset::snapshot))
        {
          GetLocalTime(&systime);
          rst.AddNew();
          rst.m_LogTime      = CTime(systime, -1); 
          rst.m_Workorder    = (LPCTSTR) WoNumBuf;
          rst.m_EqptNum      = IP21Msg.IntVals[0];
          rst.m_CycleCount   = IP21Msg.IntVals[1];
          rst.m_MessageCount = IP21Msg.IntVals[2];
          rst.m_WarningCount = IP21Msg.IntVals[3];
          rst.m_FaultCount   = IP21Msg.IntVals[4];
          if (!rst.Update())
            UpdateStat = 1;
        }
        else
        {
          UpdateStat = 1;
        }
      }
    }
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("UpdateLoaderrPerf: Loader %d performance data update error(s) detected",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Loader %d performance data updated successfully",IP21Msg.IntVals[0]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    if (rst.IsOpen())
      rst.Close();
    if (db.IsOpen())
      db.Close();
  }
  else
  {
    LogMsgTxt.Format("UpdateLoaderPerf: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update mold repair data
      [Database: EquipmentRepair Table: MoldRepairHist]
   -------------------------------------------------------------------------- */
int UpdateMoldRepair(long rec) 
{
  int   BufPos     = 0,
        idx        = 0,
        UpdateStat = 0;
  short MsgType    = 0;
  CDatabase db;
  CString MoldNameBuf,
          RepairDescriptionBuf,
          RepairCommentsBuf,
          LogMsgTxt,
          TrackIDBuf,
          DescriptionLine1,
          DescriptionLine2,
          CommentLine1,
          CommentLine2,
          dbConn;
  CPanCIMMessage IP21Msg;
  CTextFilter TxtFilter;

  MoldNameBuf.Empty();
  RepairDescriptionBuf.Empty();
  RepairCommentsBuf.Empty();
  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  
  IP21Msg.GetMsgData(); // read the IP21 PanCIMMsgDef record  
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateMoldRepair: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  try
  {
    // Make sure we have correct number of values required for message type
    if ((IP21Msg.iCount == MSG_MOLDREPAIR_ICOUNT) && (IP21Msg.cCount == MSG_MOLDREPAIR_CCOUNT) &&
        (IP21Msg.rCount == MSG_MOLDREPAIR_RCOUNT) && (IP21Msg.tCount == MSG_MOLDREPAIR_TCOUNT))
    {        
      TxtFilter.MaxLen= LEN_CHAR_OCCS; 
      TxtFilter.Strip(IP21Msg.CharVals[0]);
      TrackIDBuf = TxtFilter.TextOut;
      TxtFilter.Strip(IP21Msg.CharVals[1]);
      MoldNameBuf = TxtFilter.TextOut;
      DescriptionLine1.Empty();
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if (isprint(IP21Msg.CharVals[2][BufPos]) != 0)
        {
          DescriptionLine1 += IP21Msg.CharVals[2][idx];
          idx++;
        }
      }
      DescriptionLine2.Empty();
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if (isprint(IP21Msg.CharVals[3][BufPos]) != 0)
        {
          DescriptionLine2 += IP21Msg.CharVals[3][idx];
          idx++;
        }
      }
      CommentLine1.Empty();
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if (isprint(IP21Msg.CharVals[4][BufPos]) != 0)
        {
          CommentLine1 += IP21Msg.CharVals[4][idx];
          idx++;
        }
      }
      CommentLine2.Empty();
      idx = 0;
      for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
      {
        if (isprint(IP21Msg.CharVals[5][BufPos]) != 0)
        {
          CommentLine2 += IP21Msg.CharVals[5][idx];
          idx++;
        }
      }
      // cleanup the character strings
      TrackIDBuf.TrimRight();
    
      // remove extra characters that may be included 
      // due to different buffer lengths during copy
      // NOTE: the maximum length for the mold name is 16
      MoldNameBuf.Delete(15,4);
      MoldNameBuf.TrimRight();
      DescriptionLine1.TrimRight();
      DescriptionLine2.TrimRight();
      CommentLine1.TrimRight();
      CommentLine2.TrimRight();
      if (DescriptionLine1.GetLength() < LEN_CHAR_OCCS)
        DescriptionLine1 += " ";
      if (CommentLine1.GetLength() < LEN_CHAR_OCCS)
        CommentLine1 += " ";
      RepairDescriptionBuf = DescriptionLine1 + DescriptionLine2;
      RepairCommentsBuf = CommentLine1 + CommentLine2;
      if (TrackIDBuf.IsEmpty())
        TrackIDBuf = "????????";
      if (MoldNameBuf.IsEmpty())
        MoldNameBuf = "????????";
      if (RepairDescriptionBuf.IsEmpty())
        RepairDescriptionBuf = " ";
      if (RepairCommentsBuf.IsEmpty())
        RepairCommentsBuf = " ";
      dbConn = DSN_EQPTREPAIRHIST;
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        CMoldRepair rst(&db);
        if (rst.Open(CRecordset::snapshot))
        {          
          rst.AddNew();
          rst.m_TrackID           = TrackIDBuf;
          rst.m_MoldName          = MoldNameBuf;
          rst.m_RepairDescription = RepairDescriptionBuf;
          rst.m_RepairComments    = RepairCommentsBuf;
          rst.m_ShopOrder         = IP21Msg.IntVals[0];
          rst.m_Machine           = IP21Msg.IntVals[1];
          rst.m_CycleCount        = IP21Msg.IntVals[2];
          rst.m_RepairItem1       = IP21Msg.IntVals[3];
          rst.m_RepairItem2       = IP21Msg.IntVals[4];
          rst.m_RepairItem3       = IP21Msg.IntVals[5];
          rst.m_RepeatRepair      = IP21Msg.IntVals[6];
          rst.m_Assigned1         = IP21Msg.IntVals[7];
          rst.m_Assigned2         = IP21Msg.IntVals[8];
          rst.m_Assigned3         = IP21Msg.IntVals[9];
          rst.m_RepairAction1     = IP21Msg.IntVals[10];
          rst.m_RepairAction2     = IP21Msg.IntVals[11];
          rst.m_RepairAction3     = IP21Msg.IntVals[12];
          rst.m_RepairTime        = IP21Msg.RealVals[0];
          rst.m_ToolroomTime      = IP21Msg.RealVals[1];
          rst.m_TimeIn            = IP21Msg.TimeStamp[0];
          if (!rst.Update())
          {
            UpdateStat = 1;
          }
        }
        else
        {
          UpdateStat = 1;
        }
        if (rst.IsOpen())
          rst.Close();
      }
      if (UpdateStat)
      {
        MsgType = 1;
        LogMsgTxt.Format("UpdateMoldRepair: %s repair data update error(s) detected",MoldNameBuf);
        Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
      }
      else
      {
        MsgType = 0;
        LogMsgTxt.Format("%s repair data updated successfully",MoldNameBuf);
        Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
      }
      if (db.IsOpen())
        db.Close();
    }
    else
    {
      LogMsgTxt.Format("UpdateMoldRepair: incorrect message record format (ID=%d)", rec);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      UpdateStat = 1;
    }
  }  
  catch( CDBException* e )
  {
    UpdateStat = 1;
    LogMsgTxt += " ** ";
    LogMsgTxt += (LPCSTR) e->m_strError;
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
  }
  return UpdateStat;
}
/* --------------------------------------------------------------------------
    Update bagger repair data
      [Database: EquipmentRepair Table: BaggerRepairHist]
   -------------------------------------------------------------------------- */
int UpdateBaggerRepair(long rec) 
{
  int   BufPos     = 0,
        idx        = 0,
        UpdateStat = 0;
  short MsgType    = 0;
  CDatabase db;
  CString EquipmentNameBuf,
          RepairDescriptionBuf,
          RepairCommentsBuf,
          RepairDescriptionLine1,
          RepairDescriptionLine2,
          RepairCommentsLine1,
          RepairCommentsLine2,
          LogMsgTxt,
          dbConn;
  CPanCIMMessage IP21Msg;

  LogMsgTxt.Empty();
  IP21Msg.RecID = rec;
  IP21Msg.TransCodeFT = TRANSCODE_FT;
  IP21Msg.SourceFT = SOURCE_FT;
  IP21Msg.IntegerCountFT = INTEGERCOUNT_FT;
  IP21Msg.CharacterCountFT = CHARCOUNT_FT;
  IP21Msg.RealCountFT = REALCOUNT_FT;
  IP21Msg.TimeStampCountFT = TIMESTAMPCOUNT_FT;
  IP21Msg.IntFieldFT = INT_FIELD_FT;
  IP21Msg.CharFieldFT = CHAR_FIELD_FT;
  IP21Msg.RealFieldFT = REAL_FIELD_FT;
  IP21Msg.TimeFieldFT = TIME_FIELD_FT;
  IP21Msg.GetMsgData(); // read the IP21 PCMIfaceMsgDef record
  if (IP21Msg.status != SUCCESS)
  {
    LogMsgTxt.Format("UpdateBaggerRepair: invalid PanCIMMessage (ID=%d) format", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
    return UpdateStat;
  }
  // Make sure we have correct number of values required for message type
  if ((IP21Msg.iCount == MSG_BAGGERREPAIR_ICOUNT) && (IP21Msg.cCount == MSG_BAGGERREPAIR_CCOUNT) &&
      (IP21Msg.rCount == MSG_BAGGERREPAIR_RCOUNT) && (IP21Msg.tCount == MSG_BAGGERREPAIR_TCOUNT))
  {
    EquipmentNameBuf.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[0][BufPos]) != 0)
      {
        EquipmentNameBuf += IP21Msg.CharVals[0][idx];
        idx++;
      }
    }
    RepairDescriptionLine1.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[1][BufPos]) != 0)
      {
        RepairDescriptionLine1 += IP21Msg.CharVals[1][idx];
        idx++;
      }
    }
    RepairDescriptionLine2.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[2][BufPos]) != 0)
      {
        RepairDescriptionLine2 += IP21Msg.CharVals[2][idx];
        idx++;
      }
    }
    RepairCommentsLine1.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[3][BufPos]) != 0)
      {
        RepairCommentsLine1 += IP21Msg.CharVals[3][idx];
        idx++;
      }
    }
    RepairCommentsLine2.Empty();
    idx = 0;
    for (BufPos=0; BufPos<LEN_CHAR_OCCS; BufPos++)
    {
      if (isprint(IP21Msg.CharVals[4][BufPos]) != 0)
      {
        RepairCommentsLine2 += IP21Msg.CharVals[4][idx];
        idx++;
      }
    }
    // cleanup strings
    EquipmentNameBuf.TrimRight();
    RepairCommentsLine1.TrimRight();
    RepairCommentsLine2.TrimRight();
    RepairDescriptionLine1.TrimRight();
    RepairDescriptionLine2.TrimRight();
    if (RepairCommentsLine1.GetLength() < LEN_CHAR_OCCS)
      RepairCommentsLine1 += " ";
    if (RepairDescriptionLine1.GetLength() < LEN_CHAR_OCCS)
      RepairDescriptionLine1 += " ";
    RepairCommentsBuf = RepairCommentsLine1 + RepairCommentsLine2;
    RepairDescriptionBuf = RepairDescriptionLine1 + RepairDescriptionLine2;
    if (RepairDescriptionBuf.IsEmpty())
      RepairDescriptionBuf = " ";
    if (RepairCommentsBuf.IsEmpty())
      RepairCommentsBuf = " ";
    dbConn = DSN_EQPTREPAIRHIST;
    CBaggerRepair rst(&db);
    try
    {
      if (db.OpenEx(dbConn,CDatabase::useCursorLib))
      {
        if (rst.Open(CRecordset::snapshot))
        {
          rst.AddNew();
          rst.m_EqptName           = (LPCTSTR) EquipmentNameBuf;
          rst.m_RepairDescription  = (LPCTSTR) RepairDescriptionBuf;
          rst.m_RepairComments     = (LPCTSTR) RepairCommentsBuf;
          rst.m_ShopOrder          = IP21Msg.IntVals[0];
          rst.m_EqptID             = IP21Msg.IntVals[1];
          rst.m_TotalCycles        = IP21Msg.IntVals[2];
          rst.m_RepairCycles       = IP21Msg.IntVals[3];
          rst.m_RepairItem1        = IP21Msg.IntVals[4];
          rst.m_RepairItem2        = IP21Msg.IntVals[5];
          rst.m_RepairItem3        = IP21Msg.IntVals[6];
          rst.m_RepairItem4        = IP21Msg.IntVals[7];
          rst.m_RepairItem5        = IP21Msg.IntVals[8];
          rst.m_RepeatRepair       = IP21Msg.IntVals[9];
          rst.m_Assigned1          = IP21Msg.IntVals[10];
          rst.m_Assigned2          = IP21Msg.IntVals[11];
          rst.m_Assigned3          = IP21Msg.IntVals[12];
          rst.m_RepairAction1      = IP21Msg.IntVals[13];
          rst.m_RepairAction2      = IP21Msg.IntVals[14];
          rst.m_RepairAction3      = IP21Msg.IntVals[15];
          rst.m_RepairAction4      = IP21Msg.IntVals[16];
          rst.m_RepairAction5      = IP21Msg.IntVals[17];
          rst.m_TotalRunTime       = IP21Msg.RealVals[0];
          rst.m_RepairTimeInterval = IP21Msg.RealVals[1];
          rst.m_RepairTime         = IP21Msg.RealVals[2];
          rst.m_ToolroomTime       = IP21Msg.RealVals[3];
          rst.m_TimeIn             = IP21Msg.TimeStamp[0];
          if (!rst.Update())
          {
            UpdateStat = 1;
          }
        }
        else
        {
          UpdateStat = 1;
        }
      }
    }
    catch( CDBException* e )
    {
      UpdateStat = 1;
      LogMsgTxt += " ** ";
      LogMsgTxt += (LPCSTR) e->m_strError;
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      e->Delete();
    }
    if (UpdateStat)
    {
      MsgType = 1;
      LogMsgTxt.Format("UpdateBaggerRepair: Bagger %d repair data update error(s) detected",IP21Msg.IntVals[1]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    else
    {
      MsgType = 0;
      LogMsgTxt.Format("Bagger %d repair data updated successfully",IP21Msg.IntVals[1]);
      Log.LogMessage(MsgType, (LPCTSTR) LogMsgTxt);
    }
    if (rst.IsOpen())
      rst.Close();
    if (db.IsOpen())
      db.Close();
  }  
  else
  {
    LogMsgTxt.Format("UpdateBaggerRepair: incorrect message record format (ID=%d)", rec);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    UpdateStat = 1;
  }
  return UpdateStat;
}
/* ----------------------------------------------------------------------------
    Update OracleJobMgr record counters and status
------------------------------------------------------------------------------- */  
int UpdateJobManagerStatus(long RecID)
{
  CString LogMsgTxt;
  int rtnval  = 0;
  short wNum  = 0;
  long dbTime = 0;
  long DataTags[4] = { RECORDS_ADDED_FT, RECORDS_MODIFIED_FT,
                       RECORDS_REMOVED_FT, CURRENT_STATUS_FT};
  static short DataTypes[4] = { DTYPSHRT, DTYPSHRT, DTYPSHRT, DTYPSHRT };

  dbTime = GETDBTIM();
  WDBVALS(RecID, 4, DataTags, DataTypes, &JobMgrStatus, &wNum, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("UpdateJobManagerStatus: status write error: %.*s", ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
    rtnval = 1;        
  }
  else
  {
    // trigger history write
    TIME2DB(RecID, LAST_UPDATE_FT, dbTime, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("UpdateJobManagerStatus: update time write error: %.*s", ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
      rtnval = 1;        
    }
  }
  return rtnval;
}
/* ----------------------------------------------------------------------------
    Remove old workorder records
------------------------------------------------------------------------------- */  
void WOCleanup(void)
{
  CString LogMsgTxt;
  long  DeleteRecCount = 0,  // deleted record counter
        RemoveRecCount = 0,  // remove status record count
        TotalRecCount  = 0,  // total record count
        LastRecID      = 0,
        NumRecs        = 0,
        CurrTime       = 0,
        DeltaTime      = 0,
        RetentionDays  = 0,
        RecIDList[RECLISTSIZE];
  short NameSize[RECLISTSIZE],
        ActPriority = 1,
        OldPriority = 0,
        NumOK       = 0,
        j;
  long  WOTags[2]  = {WOSTATUS_FT, DATECREATED_FT};
  short WOTypes[2] = {DTYPLONG, DTYPTIME};
  struct {long WOStatus,
               DateCreated;
         } WOData;
  ERRBLOCK IP21error;
  char RecStat[RECLISTSIZE];
  unsigned char GrpList[8];
  NAMEARR RecNames[RECLISTSIZE];

  RetentionDays = IP21DAY * NUMRETENTIONDAYS; 
  CurrTime = GETDBTIM();
  do
  {
    GETRECLIST(&LastRecID,WorkorderDefID,1,GrpList,8,0,0,RECLISTSIZE,RecIDList,RecStat,RecNames,NameSize,&NumRecs);
    if (NumRecs > 1)
    {
      TotalRecCount += (NumRecs);
      for (j=0; j<NumRecs; j++)
      {      
        // read status and date created fields      
        RDBVALS(RecIDList[j],2, WOTags, WOTypes, &WOData, &NumOK, &IP21error);
        if (IP21error.ERRCODE != SUCCESS)
        {
          ERRMESS(&IP21error, IP21message, &ErrorLength);
          LogMsgTxt.Format("WOCleanup: data read error in record ID %ld: %.*s",
                            RecIDList[j], ErrorLength, IP21message);
          Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
          IP21error.ERRCODE = SUCCESS;
        }
        else
        {
          if (WOData.WOStatus == WOSTAT_REMOVE) // only look at records with REMOVE status 
          {  
            RemoveRecCount++;
            
            // check if date done more than 90 days old
            DeltaTime = CALCDELTIM(CurrTime, WOData.DateCreated);
            if (DeltaTime > RetentionDays)
            {
              if (DeleteIP21Record(RecIDList[j]) > 0)
              {
                LogMsgTxt.Format("WOCleanup: error deleting record ID %d",RecIDList[j]);
                Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
       
              }
              else
              {
                DeleteRecCount++;
              }
            }
          }
        }        
      }
    }
  } while (LastRecID > 0);
  // print cleanup results to log file
  LogMsgTxt.Format("WOCleanup: Records Reviewed=%d   Remove Status Count=%d   Records Deleted=%d",
                    TotalRecCount,RemoveRecCount,DeleteRecCount);
  Log.LogMessage(3, (LPCTSTR) LogMsgTxt);
}
/* ----------------------------------------------------------------------------
    Extract pack code from part name
------------------------------------------------------------------------------- */  
CString ExtractPackCode(CString partname)
{
  int   partnamelen = 0,  
        position    = 0;
  CString packcode;
 
  partnamelen = partname.GetLength();
  position = partname.ReverseFind('-');
  if ((position < 1) || (position+1 > partnamelen))
    packcode.Empty();
  else
    packcode = partname.Mid(position+1);
  return packcode;
}
/* ----------------------------------------------------------------------------
    Console application entry point
------------------------------------------------------------------------------- */  
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode   = 0,
      WC_Records = 0;
  CString ArgVal;
  
  if (argc >= 2)
  {
    ArgVal = argv[1];
    if (ArgVal == "WC")
      WC_Records = 1;   
  }
  // initialize MFC
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{   
    // initialize IP21
    InitIP21(WC_Records);
	}
	return nRetCode;
}

