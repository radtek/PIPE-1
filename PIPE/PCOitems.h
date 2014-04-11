#if !defined(AFX_PCOITEMS_H__6E688BF9_F92A_11D4_B7FD_00B0D0226342__INCLUDED_)
#define AFX_PCOITEMS_H__6E688BF9_F92A_11D4_B7FD_00B0D0226342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PCOitems.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPCOitems recordset

class CPCOitems : public CRecordset
{
public:
	CPCOitems(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CPCOitems)

// Field/Param Data
	//{{AFX_FIELD(CPCOitems, CRecordset)
	long  m_PCOkey;
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
	long	m_PCOnum;
	int		m_MachNum;
	CString	m_PartName;
	CString	m_MoldName;
	CTime	m_StartDate;
	CTime	m_EndDate;
	BOOL	m_Closed;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCOitems)
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

#endif // !defined(AFX_PCOITEMS_H__6E688BF9_F92A_11D4_B7FD_00B0D0226342__INCLUDED_)
