// MDS_Inven.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "MDS_Inven.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MDS_Inven

IMPLEMENT_DYNAMIC(MDS_Inven, CRecordset)

MDS_Inven::MDS_Inven(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(MDS_Inven)
	m_TRANSACTION_INTERFACE_ID = 0.0;
	m_LAST_UPDATED_BY = 0.0;
	m_CREATED_BY = 0.0;
	m_LOT_NUMBER = _T("");
	m_TRANSACTION_QUANTITY = 0.0;
	m_PRIMARY_QUANTITY = 0.0;
	m_nFields = 8;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString MDS_Inven::GetDefaultConnect()
{
	return _T("ODBC;DSN=MDS_INVEN");
}

CString MDS_Inven::GetDefaultSQL()
{
	return _T("[INV].[MTL_TRANSACTION_LOTS_INTERFACE]");
}

void MDS_Inven::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(MDS_Inven)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Double(pFX, _T("[TRANSACTION_INTERFACE_ID]"), m_TRANSACTION_INTERFACE_ID);
	RFX_Date(pFX, _T("[LAST_UPDATE_DATE]"), m_LAST_UPDATE_DATE);
	RFX_Double(pFX, _T("[LAST_UPDATED_BY]"), m_LAST_UPDATED_BY);
	RFX_Date(pFX, _T("[CREATION_DATE]"), m_CREATION_DATE);
	RFX_Double(pFX, _T("[CREATED_BY]"), m_CREATED_BY);
  RFX_Text(pFX, _T("[LOT_NUMBER]"), m_LOT_NUMBER);
	RFX_Double(pFX, _T("[TRANSACTION_QUANTITY]"), m_TRANSACTION_QUANTITY);
  RFX_Double(pFX, _T("[PRIMARY_QUANTITY]"), m_PRIMARY_QUANTITY);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// MDS_Inven diagnostics

#ifdef _DEBUG
void MDS_Inven::AssertValid() const
{
	CRecordset::AssertValid();
}

void MDS_Inven::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
