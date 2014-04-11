// TextFilter.cpp: implementation of the CTextFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "TextFilter.h"
// #include "Logger.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////




CTextFilter::CTextFilter()
{
  Status = 0;
	MaxLen = 0;
	TextOut.Empty();
}

CTextFilter::~CTextFilter()
{

}

void CTextFilter::Strip(CString TxtIn)
{
  // CLogger Log;
  CString LogMsgTxt;
  char cBuf;
  int  ChrCnt,
       ChrPos = 0,
       TxtLen = 0; 

  Status = 0;
	TextOut.Empty();
  TxtLen = TxtIn.GetLength();
  
  
  try
  {
    for (ChrCnt=0; ChrCnt<TxtLen; ChrCnt++)
    {
      cBuf = TxtIn.GetAt(ChrCnt);
      if (ChrPos < MaxLen)
      {
        if ((cBuf > LOWCHR) && (cBuf< HIGHCHR))
        {
          TextOut = TextOut + cBuf;
          ChrPos++;
        }
      }
      else
      {
        TextOut = TextOut + " ";
        ChrPos++;  
      }
    }
    for (ChrCnt=ChrPos; ChrCnt<MaxLen; ChrCnt++)
    {
      TextOut = TextOut + " ";
    }
  }
  catch( CDBException* e )
  {
    LogMsgTxt += " ** ";
    LogMsgTxt += (LPCSTR) e->m_strError;
    // Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
  }
}

void CTextFilter::Strip(char *TxtIn)
{
  // CLogger Log;
  CString LogMsgTxt;
  char cBuf;
  int  idx, 
       ChrPos = 0,
       TxtLen = 0; 
  
  Status = 0;
	TextOut.Empty();
  
  try
  {
    for (idx=0; idx<MaxLen; idx++)
    {
      cBuf = TxtIn[idx];
      if ((cBuf > LOWCHR) && (cBuf< HIGHCHR))
      {
        TextOut = TextOut + cBuf;
        ChrPos++;
      }
    } 
    for (idx=ChrPos; idx<MaxLen; idx++)
    {
      TextOut = TextOut + " ";
    }
  }
  
  catch( CDBException* e )
  {
    LogMsgTxt += " ** ";
    LogMsgTxt += (LPCSTR) e->m_strError;
    // Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    e->Delete();
  }
}
