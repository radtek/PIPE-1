
#if !defined(AFX_PANCIM_ORACLE_H__EB7038A4_8FB3_4096_BFB4_8B1ED93C73AB__INCLUDED_)
#define AFX_PANCIM_ORACLE_H__EB7038A4_8FB3_4096_BFB4_8B1ED93C73AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif // !defined(AFX_PANCIM_ORACLE_H__EB7038A4_8FB3_4096_BFB4_8B1ED93C73AB__INCLUDED_)

//#include "resource.h"
// #include "afx.h"
#include "PanCIM_WIP_V.h"

#define MAXFIELDS                            72 // number of predefined field tags
#define MAXRECORDS                            7 // number of predefined record names
#define MAXMTLTYPES                         180 // maximum number of material types
#define ORACLEJOBMGR                          1 // OracleJobMgr activation code
#define PANCIMMESSAGE                         2 // PanCIM message activation code
#define WAITTIME                           3000                            
#define ORACLEJOB_ACT_NONE                    0 // OracleJob activation types
#define ORACLEJOB_ACT_NEW                     1
#define ORACLEJOB_ACT_DELETE                  2
#define ORACLEJOB_ACT_MODIFIED                3
#define ORACLEJOB_ACT_COMPLETE                4
#define ORACLEJOB_ACT_NO_CHARGE               5
#define ORACLEJOB_ACT_CLOSE                   6
#define ORACLEJOB_ACT_REOPEN                  7
#define ORACLEJOB_ACT_CANCEL                 11

#define ORACLEJOB_STAT_NEW                    0 // OracleJob status codes
#define ORACLEJOB_STAT_PRODRECCREATED         1
#define ORACLEJOB_STAT_PRODRECINUSE           2
#define ORACLEJOB_STAT_PRODRECDONE            3
#define ORACLEJOB_STAT_PRODRECREMOVE          4
#define ORACLEJOB_STAT_PRODRECCLOSED          5

#define PANCIM_MSGTYPE_WO_DONE                1 // PanCIM message type codes 
#define PANCIM_MSGTYPE_JOB_DONE               2
#define PANCIM_MSGTYPE_WO_CLOSE               3 
#define PANCIM_MSGTYPE_MDS_ISSUE              4
#define PANCIM_MSGTYPE_MDS_INVEN              5
#define PANCIM_MSGTYPE_PROD_CTRL              7
#define PANCIM_MSGTYPE_JOB_PERF               8
#define PANCIM_MSGTYPE_BAGGER_PERF            9
#define PANCIM_MSGTYPE_INSERTER_PERF         10
#define PANCIM_MSGTYPE_LOADER_PERF           11
#define PANCIM_MSGTYPE_MOLD_REPAIR           13
#define PANCIM_MSGTYPE_BAGGER_REPAIR         14
#define PANCIM_MSGTYPE_SYSTEM_COMMENT        16
#define PANCIM_MSGTYPE_REACTIVATE            17
#define PANCIM_MSGTYPE_WOCLEANUP             18

#define JOBMGRSTAT_OK            0 // OracleJobMgr status codes
#define JOBMGRSTAT_PROCESSING    1
#define JOBMGRSTAT_ERROR         2
#define JOBMGRSTAT_INCOMPLETE    3
#define JOBMGRSTAT_ORGERROR      4

#define MSG_WODONE_ICOUNT        3 // PanCIMMessage workorder done integer count
#define MSG_WODONE_CCOUNT        4 // PanCIMMessage workorder done character count
#define MSG_WODONE_RCOUNT        2 // PanCIMMessage workorder done real count
#define MSG_WODONE_TCOUNT        0 // PanCIMMessage workorder done timestamp count

#define MSG_WOCLOSE_ICOUNT       6 // PanCIMMessage workorder close integer count
#define MSG_WOCLOSE_CCOUNT       4 // PanCIMMessage workorder close character count
#define MSG_WOCLOSE_RCOUNT       3 // PanCIMMessage workorder close real count
#define MSG_WOCLOSE_TCOUNT       2 // PanCIMMessage workorder close timestamp count

#define MSG_PRODCTRL_ICOUNT      3 // PanCIMMessage production control integer count
#define MSG_PRODCTRL_CCOUNT      4 // PanCIMMessage production control character count
#define MSG_PRODCTRL_RCOUNT      2 // PanCIMMessage production control real count
#define MSG_PRODCTRL_TCOUNT      0 // PanCIMMessage production control timestamp count

