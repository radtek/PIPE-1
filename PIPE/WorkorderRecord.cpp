// WorkorderRecord.cpp: implementation of the CWorkorderRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkorderRecord.h"
#include "PanCIM_Oracle.h"
#include "setcim.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkorderRecord::CWorkorderRecord()
{
  WOTags[0]  = WONUM_FT; 
  WOTags[1]  = PARTNAME_FT;
  WOTags[2]  = PACKCODE_FT;
  WOTags[3]  = MATERIAL_FT;
  WOTags[4]  = COLOR_FT;
  WOTags[5]  = MOLDNAME_FT;
  WOTags[6]  = DATECREATED_FT;
  WOTags[7]  = DATEDUE_FT;
  WOTags[8]  = PLANQTY_FT;
  WOTags[9]  = MACHNUM_FT;
  WOTags[10] = NUMCAVITIES_FT;
  WOTags[11] = WOPRIORITY_FT;
  WOTags[12] = STANDARDRATE_FT;
   
  WOTypes[0]  = WONUMLEN;
  WOTypes[1]  = PARTNAMELEN;
  WOTypes[2]  = PACKCODELEN;
  WOTypes[3]  = MATERIALLEN;
  WOTypes[4]  = COLORLEN;
  WOTypes[5]  = MOLDNAMELEN;
  WOTypes[6]  = DTYPTIME;
  WOTypes[7]  = DTYPTIME;
  WOTypes[8]  = DTYPLONG;
  WOTypes[9]  = DTYPLONG;
  WOTypes[10] = DTYPLONG;
  WOTypes[11] = DTYPSHRT;
  WOTypes[12] = DTYPREAL;
}

CWorkorderRecord::~CWorkorderRecord()
{

}
