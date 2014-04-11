// OracleSeqNum1.cpp: implementation of the COracleSeqNum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "OracleSeqNum1.h"
#include "obound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef ORACL_ORACLE
#include "oracl.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COracleSeqNum::COracleSeqNum()
{
  strSQL = "select inv.mtl_material_transactions_s.nextval from dual";
}

COracleSeqNum::~COracleSeqNum()
{

}

/* ----------------------------------------------
    retrieves the next Oracle sequence number
   ---------------------------------------------- */
double COracleSeqNum::GetSeqNum()
{
  int oResult;
  ODatabase odb;
  ODynaset oDyn;
  SeqNum = 0;  
   
  // OStartup();
  oResult = odb.Open((LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);
  if (oResult == OSUCCESS)
  {
    oDyn.Open(odb, strSQL);
    if (oDyn.IsOpen())
    {
      oDyn.GetFieldValue(0, &SeqNum);
    }
    oDyn.Close();
  }
  odb.Close();
  // OShutdown();
  return SeqNum;
}