#define MSG_JOBDONE_ICOUNT       3 // PanCIMMessage Oracle job done integer count
#define MSG_JOBDONE_CCOUNT       1 // PanCIMMessage Oracle job done character count
#define MSG_JOBDONE_RCOUNT       3 // PanCIMMessage Oracle job done real count
#define MSG_JOBDONE_TCOUNT       0 // PanCIMMessage Oracle job done timestamp count

#define MSG_MSDISSUE_ICOUNT      7 // PanCIMMessage MDS issue integer count
#define MSG_MDSISSUE_CCOUNT      1 // PanCIMMessage MDS issue character count
#define MSG_MDSISSUE_RCOUNT      0 // PanCIMMessage MDS issue real count
#define MSG_MDSISSUE_TCOUNT      0 // PanCIMMessage MDS issue timestamp count

#define MSG_MSDINVEN_ICOUNT      8 // PanCIMMessage MDS inventory move iteger count
#define MSG_MDSINVEN_CCOUNT      0 // PanCIMMessage MDS inventory move character count
#define MSG_MDSINVEN_RCOUNT      0 // PanCIMMessage MDS inventory move real count
#define MSG_MDSINVEN_TCOUNT      0 // PanCIMMessage MDS inventory move timestamp count

#define MSG_JOBPERF_ICOUNT       6 // PanCIMMessage job performance integer count
#define MSG_JOBPERF_CCOUNT       2 // PanCIMMessage job performance character count
#define MSG_JOBPERF_RCOUNT       6 // PanCIMMessage job performance real count
#define MSG_JOBPERF_TCOUNT       0 // PanCIMMessage job performance timestamp count

#define MSG_SYSCOMMENT_ICOUNT    3 // PanCIMMessage system comment integer count
#define MSG_SYSCOMMENT_CCOUNT    4 // PanCIMMessage system comment character count
#define MSG_SYSCOMMENT_RCOUNT    0 // PanCIMMessage system comment real count
#define MSG_SYSCOMMENT_TCOUNT    0 // PanCIMMessage system comment timestamp count

#define MSG_BAGGERPERF_ICOUNT    6 // PanCIMMessage bagger performance integer count
#define MSG_BAGGERPERF_CCOUNT    1 // PanCIMMessage bagger performance character count
#define MSG_BAGGERPERF_RCOUNT    0 // PanCIMMessage bagger performance real count
#define MSG_BAGGERPERF_TCOUNT    0 // PanCIMMessage bagger performance timestamp count

#define MSG_INSERTERPERF_ICOUNT  5 // PanCIMMessage inserter performance integer count
#define MSG_INSERTERPERF_CCOUNT  1 // PanCIMMessage inserter performance character count
#define MSG_INSERTERPERF_RCOUNT  0 // PanCIMMessage inserter performance real count
#define MSG_INSERTERPERF_TCOUNT  0 // PanCIMMessage inserter performance timestamp count

#define MSG_LOADERPERF_ICOUNT    5 // PanCIMMessage loader performance integer count
#define MSG_LOADERPERF_CCOUNT    1 // PanCIMMessage loader performance character count
#define MSG_LOADERPERF_RCOUNT    0 // PanCIMMessage loader performance real count
#define MSG_LOADERPERF_TCOUNT    0 // PanCIMMessage loader performance timestamp count

#define MSG_MOLDREPAIR_ICOUNT   13 // PanCIMMessage mold repair integer count
#define MSG_MOLDREPAIR_CCOUNT    6 // PanCIMMessage mold repair character count
#define MSG_MOLDREPAIR_RCOUNT    2 // PanCIMMessage mold repair real count
#define MSG_MOLDREPAIR_TCOUNT    1 // PanCIMMessage mold repair timesatmp count

#define MSG_BAGGERREPAIR_ICOUNT 18 // PanCIMMessage bagger repair integer count
#define MSG_BAGGERREPAIR_CCOUNT  5 // PanCIMMessage bagger repair character count
#define MSG_BAGGERREPAIR_RCOUNT  4 // PanCIMMessage bagger repair real count
#define MSG_BAGGERREPAIR_TCOUNT  1 // PanCIMMessage bagger repair timestamp count

