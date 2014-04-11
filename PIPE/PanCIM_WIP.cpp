// PanCIM_WIP.cpp : implementation file
//

#include "stdafx.h"
#include "PanCIM_WIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP

IMPLEMENT_DYNAMIC(PanCIM_WIP, CRecordset)

PanCIM_WIP::PanCIM_WIP(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(PanCIM_WIP)
	m_ORGANIZATION_ID = 0.0;
	m_WIP_ENTITY_ID = 0.0;
	m_CREATED_BY = 0.0;
	m_LAST_UPDATE_BY = 0.0;
	m_WIP_STATUS_CODE = 0.0;
	m_WIP_STATUS_FLAG = _T("");
	m_MATERIAL_ID = 0.0;
	m_DEPARTMENT_ID = 0.0;
	m_MACHINE_ID = 0;
	m_MACHINE_RESOURCE_FLAG = _T("");
	m_MOLD_ID = 0.0;
	m_START_QTY = 0.0;
	m_START_QTY_FLAG = _T("");
	m_COMPLETED_QTY = 0.0;
	m_COMPLETED_QTY_FLAG = _T("");
	m_MEASURED_QTY = 0.0;
	m_DATE_COMPLETED_FLAG = _T("");
	m_DATE_CLOSED_FLAG = _T("");
	m_MOLD_ID_FLAG = _T("");
	m_nFields = 24;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString PanCIM_WIP::GetDefaultConnect()
{
	return _T("ODBC;DSN=PanCIM_WIP");
}

CString PanCIM_WIP::GetDefaultSQL()
{
	return _T("[PANDUIT].[PANDUIT_WIP_PANCIM_JOB]");
}

void PanCIM_WIP::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(PanCIM_WIP)
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
	RFX_Double(pFX, _T("[MATERIAL_ID]"), m_MATERIAL_ID);
	RFX_Double(pFX, _T("[DEPARTMENT_ID]"), m_DEPARTMENT_ID);
	RFX_Double(pFX, _T("[MACHINE_ID]"), m_MACHINE_ID);
	RFX_Text(pFX, _T("[MACHINE_RESOURCE_FLAG]"), m_MACHINE_RESOURCE_FLAG);
	RFX_Double(pFX, _T("[MOLD_ID]"), m_MOLD_ID);
	RFX_Double(pFX, _T("[START_QTY]"), m_START_QTY);
	RFX_Text(pFX, _T("[START_QTY_FLAG]"), m_START_QTY_FLAG);
	RFX_Double(pFX, _T("[COMPLETED_QTY]"), m_COMPLETED_QTY);
	RFX_Text(pFX, _T("[COMPLETED_QTY_FLAG]"), m_COMPLETED_QTY_FLAG);
	RFX_Double(pFX, _T("[MEASURED_QTY]"), m_MEASURED_QTY);
	RFX_Date(pFX, _T("[DATE_COMPLETED]"), m_DATE_COMPLETED);
	RFX_Text(pFX, _T("[DATE_COMPLETED_FLAG]"), m_DATE_COMPLETED_FLAG);
	RFX_Date(pFX, _T("[DATE_CLOSED]"), m_DATE_CLOSED);
	RFX_Text(pFX, _T("[DATE_CLOSED_FLAG]"), m_DATE_CLOSED_FLAG);
	RFX_Text(pFX, _T("[MOLD_ID_FLAG]"), m_MOLD_ID_FLAG);

	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// PanCIM_WIP diagnostics

#ifdef _DEBUG
void PanCIM_WIP::AssertValid() const
{
	CRecordset::AssertValid();
}

void PanCIM_WIP::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


