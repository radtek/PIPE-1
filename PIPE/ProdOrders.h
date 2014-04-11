#if !defined(AFX_PRODORDERS_H__6E688BFA_F92A_11D4_B7FD_00B0D0226342__INCLUDED_)
#define AFX_PRODORDERS_H__6E688BFA_F92A_11D4_B7FD_00B0D0226342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProdOrders.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProdOrders recordset

class CProdOrders : public CRecordset
{
public:
	CProdOrders(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CProdOrders)

// Field/Param Data
	//{{AFX_FIELD(CProdOrders, CRecordset)
	CTime	m_UpdateTime;
	CString	m_Workorder;
	int		m_MachNum;
	CString	m_PartName;
	CString	m_PackCode;
	CString	m_MoldName;
	long	m_ProdQty;
	long	m_ScrapQty;
  long  m_EstQty;
	double	m_RunHrs;
	long	m_CycleCount;
  double m_StandardRate;
	int		m_ShiftNum;
	int		m_EmpNum;
	long	m_PCOnum;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdOrders)
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

#endif // !defined(AFX_PRODORDERS_H__6E688BFA_F92A_11D4_B7FD_00B0D0226342__INCLUDED_)
