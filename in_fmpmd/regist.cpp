/**
 *
 * @file regist.cpp
 * @brief
 * @author rururu
 *
 */

#include <windows.h>
#include "winamp/in2.h"
#include "winamp/wa_ipc.h"
#include "interface.hpp"
#include "version.hpp"

namespace FmPmd {

	In_Module module = {
		IN_VER,
		PLUGIN_DETAIL,
		0,	/* hMainWindow (filled in by winamp after init() ) */
		0,  /* hDllInstance ( Also filled in by winamp) */
		NULL, // FileExtensions handled by this DLL

		1,	/* is_seekable (is this stream seekable?) */
		1, /* uses output (does this plug-in use the output plug-ins?) */
		Config,
		About,
		Init,
		Quit,
		GetFileInfo,
		InfoBox,
		IsOurFile,

		/* PLAYBACK STUFF */
		Play,
		Pause,
		UnPause,
		IsPaused,
		Stop,

		/* TIME STUFF */
		GetLength,
		GetOutputTime,
		SetOutputTime,

		/* VOLUME STUFF */
		SetVolume,
		SetPan,

		/* VIS STUFF */
		0,0,0,0,0,0,0,0,0,

		/* DSP STUFF */
		0,0,

		/* EQ STUFF */
		SetEq,

		/* info setting (filled in by winamp) */
		NULL,

		/* Out_Module *outMod; (filled in by winamp, optionally used) */
		0
	};
}

extern "C" {

/**
 * Get module pointer
 *
 * @return pointer of input plugin's module
 */
__declspec(dllexport) In_Module *winampGetInModule2()
{
	// Load DLL
	if (!FmPmd::module.FileExtensions)
		FmPmd::LoadDll(const_cast<const char*&>(FmPmd::module.FileExtensions));

	return &FmPmd::module;
}

#ifndef UNICODE_INPUT_PLUGIN
/**
 * Get extendeed file information(ANSI/MultiByte)
 *
 * @param p_file [in]
 *
 * @param p_metadeta [in]
 *
 * @param p_ret [out]
 *
 * @param p_retlen [out]
 *
 * @return ok(1) / ng(0)
 */
__declspec(dllexport) int winampGetExtendedFileInfo(char *p_file, char *p_metadata, char *p_ret, int p_retlen)
{
	return FmPmd::GetExtendedFileInfo(p_file, p_metadata, p_ret, p_retlen);
}
#else
/**
 * Get extendeed file information(UTF-16)
 *
 * @param p_extended_info [in/out]
 *
 * @return ok(1) / ng(0)
 */
__declspec(dllexport) int winampGetExtendedFileInfoW(extendedFileInfoStructW p_extended_info)
{
	return FmPmd::GetExtendedFileInfoW(p_extended_info);
}

}
#endif
