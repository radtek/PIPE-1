// IssueData.cpp: implementation of the CIssueData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "IssueData.h"
#include "obound.h"

#ifndef ORACL_ORACLE
#include "oracl.h"
#endif
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIssueData::CIssueData()
{

  WIPEntityName.Empty();
	ItemCode.Empty();
	OrgIdentity   = 0;
	FirstOpSeqNum = 0;
	JobType       = 0; 
	WIPEntityID   = 0;
  MaterialID    = 0;
}

CIssueData::~CIssueData()
{

}

void CIssueData::GetIssueData()
{
  CString strSQL;
  ODatabase odb;
  ODynaset oDyn;  
  char cBuf1[40],
       cBuf2[40];

	ItemCode.Empty();
  MaterialCode.Empty();
	dbErrors      = FALSE;
	FirstOpSeqNum = 0;
	JobType       = 0;
	WIPEntityID   = 0;
  MaterialID    = 0;
  // OStartup(); 
  
  strSQL.Format("SELECT item_code,material_code, material_id,wip_entity_id, job_type, first_operation_seq_num from apps.panduit_wip_pancim_job_v where wip_entity_name = '%s' and organization_id=%f",
                 WIPEntityName, OrgIdentity);  
  oResult = odb.Open((LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);  
  if (oResult == OSUCCESS)
  {
    oDyn.Open(odb, (LPCTSTR) strSQL, ODYNASET_READONLY);
    if (oDyn.IsOpen())
    {
      oDyn.GetFieldValue("item_code",  cBuf1, 40);
      oDyn.GetFieldValue("material_code",  cBuf2, 40);
      oDyn.GetFieldValue("material_id", &MaterialID);
      oDyn.GetFieldValue("wip_entity_id", &WIPEntityID);
      oDyn.GetFieldValue("job_type", &JobType);
      oDyn.GetFieldValue("first_operation_seq_num", &FirstOpSeqNum);    
      // transfer character buffer data into public CStrings
      ItemCode = cBuf1;
      MaterialCode = cBuf2;
    }
    else
    {
      dbErrors = TRUE;
    }
  }
  else
  {
    dbErrors = TRUE;
  }
  oDyn.Close();
  odb.Close();
  // OShutdown();
}
