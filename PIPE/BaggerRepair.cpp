// BaggerRepair.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "BaggerRepair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaggerRepair

IMPLEMENT_DYNAMIC(CBaggerRepair, CRecordset)

CBaggerRepair::CBaggerRepair(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBaggerRepair)
	m_RefID = 0;
	m_EqptID = 0;
	m_EqptName = _T("");
	m_ShopOrder = 0;
	m_TotalCycles = 0;
	m_RepairCycles = 0;
	m_TotalRunTime = 0.0f;
	m_RepairTimeInterval = 0.0f;
	m_RepairItem1 = 0;
	m_RepairItem2 = 0;
	m_RepairItem3 = 0;
	m_RepairItem4 = 0;
	m_RepairItem5 = 0;
	m_RepairDescription = _T("");
	m_RepeatRepair = FALSE;
	m_Assigned1 = 0;
	m_Assigned2 = 0;
	m_Assigned3 = 0;
	m_RepairAction1 = 0;
	m_RepairAction2 = 0;
	m_RepairAction3 = 0;
	m_RepairAction4 = 0;
	m_RepairAction5 = 0;
	m_RepairComments = _T("");
	m_RepairTime = 0.0f;
	m_ToolroomTime = 0.0f;
	m_nFields = 27;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CBaggerRepair::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptRepHist");
}

CString CBaggerRepair::GetDefaultSQL()
{
	return _T("[dbo].[BaggerRepairHist]");
}

void CBaggerRepair::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CBaggerRepair)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
	RFX_Long(pFX, _T("[EqptID]"), m_EqptID);
	RFX_Text(pFX, _T("[EqptName]"), m_EqptName);
	RFX_Date(pFX, _T("[TimeIn]"), m_TimeIn);
	RFX_Long(pFX, _T("[ShopOrder]"), m_ShopOrder);
	RFX_Long(pFX, _T("[TotalCycles]"), m_TotalCycles);
	RFX_Long(pFX, _T("[RepairCycles]"), m_RepairCycles);
	RFX_Single(pFX, _T("[TotalRunTime]"), m_TotalRunTime);
	RFX_Single(pFX, _T("[RepairTimeInterval]"), m_RepairTimeInterval);
	RFX_Long(pFX, _T("[RepairItem1]"), m_RepairItem1);
	RFX_Long(pFX, _T("[RepairItem2]"), m_RepairItem2);
	RFX_Long(pFX, _T("[RepairItem3]"), m_RepairItem3);
	RFX_Long(pFX, _T("[RepairItem4]"), m_RepairItem4);
	RFX_Long(pFX, _T("[RepairItem5]"), m_RepairItem5);
	RFX_Text(pFX, _T("[RepairDescription]"), m_RepairDescription);
	RFX_Bool(pFX, _T("[RepeatRepair]"), m_RepeatRepair);
	RFX_Long(pFX, _T("[Assigned1]"), m_Assigned1);
	RFX_Long(pFX, _T("[Assigned2]"), m_Assigned2);
	RFX_Long(pFX, _T("[Assigned3]"), m_Assigned3);
	RFX_Long(pFX, _T("[RepairAction1]"), m_RepairAction1);
	RFX_Long(pFX, _T("[RepairAction2]"), m_RepairAction2);
	RFX_Long(pFX, _T("[RepairAction3]"), m_RepairAction3);
	RFX_Long(pFX, _T("[RepairAction4]"), m_RepairAction4);
	RFX_Long(pFX, _T("[RepairAction5]"), m_RepairAction5);
	RFX_Text(pFX, _T("[RepairComments]"), m_RepairComments);
	RFX_Single(pFX, _T("[RepairTime]"), m_RepairTime);
	RFX_Single(pFX, _T("[ToolroomTime]"), m_ToolroomTime);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CBaggerRepair diagnostics

#ifdef _DEBUG
void CBaggerRepair::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBaggerRepair::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
