// OracleDual.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "OracleDual.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COracleDual

IMPLEMENT_DYNAMIC(COracleDual, CRecordset)

COracleDual::COracleDual(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(COracleDual)
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString COracleDual::GetDefaultConnect()
{
	return _T("ODBC;DSN=OracleDual");
}

CString COracleDual::GetDefaultSQL()
{
	return _T("");
}

void COracleDual::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(COracleDual)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// COracleDual diagnostics

#ifdef _DEBUG
void COracleDual::AssertValid() const
{
	CRecordset::AssertValid();
}

void COracleDual::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
