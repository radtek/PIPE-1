// OracleMAP.h: interface for the OracleMAP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLEMAP_H__5D2C7CC8_DF55_402D_8B70_B1B9124E0003__INCLUDED_)
#define AFX_ORACLEMAP_H__5D2C7CC8_DF55_402D_8B70_B1B9124E0003__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAP_OK  0
#define MAP_ERR 1
#define MAXMAPFIELDS 80 // maximum number of Oracle fields to map

#define USE_NO  0       // OracleMapDef Use codes
#define USE_YES 1
#define USE_ERR 2

#define TOTAL_I_FIELDS  14
#define TOTAL_R_FIELDS   6
#define TOTAL_C_FIELDS  14
#define TOTAL_T_FIELDS   7

class OracleMAP  
{
public:
	int rtnStatus,
  ReadOracleMAP();  
  char IP21MapFields[10][20];
  CString JobFields_I[TOTAL_I_FIELDS];
  CString JobFields_R[TOTAL_R_FIELDS];
  CString JobFields_C[TOTAL_C_FIELDS];
  CString JobFields_T[TOTAL_T_FIELDS];

  // mapped field data
  long MAXiOcc,
       MAXrOcc,
       MAXcOcc,
       MAXtOcc,
       IntegerCount,
       RealCount,
       CharCount,
       TsCount,
       TotalMapFields,
       QueryRecordID;

  CString OracleField[MAXMAPFIELDS];
  long IP21Field[MAXMAPFIELDS];
  long IP21OccNum[MAXMAPFIELDS];
  long IP21DataType[MAXMAPFIELDS];
  CString IP21FieldLabel[MAXMAPFIELDS];
  
  OracleMAP();
	virtual ~OracleMAP();

private:
	long GetMapRecordID();
	long OracleMapRecID;

  long NUM_DB_FIELDS_FT,
       MESSAGE_FT,
       ERRPR_TYPE_FT,
       ORACLE_FIELD_FT,
       IP21_FIELD_FT,
       OCCURRENCE_NUM_FT,
       DATA_TYPE_FT,
       FIELD_LABEL_FT,
       USE_FT,
       COMMENT_FT,
       SQL_RECORD_FT;

};

#endif // !defined(AFX_ORACLEMAP_H__5D2C7CC8_DF55_402D_8B70_B1B9124E0003__INCLUDED_)
