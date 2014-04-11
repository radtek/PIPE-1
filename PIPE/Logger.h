#if !defined(AFX_LOGGER_H__54CE9D2B_809B_11D4_BA66_00A0CC251425__INCLUDED_)
#define AFX_LOGGER_H__54CE9D2B_809B_11D4_BA66_00A0CC251425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Logger.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogger window

class CLogger : public CWnd
{
// Construction
public:
	CLogger();

// Attributes
public:
  CString AppVersion,
          BuildNumber,
          BuildDate,
          DivisionName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogger)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogger();
  void LogMessage(short msgid, LPCTSTR msgtxt);
	// Generated message map functions
protected:
	//{{AFX_MSG(CLogger)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  char *pFilePath;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGGER_H__54CE9D2B_809B_11D4_BA66_00A0CC251425__INCLUDED_)
