// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define APPVER         "3.0"                  // major and minor version 
#define BUILDNUMBER    "000"                 // application build number
#define BUILDDATE      "06/05/2009"           // application build date

#define VC_EXTRALEAN		      // Exclude rarely-used stuff from Windows headers
#define ORGCODELEN         4  // Organization Code character length
#define ORACLEDBINFOLEN   24  // Oracle connection information string length

#define NAMELEN           16  // OracleMap character field lengths
#define ERRORTEXTLEN      40
#define LABELLEN          24
#define CHARSTRINGLEN    120

#define MAPORACLEFIELDLEN 40
#define MAPMESSAGELEN     40
#define MAPCOMMENTLEN     40

#define MAPDATATYPE_UNKNOWN   0
#define MAPDATATYPE_INTEGER   1
#define MAPDATATYPE_REAL      2
#define MAPDATATYPE_CHARACTER 3
#define MAPDATATYPE_TIMESTAMP 4

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
#include "afxdb.h"
#include "setcim.h"
#include <adoid.h>
#include <adoint.h>
#include <icrsint.h>
#include "memory.h"
#include "string.h"

static ERRBLOCK  IP21error;     // IP21 errors from function calls
static ERRARRAY  IP21message;   // string for IP21 error messages
static short     ErrorLength;   // length of error message

// TODO: reference additional headers your program requires here
