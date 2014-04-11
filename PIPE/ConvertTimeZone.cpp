// ConvertTimeZone.cpp: implementation of the CConvertTimeZone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "ConvertTimeZone.h"
#include "obound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvertTimeZone::CConvertTimeZone()
{

}

CConvertTimeZone::~CConvertTimeZone()
{

}

CTime CConvertTimeZone::ConvertTS(CTime OracleTS, long OracleOffset, long LocalOffset)
{
  time_t tmpTS;
  tmpTS = OracleTS.GetTime();      // convert supplied timestamp to integer
  tmpTS += OracleOffset;   // convert Oracle time to UTC
  tmpTS -= LocalOffset;    // convert Oracle UTC time to local time
  CTime tmpLocalTS(tmpTS);
  return tmpLocalTS;
}
