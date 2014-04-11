// ReactivateMsg.h: interface for the CReactivateMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REACTIVATEMSG_H__AB2960BC_722F_4B1B_957F_209E2F3B877E__INCLUDED_)
#define AFX_REACTIVATEMSG_H__AB2960BC_722F_4B1B_957F_209E2F3B877E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXLISTRECS  300

#include "setcim.h"

class CReactivateMsg  
{
public:
	long DefinitionRecordID;
	long DefaultRecordID;
  long MessageCheckerID;
  long WOCleanupRecordID;
	int  ActRecordCount;
	void ReactivateRecords();
	CReactivateMsg();
	virtual ~CReactivateMsg();

};

#endif // !defined(AFX_REACTIVATEMSG_H__AB2960BC_722F_4B1B_957F_209E2F3B877E__INCLUDED_)