#define WONAMELEN               12  // field length constants
#define WONUMLEN                 8
#define JOBLEN                   8
#define PARTNAMELEN             18
#define PACKCODELEN             12
#define MATERIALLEN             12
#define COLORLEN                 8      
#define MOLDNAMELEN             18
#define PARTLEN                 15
#define MATERIALTYPELEN         16
#define STRTIMELEN              12
#define MSGFIELDCHARLEN         20 
#define MINRECID              2000  // minimum record ID
#define RECLISTSIZE           1000  // record list buffer size
#define WOSTAT_NEWORDER          0  // Workorder status codes
#define WOSTAT_SCHEDULED         1
#define WOSTAT_PRINTED           2
#define WOSTAT_UNSCHED           3
#define WOSTAT_RUN               4
#define WOSTAT_SUBTOTAL          5
#define WOSTAT_COSRESET          6
#define WOSTAT_LOCERROR          7
#define WOSTAT_EMPERROR          8 
#define WOSTAT_COMMERROR         9
#define WOSTAT_RUNNING          10
#define WOSTAT_DONE             11
#define WOSTAT_UNAPPROVED_MATL  12
#define WOSTAT_BLANK            14
#define WOSTAT_CLOSE            15          
#define WOSTAT_DELETE           16
#define WOSTAT_REMOVE           17

#define ORACLE_WIP_UNRELEASED          1  // Oracle WIP status codes
#define ORACLE_WIP_RELEASED            3
#define ORACLE_WIP_COMPLETE            4
#define ORACLE_WIP_NO_CHARGE           5
#define ORACLE_WIP_HOLD                6
#define ORACLE_WIP_CANCELLED           7
#define ORACLE_WIP_PENDING_BILL_LOAD   8
#define ORACLE_WIP_FAILED_BILL_LOAD    9
#define ORACLE_WIP_PENDING_ROUTE_LOAD 10
#define ORACLE_WIP_FAILED_ROUTE_LOAD  11
#define ORACLE_WIP_CLOSED             12
#define ORACLE_WIP_PENDING_MASS_LOAD  13
#define ORACLE_WIP_PENDING_CLOSE      14
#define ORACLE_WIP_FAILED_CLOSED      15
#define ORACLE_WIP_PENDING_SCHEDULE   16
#define ORACLE_WIP_DRAFT              17

#define SECONDS_IN_HOUR             3600
#define IP21SECOND                    10
#define IP21MINUTE                   600
#define IP21HOUR                   36000
#define IP21DAY                   864000    
#define NUMRETENTIONDAYS              90  // number of days to hold workorder records


// #define DSN_WIP_V          "DSN=PanCIM_WIP_V;UID=pancim;PWD=panduit"
// #define DSN_WIP            "DSN=PanCIM_WIP;UID=pancim;PWD=panduit"

// use Oracle 11.1 ODBC connection -----------------------------------
#define DSN_WIP_V          "DSN=PanCIM_JOB_V;UID=pancim;PWD=panduit"
#define DSN_WIP            "DSN=PanCIM_JOB;UID=pancim;PWD=panduit"
// -------------------------------------------------------------------
#define DSN_MTL_ISSUE      "DSN=MDS_ISSUE;UID=pancim;PWD=panduit"
#define DSN_MTL_INVEN      "DSN=MDS_INVEN;UID=pancim;PWD=panduit"
#define DSN_PRODSCHED      "DSN=ProdSched;UID=ip21task;PWD=wcdiv" // (WC and Singapore only)
#define DSN_PRODJOBHIST    "DSN=ProdJobHist;UID=JobHistInterface;PWD=poiapp2005"
#define DSN_EQPTPERFHIST   "DSN=EqptPerfHist;UID=JobHistInterface;PWD=poiapp2005"
#define DSN_EQPTREPAIRHIST "DSN=EqptRepHist;UID=JobHistInterface;PWD=poiapp2005"

static short TotalMaterialTypes = 0;
static long  OracleTimeZoneOffset = 0,  // Oracle server UTC time zone offset
             LocalTimeZoneOffset  = 0;
static int   LocalDST_Flag        = 0; 

// record definition ID constatnts
static long WorkorderDefID = 0,
            OracleJobDefID = 0,
            MessageCheckID = 0,
            DefaultMsgID   = 0,
            PCMItemNameID  = 0,
            WOCleanupID    = 0,
            PanCIMMsgDefID = 0;
