// BaggerPerf.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "BaggerPerf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaggerPerf

IMPLEMENT_DYNAMIC(CBaggerPerf, CRecordset)

CBaggerPerf::CBaggerPerf(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CBaggerPerf)
	m_RefID = 0;
	m_EqptNum = 0;
	m_Workorder = _T("");
	m_BagCount = 0;
	m_DivertCount = 0;
	m_MessageCount = 0;
	m_WarningCount = 0;
	m_FaultCount = 0;
	m_nFields = 9;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CBaggerPerf::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptPerfHist");
}

CString CBaggerPerf::GetDefaultSQL()
{
	return _T("[dbo].[BaggerPerf]");
}

void CBaggerPerf::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CBaggerPerf)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
	RFX_Date(pFX, _T("[LogTime]"), m_LogTime);
	RFX_Long(pFX, _T("[EqptNum]"), m_EqptNum);
	RFX_Text(pFX, _T("[Workorder]"), m_Workorder);
	RFX_Long(pFX, _T("[BagCount]"), m_BagCount);
	RFX_Long(pFX, _T("[DivertCount]"), m_DivertCount);
	RFX_Long(pFX, _T("[MessageCount]"), m_MessageCount);
	RFX_Long(pFX, _T("[WarningCount]"), m_WarningCount);
	RFX_Long(pFX, _T("[FaultCount]"), m_FaultCount);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CBaggerPerf diagnostics

#ifdef _DEBUG
void CBaggerPerf::AssertValid() const
{
	CRecordset::AssertValid();
}

void CBaggerPerf::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
