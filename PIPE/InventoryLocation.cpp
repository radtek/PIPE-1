// InventoryLocation.cpp: implementation of the CInventoryLocation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "pancim_oracle.h"
#include "InventoryLocation.h"
#include "obound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInventoryLocation::CInventoryLocation()
{
  oResult = OFAILURE;

}

CInventoryLocation::~CInventoryLocation()
{

}

void CInventoryLocation::GetInventoryLocationID()
{

  ODatabase odb;
  ODynaset oDyn;
  InvenLocationID = 0;
  // OStartup();
  
  strSQL.Format("select inventory_location_id from apps.mtl_item_locations_kfv where organization_id=%d and segment1= '%s' and subinventory_code = '%s'",
                  OrgID, Seg1, SubInvenCode);
  oResult = odb.Open((LPCTSTR) dbNam,(LPCTSTR) dbUsr,(LPCTSTR) dbPwd);  
  if (oResult == OSUCCESS)
  {
    oDyn.Open(odb, (LPCTSTR) strSQL, ODYNASET_READONLY);
    if (oDyn.IsOpen())
    {
      oDyn.GetFieldValue("inventory_location_id", &InvenLocationID);
    }
  }
  oDyn.Close();
  odb.Close();
  // OShutdown();
}
