// JobRefID.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "JobRefID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJobRefID

IMPLEMENT_DYNAMIC(CJobRefID, CRecordset)

CJobRefID::CJobRefID(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CJobRefID)
  m_RefID = 0;
  // m_JobID = _T("");
	m_nFields = 1;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CJobRefID::GetDefaultConnect()
{
	return _T("ODBC;DSN=OracleJobHistory");
}

CString CJobRefID::GetDefaultSQL()
{
	return _T("[JobHistory]");
}

void CJobRefID::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CJobRefID)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
  // RFX_Text(pFX, _T("[JobID]"), m_JobID);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CJobRefID diagnostics

#ifdef _DEBUG
void CJobRefID::AssertValid() const
{
	CRecordset::AssertValid();
}

void CJobRefID::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
