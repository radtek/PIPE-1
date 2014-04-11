// OracleSeqNum1.h: interface for the COracleSeqNum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLESEQNUM1_H__39A219AD_57C7_40D7_9F29_5504AED15857__INCLUDED_)
#define AFX_ORACLESEQNUM1_H__39A219AD_57C7_40D7_9F29_5504AED15857__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COracleSeqNum  
{
public:
	CString dbNam,
          dbUsr,
          dbPwd;
  double GetSeqNum();
  double SeqNum;
  COracleSeqNum();
	virtual ~COracleSeqNum();

private:
  CString strSQL;
};

#endif // !defined(AFX_ORACLESEQNUM1_H__39A219AD_57C7_40D7_9F29_5504AED15857__INCLUDED_)
