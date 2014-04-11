// Logger.cpp : implementation file
//

#include "stdafx.h"
#include "Logger.h"
#include <iostream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;


/////////////////////////////////////////////////////////////////////////////
// CLogger

CLogger::CLogger()
{
  // Get the value of the SETCIMLOGS environment variable
 
  pFilePath = NULL;
  pFilePath = getenv("SETCIMLOGS");
  if( pFilePath != NULL )
  {
    strcat(pFilePath, "\\PIPE.log");
  }
  else
  {
    pFilePath = getenv("SETCIM");
    if( pFilePath != NULL )
    {
      strcat(pFilePath, "\\Group200\\Logs\\PIPE.log");
    }
    else
    {
      pFilePath = "C:\\PIPE.log";
    }
  }
  AppVersion  = APPVER;
  BuildNumber = BUILDNUMBER;
  BuildDate   = BUILDDATE;
  //DivisionName = DIVNAME;
}
CLogger::~CLogger()
{
}

BEGIN_MESSAGE_MAP(CLogger, CWnd)
	//{{AFX_MSG_MAP(CLogger)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLogger message handlers
void CLogger::LogMessage(short msgid, LPCTSTR msgtxt)
{
  CStdioFile LogFile;
  CFileException e; 
  CString MsgBuf;
  SYSTEMTIME systime;
  char MsgType[5][4] = {"   ", "***", "+++", "$$$", "@@@"};

  GetLocalTime(&systime);
  MsgBuf.Empty();
  MsgBuf.Format("%s %02d/%02d/%04d %02d:%02d:%02d ",MsgType[msgid], systime.wMonth, systime.wDay,systime.wYear,
                      systime.wHour, systime.wMinute, systime.wSecond);
  MsgBuf += msgtxt;
  MsgBuf += "\n";
  if (LogFile.Open((LPCTSTR)pFilePath, CFile::modeWrite, &e))
  { 
    LogFile.SeekToEnd();
    LogFile.WriteString((LPCTSTR)MsgBuf);
    LogFile.Close();
  } 
  else
  {
    if (LogFile.Open((LPCTSTR)pFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &e))
    {
      CString LogHeading;
      LogHeading.Empty();
      LogHeading.Format(" %s Production Information PanCIM Exporter Version %s.%s\n", DivisionName, AppVersion, BuildNumber);
      LogHeading += "--------------------------------------------------------------------------------------------\n";
      LogFile.WriteString((LPCTSTR) LogHeading);
      LogFile.WriteString((LPCTSTR) MsgBuf);
      LogFile.Close();
    }
    else
    {
      cout << " Unable to open PIPE.log file! " <<  e.m_cause << endl;
    }
  }
}
