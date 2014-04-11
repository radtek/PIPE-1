#if !defined(AFX_SYSTEMCOMMENT_H__22C51CFE_2252_4AC2_AADF_A00F63452268__INCLUDED_)
#define AFX_SYSTEMCOMMENT_H__22C51CFE_2252_4AC2_AADF_A00F63452268__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemComment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemComment recordset

class CSystemComment : public CRecordset
{
public:
	CSystemComment(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSystemComment)

// Field/Param Data
	//{{AFX_FIELD(CSystemComment, CRecordset)
	long	m_RefID;
	CTime	m_LogTime;
	CString	m_Workorder;
	int		m_Machine;
	long	m_Employee;
	int		m_MsgCode;
	CString	m_Comment;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemComment)
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

#endif // !defined(AFX_SYSTEMCOMMENT_H__22C51CFE_2252_4AC2_AADF_A00F63452268__INCLUDED_)
