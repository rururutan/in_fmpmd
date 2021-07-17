#if !defined(__IN_FMPMD_HPP__)
#define __IN_FMPMD_HPP__
/**
 *
 * @file interface.hpp
 * @brief interface class
 * @author RuRuRu
 *
 */

#include <windows.h>
#include "winamp/wa_ipc.h"

namespace FmPmd
{
	void Init(void);
	void Quit(void);
	void Config(HWND);
	void About(HWND);
	void GetFileInfo(const in_char *, in_char *, int *);
	int  InfoBox(const in_char *, HWND);
	int  IsOurFile(const in_char *);

	int  Play(const in_char *);
	void Pause();
	void UnPause();
	int  IsPaused();
	void Stop();

	int  GetLength();
	int  GetOutputTime();
	void SetOutputTime(int);

	void SetVolume(int);
	void SetPan(int);

	void SetEq(int, char data[10], int);

#ifndef UNICODE_INPUT_PLUGIN
	int  GetExtendedFileInfo(const char *p_file, char *metadata, char *ret, int retlen);
#else
	int  GetExtendedFileInfoW(extendedFileInfoStructW&);
#endif
	void LoadDll(const char*&);
}

#endif /* __IN_FMPMD_HPP__ */
