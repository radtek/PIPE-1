#if !defined(AFX_MDS_INVEN_H__42690D18_38A2_45F4_8BA2_A0B3DB2D57A4__INCLUDED_)
#define AFX_MDS_INVEN_H__42690D18_38A2_45F4_8BA2_A0B3DB2D57A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDS_Inven.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MDS_Inven recordset

class MDS_Inven : public CRecordset
{
public:
	MDS_Inven(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(MDS_Inven)

// Field/Param Data
	//{{AFX_FIELD(MDS_Inven, CRecordset)
	double	m_TRANSACTION_INTERFACE_ID;
	CTime	m_LAST_UPDATE_DATE;
	double	m_LAST_UPDATED_BY;
	CTime	m_CREATION_DATE;
	double	m_CREATED_BY;
	CString	m_LOT_NUMBER;
	double	m_TRANSACTION_QUANTITY;
	double	m_PRIMARY_QUANTITY;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MDS_Inven)
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

#endif // !defined(AFX_MDS_INVEN_H__42690D18_38A2_45F4_8BA2_A0B3DB2D57A4__INCLUDED_)
