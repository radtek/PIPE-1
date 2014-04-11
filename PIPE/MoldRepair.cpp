// MoldRepair.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "MoldRepair.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoldRepair

IMPLEMENT_DYNAMIC(CMoldRepair, CRecordset)

CMoldRepair::CMoldRepair(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CMoldRepair)
	m_RefID = 0;
	m_TrackID = _T("");
	m_MoldName = _T("");
	m_ShopOrder = 0;
	m_Machine = 0;
	m_CycleCount = 0;
	m_RepairItem1 = 0;
	m_RepairItem2 = 0;
	m_RepairItem3 = 0;
	m_RepairDescription = _T("");
	m_RepeatRepair = FALSE;
	m_Assigned1 = 0;
	m_Assigned2 = 0;
	m_Assigned3 = 0;
	m_RepairAction1 = 0;
	m_RepairAction2 = 0;
	m_RepairAction3 = 0;
	m_RepairComments = _T("");
	m_RepairTime = 0.0;
	m_ToolroomTime = 0.0;
	m_nFields = 21;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CMoldRepair::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptRepHist");
}

CString CMoldRepair::GetDefaultSQL()
{
	return _T("[dbo].[MoldRepairHist]");
}

void CMoldRepair::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CMoldRepair)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
	RFX_Text(pFX, _T("[TrackID]"), m_TrackID);
	RFX_Text(pFX, _T("[MoldName]"), m_MoldName);
	RFX_Date(pFX, _T("[TimeIn]"), m_TimeIn);
	RFX_Long(pFX, _T("[ShopOrder]"), m_ShopOrder);
	RFX_Long(pFX, _T("[Machine]"), m_Machine);
	RFX_Long(pFX, _T("[CycleCount]"), m_CycleCount);
	RFX_Long(pFX, _T("[RepairItem1]"), m_RepairItem1);
	RFX_Long(pFX, _T("[RepairItem2]"), m_RepairItem2);
	RFX_Long(pFX, _T("[RepairItem3]"), m_RepairItem3);
	RFX_Text(pFX, _T("[RepairDescription]"), m_RepairDescription);
	RFX_Bool(pFX, _T("[RepeatRepair]"), m_RepeatRepair);
	RFX_Long(pFX, _T("[Assigned1]"), m_Assigned1);
	RFX_Long(pFX, _T("[Assigned2]"), m_Assigned2);
	RFX_Long(pFX, _T("[Assigned3]"), m_Assigned3);
	RFX_Long(pFX, _T("[RepairAction1]"), m_RepairAction1);
	RFX_Long(pFX, _T("[RepairAction2]"), m_RepairAction2);
	RFX_Long(pFX, _T("[RepairAction3]"), m_RepairAction3);
	RFX_Text(pFX, _T("[RepairComments]"), m_RepairComments);
	RFX_Double(pFX, _T("[RepairTime]"), m_RepairTime);
	RFX_Double(pFX, _T("[ToolroomTime]"), m_ToolroomTime);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMoldRepair diagnostics

#ifdef _DEBUG
void CMoldRepair::AssertValid() const
{
	CRecordset::AssertValid();
}

void CMoldRepair::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
