#if !defined(AFX_INSERTERPERF_H__51308BC5_D846_4D84_B707_6BF2DD5B99A9__INCLUDED_)
#define AFX_INSERTERPERF_H__51308BC5_D846_4D84_B707_6BF2DD5B99A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InserterPerf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInserterPerf recordset

class CInserterPerf : public CRecordset
{
public:
	CInserterPerf(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CInserterPerf)

// Field/Param Data
	//{{AFX_FIELD(CInserterPerf, CRecordset)
	long	m_RefID;
	CTime	m_LogTime;
	long	m_EqptNum;
	CString	m_Workorder;
	long	m_CycleCount;
	long	m_MessageCount;
	long	m_WarningCount;
	long	m_FaultCount;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInserterPerf)
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

#endif // !defined(AFX_INSERTERPERF_H__51308BC5_D846_4D84_B707_6BF2DD5B99A9__INCLUDED_)
