// WorkorderRecord.h: interface for the CWorkorderRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKORDERRECORD_H__2911AF9C_3173_4E46_A3A0_CB9FB83CF80A__INCLUDED_)
#define AFX_WORKORDERRECORD_H__2911AF9C_3173_4E46_A3A0_CB9FB83CF80A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WONAMELEN     12
#define WONUMLEN       8
#define PARTNAMELEN   18
#define PACKCODELEN   12
#define MATERIALLEN   12
#define COLORLEN       8      
#define MOLDNAMELEN   18

class CWorkorderRecord  
{
public:
  
  struct Workorder { 
          char  WONum[WONUMLEN],
                 PartName[PARTNAMELEN],
                 PackCode[PACKCODELEN],
                 Material[MATERIALLEN],
                 Color[COLORLEN],
                 MoldName[MOLDNAMELEN];
          long   DateCreated,
                 DateDue,
                 PlanQty,
                 MachNum,
                 NumCavities;
           int   WOPriority;
           float StandardRate;};

	static short WOTypes[13];
  static long WOTags[13];
           
  CWorkorderRecord();
  virtual ~CWorkorderRecord();
};

#endif // !defined(AFX_WORKORDERRECORD_H__2911AF9C_3173_4E46_A3A0_CB9FB83CF80A__INCLUDED_)
