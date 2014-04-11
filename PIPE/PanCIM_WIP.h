#if !defined(AFX_PANCIM_WIP_H__C0FA8C5D_CEC3_4D78_94CA_751DFE1E12BB__INCLUDED_)
#define AFX_PANCIM_WIP_H__C0FA8C5D_CEC3_4D78_94CA_751DFE1E12BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PanCIM_WIP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP recordset

class PanCIM_WIP : public CRecordset
{
public:
	PanCIM_WIP(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(PanCIM_WIP)

// Field/Param Data
	//{{AFX_FIELD(PanCIM_WIP, CRecordset)
	double	m_ORGANIZATION_ID;
	double	m_WIP_ENTITY_ID;
	CTime	m_CREATION_DATE;
	double	m_CREATED_BY;
	CTime	m_LAST_UPDATE_DATE;
	double	m_LAST_UPDATE_BY;
	double	m_WIP_STATUS_CODE;
	CString	m_WIP_STATUS_FLAG;
	CString	m_PANCIM_STATUS;
	double	m_MATERIAL_ID;
	double	m_DEPARTMENT_ID;
	double	m_MACHINE_ID;
	CString	m_MACHINE_RESOURCE_FLAG;
	double	m_MOLD_ID;
	double	m_START_QTY;
	CString	m_START_QTY_FLAG;
	double	m_COMPLETED_QTY;
	CString	m_COMPLETED_QTY_FLAG;
	double	m_MEASURED_QTY;
	CTime	m_DATE_COMPLETED;
	CString	m_DATE_COMPLETED_FLAG;
	CTime	m_DATE_CLOSED;
	CString	m_DATE_CLOSED_FLAG;
	CString	m_MOLD_ID_FLAG;

	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PanCIM_WIP)
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

#endif // !defined(AFX_PANCIM_WIP_H__C0FA8C5D_CEC3_4D78_94CA_751DFE1E12BB__INCLUDED_)
