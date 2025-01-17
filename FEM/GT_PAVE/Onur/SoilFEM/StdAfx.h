// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C6F19A77_52E5_49DC_90D3_6FCA1F444815__INCLUDED_)
#define AFX_STDAFX_H__C6F19A77_52E5_49DC_90D3_6FCA1F444815__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//C++ STL include files
#include <vector>
#include <iostream>
#include <fstream>

//OpenGL include files
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

//Global include files
#include "TypeDefinitions.h"
#include "Vertex3.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C6F19A77_52E5_49DC_90D3_6FCA1F444815__INCLUDED_)
