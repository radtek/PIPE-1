#if !defined(AFX_PANCIM_WIP_V_H__A4441AB2_9105_4394_A6E8_1247633D40BF__INCLUDED_)
#define AFX_PANCIM_WIP_V_H__A4441AB2_9105_4394_A6E8_1247633D40BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanCIM_WIP_V.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP_V recordset

class PanCIM_WIP_V : public CRecordset
{
public:
	PanCIM_WIP_V(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(PanCIM_WIP_V)

// Field/Param Data
	//{{AFX_FIELD(PanCIM_WIP_V, CRecordset)
	double	m_ORGANIZATION_ID;
	double	m_WIP_ENTITY_ID;
	CTime	m_CREATION_DATE;
	double	m_CREATED_BY;
	CTime	m_LAST_UPDATE_DATE;
	double	m_LAST_UPDATE_BY;
	double	m_WIP_STATUS_CODE;
	CString	m_WIP_STATUS_FLAG;
  CString m_PANCIM_STATUS;
  long    m_DEPARTMENT_ID;
	long    m_MACHINE_ID;
	CString	m_MACHINE_RESOURCE_FLAG;
	double	m_START_QTY;
	CString	m_START_QTY_FLAG;
	double	m_COMPLETED_QTY;
	CString	m_COMPLETED_QTY_FLAG;
 	double	m_MEASURED_QTY;
	CTime	m_DATE_COMPLETED;
	CString	m_DATE_COMPLETED_FLAG;
	CTime	m_DATE_CLOSED;
	CString	m_DATE_CLOSED_FLAG;
	CString	m_ORGANIZATION_CODE;
	CString	m_WIP_ENTITY_NAME;
	CString	m_ITEM_CODE;
	CString	m_ITEM_UOM;
	CString	m_COLOR;
	CString	m_MATERIAL_CODE;
	CString	m_MATERIAL_UOM;
  CString m_DEPARTMENT_CODE;
	double	m_PRIORITY;
	double	m_QUANTITY_SCRAPPED;
	CTime	m_SCHED_START_DATE;
	CTime	m_SCHED_COMPLETION_DATE;
	CTime	m_PRINT_DATE;
	double	m_STANDARD_COST;
	double	m_RESOURCE_RATE;
	double	m_MATERIAL_RATE;
	double	m_MATERIAL_OVERHEAD_RATE;
	double	m_OVERHEAD_RATE;
	double	m_OUTSIDE_PROCESS_RATE;
	CString	m_EMPLOYEE_ID;
	CString	m_WIP_JOB_STATUS;
	CString	m_MOLD_NAME;
	CString	m_MOLD_BASE;
	CString	m_MACHINE_NAME;
  double  m_DESIGN_CAVITIES;
  double  m_ACTUAL_CAVITIES;
	CString	m_MOLD_ID_FLAG;

	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PanCIM_WIP_V)
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

#endif // !defined(AFX_PANCIM_WIP_V_H__A4441AB2_9105_4394_A6E8_1247633D40BF__INCLUDED_)
