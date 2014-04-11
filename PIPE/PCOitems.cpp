// PCOitems.cpp : implementation file
//

#include "stdafx.h"
#include "PanCIM_ORACLE.h"
#include "PCOitems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCOitems

IMPLEMENT_DYNAMIC(CPCOitems, CRecordset)

CPCOitems::CPCOitems(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CPCOitems)
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
	m_PCOnum = 0;
	m_MachNum = 0;
	m_PartName = _T("");
	m_MoldName = _T("");
	m_Closed = FALSE;
	m_nFields = 4;
  // m_nFields = 18;

	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CPCOitems::GetDefaultConnect()
{
	return _T("ODBC;DSN=PRODSched");
}

CString CPCOitems::GetDefaultSQL()
{
	return _T("[PCOitem],[PCOmaster]");
}

void CPCOitems::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CPCOitems)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[PCOmaster.PCOnum]"), m_PCOnum);
	RFX_Long(pFX, _T("[PCOitem.PCOref]"), m_PCOref);
	RFX_Long(pFX, _T("[PCOitem.PCOkey]"), m_PCOkey);
	RFX_Long(pFX, _T("[PCOitem.QtyDone]"), m_QtyDone);
	//RFX_Int(pFX, _T("[TabNum]"), m_TabNum);
	//RFX_Int(pFX, _T("[TabPos]"), m_TabPos);
	//RFX_Int(pFX, _T("[RunSeq]"), m_RunSeq);
	//RFX_Text(pFX, _T("[Material]"), m_Material);
	//RFX_Text(pFX, _T("[Color]"), m_Color);
	//RFX_Text(pFX, _T("[PackCode]"), m_PackCode);
	//RFX_Long(pFX, _T("[QtyReq]"), m_QtyReq);
	//RFX_Long(pFX, _T("[QtyNeg]"), m_QtyNeg);
	//RFX_Bool(pFX, _T("[Complete]"), m_Complete);
	//RFX_Int(pFX, _T("[MachNum]"), m_MachNum);
	//RFX_Text(pFX, _T("[PartName]"), m_PartName);
	//RFX_Text(pFX, _T("[MoldName]"), m_MoldName);
	//RFX_Date(pFX, _T("[StartDate]"), m_StartDate);
	//RFX_Date(pFX, _T("[EndDate]"), m_EndDate);
	//RFX_Bool(pFX, _T("[Closed]"), m_Closed);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CPCOitems diagnostics

#ifdef _DEBUG
void CPCOitems::AssertValid() const
{
	CRecordset::AssertValid();
}

void CPCOitems::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
