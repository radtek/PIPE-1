// PanCIMMessage.h: interface for the CPanCIMMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANCIMMESSAGE_H__C65E715F_9EE5_4A2E_B664_4FB84C141F83__INCLUDED_)
#define AFX_PANCIMMESSAGE_H__C65E715F_9EE5_4A2E_B664_4FB84C141F83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "setcim.h"

#define MAX_INT_OCCS     20 // maximum integer occurrances 
#define MAX_CHAR_OCCS    10 // maximum character occurrances  
#define MAX_REAL_OCCS    10 // maximum real occurrances 
#define MAX_TIME_OCCS     2 // maximum time occurrances 
#define LEN_CHAR_OCCS    20 // character occurrance length

class CPanCIMMessage  
{
public:
	CPanCIMMessage();
	void DeleteMessage(void);
	void GetMsgData(void);
  virtual ~CPanCIMMessage();

public:
  long    RecID,
          TransCode,
          MsgSource,
          status,
          iCount,
          rCount,
          tCount,
          cCount,
          TransCodeFT,
          SourceFT,
          IntegerCountFT,
          CharacterCountFT,
          RealCountFT,
          TimeStampCountFT,
          IntFieldFT,
          CharFieldFT,
          RealFieldFT,
          TimeFieldFT,
          IntVals[MAX_INT_OCCS],
          TimeVals[MAX_TIME_OCCS];

  float   RealVals[MAX_REAL_OCCS];
  char    CharVals[MAX_CHAR_OCCS][LEN_CHAR_OCCS];
  CString asciiTime[MAX_CHAR_OCCS];
	CTime   TimeStamp[MAX_TIME_OCCS];

private:
	void ASCIItoCTime(short idx);
  void CPanCIMMessage::MakeString(short idx);
	short IP21errorlength;
	ERRARRAY IP21errormsg;
	ERRBLOCK IP21error;
	long tsVals[MAX_TIME_OCCS];

};

#endif // !defined(AFX_PANCIMMESSAGE_H__C65E715F_9EE5_4A2E_B664_4FB84C141F83__INCLUDED_)
