// InventoryLocation.h: interface for the CInventoryLocation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVENTORYLOCATION_H__5451BF39_5433_4AA8_9C23_B448749FD09B__INCLUDED_)
#define AFX_INVENTORYLOCATION_H__5451BF39_5433_4AA8_9C23_B448749FD09B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInventoryLocation  
{
public:
	void GetInventoryLocationID();
	long OrgID;
	CString SubInvenCode,
	        Seg1,
          dbNam,
          dbUsr,
          dbPwd;
	long InvenLocationID;
	CInventoryLocation();
	virtual ~CInventoryLocation();

private:
	CString strSQL;
  int oResult;
};

#endif // !defined(AFX_INVENTORYLOCATION_H__5451BF39_5433_4AA8_9C23_B448749FD09B__INCLUDED_)
