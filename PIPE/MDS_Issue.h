#if !defined(AFX_MDS_ISSUE_H__87983031_CAEF_4CA0_B013_7DC5F279CA07__INCLUDED_)
#define AFX_MDS_ISSUE_H__87983031_CAEF_4CA0_B013_7DC5F279CA07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDS_Issue.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MDS_Issue recordset

class MDS_Issue : public CRecordset
{
public:
	MDS_Issue(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(MDS_Issue)

// Field/Param Data
	//{{AFX_FIELD(MDS_Issue, CRecordset)
	double	m_TRANSACTION_INTERFACE_ID;
	CString	m_SOURCE_CODE;
	double	m_SOURCE_LINE_ID;
	double	m_SOURCE_HEADER_ID;
	CString	m_PROCESS_FLAG;
	double	m_VALIDATION_REQUIRED;
	double	m_TRANSACTION_MODE;
	CTime	  m_TRANSACTION_DATE;
  CTime	  m_LAST_UPDATE_DATE;
  double  m_LAST_UPDATED_BY;
  CTime	  m_CREATION_DATE;
  double	m_CREATED_BY;
	double	m_INVENTORY_ITEM_ID;
	CString	m_ITEM_SEGMENT1;
  double	m_ORGANIZATION_ID;
	double	m_TRANSACTION_QUANTITY;
  CString	m_TRANSACTION_UOM;
	double	m_TRANSACTION_TYPE_ID;
	double	m_TRANSACTION_SOURCE_TYPE_ID;
	double	m_TRANSACTION_ACTION_ID;
	CString	m_TRANSACTION_SOURCE_NAME;
  double	m_TRANSACTION_SOURCE_ID;
  CString	m_SUBINVENTORY_CODE;
	double	m_LOCATOR_ID;
	CString	m_FLOW_SCHEDULE;
  double	m_SCHEDULED_FLAG;
  double	m_WIP_ENTITY_TYPE;
	double	m_TRANSFER_LOCATOR;
  double	m_OPERATION_SEQ_NUM;
	
  //}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MDS_Issue)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDS_ISSUE_H__87983031_CAEF_4CA0_B013_7DC5F279CA07__INCLUDED_)
