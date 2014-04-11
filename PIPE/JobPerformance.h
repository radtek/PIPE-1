#if !defined(AFX_JOBPERFORMANCE_H__E7D54466_525A_4783_BB6F_188D4FBF2DD2__INCLUDED_)
#define AFX_JOBPERFORMANCE_H__E7D54466_525A_4783_BB6F_188D4FBF2DD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JobPerformance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// JobPerformance recordset

class JobPerformance : public CRecordset
{
public:
	JobPerformance(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(JobPerformance)

// Field/Param Data
	//{{AFX_FIELD(JobPerformance, CRecordset)
	long m_RefID;
  CString	m_JobID;
  long  m_Machine;
	CString	m_SystemID;
	float	m_DesignCycleTime;
	float	m_ActualCycleTime;
	float	m_ProductionTime;
	CString	m_InterruptDuration;
	long	m_InterruptCount;
	long	m_EventMessages;
	long	m_EventWarnings;
	long	m_EventFaults;
	float	m_PurgeWeight;
  float m_PurgeDistance;
  float m_PurgeTime;
	long	m_SetPointChanges;
	float	m_Yield;
	float	m_Efficiency;
	float	m_Availability;
	float	m_SysEffectiveness;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JobPerformance)
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

#endif // !defined(AFX_JOBPERFORMANCE_H__E7D54466_525A_4783_BB6F_188D4FBF2DD2__INCLUDED_)
