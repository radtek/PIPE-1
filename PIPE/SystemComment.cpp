// SystemComment.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "SystemComment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemComment

IMPLEMENT_DYNAMIC(CSystemComment, CRecordset)

CSystemComment::CSystemComment(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSystemComment)
	m_RefID = 0;
	m_Workorder = _T("");
	m_Machine = 0;
	m_Employee = 0;
	m_MsgCode = 0;
	m_Comment = _T("");
	m_nFields = 7;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}


CString CSystemComment::GetDefaultConnect()
{
	return _T("ODBC;DSN=EqptPerfHist");
}

CString CSystemComment::GetDefaultSQL()
{
	return _T("[dbo].[SystemComments]");
}

void CSystemComment::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSystemComment)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Long(pFX, _T("[RefID]"), m_RefID);
	RFX_Date(pFX, _T("[LogTime]"), m_LogTime);
	RFX_Text(pFX, _T("[Workorder]"), m_Workorder);
	RFX_Int(pFX, _T("[Machine]"), m_Machine);
	RFX_Long(pFX, _T("[Employee]"), m_Employee);
	RFX_Int(pFX, _T("[MsgCode]"), m_MsgCode);
	RFX_Text(pFX, _T("[Comment]"), m_Comment);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSystemComment diagnostics

#ifdef _DEBUG
void CSystemComment::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSystemComment::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
