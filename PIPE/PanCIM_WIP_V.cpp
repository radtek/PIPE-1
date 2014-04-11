// PanCIM_WIP_V.cpp : implementation file
//

#include "stdafx.h"
#include "PanCIM_WIP_V.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP_V

IMPLEMENT_DYNAMIC(PanCIM_WIP_V, CRecordset)

PanCIM_WIP_V::PanCIM_WIP_V(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(PanCIM_WIP_V)
	m_ORGANIZATION_ID = 0.0;
	m_WIP_ENTITY_ID = 0.0;
	m_CREATED_BY = 0.0;
	m_LAST_UPDATE_BY = 0.0;
	m_WIP_STATUS_CODE = 0.0;
	m_WIP_STATUS_FLAG = _T("");
  m_PANCIM_STATUS = _T("");
	m_DEPARTMENT_ID = 0;
  m_MACHINE_ID = 0;
	m_MACHINE_RESOURCE_FLAG = _T("");
	m_START_QTY = 0.0;
	m_START_QTY_FLAG = _T("");
	m_COMPLETED_QTY = 0.0;
	m_COMPLETED_QTY_FLAG = _T("");
	m_DATE_COMPLETED_FLAG = _T("");
	m_DATE_CLOSED_FLAG = _T("");
	m_ORGANIZATION_CODE = _T("");
	m_WIP_ENTITY_NAME = _T("");
	m_ITEM_CODE = _T("");
	m_ITEM_UOM = _T("");
	m_COLOR = _T("");
	m_MATERIAL_CODE = _T("");
	m_MATERIAL_UOM = _T("");
	m_DEPARTMENT_CODE = _T("");
	m_PRIORITY = 0.0;
	m_QUANTITY_SCRAPPED = 0.0;
	m_STANDARD_COST = 0.0;
	m_RESOURCE_RATE = 0.0;
	m_MATERIAL_RATE = 0.0;
	m_MATERIAL_OVERHEAD_RATE = 0.0;
	m_OVERHEAD_RATE = 0.0;
	m_OUTSIDE_PROCESS_RATE = 0.0;
	m_EMPLOYEE_ID = _T("");
	m_WIP_JOB_STATUS = _T("");
	m_MOLD_NAME = _T("");
	m_MOLD_BASE = _T("");
	m_MACHINE_NAME = _T("");
	m_ACTUAL_CAVITIES = 0.0;
	m_DESIGN_CAVITIES = 0.0;
  m_MOLD_ID_FLAG = _T("");
	m_nFields = 47;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString PanCIM_WIP_V::GetDefaultConnect()
{
	return _T("ODBC;DSN=PanCIM_WIP_V");
}

CString PanCIM_WIP_V::GetDefaultSQL()
{
	return _T("[APPS].[PANDUIT_WIP_PANCIM_JOB_V]");
}

void PanCIM_WIP_V::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(PanCIM_WIP_V)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Double(pFX, _T("[ORGANIZATION_ID]"), m_ORGANIZATION_ID);
	RFX_Double(pFX, _T("[WIP_ENTITY_ID]"), m_WIP_ENTITY_ID);
	RFX_Date(pFX, _T("[CREATION_DATE]"), m_CREATION_DATE);
	RFX_Double(pFX, _T("[CREATED_BY]"), m_CREATED_BY);
	RFX_Date(pFX, _T("[LAST_UPDATE_DATE]"), m_LAST_UPDATE_DATE);
	RFX_Double(pFX, _T("[LAST_UPDATE_BY]"), m_LAST_UPDATE_BY);
	RFX_Double(pFX, _T("[WIP_STATUS_CODE]"), m_WIP_STATUS_CODE);
	RFX_Text(pFX, _T("[WIP_STATUS_FLAG]"), m_WIP_STATUS_FLAG);
	RFX_Text(pFX, _T("[PANCIM_STATUS]"), m_PANCIM_STATUS);
	RFX_Long(pFX, _T("[DEPARTMENT_ID]"), m_DEPARTMENT_ID);
  RFX_Long(pFX, _T("[MACHINE_ID]"), m_MACHINE_ID);
	RFX_Text(pFX, _T("[MACHINE_RESOURCE_FLAG]"), m_MACHINE_RESOURCE_FLAG);
	RFX_Double(pFX, _T("[START_QTY]"), m_START_QTY);
	RFX_Text(pFX, _T("[START_QTY_FLAG]"), m_START_QTY_FLAG);
	RFX_Double(pFX, _T("[COMPLETED_QTY]"), m_COMPLETED_QTY);
	RFX_Text(pFX, _T("[COMPLETED_QTY_FLAG]"), m_COMPLETED_QTY_FLAG);
	RFX_Date(pFX, _T("[DATE_COMPLETED]"), m_DATE_COMPLETED);
	RFX_Text(pFX, _T("[DATE_COMPLETED_FLAG]"), m_DATE_COMPLETED_FLAG);
	RFX_Date(pFX, _T("[DATE_CLOSED]"), m_DATE_CLOSED);
	RFX_Text(pFX, _T("[DATE_CLOSED_FLAG]"), m_DATE_CLOSED_FLAG);
	RFX_Text(pFX, _T("[ORGANIZATION_CODE]"), m_ORGANIZATION_CODE);
	RFX_Text(pFX, _T("[WIP_ENTITY_NAME]"), m_WIP_ENTITY_NAME);
	RFX_Text(pFX, _T("[ITEM_CODE]"), m_ITEM_CODE);
	RFX_Text(pFX, _T("[ITEM_UOM]"), m_ITEM_UOM);
	RFX_Text(pFX, _T("[COLOR]"), m_COLOR);
	RFX_Text(pFX, _T("[MATERIAL_CODE]"), m_MATERIAL_CODE);
	RFX_Text(pFX, _T("[MATERIAL_UOM]"), m_MATERIAL_UOM);
  RFX_Text(pFX, _T("[DEPARTMENT_CODE]"), m_DEPARTMENT_CODE);
  RFX_Double(pFX, _T("[PRIORITY]"), m_PRIORITY);
	RFX_Double(pFX, _T("[QUANTITY_SCRAPPED]"), m_QUANTITY_SCRAPPED);
	RFX_Date(pFX, _T("[SCHED_START_DATE]"), m_SCHED_START_DATE);
	RFX_Date(pFX, _T("[SCHED_COMPLETION_DATE]"), m_SCHED_COMPLETION_DATE);
	RFX_Date(pFX, _T("[PRINT_DATE]"), m_PRINT_DATE);
	RFX_Double(pFX, _T("[STANDARD_COST]"), m_STANDARD_COST);
	RFX_Double(pFX, _T("[RESOURCE_RATE]"), m_RESOURCE_RATE);
	RFX_Double(pFX, _T("[MATERIAL_RATE]"), m_MATERIAL_RATE);
	RFX_Double(pFX, _T("[MATERIAL_OVERHEAD_RATE]"), m_MATERIAL_OVERHEAD_RATE);
	RFX_Double(pFX, _T("[OVERHEAD_RATE]"), m_OVERHEAD_RATE);
	RFX_Double(pFX, _T("[OUTSIDE_PROCESS_RATE]"), m_OUTSIDE_PROCESS_RATE);
	RFX_Text(pFX, _T("[EMPLOYEE_ID]"), m_EMPLOYEE_ID);
	RFX_Text(pFX, _T("[WIP_JOB_STATUS]"), m_WIP_JOB_STATUS);
	RFX_Text(pFX, _T("[MOLD_NAME]"), m_MOLD_NAME);
	RFX_Text(pFX, _T("[MOLD_BASE]"), m_MOLD_BASE);
	RFX_Text(pFX, _T("[MACHINE_NAME]"), m_MACHINE_NAME);
	RFX_Double(pFX, _T("[ACTUAL_CAVITIES]"), m_ACTUAL_CAVITIES);
	RFX_Double(pFX, _T("[DESIGN_CAVITIES]"), m_DESIGN_CAVITIES);
	RFX_Text(pFX, _T("[MOLD_ID_FLAG]"), m_MOLD_ID_FLAG);

	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP_V diagnostics

#ifdef _DEBUG
void PanCIM_WIP_V::AssertValid() const
{
	CRecordset::AssertValid();
}

void PanCIM_WIP_V::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
