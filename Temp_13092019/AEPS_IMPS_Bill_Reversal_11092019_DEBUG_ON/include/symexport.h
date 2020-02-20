// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
/*!
 * @file symexport.h
 * @brief Symbol export macro definitions.
 *
 * Copyright &copy; 2012, Morpho.
 *
 */


#ifndef __SYMEXPORT_H__
#define __SYMEXPORT_H__


#ifndef LIB_API
	// Generic helper definitions for shared library support
	#if defined _WIN32 || defined __CYGWIN__
		#define LIB_HELPER_DLL_IMPORT		__declspec(dllimport)
		#define LIB_HELPER_DLL_EXPORT		__declspec(dllexport)
	#else
		#if __GNUC__ >= 4
			#define LIB_HELPER_DLL_IMPORT	__attribute__((visibility("default")))
			#define LIB_HELPER_DLL_EXPORT	__attribute__((visibility("default")))
		#else
			#define LIB_HELPER_DLL_IMPORT
			#define LIB_HELPER_DLL_EXPORT
		#endif
	#endif

	#ifndef LIB_SLIB // LIB_SLIB is not defined: LIBSRV is a shared library
		#ifdef LIB_DLL_EXPORTS // defined if LIBSRV is being built instead of being used
			#define LIB_API		LIB_HELPER_DLL_EXPORT
		#else
			#define LIB_API		LIB_HELPER_DLL_IMPORT
		#endif
	#else // LIB_SLIB is defined: LIBSRV is a static library
		#define LIB_API
	#endif
#endif


#endif //__SYMEXPORT_H__
