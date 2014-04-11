// OracleOrgInfo.h: interface for the OracleOrgInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLEORGINFO_H__526F7271_420A_4641_9F72_5464A96A6B03__INCLUDED_)
#define AFX_ORACLEORGINFO_H__526F7271_420A_4641_9F72_5464A96A6B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXORG       9       // maximum number of organization IDs
#define MAXDEP      20       // maximum number of departments
#define ORGID_WC  2083
#define ORGID_SGP 2535
#define ORGID_OP  2264
#define ORGID_NL  2284
#define ORGID_WA  2285
#define ORGID_ID  2286
#define ORGID_LP  1787
#define ORGID_CR   138
#define ORGID_BR  2263
 

class OracleOrgInfo  
{
public:
	OracleOrgInfo();
  int OrgCheck(long, char[ORGCODELEN]);
  void OracleOrgInfo::GetOrganizationID(long ftag1, long ftag2, long ftag3);
  void OracleOrgInfo::GetOracleConnInfo(long ftag);
	virtual ~OracleOrgInfo();

  // variables to hold valid organization IDs and codes
  double OrgID[MAXORG];
  double DepID[MAXDEP];
  long   NumDepartments;
  char OrgCode[MAXORG][ORGCODELEN];
  char ConnInfo[ORACLEDBINFOLEN];
  CString OrgName;
  double OrgNum;
};

#endif // !defined(AFX_ORACLEORGINFO_H__526F7271_420A_4641_9F72_5464A96A6B03__INCLUDED_)
