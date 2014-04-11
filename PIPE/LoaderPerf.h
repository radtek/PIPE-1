#if !defined(AFX_LOADERPERF_H__34E09FCD_3429_4EA8_A194_40F271A4065C__INCLUDED_)
#define AFX_LOADERPERF_H__34E09FCD_3429_4EA8_A194_40F271A4065C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoaderPerf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoaderPerf recordset

class CLoaderPerf : public CRecordset
{
public:
	CLoaderPerf(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CLoaderPerf)

// Field/Param Data
	//{{AFX_FIELD(CLoaderPerf, CRecordset)
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
	//{{AFX_VIRTUAL(CLoaderPerf)
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

#endif // !defined(AFX_LOADERPERF_H__34E09FCD_3429_4EA8_A194_40F271A4065C__INCLUDED_)
