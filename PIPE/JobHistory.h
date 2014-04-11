#if !defined(AFX_JOBHISTORY_H__57257F88_A9A2_46EF_8A78_6C7933B2EF2D__INCLUDED_)
#define AFX_JOBHISTORY_H__57257F88_A9A2_46EF_8A78_6C7933B2EF2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// JobHistory recordset

class JobHistory : public CRecordset
{
public:
	JobHistory(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(JobHistory)

// Field/Param Data
	//{{AFX_FIELD(JobHistory, CRecordset)

	long m_RefID;
  CString	m_JobID;
	CTime	m_PrintDate;
	int		m_Priority;
	CTime	m_DueDate;
	double	m_PlanQty;
	CString	m_CellID;
	long	m_Machine;
	CString	m_PartName;
	CString	m_PackCode;
	CString	m_Material;
	CString	m_Color;
	CString	m_DCI;
	CString	m_MoldName;
	CString	m_MoldCellID;
	long	m_CycleCount;
	double	m_StandardRate;
	CTime	m_ProdDoneDate;
	CTime	m_ClosedDate;
	double m_MeasuredQty;
	double m_CompleteQty;
	double m_ScrapQty;
	double	m_StandardCost;
	double	m_ResourceRate;
	double	m_OverHeadRate;
  double  m_MaterialCost;
	CString	m_RejectCode;
	float	m_RunTime;
	float	m_ProdTime;
	float	m_DownTime;
	CTime	m_StartDate;
	CTime	m_CompDate;
	CString	m_EmployeeID;
	CString	m_Status;
  unsigned char m_ModFlag;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JobHistory)
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

#endif // !defined(AFX_JOBHISTORY_H__57257F88_A9A2_46EF_8A78_6C7933B2EF2D__INCLUDED_)
