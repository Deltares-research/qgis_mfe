#ifndef _INC_F90LINKAGE
#define _INC_F90LINKAGE

#if defined(WIN32) || defined(WIN64)
//
// VS2005: __stdcall changed to __cdecl
//
#  define FC_FUNC(name,NAME) NAME
#  define F90_FUNCTION_POINTER __cdecl *
#  define F90_EXPORT(type, name) extern "C" __declspec(dllexport) type  __cdecl name
#  define F90_IMPORT(type, name) extern "C" type  __cdecl name
#else
#  include "config.h"
#  define F90_FUNCTION_POINTER *
#  define F90_EXPORT(type, name) extern "C" type name
#  define F90_IMPORT(type, name) extern "C" type name
#endif  /* WIN32 */
#endif  /* _INC_F90LINKAGE */
