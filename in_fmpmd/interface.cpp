/**
 *
 * @file interface.cpp
 * @brief
 * @author RuRuRu
 * @todo ch mask(real time)
 * @todo ch volume(real time)
 * @todo getmemo sub routine
 *
 */

#include <windows.h>
#include <unknwn.h>
#include "PMDWinImport.h"
#include "WinFMPImport.h"
#include "winamp/in2.h"
#include "code/code_conv.hpp"
#include "config.hpp"
#include "dialog.hpp"
#include "interface.hpp"

namespace FmPmd
{
	static const size_t MAX_MEMO_LENGTH = 1024;
	static const int MAX_CH = 2;
	static const int MAX_BPS = 16;
	static const char pmd_extension[] = "M;M2;MZ;MP;MS\0PMD music files (*.m;*m2;*.mz;*.mp;*.ms)";
	static const char fmp_extension[] = "OPI;OVI;OZI\0FMP music files (*.opi;*ovi;*.ozi)";

	static const int PMD_MASK_TABLE_LENGTH = 22;
	static const int PmdMaskTable[PMD_MASK_TABLE_LENGTH] = {
		0, 1, 2, 3, 4, 5,
		11, 12, 13,
		6, 7, 8,
		9,
		14,
		16, 17, 18, 19, 20, 21, 22, 23
	};
	static const int FMP_MASK_TABLE_LENGTH = 13;
	static const int FmpMaskTable[FMP_MASK_TABLE_LENGTH] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8,
		18, 19, 20,
		21
	};

	/* sequencer struct */
	struct PluginWork
	{
	  public:
		char file_extension[sizeof(pmd_extension) + sizeof(fmp_extension) + 1];
									/**< File extension string */
		HINSTANCE PmdDll;			/**< PDMWin.dll handle */
		HINSTANCE FmpDll;			/**< WinFMP.dll handle */
		IPMDWIN *PmdWin;			/**< PMDWin instance(play) */
		IWINFMP *FmpWin;			/**< WinFMP instance(play) */
		IPMDWIN *PmdWin2;			/**< PMDWin instance(info) */
		IWINFMP *FmpWin2;			/**< WinFMP instance(info) */
		CRITICAL_SECTION InfoCs;	/**< Critical section for xxxWin2 */

		enum PlayMode {
			mode_pmd,
			mode_fmp
		};
		// 演奏時設定
		PlayMode PlayMode;
		std::string file_name;		/**< File name */
		int LoopLimit;				/**< loop count limit */
		int FadeState;				/**< 0:normal 1-:fading out(dB/(1 >> FADE_SHIFT)) */
		HANDLE ThreadHandle;		/**< decode thread handle */
		bool DecodeFlag;			/**< decode flag */
		bool Paused;				/**< true:paused false:not paused */
		int StartLength;			/**< length */
		int LoopLength;				/**< length */
		int FadeLength;				/**< length */
		int Frequency;				/**< mixing frequency */
		bool InfiniteLoop;			/**< infinite loop */

	  public:
		PluginWork() :
		PmdDll(NULL),
		FmpDll(NULL),
		PmdWin(NULL),
		PmdWin2(NULL),
		FmpWin(NULL),
		FmpWin2(NULL),
		ThreadHandle(NULL),
		DecodeFlag(false),
		FadeState(0),
		LoopLimit(1),
		StartLength(0),
		LoopLength(0),
		FadeLength(0),
		Paused(false),
		InfiniteLoop(false)
		{}
	};

	DWORD WINAPI DecodeThread(void *p_bDecode);

	extern In_Module module;
	PluginSetting setting;
	PluginWork work;

	/**
	 * Delete multiplex string
	 *
	 * @param p_str [in/out]
	 *
	 * @param p_space [in]
	 *
	 */
	static void DeleteMultiplexString(std::wstring &p_str, in_char p_space)
	{
		std::wstring::size_type pos, nextpos = 0;
		std::wstring str_space;
		str_space.append(&p_space, 1);
		while ((pos = p_str.find(p_space, nextpos)) != std::wstring::npos) {
			std::wstring::size_type i =0;
			for (i=0; i < p_str.size(); i++) {
				if (p_str[pos + i] != p_space)
				  break;
			}
			if (i > 1)
			  p_str.replace(pos, i, str_space);
			nextpos++;
		}
	}

	/**
	 * Trim space
	 *
	 * @param p_str [in/out]
	 *
	 */
	static void TrimString(std::wstring &p_str)
	{
		std::wstring::size_type pos;
		pos = p_str.find_first_not_of(L" ");
		if (pos > 0)
			p_str.erase(0, pos);
		pos = p_str.find_first_not_of(L"　");
		if (pos > 0)
			p_str.erase(0, pos);
	}

	/**
	 * Check PMD file name.
	 *
	 * @param p_filename [in]
	 *     file name string.
	 * @return true / false
	 */
	static bool IsPmdName(const in_char *p_filename) {
		std::wstring file_name(p_filename);
		size_t ofs = file_name.find_last_of(L".");
		std::wstring file_ext;
		if (ofs) {
			file_ext.assign(file_name.substr(ofs+1, 3));
		}
		if (_wcsicmp(file_ext.c_str(), L"M") == 0 ||
			_wcsicmp(file_ext.c_str(), L"M2") == 0 ||
			_wcsicmp(file_ext.c_str(), L"MZ") == 0 ||
			_wcsicmp(file_ext.c_str(), L"MS") == 0 ||
			_wcsicmp(file_ext.c_str(), L"MP") == 0 )
		{
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Check FMP file name.
	 *
	 * @param p_filename [in]
	 *     file name string.
	 * @return true / false
	 */
	static bool IsFmpName(const in_char *p_filename) {
		std::wstring file_name(p_filename);
		size_t ofs = file_name.find_last_of(L".");
		std::wstring file_ext;
		if (ofs) {
			file_ext.assign(file_name.substr(ofs+1, 3));
		}
		if (_wcsicmp(file_ext.c_str(), L"OPI") == 0 ||
			_wcsicmp(file_ext.c_str(), L"OVI") == 0 ||
			_wcsicmp(file_ext.c_str(), L"OZI") == 0)
		{
			return true;
		} else {
			return false;
		}
	}

	/**
	 *
	 *
	 * @param p_path [in]
	 * @param p_comment [out]
	 * @return true / flase
	 */
	static bool GetFmpComment(const char *p_path, std::wstring &p_comment) {
		char *comment[3], comment1[MAX_MEMO_LENGTH], comment2[MAX_MEMO_LENGTH], comment3[MAX_MEMO_LENGTH];
		comment[0] = comment1;
		comment[1] = comment2;
		comment[2] = comment3;
		if (work.FmpWin2->fgetcomment3(comment, const_cast<char*>(p_path)) == WINFMP_OK) {
			std::string scomment(comment1);
			scomment.append(comment2);
			scomment.append(comment3);
			CharacterCode::SjisToUtf16 wconv(scomment);
			p_comment = wconv.GetData();
			DeleteMultiplexString(p_comment, L' ');
			DeleteMultiplexString(p_comment, L'　');
			TrimString(p_comment);
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Calculate total playing length.
	 * 
	 * @param p_StartTime [in]
	 *
	 * @param p_LoopTime [in]
	 *
	 * @param p_FadeTime [in]
	 *
	 * @return total time.
	 */
	static int CalcLength(int p_StartTime, int p_LoopTime, int p_FadeTime, int p_LoopLimit)
	{
		int Result(p_StartTime);
		if (p_LoopTime > 0 && p_LoopLimit > 0) {
			Result += p_LoopTime * (p_LoopLimit -1);
			Result += p_FadeTime;
		}
		return Result;
	}

	/**
	 * Load DLL
	 *
	 * @param p_extension [out]
	 *
	 */
	void LoadDll(const char *&p_extension)
	{
		size_t extension_pos(0);

		// DLLのロード
		if (!work.PmdDll) {
			work.PmdDll = ::LoadLibrary(L"PMDWin.dll");
			if (work.PmdDll) {
				::memcpy(work.file_extension, pmd_extension, sizeof(pmd_extension));
				extension_pos = sizeof(pmd_extension);
			}
		}
		if (!work.FmpDll) {
			work.FmpDll = ::LoadLibrary(L"WinFMP.dll");
			if (work.FmpDll) {
				::memcpy(work.file_extension+extension_pos, fmp_extension, sizeof(fmp_extension));
				extension_pos += sizeof(fmp_extension);
			}
		}
		work.file_extension[extension_pos] = '\0';
		p_extension = work.file_extension;
	}


	/**
	 * Create context
	 *
	 */
	void Init(void)
	{
		// 設定のロード
		in_char my_path[MAX_PATH];
		::GetModuleFileName(module.hDllInstance, my_path, MAX_PATH);
		setting.Load(my_path);

		// 関数アドレスの取得
		if (work.PmdDll) {
			HRESULT (WINAPI*pmd_CoCreateInstance)(
				REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
			pmd_CoCreateInstance = (HRESULT (WINAPI*)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*))GetProcAddress(work.PmdDll, "pmd_CoCreateInstance");
			if (pmd_CoCreateInstance && SUCCEEDED(pmd_CoCreateInstance(CLSID_PMDWIN, NULL, CLSCTX_ALL, IID_IPMDWIN, (void**)&work.PmdWin))) {
				CharacterCode::Utf16ToMb sryhthm((const LPWSTR)setting.RhythmPath.c_str());
				work.PmdWin->init(const_cast<char*>(sryhthm.GetData()));
			}
			if (pmd_CoCreateInstance && SUCCEEDED(pmd_CoCreateInstance(CLSID_PMDWIN, NULL, CLSCTX_ALL, IID_IPMDWIN, (void**)&work.PmdWin2))) {
				work.PmdWin2->init("");
			}
		}
		if (work.FmpDll) {
			HRESULT (WINAPI*fmp_CoCreateInstance)(
				REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*);
			fmp_CoCreateInstance = (HRESULT (WINAPI*)(REFCLSID, LPUNKNOWN, DWORD, REFIID, LPVOID*))GetProcAddress(work.FmpDll, "fmp_CoCreateInstance");
			if (fmp_CoCreateInstance && SUCCEEDED(fmp_CoCreateInstance(CLSID_WINFMP, NULL, CLSCTX_ALL, IID_IWINFMP, (void**)&work.FmpWin))) {
				CharacterCode::Utf16ToMb sryhthm((const LPWSTR)setting.RhythmPath.c_str());
				work.FmpWin->init(const_cast<char*>(sryhthm.GetData()));
			}
			if (fmp_CoCreateInstance && SUCCEEDED(fmp_CoCreateInstance(CLSID_WINFMP, NULL, CLSCTX_ALL, IID_IWINFMP, (void**)&work.FmpWin2))) {
				work.FmpWin2->init("");
			}
		}

		::InitializeCriticalSection(&work.InfoCs);
	}

	/**
	 * Destroy context
	 *
	 */
	void Quit(void)
	{
		// コンテキストの破棄
		if (work.PmdWin)
			work.PmdWin->Release();
		if (work.PmdWin2)
			work.PmdWin2->Release();
		if (work.FmpWin)
			work.FmpWin->Release();
		if (work.FmpWin2)
			work.FmpWin2->Release();

		// DLLの破棄
		if (work.PmdDll)
		  ::FreeLibrary(work.PmdDll);
		if (work.FmpDll)
		  ::FreeLibrary(work.FmpDll);

		// 設定のセーブ
		setting.Save();

		::DeleteCriticalSection(&work.InfoCs);
	}

	/**
	 * Display config dialog.
	 *
	 * @param p_parent [in]
	 *     parent window handle
	 */
	void Config(HWND p_parent)
	{
		// config dialog
		ConfigDialog(p_parent);
	}

	/**
	 * Display about dialog.
	 *
	 * @param p_parent [in]
	 *     parent window handle
	 */
	void About(HWND p_parent)
	{
		// abort dialog
		AboutDialog(p_parent);
	}

	/**
	 * Get file information
	 *
	 * @param p_file [in]
	 *     file name string
	 *     NULLの場合は演奏中ファイルを指定
	 * @param p_title [out]
	 *     title string
	 * @param p_length [out]
	 *     time length (ms)
	 */
	void GetFileInfo(const in_char *p_file, in_char *p_title, int *p_length)
	{
		if (p_file == NULL) {
			*p_length = GetLength();
			p_title[0] = '\0';			// @todo ちゃんと設定する!
			return;
		}

		CharacterCode::Utf16ToMb sfile((const LPWSTR)p_file);
		int StartLen(0), LoopLen(0);

		if (IsPmdName(p_file) && work.PmdWin2) {
			char title[GETFILEINFO_TITLE_LENGTH] = {0};
			::EnterCriticalSection(&work.InfoCs);
			if (work.PmdWin2->getlength(const_cast<char*>(sfile.GetData()), &StartLen, &LoopLen) == true) {
				*p_length = CalcLength(StartLen, LoopLen, setting.FadeLength, setting.LoopLimit);
				work.PmdWin2->fgetmemo3(title, const_cast<char*>(sfile.GetData()), 1);
				CharacterCode::SjisToUtf16 wtitle(title);
				wcsncpy(p_title, wtitle.GetData(), GETFILEINFO_TITLE_LENGTH);
			}
			::LeaveCriticalSection(&work.InfoCs);
			return;
		}
		if (IsFmpName(p_file) && work.FmpWin2) {
			std::wstring WideComment;

			::EnterCriticalSection(&work.InfoCs);
			if (work.FmpWin2->getlength(const_cast<char*>(sfile.GetData()), &StartLen, &LoopLen) == true) {
				*p_length = CalcLength(StartLen, LoopLen, setting.FadeLength, setting.LoopLimit);
			}
			bool Result = GetFmpComment(sfile.GetData(), WideComment);
//				work.FmpWin2->fgetcomment3(comment, const_cast<char*>(sfile.GetData()));
//			}
			::LeaveCriticalSection(&work.InfoCs);
			if (Result == true) {
				wcsncpy(p_title, WideComment.c_str(), GETFILEINFO_TITLE_LENGTH);
			}
			return;
		}
	}

#ifndef UNICODE_INPUT_PLUGIN
	/**
	 * Get extra file information (ansi)
	 *
	 * @param p_file [in]
	 *     file name string
	 * @param p_metadata [in]
	 * @param p_ret [out]
	 * @param p_retlen [out]
	 * @return 0(fail) / 1(success)
	 */
	int GetExtendedFileInfo(const char *p_file, char *p_metadata, char *p_ret, int p_retlen)
	{
		if (_stricmp(p_metadata, "type") == 0) {
			::strncpy(p_ret, "0", p_retlen);	// type audio
			return 1;
		}

		if (_stricmp(p_metadata, "length") == 0) {
			int length = 0, loop = 0;

			::EnterCriticalSection(&work.InfoCs);
			bool result = work.PmdWin2->getlength(const_cast<char*>(p_file), &length, &loop);
			if (result == false)
			  result = work.FmpWin2->getlength(const_cast<char*>(p_file), &length, &loop);
			::LeaveCriticalSection(&work.InfoCs);
			if (result == false)
			  return 0;

			char buff[16];
			int length_in_ms = CalcLength(length, loop, setting.m_fade_time, setting.m_loop_limit);
			_itoa(length_in_ms, buff, 10);
			strncpy(p_ret, buff, p_retlen);
			return 1;
		}
		if (_stricmp(p_metadata, "title") == 0) {
			::EnterCriticalSection(&work.InfoCs);
			int result = work.PmdWin2->fgetmemo3(p_ret, const_cast<char*>(p_file), 1);
			::LeaveCriticalSection(&work.InfoCs);
			if (result == PMDWIN_OK) {
				p_retlen = strlen(p_ret);
				return 1;
			}
		}
		if (_stricmp(p_metadata, "artist") == 0) {
			::EnterCriticalSection(&work.InfoCs);
			int result = work.PmdWin2->fgetmemo3(p_ret, const_cast<char*>(p_file), 3);
			::LeaveCriticalSection(&work.InfoCs);
			if (result == WINFMP_OK) {
				p_retlen = strlen(p_ret);
				return 1;
			}
		}
		return 0;
	}
#else
	/**
	 * Get extra file information (unicode)
	 *
	 * @param p_file_info [in]
	 *     file info structure
	 * @return 0:fail / 1:success
	 */
	int GetExtendedFileInfoW(extendedFileInfoStructW &p_file_info)
	{
		CharacterCode::Utf16ToMb sfile((const LPWSTR)p_file_info.filename);

		// metadataは何故かutf-16ではなくasciiで入ってくる。
		const char *metadata = reinterpret_cast<const char*>(p_file_info.metadata);

		if (::_stricmp(metadata, "type") == 0) {
			wcsncpy(p_file_info.ret, L"0",  p_file_info.retlen);	// type audio
			return 1;
		}

		if (::_stricmp(metadata, "length") == 0) {
			int StartLen(0), LoopLen(0);
			::EnterCriticalSection(&work.InfoCs);
			bool result = work.PmdWin2->getlength(const_cast<char*>(sfile.GetData()), &StartLen, &LoopLen);
			if (result == false)
			  result = work.FmpWin2->getlength(const_cast<char*>(sfile.GetData()), &StartLen, &LoopLen);
			::LeaveCriticalSection(&work.InfoCs);
			if (result == false)
				return 0;

			int length_in_ms = CalcLength(StartLen, LoopLen, setting.FadeLength, setting.LoopLimit);
			in_char buff[16];
			_itow(length_in_ms, buff, 10);
			wcsncpy(p_file_info.ret, buff, p_file_info.retlen);
			return 1;
		}

		if (::_stricmp(metadata, "artist") == 0) {
			char smemo[MAX_MEMO_LENGTH];
			::EnterCriticalSection(&work.InfoCs);
			int result = work.PmdWin2->fgetmemo3(smemo, const_cast<char*>(sfile.GetData()), 3);
			::LeaveCriticalSection(&work.InfoCs);
			if (result == PMDWIN_OK) {
				CharacterCode::SjisToUtf16 wmemo((const LPSTR)smemo);
				wcsncpy(p_file_info.ret, wmemo.GetData(), p_file_info.retlen);
				p_file_info.retlen = static_cast<int>(wcslen(p_file_info.ret));
				return 1;
			}
		}

		if (::_stricmp(metadata, "title") == 0) {
			if (IsPmdName(p_file_info.filename)) {
				char smemo[MAX_MEMO_LENGTH];
				::EnterCriticalSection(&work.InfoCs);
				int result = work.PmdWin2->fgetmemo3(smemo, const_cast<char*>(sfile.GetData()), 1);
				::LeaveCriticalSection(&work.InfoCs);
				if (result == PMDWIN_OK) {
					CharacterCode::SjisToUtf16 wmemo((const LPSTR)smemo);
					wcsncpy(p_file_info.ret, wmemo.GetData(), p_file_info.retlen);
					p_file_info.retlen = static_cast<int>(wcslen(p_file_info.ret));
					return 1;
				}
			} else {
				char *comment[3], comment1[MAX_MEMO_LENGTH], comment2[MAX_MEMO_LENGTH], comment3[MAX_MEMO_LENGTH];
				comment[0] = comment1;
				comment[1] = comment2;
				comment[2] = comment3;
				std::wstring WideComment;

				::EnterCriticalSection(&work.InfoCs);
				bool Result = GetFmpComment(sfile.GetData(), WideComment);
//				int result = work.FmpWin2->fgetcomment3(comment, (char*)(sfile.GetData()));
				::LeaveCriticalSection(&work.InfoCs);
				if (Result == true) {
					wcsncpy(p_file_info.ret, WideComment.c_str(), p_file_info.retlen);
					p_file_info.retlen = static_cast<int>(wcslen(p_file_info.ret));
					return 1;
				}
			}
		}

		if (::_stricmp(metadata, "comment") == 0) {
		}

		return 0;
	}
#endif

	/**
	 * Display file info dialog.
	 *
	 * @param p_file [in]
	 *     file name string
	 * @param p_parent_handle [in]
     *
	 * @return 0:success / 1:error
	 */
	int InfoBox(const in_char *p_file, HWND p_parent_handle)
	{
		CharacterCode::Utf16ToMb sfile((const LPWSTR)p_file);

		if (IsPmdName(p_file)) {
			if (!work.PmdWin2)
			  return 1;

			char smemo[MAX_MEMO_LENGTH];
			PmdInfo info;
			static const int PmdMemo[] = {1,2,3,-2,-1,0,4};

			int result;
			::EnterCriticalSection(&work.InfoCs);
			for (int i=0; i < 6; i++) {
				smemo[0] = '\0';
				result =work.PmdWin2->fgetmemo3(smemo, (char*)(sfile.GetData()), PmdMemo[i]);
				if (result != PMDWIN_OK)
					break;
				CharacterCode::MbToUtf16 umemo1(smemo);
				info.memo[i].assign(umemo1.GetData());
			}
			int j=0;
			do {
				smemo[0] = '\0';
				result =work.PmdWin2->fgetmemo3(smemo, (char*)(sfile.GetData()), PmdMemo[6]+j);
				if (result != PMDWIN_OK)
					break;
				CharacterCode::MbToUtf16 umemo1(smemo);
				if (j > 0)
					info.memo[6].append(L"\r\n");
				info.memo[6].append(umemo1.GetData());
				j++;
			} while(smemo[0] != '\0');
			::LeaveCriticalSection(&work.InfoCs);

			// info dialog
			if (result == PMDWIN_OK)
				InfoPmdDialog(p_parent_handle, info);

			return 0;
		}

		if (IsFmpName(p_file)) {
			if (!work.FmpWin2)
			  return 1;

			FmpInfo info;
			char *comment[3], comment1[MAX_MEMO_LENGTH], comment2[MAX_MEMO_LENGTH], comment3[MAX_MEMO_LENGTH];
			comment[0] = comment1;
			comment[1] = comment2;
			comment[2] = comment3;
			int result;

			::EnterCriticalSection(&work.InfoCs);
			result = work.FmpWin2->fgetcomment3(comment, const_cast<char*>(sfile.GetData()));
			if (result == WINFMP_OK) {
				for (int i=0; i < 3; i++) {
					CharacterCode::MbToUtf16 ucomment(comment[i]);
					info.comment[i].assign(ucomment.GetData());
				}
				comment1[0] = '\0';
				work.FmpWin2->fgetdefinedpcmfilename(comment1, const_cast<char*>(sfile.GetData()));
				CharacterCode::MbToUtf16 upcmname(comment1);
				info.pcmname.assign(upcmname.GetData());
				comment1[0] = '\0';
				work.FmpWin2->fgetdefinedppzfilename(comment1, const_cast<char*>(sfile.GetData()), 0);
				CharacterCode::MbToUtf16 uppzname(comment1);
				info.ppzname.assign(uppzname.GetData());
			}
			::LeaveCriticalSection(&work.InfoCs);

			// info dialog
			if (result == WINFMP_OK)
				InfoFmpDialog(p_parent_handle, info);

			return 0;
		}

		return 1;
	}

	/**
	 * Check support file
	 *
	 * @param p_file [in]
	 *     file name string.
	 * @return
	 *     0 only
	 */
	int IsOurFile(const in_char *p_file)
	{
		return 0;
	}

	/**
	 * Play start
	 *
	 * @param p_file [in]
	 *     file name staring
	 * @return 0:success / -1:no file / other:error
	 */
	int Play(const in_char *p_file)
	{
		CharacterCode::Utf16ToMb sfile((const LPWSTR)p_file);
		work.Paused = false;

		// start playing
		if (IsPmdName(p_file)) {
			if (!work.PmdWin)
			  return 1;

			int error = work.PmdWin->music_load(const_cast<char*>(sfile.GetData()));
			switch(error) {
			  case ERR_OPEN_MUSIC_FILE:
				return -1;
			  case ERR_WRONG_MUSIC_FILE:
			  case ERR_WRONG_PPZ1_FILE:
			  case ERR_WRONG_PPZ2_FILE:
			  case ERR_WRONG_PPC_FILE:
			  case ERR_OUT_OF_MEMORY:
			  case ERR_OTHER:
				return 1;
			  case PMDWIN_OK:
			  default:
				break;
			}

			work.PlayMode = PluginWork::mode_pmd;
			work.PmdWin->setpcmrate(setting.Frequency);
			if (setting.EnablePpzFrequency)
			  work.PmdWin->setppzrate(setting.PpzFrequency);
			work.PmdWin->setppzinterpolation(setting.PcmInterpolation);
			work.PmdWin->setp86interpolation(setting.PcmInterpolation);
			work.PmdWin->setppsinterpolation(setting.PcmInterpolation);
			work.PmdWin->setfmcalc55k(setting.FmCalc55k);
			work.PmdWin->setpmd86pcmmode(setting.Pmdb2PcmMode);
			work.PmdWin->setrhythmwithssgeffect(setting.RhythmWithPps);
			work.PmdWin->setppsuse(setting.EnablePps);
			work.PmdWin->setfmvoldown(setting.PmdVolume[0]);
			work.PmdWin->setssgvoldown(setting.PmdVolume[1]);
			work.PmdWin->setrhythmvoldown(setting.PmdVolume[2]);
			work.PmdWin->setadpcmvoldown(setting.PmdVolume[3]);
			work.PmdWin->setppzvoldown(setting.PmdVolume[4]);
			{
				char *PathLists[MAX_PCMDIR+1] = {0};
				int MaxNum = (setting.PcmSearchPath.size() < MAX_PCMDIR)?setting.PcmSearchPath.size():MAX_PCMDIR;
				for (int i=0; i < MaxNum; i++) {
					PathLists[i] = new char[MAX_PATH];
					CharacterCode::Utf16ToMb spath((const LPWSTR)setting.PcmSearchPath[i].c_str());
					strncpy(PathLists[i], spath.GetData(), MAX_PCMDIR);
				}
				work.PmdWin->setpcmdir(PathLists);
				for (int i=0; i < MaxNum; i++) {
					delete [] PathLists[i];
				}
			}
			for (int i=0; i<PMD_MASK_TABLE_LENGTH; i++) {
				if (setting.m_ch_mask[i])
				  work.PmdWin->maskon(PmdMaskTable[i]);
				else
				  work.PmdWin->maskoff(PmdMaskTable[i]);
			}
			work.PmdWin->getlength(const_cast<char*>(sfile.GetData()), &work.StartLength, &work.LoopLength);
			work.PmdWin->music_start();
		} else if (IsFmpName(p_file)) {
			if (!work.FmpWin)
			  return 1;

			int error = work.FmpWin->music_load(const_cast<char*>(sfile.GetData()));
			switch(error) {
			  case FMP_ERR_OPEN_MUSIC_FILE:
				return -1;
			  case FMP_ERR_WRONG_MUSIC_FILE:
			  case FMP_ERR_WRONG_PVI_FILE:
			  case FMP_ERR_WRONG_PPZ1_FILE:
			  case FMP_ERR_OUT_OF_MEMORY:
				return 1;
			  case WINFMP_OK:
			  default:
				break;
			}

			work.PlayMode = PluginWork::mode_fmp;
			work.FmpWin->setpcmrate(setting.Frequency);
			if (setting.EnablePpzFrequency)
			  work.FmpWin->setppzrate(setting.PpzFrequency);
			work.FmpWin->setppzinterpolation(setting.PcmInterpolation);
			work.FmpWin->setfmcalc55k(setting.FmCalc55k);
			work.FmpWin->setadpcmppz8emulate(setting.EmulateAdpcmAtPpz8);
			work.FmpWin->setfmvoldown(setting.FmpVolume[0]);
			work.FmpWin->setssgvoldown(setting.FmpVolume[1]);
			work.FmpWin->setrhythmvoldown(setting.FmpVolume[2]);
			work.FmpWin->setadpcmvoldown(setting.FmpVolume[3]);
			work.FmpWin->getlength(const_cast<char*>(sfile.GetData()), &work.StartLength, &work.LoopLength);
			work.FmpWin->music_start();

			// FMPは演奏開始後にmask設定しないと反映されない
			{
				char *PathLists[MAX_PCMDIR+1] = {0};
				int MaxNum = (setting.PcmSearchPath.size() < MAX_PCMDIR)?setting.PcmSearchPath.size():MAX_PCMDIR;
				for (int i=0; i < MaxNum; i++) {
					PathLists[i] = new char[MAX_PATH];
					CharacterCode::Utf16ToMb spath((const LPWSTR)setting.PcmSearchPath[i].c_str());
					strncpy(PathLists[i], spath.GetData(), MAX_PCMDIR);
				}
				work.FmpWin->setpcmdir(PathLists);
				for (int i=0; i < MaxNum; i++) {
					delete [] PathLists[i];
				}
			}
			for (int i=0; i<FMP_MASK_TABLE_LENGTH; i++)
			  if (setting.m_ch_mask[i])
				work.FmpWin->maskon(FALSE, FmpMaskTable[i]);
			  else
				work.FmpWin->maskoff(FALSE, FmpMaskTable[i]);
			// rhythm用mask処理
			if (setting.m_ch_mask[13])
			  work.FmpWin->maskon(TRUE, 0x3f);
			else
			  work.FmpWin->maskoff(TRUE, 0x3f);
		} else {
			return 1;
		}

		work.Frequency = setting.Frequency;
		work.LoopLimit = setting.LoopLimit;
		work.FadeLength = setting.FadeLength;
		work.InfiniteLoop = setting.InfiniteLoop;

		// open output
		int MaxLatency = module.outMod->Open(work.Frequency, MAX_CH, MAX_BPS, 0, 0);
		if (MaxLatency < 0) {
			return 1;
		}

		// update info
		module.SetInfo((work.Frequency*MAX_CH*MAX_BPS)/1000, work.Frequency/1000, MAX_CH, 1);

		// update vis
		module.SAVSAInit(MaxLatency, work.Frequency);
		module.VSASetInfo(work.Frequency, MAX_CH);

		// create thread
		unsigned long ThreadId;
		work.ThreadHandle = static_cast<HANDLE>(::CreateThread(NULL, 0,
																static_cast<LPTHREAD_START_ROUTINE>(DecodeThread),
																static_cast<void *>(&work), 0,
																&ThreadId));
		::SetThreadPriority(work.ThreadHandle, THREAD_PRIORITY_NORMAL);
		return 0;
	}

	/**
	 * Pause
	 *
	 */
	void Pause()
	{
		module.outMod->Pause(1);
		work.Paused = true;
	}

	/**
	 * Un-pause
	 *
	 */
	void UnPause()
	{
		module.outMod->Pause(0);
		work.Paused = false;
	}

	/**
	 * Get pause statuse
	 *
	 * @return 0:no pause / 1:pause
	 */
	int IsPaused()
	{
		return (work.Paused==true)?1:0;
	}

	/**
	 * Play stop
	 *
	 */
	void Stop()
	{
		if (work.ThreadHandle != NULL) {
			work.DecodeFlag = false;
			if (::WaitForSingleObject(work.ThreadHandle, INFINITE) == WAIT_TIMEOUT) {
				::TerminateThread(work.ThreadHandle, 0);
			}
			::CloseHandle(work.ThreadHandle);
			work.ThreadHandle = NULL;
		}
		module.outMod->Close();
		module.SAVSADeInit();
	}

	/**
	 * Get total length
	 *
	 * @return total length [msec]
	 */
	int GetLength()
	{
		return CalcLength(work.StartLength, work.LoopLength, work.FadeLength, work.LoopLimit);
	}

	/**
	 * Get output time
	 *
	 * @return output time [msec]
	 */
	int GetOutputTime()
	{
		return module.outMod->GetOutputTime();
	}

	/**
	 * Seek
	 *
	 * @param p_time [in]
	 *     seek time [ms]
	 */
	void SetOutputTime(int p_time_in_ms)
	{
		if (work.PlayMode == PluginWork::mode_pmd) {
			work.PmdWin->setpos(p_time_in_ms);
		} else {
			work.FmpWin->setpos(p_time_in_ms);
		}
		module.outMod->Flush(p_time_in_ms);
	}

	/**
	 * Set volume value
	 *
	 * @param p_volume [in]
	 *     volume (0 ~ 255)
	 */
	void SetVolume(int p_volume)
	{
		module.outMod->SetVolume(p_volume);
	}

	/**
	 * Set panning position
	 *
	 * @param p_pan [in]
	 *     pan (-127 ~ 127)
	 */
	void SetPan(int p_pan)
	{
		module.outMod->SetPan(p_pan);
	}

	/**
	 * Set equlizer parameter
	 *
	 * @param p_on [in]
	 *
	 * @param p_data [in]
	 *
	 * @param p_preamp [in]
	 *
	 */
	void SetEq(int p_on, char p_data[10], int p_preamp)
	{
		// do nothing
	}

	/**
	 * Decode thread procedure
	 *
	 * @param p_Work [in]
	 *     PluginWork pointer
	 * @return 0
	 */
	DWORD WINAPI DecodeThread(void *p_Work)
	{
		static const int sample_len = 784;
		static short DecodeBuffer[MAX_CH * sample_len * 2];		// DSPで最大x2になるらしい
		PluginWork *Work = static_cast<PluginWork*>(p_Work);
		Work->DecodeFlag = true;
		Work->FadeState = 0;
		int LoopCount = 0;

		IPCMMUSICDRIVER *Driver;
		if (Work->PlayMode == PluginWork::mode_pmd) {
			Driver = Work->PmdWin;
		} else {
			Driver = Work->FmpWin;
		}

		while (Work->DecodeFlag) {
			LoopCount = Driver->getloopcount();
			if (LoopCount == -1) {
				if (!module.outMod->IsPlaying()) {
					::PostMessage(module.hMainWindow, WM_USER+2, 0, 0);
					break;
				} else {
					::Sleep(30);
					continue;
				}
			}
			if (LoopCount >= Work->LoopLimit && Work->FadeState == 0 && Work->InfiniteLoop == false) {
				Work->FadeState = 1;
				if (Work->PlayMode == PluginWork::mode_pmd) {
					Work->PmdWin->fadeout2(Work->FadeLength);
				} else {
					Work->FmpWin->fadeout2(Work->FadeLength);
				}
			}

			if (module.outMod->CanWrite() >= sizeof(DecodeBuffer)) {
				::ZeroMemory(DecodeBuffer, sizeof(DecodeBuffer));
				Driver->getpcmdata(DecodeBuffer, sample_len);
				int len(sample_len);
				if (module.dsp_isactive()) {
					len = module.dsp_dosamples(DecodeBuffer, len/MAX_CH/(MAX_BPS/8), MAX_BPS, MAX_CH, Work->Frequency)*(MAX_CH*(MAX_BPS/8));
				}
				int OutLength = module.outMod->GetWrittenTime();
				module.SAAddPCMData(DecodeBuffer, MAX_CH, MAX_BPS, OutLength);
				module.VSAAddPCMData(DecodeBuffer, MAX_CH, MAX_BPS, OutLength);
				module.outMod->Write(reinterpret_cast<char*>(DecodeBuffer), len*MAX_CH*(MAX_BPS/8));
			} else {
				// @todo WaitForSingleObjectに変更すること
				::Sleep(30);
			}
		}

		// music stop
		Driver->music_stop();

		return 0;
	}
}
