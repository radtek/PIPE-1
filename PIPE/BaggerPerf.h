#if !defined(AFX_BAGGERPERF_H__A2A25FDE_30DA_41E9_B254_54FEA93CC548__INCLUDED_)
#define AFX_BAGGERPERF_H__A2A25FDE_30DA_41E9_B254_54FEA93CC548__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaggerPerf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaggerPerf recordset

class CBaggerPerf : public CRecordset
{
public:
	CBaggerPerf(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBaggerPerf)

// Field/Param Data
	//{{AFX_FIELD(CBaggerPerf, CRecordset)
	long	m_RefID;
	CTime	m_LogTime;
	long	m_EqptNum;
	CString	m_Workorder;
	long	m_BagCount;
	long	m_DivertCount;
	long	m_MessageCount;
	long	m_WarningCount;
	long	m_FaultCount;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaggerPerf)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAGGERPERF_H__A2A25FDE_30DA_41E9_B254_54FEA93CC548__INCLUDED_)
