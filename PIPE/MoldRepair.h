#if !defined(AFX_MOLDREPAIR_H__36085355_31B7_4853_BA67_53E36C8204F6__INCLUDED_)
#define AFX_MOLDREPAIR_H__36085355_31B7_4853_BA67_53E36C8204F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoldRepair.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMoldRepair recordset

class CMoldRepair : public CRecordset
{
public:
	CMoldRepair(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CMoldRepair)

// Field/Param Data
	//{{AFX_FIELD(CMoldRepair, CRecordset)
	long	m_RefID;
	CString	m_TrackID;
	CString	m_MoldName;
	CTime	m_TimeIn;
	long	m_ShopOrder;
	long	m_Machine;
	long	m_CycleCount;
	long	m_RepairItem1;
	long	m_RepairItem2;
	long	m_RepairItem3;
	CString	m_RepairDescription;
	BOOL	m_RepeatRepair;
	long	m_Assigned1;
	long	m_Assigned2;
	long	m_Assigned3;
	long	m_RepairAction1;
	long	m_RepairAction2;
	long	m_RepairAction3;
	CString	m_RepairComments;
	double	m_RepairTime;
	double	m_ToolroomTime;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMoldRepair)
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

#endif // !defined(AFX_MOLDREPAIR_H__36085355_31B7_4853_BA67_53E36C8204F6__INCLUDED_)
