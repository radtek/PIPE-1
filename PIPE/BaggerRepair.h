#if !defined(AFX_BAGGERREPAIR_H__FF463CE9_419B_49B4_9B6E_B05A71DD612F__INCLUDED_)
#define AFX_BAGGERREPAIR_H__FF463CE9_419B_49B4_9B6E_B05A71DD612F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaggerRepair.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaggerRepair recordset

class CBaggerRepair : public CRecordset
{
public:
	CBaggerRepair(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CBaggerRepair)

// Field/Param Data
	//{{AFX_FIELD(CBaggerRepair, CRecordset)
	long	m_RefID;
	long	m_EqptID;
	CString	m_EqptName;
	CTime	m_TimeIn;
	long	m_ShopOrder;
	long	m_TotalCycles;
	long	m_RepairCycles;
	float	m_TotalRunTime;
	float	m_RepairTimeInterval;
	long	m_RepairItem1;
	long	m_RepairItem2;
	long	m_RepairItem3;
	long	m_RepairItem4;
	long	m_RepairItem5;
	CString	m_RepairDescription;
	BOOL	m_RepeatRepair;
	long	m_Assigned1;
	long	m_Assigned2;
	long	m_Assigned3;
	long	m_RepairAction1;
	long	m_RepairAction2;
	long	m_RepairAction3;
	long	m_RepairAction4;
	long	m_RepairAction5;
	CString	m_RepairComments;
	float	m_RepairTime;
	float	m_ToolroomTime;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaggerRepair)
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

#endif // !defined(AFX_BAGGERREPAIR_H__FF463CE9_419B_49B4_9B6E_B05A71DD612F__INCLUDED_)
