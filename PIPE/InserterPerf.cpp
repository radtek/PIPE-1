// InserterPerf.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "InserterPerf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInserterPerf

IMPLEMENT_DYNAMIC(CInserterPerf, CRecordset)

CInserterPerf::CInserterPerf(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CInserterPerf)
	m_RefID = 0;
	m_EqptNum = 0;
	m_Workorder = _T("");
	m_CycleCount = 0;
	m_MessageCount = 0;
	m_WarningCount = 0;
	m_FaultCount = 0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CInserterPerf::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptPerfHist");
}

CString CInserterPerf::GetDefaultSQL()
{
	return _T("[dbo].[InserterPerf]");
}

void CInserterPerf::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CInserterPerf)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
	RFX_Date(pFX, _T("[LogTime]"), m_LogTime);
	RFX_Long(pFX, _T("[EqptNum]"), m_EqptNum);
	RFX_Text(pFX, _T("[Workorder]"), m_Workorder);
	RFX_Long(pFX, _T("[CycleCount]"), m_CycleCount);
	RFX_Long(pFX, _T("[MessageCount]"), m_MessageCount);
	RFX_Long(pFX, _T("[WarningCount]"), m_WarningCount);
	RFX_Long(pFX, _T("[FaultCount]"), m_FaultCount);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CInserterPerf diagnostics

#ifdef _DEBUG
void CInserterPerf::AssertValid() const
{
	CRecordset::AssertValid();
}

void CInserterPerf::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
