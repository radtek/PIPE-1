// MDS_Issue.cpp : implementation file
//

#include "stdafx.h"
#include "pancim_oracle.h"
#include "MDS_Issue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MDS_Issue

IMPLEMENT_DYNAMIC(MDS_Issue, CRecordset)

MDS_Issue::MDS_Issue(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(MDS_Issue)
	m_TRANSACTION_INTERFACE_ID = 0.0;
	m_SOURCE_CODE = _T("");
	m_SOURCE_LINE_ID = 0.0;
	m_SOURCE_HEADER_ID = 0.0;
	m_PROCESS_FLAG = _T("");
	m_VALIDATION_REQUIRED = 0.0;
	m_TRANSACTION_MODE = 0.0;
  m_LAST_UPDATED_BY = 0.0;
  m_CREATED_BY = 0.0;
	m_INVENTORY_ITEM_ID = 0.0;
	m_ITEM_SEGMENT1 = _T("");
	m_ORGANIZATION_ID = 0.0;
	m_TRANSACTION_QUANTITY = 0.0;
	m_TRANSACTION_UOM = _T("");
	m_TRANSACTION_TYPE_ID = 0.0;
  m_TRANSACTION_SOURCE_TYPE_ID = 0.0;
	m_TRANSACTION_ACTION_ID = 0.0;
	m_TRANSACTION_SOURCE_NAME = _T("");
	m_TRANSACTION_SOURCE_ID = 0.0;
	m_SUBINVENTORY_CODE = _T("");
	m_LOCATOR_ID = 0.0;
	m_FLOW_SCHEDULE = _T("");
	m_SCHEDULED_FLAG = 0.0;
  m_WIP_ENTITY_TYPE = 0.0;
	m_TRANSFER_LOCATOR = 0.0;
	m_OPERATION_SEQ_NUM = 0.0;
	m_nFields = 29;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString MDS_Issue::GetDefaultConnect()
{
	return _T("ODBC;DSN=MDS_ISSUE");
}

CString MDS_Issue::GetDefaultSQL()
{
	return _T("[INV].[MTL_TRANSACTIONS_INTERFACE]");
}

void MDS_Issue::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(MDS_Issue)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Double(pFX, _T("[TRANSACTION_INTERFACE_ID]"), m_TRANSACTION_INTERFACE_ID);
	RFX_Text(pFX, _T("[SOURCE_CODE]"), m_SOURCE_CODE);
	RFX_Double(pFX, _T("[SOURCE_LINE_ID]"), m_SOURCE_LINE_ID);
	RFX_Double(pFX, _T("[SOURCE_HEADER_ID]"), m_SOURCE_HEADER_ID);
  RFX_Text(pFX, _T("[PROCESS_FLAG]"), m_PROCESS_FLAG);
	RFX_Double(pFX, _T("[VALIDATION_REQUIRED]"), m_VALIDATION_REQUIRED);
	RFX_Double(pFX, _T("[TRANSACTION_MODE]"), m_TRANSACTION_MODE);
	RFX_Date(pFX, _T("[TRANSACTION_DATE]"), m_TRANSACTION_DATE);
	RFX_Date(pFX, _T("[LAST_UPDATE_DATE]"), m_LAST_UPDATE_DATE);
  RFX_Double(pFX, _T("[LAST_UPDATED_BY]"), m_LAST_UPDATED_BY);
	RFX_Date(pFX, _T("[CREATION_DATE]"), m_CREATION_DATE);
  RFX_Double(pFX, _T("[CREATED_BY]"), m_CREATED_BY);
	RFX_Double(pFX, _T("[INVENTORY_ITEM_ID]"), m_INVENTORY_ITEM_ID);
	RFX_Text(pFX, _T("[ITEM_SEGMENT1]"), m_ITEM_SEGMENT1);
  RFX_Double(pFX, _T("[ORGANIZATION_ID]"), m_ORGANIZATION_ID);
	RFX_Double(pFX, _T("[TRANSACTION_QUANTITY]"), m_TRANSACTION_QUANTITY);
	RFX_Text(pFX, _T("[TRANSACTION_UOM]"), m_TRANSACTION_UOM);
	RFX_Double(pFX, _T("[TRANSACTION_TYPE_ID]"), m_TRANSACTION_TYPE_ID);	
  RFX_Double(pFX, _T("[TRANSACTION_SOURCE_TYPE_ID]"), m_TRANSACTION_SOURCE_TYPE_ID);
	RFX_Double(pFX, _T("[TRANSACTION_ACTION_ID]"), m_TRANSACTION_ACTION_ID);
	RFX_Text(pFX, _T("[TRANSACTION_SOURCE_NAME]"), m_TRANSACTION_SOURCE_NAME);
	RFX_Double(pFX, _T("[TRANSACTION_SOURCE_ID]"), m_TRANSACTION_SOURCE_ID);
	RFX_Text(pFX, _T("[SUBINVENTORY_CODE]"), m_SUBINVENTORY_CODE);
	RFX_Double(pFX, _T("[LOCATOR_ID]"), m_LOCATOR_ID);
	RFX_Text(pFX, _T("[FLOW_SCHEDULE]"), m_FLOW_SCHEDULE);
  RFX_Double(pFX, _T("[SCHEDULED_FLAG]"), m_SCHEDULED_FLAG);
  RFX_Double(pFX, _T("[WIP_ENTITY_TYPE]"), m_WIP_ENTITY_TYPE);
  RFX_Double(pFX, _T("[TRANSFER_LOCATOR]"), m_TRANSFER_LOCATOR);
	RFX_Double(pFX, _T("[OPERATION_SEQ_NUM]"), m_OPERATION_SEQ_NUM);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// MDS_Issue diagnostics

#ifdef _DEBUG
void MDS_Issue::AssertValid() const
{
	CRecordset::AssertValid();
}

void MDS_Issue::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
