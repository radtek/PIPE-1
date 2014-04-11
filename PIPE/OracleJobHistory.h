#if !defined(AFX_ORACLEJOBHISTORY_H__7E74C9A6_0D3B_4757_B238_079EC2703961__INCLUDED_)
#define AFX_ORACLEJOBHISTORY_H__7E74C9A6_0D3B_4757_B238_079EC2703961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OracleJobHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COracleJobHistory recordset

class COracleJobHistory : public CRecordset
{
public:
	COracleJobHistory(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(COracleJobHistory)

// Field/Param Data
	//{{AFX_FIELD(COracleJobHistory, CRecordset)
	CString	m_JobID;
	CTime	m_PrintDate;
	int		m_Priority;
	CTime	m_DueDate;
	CString	m_PlanQty;
	CString	m_CellID;
	long	m_Machine;
	CString	m_PartName;
	CString	m_PackCode;
	CString	m_Material;
	CString	m_Color;
	CString	m_DCI;
	CString	m_MoldName;
	CString	m_MoldCellID;
	CString	m_CycleCount;
	CString	m_StandardRate;
	CTime	m_ProdDoneDate;
	CTime	m_ClosedDate;
	CString	m_MeasuredQty;
	CString	m_CompleteQty;
	CString	m_ScrapQty;
	double	m_StandardCost;
	double	m_ResourceRate;
	double	m_OverHeadRate;
	CString	m_RejectCode;
	float	m_RunTime;
	float	m_ProdTime;
	float	m_DownTime;
	CTime	m_StartDate;
	CTime	m_CompDate;
	CString	m_EmployeeID;
	CString	m_Status;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COracleJobHistory)
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

#endif // !defined(AFX_ORACLEJOBHISTORY_H__7E74C9A6_0D3B_4757_B238_079EC2703961__INCLUDED_)
