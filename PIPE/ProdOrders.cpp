// ProdOrders.cpp : implementation file
//

#include "stdafx.h"
#include "PanCIM_Oracle.h"
#include "ProdOrders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProdOrders

IMPLEMENT_DYNAMIC(CProdOrders, CRecordset)

CProdOrders::CProdOrders(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CProdOrders)
	m_Workorder = _T("");
	m_MachNum = 0;
	m_PartName = _T("");
	m_PackCode = _T("");
	m_MoldName = _T("");
	m_ProdQty = 0;
	m_ScrapQty = 0;
  m_EstQty = 0;
	m_RunHrs = 0.0;
	m_CycleCount = 0;
  m_StandardRate = 0.0;
	m_ShiftNum = 0;
	m_EmpNum = 0;
	m_PCOnum = 0;
	m_nFields = 15;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CProdOrders::GetDefaultConnect()
{
	return _T("ODBC;DSN=PRODSched");
}

CString CProdOrders::GetDefaultSQL()
{
	return _T("[ProdOrders]");
}

void CProdOrders::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CProdOrders)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Date(pFX, _T("[UpdateTime]"), m_UpdateTime);
	RFX_Text(pFX, _T("[Workorder]"), m_Workorder);
	RFX_Int(pFX, _T("[MachNum]"), m_MachNum);
	RFX_Text(pFX, _T("[PartName]"), m_PartName);
	RFX_Text(pFX, _T("[PackCode]"), m_PackCode);
	RFX_Text(pFX, _T("[MoldName]"), m_MoldName);
	RFX_Long(pFX, _T("[ProdQty]"), m_ProdQty);
	RFX_Long(pFX, _T("[ScrapQty]"), m_ScrapQty);
	RFX_Long(pFX, _T("[EstQty]"), m_EstQty);
	RFX_Double(pFX, _T("[RunHrs]"), m_RunHrs);
	RFX_Long(pFX, _T("[CycleCount]"), m_CycleCount);
	RFX_Double(pFX, _T("[StandardRate]"), m_StandardRate);
	RFX_Int(pFX, _T("[ShiftNum]"), m_ShiftNum);
	RFX_Int(pFX, _T("[EmpNum]"), m_EmpNum);
	RFX_Long(pFX, _T("[PCOnum]"), m_PCOnum);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CProdOrders diagnostics

#ifdef _DEBUG
void CProdOrders::AssertValid() const
{
	CRecordset::AssertValid();
}

void CProdOrders::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
