// JobPerformance.cpp : implementation file
//

#include "stdafx.h"
#include "JobPerformance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JobPerformance

IMPLEMENT_DYNAMIC(JobPerformance, CRecordset)

JobPerformance::JobPerformance(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(JobPerformance)
	m_RefID = 0;
  m_JobID = _T("");
  m_Machine = 0;
	m_SystemID = _T("");
	m_DesignCycleTime = 0.0f;
	m_ActualCycleTime = 0.0f;
	m_ProductionTime = 0.0f;
	m_InterruptDuration = _T("");
	m_InterruptCount = 0;
	m_EventMessages = 0;
	m_EventWarnings = 0;
	m_EventFaults = 0;
	m_PurgeWeight = 0.0f;
  m_PurgeDistance = 0.0f;
  m_PurgeTime = 0.0f;
	m_SetPointChanges = 0;
	m_Yield = 0.0f;
	m_Efficiency = 0.0f;
	m_Availability = 0.0f;
	m_SysEffectiveness = 0.0f;
	m_nFields = 20;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString JobPerformance::GetDefaultConnect()
{
	return _T("ODBC;DSN=ProdJobHist");
}

CString JobPerformance::GetDefaultSQL()
{
	return _T("[dbo].[JobPerformance]");
}

void JobPerformance::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(JobPerformance)
	pFX->SetFieldType(CFieldExchange::outputColumn);
  RFX_Long(pFX, _T("[RefID]"), m_RefID);
  RFX_Text(pFX, _T("[JobID]"), m_JobID);
	RFX_Long(pFX, _T("[Machine]"), m_Machine);
	RFX_Text(pFX, _T("[SystemID]"), m_SystemID);
	RFX_Single(pFX, _T("[DesignCycleTime]"), m_DesignCycleTime);
	RFX_Single(pFX, _T("[ActualCycleTime]"), m_ActualCycleTime);
	RFX_Single(pFX, _T("[ProductionTime]"), m_ProductionTime);
	RFX_Text(pFX, _T("[InterruptDuration]"), m_InterruptDuration);
	RFX_Long(pFX, _T("[InterruptCount]"), m_InterruptCount);
	RFX_Long(pFX, _T("[EventMessages]"), m_EventMessages);
	RFX_Long(pFX, _T("[EventWarnings]"), m_EventWarnings);
	RFX_Long(pFX, _T("[EventFaults]"), m_EventFaults);
	RFX_Single(pFX, _T("[PurgeWeight]"), m_PurgeWeight);
	RFX_Single(pFX, _T("[PurgeDistance]"), m_PurgeDistance);
	RFX_Single(pFX, _T("[PurgeTime]"), m_PurgeTime);
	RFX_Long(pFX, _T("[SetPointChanges]"), m_SetPointChanges);
	RFX_Single(pFX, _T("[Yield]"), m_Yield);
	RFX_Single(pFX, _T("[Efficiency]"), m_Efficiency);
	RFX_Single(pFX, _T("[Availability]"), m_Availability);
	RFX_Single(pFX, _T("[SysEffectiveness]"), m_SysEffectiveness);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// JobPerformance diagnostics

#ifdef _DEBUG
void JobPerformance::AssertValid() const
{
	CRecordset::AssertValid();
}

void JobPerformance::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
