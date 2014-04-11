// PanCIMMessage1.cpp: implementation of the CPanCIMMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "PanCIMMessage.h"
#include "Logger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanCIMMessage::CPanCIMMessage()
{

}

CPanCIMMessage::~CPanCIMMessage()
{

}
void CPanCIMMessage::GetMsgData()
{
   short NumOK,
         j;
   char  tsBuffer[20],
         tsError;
   
  status = SUCCESS;
  // read the PanCIMMessageDef record data
  DB2LONG(RecID, TransCodeFT, &TransCode, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: TransCode field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }
  DB2LONG(RecID, SourceFT, &MsgSource, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: Source field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }  
  DB2LONG(RecID, IntegerCountFT, &iCount, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: IntegerCount field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }
  DB2LONG(RecID, CharacterCountFT, &cCount, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: CharCount field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }
  DB2LONG(RecID, RealCountFT, &rCount, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: RealCount field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }
  DB2LONG(RecID, TimeStampCountFT, &tCount, &IP21error);
  if (IP21error.ERRCODE != SUCCESS)
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    printf("PanCIMMessage: TimeStampCount field read error: %.*s\n", ErrorLength, IP21message);
    status = IP21error.ERRCODE;
    return;
  }
  if (iCount)
  {
    RDBOCCS(RecID, IntFieldFT, 1, (short)iCount, DTYPLONG, IntVals, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("PanCIMMessage: INT_FIELD data read error: %.*s\n", ErrorLength, IP21message);
      status = IP21error.ERRCODE;
      return;
    }
  }
  if (cCount)
  {
    RDBOCCS(RecID, CharFieldFT, 1, (short)cCount, LEN_CHAR_OCCS, CharVals, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("PanCIMMessage: CHAR_FIELD data read error: %.*s\n", ErrorLength, IP21message);
      status = IP21error.ERRCODE;
      return;
    }
    for (j=0; j< cCount; j++)
    {
      // convert IP21 character buffer into C style string
      MakeString(j);              
    }
  }
  if (rCount)
  {
    RDBOCCS(RecID, RealFieldFT, 1, (short)rCount, DTYPREAL, RealVals, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("PanCIMMessage: REAL_FIELD data read error: %.*s\n", ErrorLength, IP21message);
      status = IP21error.ERRCODE;
      return;
    }
  }
  if (tCount)
  {
    RDBOCCS(RecID, TimeFieldFT, 1, (short)tCount, DTYPTIME, tsVals, &NumOK, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      printf("PanCIMMessage: TIME_FIELD data read error: %.*s\n", ErrorLength, IP21message);
      status = IP21error.ERRCODE;
      return;
    }
    for (j=0; j< tCount; j++)
    {
      // convert IP21 timestamps to ascii
      TMST2ASCII(tsVals[j], tsBuffer, 20, &tsError);
      tsBuffer[18] = '\0';
      asciiTime[j] = tsBuffer;
      if (tsError)
      {
        asciiTime[j].Empty();
      }
      // convert IP21 timestamps to CTime 
      ASCIItoCTime(j);    
    }
  }
}
/* ----------------------------------------------------------------------------
    Convert ascii timestamps from IP21 into CTime 
   ------------------------------------------------------------------------- */