// global counters
struct CJobMgrStatus {short RecordsAdded,
                            RecordsModified,
                            RecordsDeleted,
                            Status;};

static char MaterialType[MAXMTLTYPES][MATERIALTYPELEN],
            ORACLE_DATABASE[ORACLEDBINFOLEN],
            ORACLE_USER[ORACLEDBINFOLEN],
            ORACLE_PASSWORD[ORACLEDBINFOLEN];

// record name constants
static char IP21RecordNames[MAXRECORDS][20] = {"WorkorderDef",   "OracleJobDef", "PCM_ItemName",
                                               "PanCIMMsgCheck", "WOCleanup",    "d-PanCIMMessage",
                                               "PanCIMMessageDef" };

// Field name constants
static char IP21FieldName[MAXFIELDS][20] = { "Name",
                                             "WONum",
                                             "DateCreated",
                                             "DateDue",
                                             "WOPriority",
                                             "PlanQty",
                                             "MachNum",
                                             "PartName",
                                             "PackCode",
                                             "Material",
                                             "Color",
                                             "DCINumber",
                                             "MoldName",
                                             "NumberofCavities",
                                             "StandardRate",
                                             "WOStatus",
                                             "WIPQty",
                                             "DoneQty",
                                             "RejQty",
                                             "WO_Cycles",
                                             "RunTime",
                                             "DateDone",                                     
                                             "WorkOrder",
                                             "Part",
                                             "OrganizationCode",
                                             "OrganizationID",
                                             "LAST_UPDATE",
                                             "CURRENT_STATUS",
                                             "ACTIVATE_TYPE",
                                             "Records_Added",
                                             "Records_Modified",
                                             "Records_Removed",
                                             "#INTEGER_VALUES",
                                             "#REAL_VALUES",
                                             "#TIME_STAMP_VALUES",
                                             "#CHARACTER_STRINGS",
                                             "STATUS",
                                             "Job",
                                             "Cell",
                                             "MEASURED_QTY",
                                             "ERROR_TEXT",
                                             "Integer_Label",
                                             "INTEGER_VALUE",
                                             "Integer_Modified",
                                             "Real_Label",
                                             "REAL_VALUE",
                                             "Real_Modified",
                                             "Timestamp_Label",
                                             "TIME_STAMP_VALUE",
                                             "Timestamp_Modified",
                                             "Character_Label",
                                             "CHARACTER_STRING",
                                             "Character_Modified",
                                             "QUERY_RECORD",                                             
                                             "IntegerCount",
                                             "CharCount",
                                             "RealCount",
                                             "TimeStampCount",
                                             "TransCode",
                                             "INT_FIELD",
                                             "CHAR_FIELD",
                                             "REAL_FIELD",
                                             "TIME_FIELD",
                                             "Source",
                                             "DB_NAME",
                                             "DB_USER",
                                             "DB_PWD",
                                             "WIP_ID",
                                             "#_DEPARTMENTS",
                                             "Department_ID",
                                             "#_OF_SELECTIONS",
                                             "SELECT_DESCRIPTION"};

