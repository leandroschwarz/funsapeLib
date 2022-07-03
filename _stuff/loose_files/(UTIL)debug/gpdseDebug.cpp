/* =============================================================================
 * Project:		GPSDE++ AVR Library
 * File name:	gpdseDebug.cpp
 * Module:		Debug module for GPDSE++ Library project
 * Author :		Leandro Schwarz
 * Build :		1002
 * Date :		June 25, 2018
 * ========================================================================== */

// =============================================================================
// Dependencies
// =============================================================================

#include "gpdseDebug.hpp"
#if __GPDSE_DEBUG_HPP != 1002
#	error [gpdseDebug.cpp] Error 102 - Build mismatch on header and source code files.
#endif

// =============================================================================
// Global variables
// =============================================================================

const char dbgSysMainConfig[] PROGMEM			= "main=>configuration";
const char dbgSysMainLoop[] PROGMEM				= "main=>loop";

const char dbgDffConstructor[] PROGMEM			= "demiFatFs=>constructor";
const char dbgDffDestructor[] PROGMEM			= "demiFatFs=>destructor";

const char * const dbgFunctionTable[] PROGMEM = {
	// System functions
	dbgSysMainConfig,				// 0
	dbgSysMainLoop,						// 1

	// DemiFatFs
	dbgDffConstructor,					// 2
	dbgDffDestructor,					// 3

	// debugDemiFatFs_ChangeBitmap,
	// debugDemiFatFs_CheckFs,
	// debugDemiFatFs_ChkChr,
	// debugDemiFatFs_ChkLock,
	// debugDemiFatFs_ClearLock,
	// debugDemiFatFs_ClmtClust,
	// debugDemiFatFs_Clst2Sect,
	// debugDemiFatFs_CmpLfn,
	// debugDemiFatFs_CreateChain,
	// debugDemiFatFs_CreateName,
	// debugDemiFatFs_CreateXdir,
	// debugDemiFatFs_Dbc1st,
	// debugDemiFatFs_Dbc2nd,
	// debugDemiFatFs_DecLock,
	// debugDemiFatFs_DirAlloc,
	// debugDemiFatFs_DirClear,
	// debugDemiFatFs_DirFind,
	// debugDemiFatFs_DirNext,
	// debugDemiFatFs_DirRead,
	// debugDemiFatFs_DirRegister,
	// debugDemiFatFs_DirRemove,
	// debugDemiFatFs_DirSdi,
	// debugDemiFatFs_EnqLock,
	// debugDemiFatFs_FChDir,
	// debugDemiFatFs_FChDrive,
	// debugDemiFatFs_FChMod,
	// debugDemiFatFs_FClose,
	// debugDemiFatFs_FCloseDir,
	// debugDemiFatFs_FExpand,
	// debugDemiFatFs_FFdisk,
	// debugDemiFatFs_FFindFirst,
	// debugDemiFatFs_FFindNext,
	// debugDemiFatFs_FForward,
	// debugDemiFatFs_FGetCwd,
	// debugDemiFatFs_FGetFree,
	// debugDemiFatFs_FGetLabel,
	// debugDemiFatFs_Fgets,
	// debugDemiFatFs_FillFirstFrag,
	// debugDemiFatFs_FillLastFrag,
	// debugDemiFatFs_FindBitmap,
	// debugDemiFatFs_FindVolume,
	// debugDemiFatFs_FLseek,
	// debugDemiFatFs_FMkDir,
	// debugDemiFatFs_FMkFs,
	// debugDemiFatFs_FMount,
	// debugDemiFatFs_FollowPath,
	// debugDemiFatFs_FOpen,
	// debugDemiFatFs_FOpenDir,
	// debugDemiFatFs_FPrintf,
	// debugDemiFatFs_FPutc,
	// debugDemiFatFs_FPuts,
	// debugDemiFatFs_FRead,
	// debugDemiFatFs_FReadDir,
	// debugDemiFatFs_FRename,
	// debugDemiFatFs_FSetCp,
	// debugDemiFatFs_FSetLabel,
	// debugDemiFatFs_FStat,
	// debugDemiFatFs_FSync,
	// debugDemiFatFs_FTruncate,
	// debugDemiFatFs_FUnlink,
	// debugDemiFatFs_FUtime,
	// debugDemiFatFs_FWrite,
	// debugDemiFatFs_GenNunName,
	// debugDemiFatFs_GetAchar,
	// debugDemiFatFs_GetFat,
	// debugDemiFatFs_GetFileInfo,
	// debugDemiFatFs_GetLdNumber,
	// debugDemiFatFs_GetXfileInfo,
	// debugDemiFatFs_IncLock,
	// debugDemiFatFs_InitAllocInfo,
	// debugDemiFatFs_LdClust,
	// debugDemiFatFs_LdDword,
	// debugDemiFatFs_LdQword,
	// debugDemiFatFs_LdWord,
	// debugDemiFatFs_LoadObjXdir,
	// debugDemiFatFs_LoadXdir,
	// debugDemiFatFs_LockFs,
	// debugDemiFatFs_MemCmp,
	// debugDemiFatFs_MemCpy,
	// debugDemiFatFs_MemSet,
	// debugDemiFatFs_MoveWindow,
	// debugDemiFatFs_PatternMatching,
	// debugDemiFatFs_PickFln,
	// debugDemiFatFs_PutcBfd,
	// debugDemiFatFs_PutcFlush,
	// debugDemiFatFs_PutcInit,
	// debugDemiFatFs_PutFat,
	// debugDemiFatFs_PutFln,
	// debugDemiFatFs_PutUtf,
	// debugDemiFatFs_RemoveChain,
	// debugDemiFatFs_StClust,
	// debugDemiFatFs_StDword,
	// debugDemiFatFs_StoreXdir,
	// debugDemiFatFs_StQword,
	// debugDemiFatFs_StWord,
	// debugDemiFatFs_SumSfn,
	// debugDemiFatFs_SyncFs,
	// debugDemiFatFs_SyncWindow,
	// debugDemiFatFs_Tchar2Uni,
	// debugDemiFatFs_UnlockFs,
	// debugDemiFatFs_Validate,
	// debugDemiFatFs_XdirSum,
	// debugDemiFatFs_XnameSum,
	// debugDemiFatFs_Xsum32,
};

// =============================================================================
// Function definitions
// =============================================================================

void dbgFunctionEntering(dbgFunctionName_e funcName)
{
	printf_P(PSTR("BEGIN =>%d()\r"), funcName);
//		printf_P(*(dbgFunctionTable + x));
}

void dbgFunctionReturning(dbgFunctionName_e funcName, uint16_t returnCode)
{
	printf_P(PSTR("END   <=%d(%d)\r"), funcName, returnCode);
	// printf_P(PSTR("R <= "));
	// printf_P(dbgFunctionTable[funcName]);
	// printf_P(PSTR("(%u)\r"), returnCode);
}

void dbgFunctionResuming(dbgFunctionName_e funcName)
{
	printf_P(PSTR("RESUME==%d\r"), funcName);
	// printf_P(PSTR("Resuming function: "));
	// printf_P(debugDemiFatFsTable[strCode]);
	// printf_P(PSTR("\r"));
}

void dbgPointReaching(uint32_t pointCode)
{
	// printf_P(PSTR("Reached point: %d\r"), pointCode);
}
