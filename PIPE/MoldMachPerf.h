#if !defined(AFX_MOLDMACHPERF_H__01E852B5_7B1F_4617_A8A4_8546491BAE4E__INCLUDED_)
#define AFX_MOLDMACHPERF_H__01E852B5_7B1F_4617_A8A4_8546491BAE4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoldMachPerf.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoldMachPerf recordset

class CMoldMachPerf : public CRecordset
{
public:
	CMoldMachPerf(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMoldMachPerf)

// Field/Param Data
	//{{AFX_FIELD(CMoldMachPerf, CRecordset)
	long	  m_EqptNum;
	CString	m_Workorder;
	double	m_ProductionTime;
	double  m_DesignCycleTime;
  double  m_ActualCycleTime;
  long    m_InterruptCount;
  CString m_InterruptDuration;
	int		  m_SetPointChanges;
	double	m_PurgeWeight;
	double	m_PurgeDistance;
	double	m_PurgeTime;
  long    m_EventMessages;
  long    m_EventWarnings;
  long    m_EventFaults;
	CString	m_Comment;
	long	m_RefID;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoldMachPerf)
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

#endif // !defined(AFX_MOLDMACHPERF_H__01E852B5_7B1F_4617_A8A4_8546491BAE4E__INCLUDED_)
