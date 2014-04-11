// ReactivateMsg.cpp: implementation of the CReactivateMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "ReactivateMsg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReactivateMsg::CReactivateMsg()
{

}

CReactivateMsg::~CReactivateMsg()
{

}

void CReactivateMsg::ReactivateRecords()
{
  long  LastRecID = 0,
        NumRecs   = 0,
        RecIDList[MAXLISTRECS];
  short NameSize[MAXLISTRECS],
        ActPriority = 1,
        OldPriority = 0,
        j;
  ERRBLOCK IP21error;
  char RecStat[MAXLISTRECS];
  unsigned char GrpList[8];
  NAMEARR RecNames[MAXLISTRECS];

  ActRecordCount = 0;
  GETRECLIST(&LastRecID,DefinitionRecordID,0,GrpList,8,0,0,MAXLISTRECS,RecIDList,RecStat,RecNames,NameSize,&NumRecs);
  if (NumRecs > 3) // disregard the default base,message check and WOCleanup records
  {
    for (j=0; j<NumRecs; j++)
    {      
      if ((RecIDList[j] != MessageCheckerID) && (RecIDList[j] != DefaultRecordID) && 
          (RecIDList[j] != WOCleanupRecordID))
      {   
        ACTEXTSK(RecIDList[j], ActPriority, &OldPriority, &IP21error);
        if (IP21error.ERRCODE == SUCCESS)
        {
          ActRecordCount++;
        }
        else
        {
          printf("Record Activation Error: ID=%d\n", RecIDList[j]);
          IP21error.ERRCODE = SUCCESS;
        }
      }
    }
  }
}
