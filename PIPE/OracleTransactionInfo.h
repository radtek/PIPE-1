// OracleTransactionInfo.h: interface for the COracleTransactionInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLETRANSACTIONINFO_H__010EC942_B154_4655_9E60_287A16309F4D__INCLUDED_)
#define AFX_ORACLETRANSACTIONINFO_H__010EC942_B154_4655_9E60_287A16309F4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COracleTransactionInfo  
{
public:
	void GetOracleTransactionCodes();
	COracleTransactionInfo();
	virtual ~COracleTransactionInfo();
  CString TransName,
          dbNam,
          dbUsr,
          dbPwd;
  double  TransTypeID,
          TransSourceID,
          TransActionID;
  int oResult;
};

#endif // !defined(AFX_ORACLETRANSACTIONINFO_H__010EC942_B154_4655_9E60_287A16309F4D__INCLUDED_)
