// MoldMachPerf.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "MoldMachPerf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoldMachPerf

IMPLEMENT_DYNAMIC(CMoldMachPerf, CRecordset)

CMoldMachPerf::CMoldMachPerf(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CMoldMachPerf)
  m_EqptNum = 0;
	m_Workorder = _T("");;
	m_ProductionTime = 0.0;
	m_DesignCycleTime = 0.0;
  m_ActualCycleTime = 0.0;
  m_InterruptCount = 0;
  m_InterruptDuration = _T("");
	m_SetPointChanges = 0;
	m_PurgeWeight = 0.0;
	m_PurgeDistance = 0.0;
	m_PurgeTime = 0.0;
  m_EventMessages = 0;
  m_EventWarnings = 0;
  m_EventFaults = 0;
	m_Comment = _T("");
	m_nFields = 16;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}
CString CMoldMachPerf::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptPerfHist");
}

CString CMoldMachPerf::GetDefaultSQL()
{
	return _T("[dbo].[MoldMachPerf]");
}
void CMoldMachPerf::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CMoldMachPerf)
	pFX->SetFieldType(CFieldExchange::outputColumn);
  RFX_Long(pFX, _T("[RefID]"), m_RefID);
  RFX_Long(pFX, _T("[EqptNum]"), m_EqptNum);
  RFX_Text(pFX, _T("[Workorder]"), m_Workorder);
  RFX_Double(pFX, _T("[ProductionTime]"), m_ProductionTime);
	RFX_Double(pFX, _T("[DesignCycleTime]"), m_DesignCycleTime);
	RFX_Double(pFX, _T("[ActualCycleTime]"), m_ActualCycleTime);
	RFX_Long(pFX, _T("[InterruptCount]"), m_InterruptCount);
	RFX_Text(pFX, _T("[InterruptDuration]"), m_InterruptDuration);
	RFX_Int(pFX, _T("[SetPointChanges]"), m_SetPointChanges);
	RFX_Double(pFX, _T("[PurgeWeight]"), m_PurgeWeight);
	RFX_Double(pFX, _T("[PurgeDistance]"), m_PurgeDistance);
	RFX_Double(pFX, _T("[PurgeTime]"), m_PurgeTime);
	RFX_Long(pFX, _T("[EventMessages]"), m_EventMessages);
	RFX_Long(pFX, _T("[EventWarnings]"), m_EventWarnings);
	RFX_Long(pFX, _T("[EventFaults]"), m_EventFaults);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CMoldMachPerf diagnostics

#ifdef _DEBUG
void CMoldMachPerf::AssertValid() const
{
	CRecordset::AssertValid();
}

void CMoldMachPerf::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
