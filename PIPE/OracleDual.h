#if !defined(AFX_ORACLEDUAL_H__386ABA27_3725_424F_9032_02903C38C59D__INCLUDED_)
#define AFX_ORACLEDUAL_H__386ABA27_3725_424F_9032_02903C38C59D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OracleDual.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COracleDual recordset

class COracleDual : public CRecordset
{
public:
	COracleDual(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(COracleDual)

// Field/Param Data
	//{{AFX_FIELD(COracleDual, CRecordset)
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COracleDual)
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

#endif // !defined(AFX_ORACLEDUAL_H__386ABA27_3725_424F_9032_02903C38C59D__INCLUDED_)
