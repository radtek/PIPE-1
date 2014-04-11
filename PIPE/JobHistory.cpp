// JobHistory.cpp : implementation file
//

#include "stdafx.h"
#include "JobHistory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JobHistory

IMPLEMENT_DYNAMIC(JobHistory, CRecordset)

JobHistory::JobHistory(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(JobHistory)
	
  m_RefID = 0;
  m_JobID = _T("");
	m_Priority = 0;
	m_PlanQty = 0;
	m_CellID = _T("");
	m_Machine = 0;
	m_PartName = _T("");
	m_PackCode = _T("");
	m_Material = _T("");
	m_Color = _T("");
	m_DCI = _T("");
	m_MoldName = _T("");
	m_MoldCellID = _T("");
	m_CycleCount = 0;
  m_StandardRate = 0.0;
  m_MaterialCost = 0.0;
	m_MeasuredQty = 0;
	m_CompleteQty = 0;
	m_ScrapQty = 0;
	m_StandardCost = 0.0;
	m_ResourceRate = 0.0;
	m_OverHeadRate = 0.0;
	m_RejectCode = _T("");
	m_RunTime = 0.0f;
	m_ProdTime = 0.0f;
	m_DownTime = 0.0f;
	m_EmployeeID = _T("");
	m_Status = _T("");
  m_ModFlag = 0;
	m_nFields = 35;

  //}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString JobHistory::GetDefaultConnect()
{
	return _T("ODBC;DSN=ProdJobHist");
}

CString JobHistory::GetDefaultSQL()
{
	return _T("[dbo].[JobHistory]");
}

void JobHistory::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(JobHistory)
  
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
  RFX_Text(pFX, _T("[JobID]"), m_JobID);
	RFX_Date(pFX, _T("[PrintDate]"), m_PrintDate);
	RFX_Int(pFX, _T("[Priority]"), m_Priority);
	RFX_Date(pFX, _T("[DueDate]"), m_DueDate);
	RFX_Double(pFX, _T("[PlanQty]"), m_PlanQty);
	RFX_Text(pFX, _T("[CellID]"), m_CellID);
	RFX_Long(pFX, _T("[Machine]"), m_Machine);
	RFX_Text(pFX, _T("[PartName]"), m_PartName);
	RFX_Text(pFX, _T("[PackCode]"), m_PackCode);
	RFX_Text(pFX, _T("[Material]"), m_Material);
	RFX_Text(pFX, _T("[Color]"), m_Color);
	RFX_Text(pFX, _T("[DCI]"), m_DCI);
	RFX_Text(pFX, _T("[MoldName]"), m_MoldName);
	RFX_Text(pFX, _T("[MoldCellID]"), m_MoldCellID);
	RFX_Long(pFX, _T("[CycleCount]"), m_CycleCount);
	RFX_Double(pFX, _T("[StandardRate]"), m_StandardRate);
	RFX_Date(pFX, _T("[ProdDoneDate]"), m_ProdDoneDate);
	RFX_Date(pFX, _T("[ClosedDate]"), m_ClosedDate);
	RFX_Double(pFX, _T("[MeasuredQty]"), m_MeasuredQty);
	RFX_Double(pFX, _T("[CompleteQty]"), m_CompleteQty);
	RFX_Double(pFX, _T("[ScrapQty]"), m_ScrapQty);
	RFX_Double(pFX, _T("[StandardCost]"), m_StandardCost);
	RFX_Double(pFX, _T("[ResourceRate]"), m_ResourceRate);
	RFX_Double(pFX, _T("[OverHeadRate]"), m_OverHeadRate);
	RFX_Double(pFX, _T("[MaterialCost]"), m_MaterialCost);
	RFX_Text(pFX, _T("[RejectCode]"), m_RejectCode);
	RFX_Single(pFX, _T("[RunTime]"), m_RunTime);
	RFX_Single(pFX, _T("[ProdTime]"), m_ProdTime);
	RFX_Single(pFX, _T("[DownTime]"), m_DownTime);
	RFX_Date(pFX, _T("[StartDate]"), m_StartDate);
	RFX_Date(pFX, _T("[CompDate]"), m_CompDate);
	RFX_Text(pFX, _T("[EmployeeID]"), m_EmployeeID);
	RFX_Text(pFX, _T("[Status]"), m_Status);
	RFX_Byte(pFX, _T("[ModFlag]"), m_ModFlag);
      
 //}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// JobHistory diagnostics

#ifdef _DEBUG
void JobHistory::AssertValid() const
{
	CRecordset::AssertValid();
}

void JobHistory::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
