#if !defined(AFX_PCO_UPDATEQTY_H__BB7806E7_FC3B_11D4_B800_00B0D0226342__INCLUDED_)
#define AFX_PCO_UPDATEQTY_H__BB7806E7_FC3B_11D4_B800_00B0D0226342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PCO_UpdateQty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPCO_UpdateQty recordset

class CPCO_UpdateQty : public CRecordset
{
public:
	CPCO_UpdateQty(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPCO_UpdateQty)

// Field/Param Data
	//{{AFX_FIELD(CPCO_UpdateQty, CRecordset)
	long	m_PCOkey;
	long	m_PCOref;
	int		m_TabNum;
	int		m_TabPos;
	int		m_RunSeq;
	CString	m_Material;
	CString	m_Color;
	CString	m_PackCode;
	long	m_QtyReq;
	long	m_QtyNeg;
	long	m_QtyDone;
	BOOL	m_Complete;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCO_UpdateQty)
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

#endif // !defined(AFX_PCO_UPDATEQTY_H__BB7806E7_FC3B_11D4_B800_00B0D0226342__INCLUDED_)
