// OracleMAP.cpp: implementation of the OracleMAP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OracleMAP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define IP21MINCHAR 0x1f
#define IP21MAXCHAR 0x7e

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OracleMAP::OracleMAP()
{
  MAXiOcc         = 0;
  MAXrOcc         = 0;
  MAXcOcc         = 0;
  MAXtOcc         = 0;
  IntegerCount    = 0;
  RealCount       = 0;
  CharCount       = 0;
  TsCount         = 0;
  TotalMapFields  = 0;
  rtnStatus       = MAP_OK; 
  JobFields_I[0]  = "MACHINE_ID";
  JobFields_I[1]  = "ORGANIZATION_ID";
  JobFields_I[2]  = "WIP_ENTITY_ID";
  JobFields_I[3]  = "CREATED_BY";
  JobFields_I[4]  = "LAST_UPDATE_BY";
  JobFields_I[5]  = "WIP_STATUS_CODE";
  JobFields_I[6]  = "START_QTY";
  JobFields_I[7]  = "COMPLETED_QTY";
  JobFields_I[8]  = "MEASURED_QTY";  
  JobFields_I[9]  = "PRIORITY";
  JobFields_I[10] = "QUANTITY_SCRAPPED";
  JobFields_I[11] = "DESIGN_CAVITIES"; 
  JobFields_I[12] = "ACTUAL_CAVITIES";
  JobFields_I[13] = "DEPARTMENT_ID";

  JobFields_R[0]  = "STANDARD_COST"; 
  JobFields_R[1]  = "RESOURCE_RATE"; 
  JobFields_R[2]  = "MATERIAL_RATE"; 
  JobFields_R[3]  = "MATERIAL_OVERHEAD_RATE"; 
  JobFields_R[4]  = "OVERHEAD_RATE"; 
  JobFields_R[5]  = "OUTSIDE_PROCESS_RATE"; 
  
  JobFields_C[0]  = "PANCIM_STATUS";
  JobFields_C[1]  = "ORGANIZATION_CODE"; 
  JobFields_C[2]  = "WIP_ENTITY_NAME";
  JobFields_C[3]  = "ITEM_CODE";
  JobFields_C[4]  = "ITEM_UOM";
  JobFields_C[5]  = "COLOR";
  JobFields_C[6]  = "MATERIAL_CODE";
  JobFields_C[7]  = "MATERIAL_UOM";
  JobFields_C[8]  = "EMPLOYEE_ID";
  JobFields_C[9]  = "WIP_JOB_STATUS";
  JobFields_C[10] = "MOLD_NAME";
  JobFields_C[11] = "MOLD_BASE";
  JobFields_C[12] = "MACHINE_NAME";
  JobFields_C[13] = "DEPARTMENT_CODE";
  
  JobFields_T[0]  = "CREATION_DATE";
  JobFields_T[1]  = "LAST_UPDATE_DATE";
  JobFields_T[2]  = "DATE_COMPLETED";
  JobFields_T[3]  = "DATE_CLOSED";
  JobFields_T[4]  = "SCHED_START_DATE";
  JobFields_T[5]  = "SCHED_COMPLETION_DATE";
  JobFields_T[6]  = "PRINT_DATE";
}
OracleMAP::~OracleMAP()
{

}
/* ---------------------------------------------------------
    read and store the Oracle-IP21 field mapping
------------------------------------------------------------ */
int OracleMAP::ReadOracleMAP()
{
  long  NumMapOcc,
        MapRecID = 0,
        j;
  int   sLen,
        sPos,
        StoreFlag,
        k;  
  void  *ptrData[6];
  long  ptrTags[6];
  short ptrTypes[6];
  short ftOK,
        occOK;
  char  tmpOracleField[MAXMAPFIELDS][MAPORACLEFIELDLEN],
        tmpFieldLabel[MAXMAPFIELDS][LABELLEN];
  long  tmpIP21OccNum[MAXMAPFIELDS],
        tmpIP21DataType[MAXMAPFIELDS],
        tmpUseFlag[MAXMAPFIELDS],
        tmpIP21FieldID[MAXMAPFIELDS];

  MapRecID = GetMapRecordID();
  if (!MapRecID)
  {      
    printf("ReadOracleMAP: Unable to read OracleMap record.\n");
    rtnStatus = MAP_ERR;
    return rtnStatus;
  }
  // read the SQL_RECORD field 
  DB2REID(MapRecID, SQL_RECORD_FT, &QueryRecordID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("OracleMAP: OracleMap SQL_RECORD read error: %.*s\n", 
            ErrorLength, IP21message);
    rtnStatus = MAP_ERR;
    return rtnStatus;        
  }
  // read number of occurrences
  DB2LONG(MapRecID, NUM_DB_FIELDS_FT, &NumMapOcc, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("OracleMAP: OracleMap record read error: %.*s\n", 
            ErrorLength, IP21message);
    rtnStatus = MAP_ERR;
    return rtnStatus;        
  }
  if (NumMapOcc > MAXMAPFIELDS)
    NumMapOcc = MAXMAPFIELDS;
  // read all map occurrences
  ptrTypes[0] = MAPORACLEFIELDLEN;
  ptrTypes[1] = DTYPREID;
  ptrTypes[2] = DTYPLONG;
  ptrTypes[3] = DTYPLONG;
  ptrTypes[4] = LABELLEN;
  ptrTypes[5] = DTYPLONG;
   
  ptrTags[0]  = ORACLE_FIELD_FT;
  ptrTags[1]  = IP21_FIELD_FT;
  ptrTags[2]  = OCCURRENCE_NUM_FT;
  ptrTags[3]  = DATA_TYPE_FT;
  ptrTags[4]  = FIELD_LABEL_FT;
  ptrTags[5]  = USE_FT;
  
  ptrData[0]  = tmpOracleField;
  ptrData[1]  = tmpIP21FieldID;
  ptrData[2]  = tmpIP21OccNum;
  ptrData[3]  = tmpIP21DataType;
  ptrData[4]  = tmpFieldLabel;
  ptrData[5]  = tmpUseFlag;
 
  MRDBOCCS(MapRecID, 6, ptrTags, ptrTypes, 1, NumMapOcc, ptrData, &occOK, &ftOK, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("ReadOracleMAP: OracleMap record data read error: %.*s\n", ErrorLength, IP21message);
    IP21error.ERRCODE = SUCCESS; // reset error code   
    NumMapOcc = 0;
    rtnStatus = MAP_ERR;
  }
  TotalMapFields = 0;
  // only save data for fields flagged as used 
  for (j=0; j<NumMapOcc; j++)
  {
    StoreFlag = 0;
    if (tmpUseFlag[j] == USE_YES)
    { 
      // save data type counts to simplify creating OracleMap record occurrences later
      switch (tmpIP21DataType[j])
      {
        case MAPDATATYPE_UNKNOWN:
              break;

        case MAPDATATYPE_INTEGER:
              StoreFlag = 1;
              IntegerCount++;
              if (tmpIP21OccNum[j] > MAXiOcc) // save the largest integer occurrence number
                MAXiOcc = tmpIP21OccNum[j];
              break;
   
        case MAPDATATYPE_REAL:
              StoreFlag = 1;
              RealCount++;
              if (tmpIP21OccNum[j] > MAXrOcc) // save the largest real occurrence number
                MAXrOcc = tmpIP21OccNum[j];
              break;
            
        case MAPDATATYPE_CHARACTER:
              StoreFlag = 1;
              CharCount++;
               if (tmpIP21OccNum[j] > MAXcOcc)  // save the largest character occurrence number
                MAXcOcc = tmpIP21OccNum[j];
              break;
        
        case MAPDATATYPE_TIMESTAMP:
              StoreFlag = 1;
              TsCount++;
              if (tmpIP21OccNum[j] > MAXtOcc)  // save the largest timestamp occurrence number
                MAXtOcc = tmpIP21OccNum[j];
              break;
        
        default:
              break;
      
      }
      if (StoreFlag == 1)
      {
        // remove unprintable characters from IP21 strings
        OracleField[TotalMapFields].Empty();
        IP21FieldLabel[TotalMapFields].Empty();
        for (k=0; k<MAPORACLEFIELDLEN; k++)
        {
          if ((tmpOracleField[j][k] > IP21MINCHAR) && (tmpOracleField[j][k] < IP21MAXCHAR))
          {
            sPos = OracleField[TotalMapFields].GetLength();
            sLen = OracleField[TotalMapFields].Insert(sPos+1, tmpOracleField[j][k]);    
          }
        }
        for (k=0; k<LABELLEN; k++)
        {
          if ((tmpFieldLabel[j][k] > IP21MINCHAR) && (tmpFieldLabel[j][k] < IP21MAXCHAR))
          {
            sPos = IP21FieldLabel[TotalMapFields].GetLength();
            sLen = IP21FieldLabel[TotalMapFields].Insert(sPos+1, tmpFieldLabel[j][k]);    
          }
        } 
        IP21Field[TotalMapFields]    = tmpIP21FieldID[j];
        IP21OccNum[TotalMapFields]   = tmpIP21OccNum[j];
        IP21DataType[TotalMapFields] = tmpIP21DataType[j];
        OracleField[TotalMapFields].TrimRight(' ');
        TotalMapFields++;
      }
    }
  }
  return rtnStatus;
}
/* ---------------------------------------------------------
    decode the OracleMap record and required fields 
------------------------------------------------------------ */
long OracleMAP::GetMapRecordID()
{
  long RecID = 0;
  CString RecName;
  short NumChars,
        j;
  long  FieldTags[11],
        TagBuffer;
  char  *pBuf;
  
  char IP21MapFields[11][20] = { "#DATA_BASE_FIELDS",
                                 "Message",
                                 "ERROR_TYPE",
                                 "Oracle_Field",
                                 "IP21_Field",
                                 "OCCURRENCE_NUM",
                                 "DATA TYPE",
                                 "FIELD_LABEL",
                                 "Use",
                                 "COMMENT",
                                 "SQL_RECORD"};

  for (j=0; j<11; j++)
  {
    pBuf = IP21MapFields[j];
    NumChars = strlen(pBuf);
    DECODFT(pBuf, NumChars, &TagBuffer, &IP21error);
    FieldTags[j] = TagBuffer;
    if (!TagBuffer)
    {
      pBuf[NumChars] = '\0';
      printf("OracleMAP: %s decode field tag error\n", pBuf);
      rtnStatus = MAP_ERR;
    }
  }
  NUM_DB_FIELDS_FT  = FieldTags[0];
  MESSAGE_FT        = FieldTags[1];
  ERRPR_TYPE_FT     = FieldTags[2];
  ORACLE_FIELD_FT   = FieldTags[3];
  IP21_FIELD_FT     = FieldTags[4];
  OCCURRENCE_NUM_FT = FieldTags[5];
  DATA_TYPE_FT      = FieldTags[6];
  FIELD_LABEL_FT    = FieldTags[7];
  USE_FT            = FieldTags[8];
  COMMENT_FT        = FieldTags[9];
  SQL_RECORD_FT     = FieldTags[10];
  
  RecName = "OracleMap";
  NumChars = RecName.GetLength();
  DECODNAM((LPCTSTR)RecName, NumChars, &RecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    RecID = 0;  
    printf("GetMapRecordID: %s decode record ID error\n", (LPCTSTR)RecName);
    rtnStatus = MAP_ERR;
  }
  return RecID;
}
