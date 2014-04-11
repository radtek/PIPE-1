// PCO_UpdateQty.cpp : implementation file
//

#include "stdafx.h"
#include "PanCIM_Oracle.h"
#include "PCO_UpdateQty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCO_UpdateQty

IMPLEMENT_DYNAMIC(CPCO_UpdateQty, CRecordset)

CPCO_UpdateQty::CPCO_UpdateQty(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPCO_UpdateQty)
	m_PCOkey = 0;
	m_PCOref = 0;
	m_TabNum = 0;
	m_TabPos = 0;
	m_RunSeq = 0;
	m_Material = _T("");
	m_Color = _T("");
	m_PackCode = _T("");
	m_QtyReq = 0;
	m_QtyNeg = 0;
	m_QtyDone = 0;
	m_Complete = FALSE;
	m_nFields = 12;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CPCO_UpdateQty::GetDefaultConnect()
{
	return _T("ODBC;DSN=PRODSched");
}

CString CPCO_UpdateQty::GetDefaultSQL()
{
	return _T("[PCOitem]");
}

void CPCO_UpdateQty::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPCO_UpdateQty)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[PCOkey]"), m_PCOkey);
	RFX_Long(pFX, _T("[PCOref]"), m_PCOref);
	RFX_Int(pFX, _T("[TabNum]"), m_TabNum);
	RFX_Int(pFX, _T("[TabPos]"), m_TabPos);
	RFX_Int(pFX, _T("[RunSeq]"), m_RunSeq);
	RFX_Text(pFX, _T("[Material]"), m_Material);
	RFX_Text(pFX, _T("[Color]"), m_Color);
	RFX_Text(pFX, _T("[PackCode]"), m_PackCode);
	RFX_Long(pFX, _T("[QtyReq]"), m_QtyReq);
	RFX_Long(pFX, _T("[QtyNeg]"), m_QtyNeg);
	RFX_Long(pFX, _T("[QtyDone]"), m_QtyDone);
	RFX_Bool(pFX, _T("[Complete]"), m_Complete);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPCO_UpdateQty diagnostics

#ifdef _DEBUG
void CPCO_UpdateQty::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPCO_UpdateQty::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
