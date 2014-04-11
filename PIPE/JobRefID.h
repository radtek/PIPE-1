#if !defined(AFX_JOBREFID_H__E1D1AAE3_BB06_4EBF_8DDA_54032EF67339__INCLUDED_)
#define AFX_JOBREFID_H__E1D1AAE3_BB06_4EBF_8DDA_54032EF67339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobRefID.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJobRefID recordset

class CJobRefID : public CRecordset
{
public:
	CJobRefID(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CJobRefID)

// Field/Param Data
	//{{AFX_FIELD(CJobRefID, CRecordset)
	long m_RefID;
  // CString	m_JobID;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJobRefID)
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

#endif // !defined(AFX_JOBREFID_H__E1D1AAE3_BB06_4EBF_8DDA_54032EF67339__INCLUDED_)
