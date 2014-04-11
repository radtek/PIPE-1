// IssueData.h: interface for the CIssueData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISSUEDATA_H__E061C0E0_EF39_48E6_82DF_EF5B9AFB4160__INCLUDED_)
#define AFX_ISSUEDATA_H__E061C0E0_EF39_48E6_82DF_EF5B9AFB4160__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIssueData  
{
public:
	void GetIssueData();
	CString WIPEntityName;
	CString ItemCode;
  CString MaterialCode;
  CString dbNam,
          dbUsr,
          dbPwd;
	double MaterialID;
  double OrgIdentity;
	double FirstOpSeqNum;
	double JobType;
	double WIPEntityID;
  int dbErrors;
	CIssueData();
	virtual ~CIssueData();

private:
  int oResult;

};

#endif // !defined(AFX_ISSUEDATA_H__E061C0E0_EF39_48E6_82DF_EF5B9AFB4160__INCLUDED_)
