// OracleTransactionInfo.cpp: implementation of the COracleTransactionInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "OracleTransactionInfo.h"
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

COracleTransactionInfo::COracleTransactionInfo()
{
  TransName.Empty();
  oResult = OFAILURE;
}

COracleTransactionInfo::~COracleTransactionInfo()
{

}
/* -----------------------------------------------------------------
    retrieves Oracle transaction info for assigned transaction name
   ----------------------------------------------------------------- */
void COracleTransactionInfo::GetOracleTransactionCodes()
{
  CString strSQL;
  ODatabase odb;
  ODynaset oDyn;
  
  TransTypeID   = 0;
  TransSourceID = 0;
  TransActionID = 0;
  // OStartup(); 
  strSQL.Format("SELECT transaction_type_id,transaction_source_type_id,transaction_action_id from apps.panduit_transaction_types_v where upper(transaction_type_name) = '%s'",TransName);
  oResult = odb.Open((LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);  
  if (oResult == OSUCCESS)
  {
    oDyn.Open(odb, (LPCTSTR) strSQL, ODYNASET_READONLY);
    if (oDyn.IsOpen())
    {
      oDyn.GetFieldValue("transaction_type_id", &TransTypeID);
      oDyn.GetFieldValue("transaction_source_type_id", &TransSourceID);
      oDyn.GetFieldValue("transaction_action_id", &TransActionID);
    }
  }
  oDyn.Close();
  odb.Close();
  // OShutdown();
}