void CPanCIMMessage::ASCIItoCTime(short idx)
{
  int nYear  = 0,
      nMonth = 0,
      nDay   = 0,
      nHour  = 0,
      nMin   = 0,
      nSec   = 0,
      nDST   = -1,
      mn;

  char MonthName[13][4] = {"   ", "Jan", "Feb", "Mar", "Apr", "May",
                           "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };  

  CString sYear,
          sMonth,
          sDay,
          sHour,
          sMin,
          sSec;

  sYear.Empty();
  sMonth.Empty();
  sDay.Empty();
  sHour.Empty();
  sMin.Empty();
  sSec.Empty();   
  
  if (asciiTime[idx].GetLength() < 18)
  { 
    TimeStamp[idx] = CTime(2000, 1, 1, 0, 0, 0, -1);
    return;
  }
  // check for expected ascii timestamp format
  if ((asciiTime[idx].Mid(3,1) != '-') || (asciiTime[idx].Mid(6,1) != '-') ||
     (asciiTime[idx].Mid(12,1) != ':') || (asciiTime[idx].Mid(15,1) != ':'))
  {

    // TODO: handle date errors
    TimeStamp[idx] = CTime(2000, 1, 1, 0, 0, 0, -1);
    return;
  }
  // Separate the timestamp values based on expected format mmm-dd-yy hh:mm:ss

  sMonth = asciiTime[idx].Mid(0,3);
  sDay   = asciiTime[idx].Mid(4,2);
  sYear  = asciiTime[idx].Mid(7,2);
  sHour  = asciiTime[idx].Mid(10,2);
  sMin   = asciiTime[idx].Mid(13,2);
  sSec   = asciiTime[idx].Mid(16,2);
  
  sYear.Insert(0,"20"); // make 4 digit year
  nYear  = atoi((LPCTSTR) sYear);
  for (mn=0; mn<13; mn++)
  {
    if (sMonth.CompareNoCase(MonthName[mn]) == 0)
    {
      nMonth = mn;
      break;
    }
  }
  nDay   = atoi((LPCTSTR) sDay);
  nHour  = atoi((LPCTSTR) sHour);
  nMin   = atoi((LPCTSTR) sMin);
  nSec   = atoi((LPCTSTR) sSec);
  if ((nYear > 1900) && (nMonth > 0) && (nMonth < 13) && (nDay > 0) &&
      (nDay < 32) && (nHour < 25) && (nMin < 61) && (nSec < 61))
  {
    TimeStamp[idx] = CTime(nYear, nMonth, nDay, nHour, nMin, nSec, nDST);
  }
  else
  {
    // TODO: handle date errors
    TimeStamp[idx] = CTime(2000, 1, 1, 0, 0, 0, -1);
  }
}
/* ----------------------------------------------------------------------------
    Convert IP21 character buffer into null terminated C style string
   ------------------------------------------------------------------------- */
void CPanCIMMessage::MakeString(short idx)
{
  int j,
      pos = 0;

  for (j=0; j<LEN_CHAR_OCCS; j++)
  {
    if ((CharVals[idx][j] > 0x1f) && (CharVals[idx][j] < 0x7f))
    {
      CharVals[idx][pos] = CharVals[idx][j];
      pos++;
    }
  }
  if (pos > LEN_CHAR_OCCS-1)
    pos = LEN_CHAR_OCCS-1;
  CharVals[idx][pos] = '\0';
}
/* ----------------------------------------------------------------------------
    Remove PCMIfaceMsg record from IP21 database
   ------------------------------------------------------------------------- */
void CPanCIMMessage::DeleteMessage()
{
  CString LogMsgTxt,
          TimeBuf;
  CLogger Log;

  MAKUNUSA(RecID, &IP21error);
  if (IP21error.ERRCODE == SUCCESS)
  {
    DELETREC(RecID, &IP21error);
    if (IP21error.ERRCODE != SUCCESS)
    {      
      ERRMESS(&IP21error, IP21message, &ErrorLength);
      LogMsgTxt.Format("DeleteMessage: Delete record %d error: %.*s\n", RecID, ErrorLength, IP21message);
      Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
      IP21error.ERRCODE = SUCCESS;
    }      
  }
  else
  {
    ERRMESS(&IP21error, IP21message, &ErrorLength);
    LogMsgTxt.Format("DeleteMessage: make record %d unusable error: %.*s\n", RecID, ErrorLength, IP21message);
    Log.LogMessage(1, (LPCTSTR) LogMsgTxt);
    IP21error.ERRCODE = SUCCESS;
  
  } 
}

