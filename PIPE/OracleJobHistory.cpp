// OracleJobHistory.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "OracleJobHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COracleJobHistory

IMPLEMENT_DYNAMIC(COracleJobHistory, CRecordset)

COracleJobHistory::COracleJobHistory(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(COracleJobHistory)
	m_JobID = _T("");
	m_Priority = 0;
	m_PlanQty = _T("");
	m_CellID = _T("");
	m_Machine = 0;
	m_PartName = _T("");
	m_PackCode = _T("");
	m_Material = _T("");
	m_Color = _T("");
	m_DCI = _T("");
	m_MoldName = _T("");
	m_MoldCellID = _T("");
	m_CycleCount = _T("");
	m_StandardRate = _T("");
	m_MeasuredQty = _T("");
	m_CompleteQty = _T("");
	m_ScrapQty = _T("");
	m_StandardCost = 0.0;
	m_ResourceRate = 0.0;
	m_OverHeadRate = 0.0;
	m_RejectCode = _T("");
	m_RunTime = 0.0f;
	m_ProdTime = 0.0f;
	m_DownTime = 0.0f;
	m_EmployeeID = _T("");
	m_Status = _T("");
	m_nFields = 32;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString COracleJobHistory::GetDefaultConnect()
{
	return _T("ODBC;DSN=OracleJobHistory");
}

CString COracleJobHistory::GetDefaultSQL()
{
	return _T("[JobHistory]");
}

void COracleJobHistory::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(COracleJobHistory)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[JobID]"), m_JobID);
	RFX_Date(pFX, _T("[PrintDate]"), m_PrintDate);
	RFX_Int(pFX, _T("[Priority]"), m_Priority);
	RFX_Date(pFX, _T("[DueDate]"), m_DueDate);
	RFX_Text(pFX, _T("[PlanQty]"), m_PlanQty);
	RFX_Text(pFX, _T("[CellID]"), m_CellID);
	RFX_Long(pFX, _T("[Machine]"), m_Machine);
	RFX_Text(pFX, _T("[PartName]"), m_PartName);
	RFX_Text(pFX, _T("[PackCode]"), m_PackCode);
	RFX_Text(pFX, _T("[Material]"), m_Material);
	RFX_Text(pFX, _T("[Color]"), m_Color);
	RFX_Text(pFX, _T("[DCI]"), m_DCI);
	RFX_Text(pFX, _T("[MoldName]"), m_MoldName);
	RFX_Text(pFX, _T("[MoldCellID]"), m_MoldCellID);
	RFX_Text(pFX, _T("[CycleCount]"), m_CycleCount);
	RFX_Text(pFX, _T("[StandardRate]"), m_StandardRate);
	RFX_Date(pFX, _T("[ProdDoneDate]"), m_ProdDoneDate);
	RFX_Date(pFX, _T("[ClosedDate]"), m_ClosedDate);
	RFX_Text(pFX, _T("[MeasuredQty]"), m_MeasuredQty);
	RFX_Text(pFX, _T("[CompleteQty]"), m_CompleteQty);
	RFX_Text(pFX, _T("[ScrapQty]"), m_ScrapQty);
	RFX_Double(pFX, _T("[StandardCost]"), m_StandardCost);
	RFX_Double(pFX, _T("[ResourceRate]"), m_ResourceRate);
	RFX_Double(pFX, _T("[OverHeadRate]"), m_OverHeadRate);
	RFX_Text(pFX, _T("[RejectCode]"), m_RejectCode);
	RFX_Single(pFX, _T("[RunTime]"), m_RunTime);
	RFX_Single(pFX, _T("[ProdTime]"), m_ProdTime);
	RFX_Single(pFX, _T("[DownTime]"), m_DownTime);
	RFX_Date(pFX, _T("[StartDate]"), m_StartDate);
	RFX_Date(pFX, _T("[CompDate]"), m_CompDate);
	RFX_Text(pFX, _T("[EmployeeID]"), m_EmployeeID);
	RFX_Text(pFX, _T("[Status]"), m_Status);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// COracleJobHistory diagnostics

#ifdef _DEBUG
void COracleJobHistory::AssertValid() const
{
	CRecordset::AssertValid();
}

void COracleJobHistory::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
