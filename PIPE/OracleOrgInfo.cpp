// OracleOrgInfo.cpp: implementation of the OracleOrgInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OracleOrgInfo.h"

OracleOrgInfo::OracleOrgInfo()
{
  short j;

  NumDepartments = 0;
  for (j=0; j<MAXDEP; j++)
    DepID[j] = 0;

  // define valid Oracle organization IDs and codes
  OrgID[0]   = ORGID_WC;
  strcpy(OrgCode[0],"WC");
  OrgID[1]   = ORGID_SGP;
  strcpy(OrgCode[1],"SGP");
  OrgID[2]   = ORGID_OP;
  strcpy(OrgCode[2],"OP");
  OrgID[3]   = ORGID_NL;
  strcpy(OrgCode[3],"NL");
  OrgID[4]   = ORGID_WA;
  strcpy(OrgCode[4],"WA");
  OrgID[5]   = ORGID_ID;
  strcpy(OrgCode[5],"ID");
  OrgID[6]   = ORGID_LP;
  strcpy(OrgCode[6],"LP");
  OrgID[7]   = ORGID_CR;
  strcpy(OrgCode[7],"CR");
  OrgID[8]   = ORGID_BR;
  strcpy(OrgCode[8],"BR");

}

OracleOrgInfo::~OracleOrgInfo()
{

}
/* ---------------------------------------------------------
    validate the organization ID and name
------------------------------------------------------------ */
int OracleOrgInfo::OrgCheck(long OrgIdent, char OrgName[ORGCODELEN])
{
  int OrgValid=0,
      OrgCodeLen,
      j;
  
  for (j=0; j<MAXORG; j++)
  {
    if (OrgID[j] == OrgIdent) // ID found - now check name
    {
      OrgCodeLen = strlen(OrgCode[j]);
      if (strncmp(OrgCode[j], OrgName, OrgCodeLen) == 0)
          return 1;         
    }
  }
  return OrgValid;
}
/* ----------------------------------------------------------------
    get the organization and department IDs from the IP21 database
------------------------------------------------------------------- */
void OracleOrgInfo::GetOrganizationID(long ftag1, long ftag2, long ftag3)
{
  long  RecID,
        OrgCode = 0;
  short NumOK;

  OrgName = "????????";
  DECODNAM("OracleJobManager", 16, &RecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("GetOrganizationID: decode error: %.*s\n", ErrorLength, IP21message);
    IP21error.ERRCODE = SUCCESS;
    OrgCode = 0;
  }
  if (RecID > 0)
  {
    // read the organization ID
    DB2LONG(RecID, ftag1, &OrgCode, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("GetOrganizationID: OrganizationID read error: %.*s\n", ErrorLength, IP21message);
      IP21error.ERRCODE = SUCCESS;
      OrgCode = 0;
    }
    // read the number of departments
    DB2LONG(RecID, ftag2, &NumDepartments, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("GetOrganizationID: number of departments read error: %.*s\n", ErrorLength, IP21message);
      IP21error.ERRCODE = SUCCESS;
      NumDepartments = 0;
    }
    if (NumDepartments > 0)
    {
      if (NumDepartments > MAXDEP)
        NumDepartments = MAXDEP;
    
      // read and store the department IDs
      RDBOCCS(RecID, ftag3, 1, NumDepartments, DTYPLONG, DepID, &NumOK, &IP21error);
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        printf("GetOrganizationID: department ID read error: %.*s\n", ErrorLength, IP21message);
        IP21error.ERRCODE = SUCCESS;
        NumDepartments = 0;
      }
    }
  }
  switch (OrgCode)
  {
    case ORGID_WC:
      OrgName = "Wiring Components";
      OrgNum  = ORGID_WC;
      break;
    case ORGID_SGP:
      OrgName = "Singapore";
      OrgNum  = ORGID_SGP;
      break;
    case ORGID_OP:
      OrgName = "Orland Park";
      OrgNum  = ORGID_OP;
      break;
    case ORGID_NL:
      OrgName = "New Lenox";
      OrgNum  = ORGID_NL;
      break;
    case ORGID_WA:
      OrgName = "Wiring Accessories";
      OrgNum  = ORGID_WA;
      break;
    case ORGID_ID:
      OrgName = "ID Division";
      OrgNum  = ORGID_ID;
      break;
    case ORGID_LP:
      OrgName = "Lockport";
      OrgNum  = ORGID_LP;
      break;
    case ORGID_CR: 
      OrgName = "Costa Rica";
      OrgNum  =  ORGID_CR;
      break;
  case ORGID_BR: 
      OrgName = "Burr Ridge";
      OrgNum  =  ORGID_BR;
      break;
    default:
      OrgName = "Unknown";
      OrgNum = 0;
      break;
  }  
}
/* ---------------------------------------------------------
    get the Oracle connection information
------------------------------------------------------------ */
void OracleOrgInfo::GetOracleConnInfo(long ftag)
{
  long  RecID;
    
  DECODNAM("OracleJobManager", 16, &RecID, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("GetOracleConnInfo: decode error: %.*s\n", ErrorLength, IP21message);
    IP21error.ERRCODE = SUCCESS;
  }
  if (RecID > 0)  
  {
    DB2CHBF(RecID,ftag,ConnInfo,ORACLEDBINFOLEN,&IP21error);  
    {
      if (IP21error.ERRCODE != SUCCESS)
      {
        ERRMESS(&IP21error, IP21message, &ErrorLength);
        printf("GetOracleConnInfo: read data error %.*s\n", ErrorLength, IP21message);
        IP21error.ERRCODE = SUCCESS;
      }
    }
  }
}