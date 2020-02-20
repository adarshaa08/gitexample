// The present software is not subject to the US Export Administration Regulations (no exportation license required), May 2012
#ifndef PAL_H_
#define PAL_H_

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#else
#include <pthread.h>
#include <linux/limits.h>
#endif

/*-----------------------------------------------------------*/
/* Simple types                                              */
/*-----------------------------------------------------------*/

#ifdef WIN32
typedef __int32          T_Int;
typedef unsigned __int32 T_UInt;
#else
typedef signed int       T_Int;
typedef unsigned int     T_UInt;
#endif


/*-----------------------------------------------------------*/
/* Files                                                     */
/*-----------------------------------------------------------*/

/* Size max for a path - a filename (with null terminal string). */
#ifdef WIN32
#define PAL_MAX_PATH_SIZE     MAX_PATH
#else
#define PAL_MAX_PATH_SIZE     PATH_MAX
#endif
#define PAL_MAX_FILENAME_SIZE 256

/*!
*	\fn char* PAL_GetExecutableLocation(void)
*	\brief Retrieves the location of the executable file of the current process
*	\return pointer on the location (to free by the caller), NULL if not retrieved
**/
char* PAL_GetExecutableLocation(void);

/*!
*	\fn T_UInt PAL_GetExecutableName(char* i_pc_ExeName)
*	\brief Retrieves the name (without extension) of the executable file of the current process
*	\param[inout] i_pc_ExeName : Pointer on the name (buffer size is at least PAL_MAX_FILENAME_SIZE).
*	\return size of the name, 0 if not retrieved
**/
T_UInt PAL_GetExecutableName(char* i_pc_ExeName);

/* File name comparison */
#ifdef WIN32
#define PAL_CmpFileName _stricmp
#else
#define PAL_CmpFileName strcmp
#endif

/*!
*	\fn T_Int PAL_GetFileSize(char *i_pc_filepath)
*	\brief Get the size of a file
*	\param[in] i_pc_filepath : file path
*	\return size of the file, -1 if it doesn't exist
**/
T_Int PAL_GetFileSize(char *i_pc_filepath);

#ifdef WIN32
/*!
*	\fn FILE* PAL_FOpen(const char *i_pc_filename, const char *i_pc_mode)
*	\brief Open a file
*	\param[in] i_pc_filepath : file path
*	\param[in] i_pc_mode : mode
*	\return pointer on opened FILE, NULL if not opened.
**/
FILE* PAL_FOpen(const char *i_pc_filepath, const char *i_pc_mode);
#else
#define PAL_FOpen fopen
#endif

/*-----------------------------------------------------------*/
/* Mutex                                                     */
/*-----------------------------------------------------------*/
#ifdef WIN32
typedef HANDLE 			 T_Mutex;
#else
typedef pthread_mutex_t* T_Mutex;
#endif

/*!
*	\fn T_Mutex PAL_CreateMutex(void)
*	\brief Create a mutex
*	\return the mutex
**/
T_Mutex PAL_CreateMutex(void);

/*!
*	\fn T_Int	PAL_GetMutex(T_Mutex i_x_Mutex)
*	\brief Get a mutex
*	\return 0 the mutex is get, -1 otherwise
**/
T_Int	PAL_GetMutex(T_Mutex i_x_Mutex);

/*!
*	\fn T_Int	PAL_ReleaseMutex(T_Mutex i_x_Mutex)
*	\brief Release a mutex
*	\return 0 the mutex is released, -1 otherwise
**/
T_Int	PAL_ReleaseMutex(T_Mutex i_x_Mutex);

/*!
*	\fn T_Int	PAL_CloseMutex(T_Mutex i_x_Mutex)
*	\brief Delete a mutex
*	\return 0 the mutex is deleted, -1 otherwise
**/
T_Int	PAL_CloseMutex(T_Mutex i_x_Mutex);


/*-----------------------------------------------------------*/
/* Date-Time                                                 */
/*-----------------------------------------------------------*/

/* Size of a buffer time formatted as :  hh:mm:ss:mmm (with null terminal string)*/
#define PAL_STR_TIME_SIZE     13

/* Size of a buffer date-time formatted as :  dd/mm/yyyy hh:mm:ss:mmm (with null terminal string)*/
#define PAL_STR_DATETIME_SIZE 24

/*!
*	\fn void PAL_GetStrCurrentDateTime(char* i_pc_StrTime, char* i_pc_StrDateTime)
*	\brief Get the current date-time as strings
*	\param[inout] i_pc_StrTime : Pointer on the time to format as hh:mm:ss:mmm  (buffer size is at least PAL_STR_TIME_SIZE)
*	\param[inout] i_pc_StrDateTime : Pointer on the date-time to format as dd/mm/yyyy hh:mm:ss  (buffer size is at least PAL_STR_DATETIME_SIZE)
**/
void PAL_GetStrCurrentDateTime(char* i_pc_StrTime, char* i_pc_StrDateTime);


/*-----------------------------------------------------------*/
/* Others                                                    */
/*-----------------------------------------------------------*/
/*!
*	\fn T_UInt PAL_GetCurrentThreadId(void)
*	\brief Get the current thread id
*	\return the current thread id
**/
T_UInt PAL_GetCurrentThreadId(void);

#ifdef WIN32
/* string compatibilty */
#define vsnprintf(buff, num, fmt, ...) _vsnprintf_s(buff, num, _TRUNCATE, fmt, __VA_ARGS__)
#define snprintf(buff, num, fmt, ...) _snprintf_s(buff, num, _TRUNCATE, fmt, __VA_ARGS__)
#define strcasecmp _stricmp
#endif



#endif /* PAL_H_ */