// Field tag constants
static long   NAME_FT, 
              WONUM_FT,
              DATECREATED_FT,
              DATEDUE_FT,
              WOPRIORITY_FT,
              PLANQTY_FT,
              MACHNUM_FT,
              PARTNAME_FT,
              PACKCODE_FT,
              MATERIAL_FT,
              COLOR_FT,
              DCINUMBER_FT,
              MOLDNAME_FT,
              NUMCAVITIES_FT,
              STANDARDRATE_FT,
              WOSTATUS_FT,
              WIPQTY_FT,
              DONEQTY_FT,
              REJQTY_FT,
              WOCYCLES_F,
              RUNTIME_FT,
              DATEDONE_FT,
              WORKORDER_FT,
              PART_FT,
              ORGANIZATIONCODE_FT,
              ORGANIZATIONID_FT,
              LAST_UPDATE_FT,
              CURRENT_STATUS_FT,
              ACTIVATE_TYPE_FT,
              RECORDS_ADDED_FT,
              RECORDS_MODIFIED_FT,
              RECORDS_REMOVED_FT,
              NUM_INTEGER_VALUES_FT,
              NUM_REAL_VALUES_FT,
              NUM_TIME_STAMP_VALUES_FT,
              NUM_CHARACTER_STRINGS_FT,
              STATUS_FT,
              JOB_FT,
              CELL_FT,
              MEASURED_QTY_FT,
              ERROR_TEXT_FT,
              INTEGER_LABEL_FT,
              INTEGER_VALUE_FT,
              INTEGER_MODIFIED_FT,
              REAL_LABEL_FT,
              REAL_VALUE_FT,
              REAL_MODIFIED_FT,
              TIMESTAMP_LABEL_FT,
              TIME_STAMP_VALUE_FT,
              TIMESTAMP_MODIFIED_FT,
              CHARACTER_LABEL_FT,
              CHARACTER_STRING_FT,
              CHARACTER_MODIFIED_FT,
              QUERY_RECORD_FT,
              INTEGERCOUNT_FT,
              CHARCOUNT_FT,
              REALCOUNT_FT,
              TIMESTAMPCOUNT_FT,
              TRANSCODE_FT,
              INT_FIELD_FT,
              CHAR_FIELD_FT,
              REAL_FIELD_FT,
              TIME_FIELD_FT,
              SOURCE_FT,
              DB_NAME_FT,
              DB_USER_FT,
              DB_PWD_FT,
              WIP_ID_FT,
              NUM_DEPARTMENTS_FT,
              DEPARTMENT_ID_FT,
              NUM_SELECTIONS_FT,
              SELECT_DESCRIPTION_FT;

//////////////////////////////////////////////////////////////////////////////////////

void  InitIP21(int WC_Records);
void  LogMessage(short msgid, LPCTSTR msgtxt);
void  IP21ActivationHandler(long RecID, short ActCode);
int   PanCIMMessageHandler(long RecID);
int   OracleJobMgr(long RecID);
int   OracleIssue(long RecID);
int   OracleInven(long RecID);
int   DeleteIP21Record(long recid);

long  CreateWorkorderRecord(PanCIM_WIP_V *rsOracleJobData);
int   ModifyWorkorderRecord(PanCIM_WIP_V* rsOracleJobData);
int   UpdateWorkorderField(long WORecID, long FieldTag, long UpdateVal, short StatusFilter);
int   UpdateWorkorderField(long WORecID, long FieldTag, CTime UpdateVal);
int   UpdateWorkorderField(long WORecID, long FieldTag, CString UpdateVal, long strSize);

int   CloseWorkorderRecord(long WORecID, PanCIM_WIP_V* rsOracleJobData, short OraStat);
int   ReopenWorkorderRecord(PanCIM_WIP_V *WOData);

long  CreateOracleJobRecord(PanCIM_WIP_V *rsOracleJobData);
int   ModifyOracleJobRecord(PanCIM_WIP_V *rsOracleJobData);
int   CloseOracleJobRecord(long JobRecID, PanCIM_WIP_V *rsOracleJobData);
int   ActivateOracleJobRecord(long JobRecID, long ActCode);

int   UpdateJobHistory(PanCIM_WIP_V *rsOracleJobData);
int   CloseJobHistory(PanCIM_WIP_V *rsOracleJobData);

int   UpdateOracleFlag(long org_id, long ent_id, CString fld_id);
int   UpdateOracleFlag(long org_id, long ent_id);

int   UpdateOracleJobRecord(long rec, long val, long occ, long tag);
int   UpdateOracleJobRecord(long rec, CTime val, long occ, long tag);
int   UpdateOracleJobRecord(long rec, double val, long occ, long tag);
int   UpdateOracleJobRecord(long rec, CString val, long occ, long tag);

int   UpdateOracleWODone(long RecID);
int   UpdateOracleJobDone(long RecID);
int   UpdateOracleMDSIssue(long RecID);
int   UpdateOracleMDSInventory(long RecID); 

int   UpdateHistWODone(long RecID);
int   UpdateHistJobDone(long RecID);
int   PanCIMHistoryClose(long org_id, long ent_id);

int   UpdateProdCtrlOrder(long rec); 
int   UpdateJobPerformance(long rec);
int   UpdateSysComment(long rec);
int   UpdateBaggerPerf(long rec); 
int   UpdateInserterPerf(long rec);
int   UpdateLoaderPerf(long rec);
int   UpdateMoldRepair(long rec);
int   UpdateBaggerRepair(long rec);
int   UpdateJobManagerStatus(long RecID);
void  WOCleanup(void);


CString ExtractPackCode(CString partname);


