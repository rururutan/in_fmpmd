/**
 *
 * @file dialog.cpp
 * @brief
 * @author RuRuRu
 *
 */

#include <windows.h>
#include <shlobj.h>
#include "dialog.hpp"
#include "config.hpp"
#include "version.hpp"
#include "resource.h"
#include "winamp/in2.h"
#include "PMDWinImport.h"

namespace FmPmd
{
	struct KeyList{
		const wchar_t *key;
		int num;
	};

	static const KeyList FreqList[] = {
		{ L"96000",		96000 },
		{ L"88200",		88200 },
		{ L"55467",		55467 },
		{ L"48000",		48000 },
		{ L"44100",		44100 },
		{ L"32000",		32000 },
		{ L"22050",		22050 },
		{ L"11025",		11025 },
		{ NULL,				0 },
	};

	static const KeyList PpzFreqList[] = {
		{ L"48000",		48000 },
		{ L"44100",		PPZ8_i0 },
		{ L"33080",		PPZ8_i1 },
		{ L"22050",		PPZ8_i2 },
		{ L"16540",		PPZ8_i3 },
		{ L"11025",		PPZ8_i4 },
		{ L"8270",		PPZ8_i5 },
		{ L"5513",		PPZ8_i6 },
		{ L"4135",		PPZ8_i7 },
		{ NULL,				0 },
	};

	extern PluginSetting setting;
	extern In_Module module;

	/**
	 * General setting dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 * @todo infinity_loopÇ∆multiplex string
	 */
	INT_PTR CALLBACK ConfigGeneralProc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
		  {
			::SendDlgItemMessage(p_hwnd, IDC_FREQUENCY, CB_SETEXTENDEDUI, TRUE, 0);
			for (int i = 0; FreqList[i].key != NULL; i++) {
				::SendDlgItemMessage(p_hwnd, IDC_FREQUENCY, CB_ADDSTRING, 0, (LPARAM)FreqList[i].key);
			}
			int idx = 0;
			for (int i = 0; FreqList[i].key != NULL; i++) {
				if (setting.Frequency == FreqList[i].num) {
					idx = i;
					break;
				}
			}
			::SendDlgItemMessage(p_hwnd, IDC_FREQUENCY, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)FreqList[idx].key);

			::SendDlgItemMessage(p_hwnd, IDC_COMBO2, CB_SETEXTENDEDUI, TRUE, 0);
			for (int i = 0; PpzFreqList[i].key != NULL; i++) {
				::SendDlgItemMessage(p_hwnd, IDC_COMBO2, CB_ADDSTRING, 0, (LPARAM)PpzFreqList[i].key);
			}
			for (int i = 0; PpzFreqList[i].key != NULL; i++) {
				if (setting.PpzFrequency == PpzFreqList[i].num) {
					idx = i;
					break;
				}
			}
			::SendDlgItemMessage(p_hwnd, IDC_COMBO2, CB_SELECTSTRING, (WPARAM)-1, (LPARAM)PpzFreqList[idx].key);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK3, BM_SETCHECK, !setting.EnablePpzFrequency, 0);
			HWND hPPZ = ::GetDlgItem(p_hwnd, IDC_COMBO2);
			if (setting.EnablePpzFrequency == false) {
				::EnableWindow(hPPZ, FALSE);
			}

			::SendDlgItemMessage(p_hwnd, IDC_CALC55K, BM_SETCHECK, setting.FmCalc55k, 0);
			::SendDlgItemMessage(p_hwnd, IDC_PCM_INTERP, BM_SETCHECK, setting.PcmInterpolation, 0);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK4, BM_SETCHECK, setting.EnablePps, 0);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK5, BM_SETCHECK, setting.RhythmWithPps, 0);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK6, BM_SETCHECK, setting.Pmdb2PcmMode, 0);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK7, BM_SETCHECK, setting.EmulateAdpcmAtPpz8, 0);
			::SendDlgItemMessage(p_hwnd, IDC_CHECK11, BM_SETCHECK, setting.InfiniteLoop, 0);
			::SendDlgItemMessage(p_hwnd, IDC_SPIN1, UDM_SETRANGE, 0, MAKELONG(0xff,0));
			::SendDlgItemMessage(p_hwnd, IDC_SPIN2, UDM_SETRANGE, 0, MAKELONG(0x7fff,0));
			::SetDlgItemInt(p_hwnd, IDC_LOOPLIMIT, setting.LoopLimit, FALSE);
			::SetDlgItemInt(p_hwnd, IDC_FADEOUT, setting.FadeLength, FALSE);
			break;
		  }
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			  case IDCANCEL:
				EndDialog(p_hwnd, IDCANCEL);
				return TRUE;
			  case IDC_CHECK3:
			  {
				LRESULT check = ::SendDlgItemMessage(p_hwnd, IDC_CHECK3, BM_GETCHECK, 0, 0);
				HWND hPPZ = ::GetDlgItem(p_hwnd, IDC_COMBO2);
				if (check) {
					::EnableWindow(hPPZ, FALSE);
				} else {
					::EnableWindow(hPPZ, TRUE);
				}
			  }
				break;
			  default:
				break;
			}
			break;
		  case WM_NOTIFY:
			{
			  PSHNOTIFY *PSHNotify =(PSHNOTIFY*)p_lp;
			  switch (PSHNotify->hdr.code) {
				case PSN_APPLY:
				{
					int i;
					wchar_t buf[MAX_PATH];
					::GetDlgItemText(p_hwnd, IDC_FREQUENCY, buf, sizeof(buf));
					for (i=0; FreqList[i].key != NULL; i++) {
						if (::lstrcmpi(buf, FreqList[i].key) == 0) break;
					}
					setting.Frequency = FreqList[i].num;

					::GetDlgItemText(p_hwnd, IDC_COMBO2, buf, sizeof(buf));
					for (i=0; PpzFreqList[i].key != NULL; i++) {
						if (::lstrcmpi(buf, PpzFreqList[i].key) == 0) break;
					}
					setting.PpzFrequency = PpzFreqList[i].num;

					setting.FmCalc55k = ::SendDlgItemMessage(p_hwnd, IDC_CALC55K, BM_GETCHECK, 0, 0)?true:false;
					setting.PcmInterpolation = ::SendDlgItemMessage(p_hwnd, IDC_PCM_INTERP, BM_GETCHECK, 0, 0)?true:false;
					setting.EnablePpzFrequency = ::SendDlgItemMessage(p_hwnd, IDC_CHECK3, BM_GETCHECK, 0, 0)?false:true;
					setting.EnablePps = ::SendDlgItemMessage(p_hwnd, IDC_CHECK4, BM_GETCHECK, 0, 0)?true:false;
					setting.RhythmWithPps = ::SendDlgItemMessage(p_hwnd, IDC_CHECK5, BM_GETCHECK, 0, 0)?true:false;
					setting.Pmdb2PcmMode = ::SendDlgItemMessage(p_hwnd, IDC_CHECK6, BM_GETCHECK, 0, 0)?true:false;
					setting.EmulateAdpcmAtPpz8 = ::SendDlgItemMessage(p_hwnd, IDC_CHECK7, BM_GETCHECK, 0, 0)?true:false;
					setting.InfiniteLoop = ::SendDlgItemMessage(p_hwnd, IDC_CHECK11, BM_GETCHECK, 0, 0)?true:false;
					setting.LoopLimit = ::SendDlgItemMessage(p_hwnd, IDC_SPIN1, UDM_GETPOS, 0, 0);
					setting.FadeLength = ::SendDlgItemMessage(p_hwnd, IDC_SPIN2, UDM_GETPOS, 0, 0);
					if (setting.LoopLimit > 0xff)
						setting.LoopLimit = 0xff;
					if (setting.FadeLength > 0x7fff)
						setting.FadeLength = 0x7fff;

					break;
				}
			  }
			  break;
			}
		}
		return FALSE;
	}

	/**
	 * Path setting dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 */
	INT_PTR CALLBACK ConfigPathProc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
			  ::SetDlgItemText(p_hwnd, IDC_EDIT1, setting.RhythmPath.c_str());
			  for (unsigned int i=0; i < setting.PcmSearchPath.size(); i++) {
				  ::SendMessage(GetDlgItem(p_hwnd, IDC_LIST1), LB_INSERTSTRING, (WPARAM)i, (LPARAM)setting.PcmSearchPath[i].c_str());
			  }
			  break;
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDC_BUTTON1:
				if (HIWORD(p_wp) == BN_CLICKED) {
					wchar_t aPath[MAX_PATH] = {'\0'};
					BROWSEINFO BrowseInfo;
					LPITEMIDLIST IdList;
					::ZeroMemory(&BrowseInfo, sizeof(BROWSEINFO));
					BrowseInfo.hwndOwner = p_hwnd;
					BrowseInfo.pszDisplayName = aPath;
					BrowseInfo.lpszTitle = L"Select rhythm wav directory";
					BrowseInfo.ulFlags = (BIF_EDITBOX | BIF_RETURNONLYFSDIRS);
					IdList = ::SHBrowseForFolder(&BrowseInfo);
					if (IdList != NULL) {
						::SHGetPathFromIDList(IdList, aPath);
						::CoTaskMemFree(IdList);
						::SetDlgItemText(p_hwnd, IDC_EDIT1, aPath);
					}
					return TRUE;
				}
				break;
			  case IDC_BUTTON2:
				if (HIWORD(p_wp) == BN_CLICKED) {
					wchar_t aPath[MAX_PATH] = {'\0'};
					BROWSEINFO BrowseInfo;
					LPITEMIDLIST IdList;
					::ZeroMemory(&BrowseInfo, sizeof(BROWSEINFO));
					BrowseInfo.hwndOwner = p_hwnd;
					BrowseInfo.pszDisplayName = aPath;
					BrowseInfo.lpszTitle = L"Select pcm directory";
					BrowseInfo.ulFlags = (BIF_EDITBOX | BIF_RETURNONLYFSDIRS);
					IdList = ::SHBrowseForFolder(&BrowseInfo);
					if (IdList != NULL) {
						::SHGetPathFromIDList(IdList, aPath);
						::CoTaskMemFree(IdList);
						::SendMessage(GetDlgItem(p_hwnd, IDC_LIST1), LB_INSERTSTRING, (WPARAM)::SendMessage(GetDlgItem(p_hwnd, IDC_LIST1), LB_GETCOUNT, (WPARAM)0, (LPARAM)0), (LPARAM)aPath);
					}
					return TRUE;
				}
				break;
			  case IDC_BUTTON3:
				if (HIWORD(p_wp) == BN_CLICKED) {
					HWND list = GetDlgItem(p_hwnd, IDC_LIST1);
					LRESULT current = ::SendMessage(list, LB_GETCURSEL, 0, 0);
					if (current == LB_ERR) break;
					::SendMessage(list, LB_DELETESTRING, current, 0);
				}
				break;
			  case IDC_BUTTON4:
				if (HIWORD(p_wp) == BN_CLICKED) {
					::SendMessage(GetDlgItem(p_hwnd, IDC_LIST1), LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
				}
				break;
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			  case IDCANCEL:
				EndDialog(p_hwnd, IDCANCEL);
				return TRUE;
			}
			break;
		  case WM_NOTIFY:
			{
			  PSHNOTIFY *PSHNotify =(PSHNOTIFY*)p_lp;
			  switch (PSHNotify->hdr.code) {
				case PSN_APPLY:
				{
					wchar_t buf[MAX_PATH];
					::GetDlgItemText(p_hwnd, IDC_EDIT1, buf, sizeof(buf));
					setting.RhythmPath.assign(buf);

					// clearÇµÇƒsearchpathÇçƒê›íË
					HWND hList = GetDlgItem(p_hwnd, IDC_LIST1);
					int MaxListCount = static_cast<int>(::SendMessage(hList, LB_GETCOUNT, (WPARAM)0, (LPARAM)0));
					setting.PcmSearchPath.clear();
					for (int i=0; i < MaxListCount; i++) {
						::SendMessage(hList, LB_GETTEXT, i, (LPARAM)buf);
						setting.PcmSearchPath.push_back(buf);
					}
				}
			  }
			  break;
			}
		  default:
			break;
		}
		return FALSE;
	}

#define InitialVolumeSlider(hwnd, slide_id, text_id, freq, max, value) {\
	HWND hSlider = ::GetDlgItem(hwnd, slide_id); \
	::SendMessage(hSlider, TBM_SETTICFREQ, freq, 0); \
	::SendMessage(hSlider, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, max)); \
	::SendMessage(hSlider, TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(value)); \
	::SetDlgItemInt(hwnd, text_id, value, 0); \
}  

	/**
	 * Mixer setting dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 */
	INT_PTR CALLBACK ConfigMixerProc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
		  {
			  InitialVolumeSlider(p_hwnd, IDC_FMP_VOL_FM,    IDC_FMP_VOL_TEXT_FM,     12, MAX_FMP_VOL_FM,     setting.FmpVolume[0]);
			  InitialVolumeSlider(p_hwnd, IDC_FMP_VOL_SSG,   IDC_FMP_VOL_TEXT_SSG,     2, MAX_FMP_VOL_SSG,    setting.FmpVolume[1]);
			  InitialVolumeSlider(p_hwnd, IDC_FMP_VOL_RHYTHM,IDC_FMP_VOL_TEXT_RHYTHM,  8, MAX_FMP_VOL_RHYTHM, setting.FmpVolume[2]);
			  InitialVolumeSlider(p_hwnd, IDC_FMP_VOL_ADPCM, IDC_FMP_VOL_TEXT_ADPCM,  12, MAX_FMP_VOL_ADPCM,  setting.FmpVolume[3]);
			  InitialVolumeSlider(p_hwnd, IDC_PMD_VOL_FM,    IDC_PMD_VOL_TEXT_FM,     32, MAX_PMD_VOL,        setting.PmdVolume[0]);
			  InitialVolumeSlider(p_hwnd, IDC_PMD_VOL_SSG,   IDC_PMD_VOL_TEXT_SSG,    32, MAX_PMD_VOL,        setting.PmdVolume[1]);
			  InitialVolumeSlider(p_hwnd, IDC_PMD_VOL_RHYTHM,IDC_PMD_VOL_TEXT_RHYTHM, 32, MAX_PMD_VOL,        setting.PmdVolume[2]);
			  InitialVolumeSlider(p_hwnd, IDC_PMD_VOL_ADPCM, IDC_PMD_VOL_TEXT_ADPCM,  32, MAX_PMD_VOL,        setting.PmdVolume[3]);
			  InitialVolumeSlider(p_hwnd, IDC_PMD_VOL_PPZ8,  IDC_PMD_VOL_TEXT_PPZ8,   32, MAX_PMD_VOL,        setting.PmdVolume[4]);
			  break;
		  }
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			  case IDCANCEL:
				EndDialog(p_hwnd, IDCANCEL);
				return TRUE;
			}
		  case WM_VSCROLL:
		  {
			  switch (::GetWindowLong((HWND)p_lp, GWL_ID)) {
				case IDC_FMP_VOL_FM:
				  ::SetDlgItemInt(p_hwnd, IDC_FMP_VOL_TEXT_FM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_FMP_VOL_SSG:
				  ::SetDlgItemInt(p_hwnd, IDC_FMP_VOL_TEXT_SSG, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_FMP_VOL_RHYTHM:
				  ::SetDlgItemInt(p_hwnd, IDC_FMP_VOL_TEXT_RHYTHM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_FMP_VOL_ADPCM:
				  ::SetDlgItemInt(p_hwnd, IDC_FMP_VOL_TEXT_ADPCM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_PMD_VOL_FM:
				  ::SetDlgItemInt(p_hwnd, IDC_PMD_VOL_TEXT_FM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_PMD_VOL_SSG:
				  ::SetDlgItemInt(p_hwnd, IDC_PMD_VOL_TEXT_SSG, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_PMD_VOL_RHYTHM:
				  ::SetDlgItemInt(p_hwnd, IDC_PMD_VOL_TEXT_RHYTHM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_PMD_VOL_ADPCM:
				  ::SetDlgItemInt(p_hwnd, IDC_PMD_VOL_TEXT_ADPCM, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
				case IDC_PMD_VOL_PPZ8:
				  ::SetDlgItemInt(p_hwnd, IDC_PMD_VOL_TEXT_PPZ8, (int)SendMessage((HWND)p_lp, TBM_GETPOS, 0, 0), 0);
				  break;
			  }
			  break;
		  }
		  case WM_NOTIFY:
			{
			  PSHNOTIFY *PSHNotify =(PSHNOTIFY*)p_lp;
			  switch (PSHNotify->hdr.code) {
				case PSN_APPLY:
				{
					HWND hSlider = ::GetDlgItem(p_hwnd, IDC_FMP_VOL_FM);
					setting.FmpVolume[0] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_FMP_VOL_SSG);
					setting.FmpVolume[1] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_FMP_VOL_RHYTHM);
					setting.FmpVolume[2] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_FMP_VOL_ADPCM);
					setting.FmpVolume[3] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_PMD_VOL_FM);
					setting.PmdVolume[0] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_PMD_VOL_SSG);
					setting.PmdVolume[1] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_PMD_VOL_RHYTHM);
					setting.PmdVolume[2] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_PMD_VOL_ADPCM);
					setting.PmdVolume[3] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
					hSlider = ::GetDlgItem(p_hwnd, IDC_PMD_VOL_PPZ8);
					setting.PmdVolume[4] = (int)::SendMessage(hSlider, TBM_GETPOS, 0, 0);
				}
			  }
			}
			break;
		  default:
			break;
		}
		return FALSE;
	}

	/**
	 * Mask setting dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 */
	INT_PTR CALLBACK ConfigMaskProc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
			for (int i=0; i < MAX_CH_MASK; i++) {
				if (setting.m_ch_mask[i] == 1) {
					::SendDlgItemMessage(p_hwnd, IDC_MASK_FM1+i, BM_SETCHECK, setting.m_ch_mask[i], 0);
				}
			}
			break;
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			  case IDCANCEL:
				EndDialog(p_hwnd, IDCANCEL);
				return TRUE;
			}
			break;
		  case WM_NOTIFY:
			{
			  PSHNOTIFY *PSHNotify =(PSHNOTIFY*)p_lp;
			  switch (PSHNotify->hdr.code) {
				case PSN_APPLY:
				{
					for (int i=0; i < MAX_CH_MASK; i++) {
						setting.m_ch_mask[i] = ::SendDlgItemMessage(p_hwnd, IDC_MASK_FM1+i, BM_GETCHECK, 0, 0);
					}
				}
			  }
			}
			break;
		  default:
			break;
		}
		return FALSE;
	}

	/**
	 * PMD information dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 */
	INT_PTR CALLBACK info_pmd_proc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
			{
			  PmdInfo *info = reinterpret_cast<PmdInfo*>(p_lp);
			  ::SetDlgItemText(p_hwnd, IDC_EDIT1, info->memo[0].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT2, info->memo[1].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT3, info->memo[2].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT6, info->memo[3].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT7, info->memo[4].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT8, info->memo[5].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT4, info->memo[6].c_str());

			  break;
			}
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			}
		  default:
			break;
		}
		return FALSE;
	}

	/**
	 * FMP information dialog proc
	 *
	 * @param p_hwnd [in]
	 *
	 * @param p_msg [in]
	 *
	 * @param p_wp [in]
	 *
	 * @param p_lp [in]
	 *
	 * @return
	 */
	INT_PTR CALLBACK info_fmp_proc(HWND p_hwnd, UINT p_msg, WPARAM p_wp, LPARAM p_lp)
	{
		switch (p_msg) {
		  case WM_INITDIALOG:
			{
			  FmpInfo *info = reinterpret_cast<FmpInfo*>(p_lp);
			  ::SetDlgItemText(p_hwnd, IDC_EDIT1, info->comment[0].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT2, info->comment[1].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT3, info->comment[2].c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT4, info->pcmname.c_str());
			  ::SetDlgItemText(p_hwnd, IDC_EDIT5, info->ppzname.c_str());
			  break;
			}
		  case WM_COMMAND:
			switch (LOWORD(p_wp)) {
			  case IDOK:
				EndDialog(p_hwnd, IDOK);
				return TRUE;
			}
		  default:
			break;
		}
		return FALSE;
	}

	/**
	 * Show property page
	 *
	 * @param p_parent [in]
	 *     Parent window's handle
	 */
	void ConfigDialog(HWND p_parent)
	{
		PROPSHEETPAGE aPSPage[4];
		::ZeroMemory(aPSPage, sizeof(aPSPage));
		for (int i=0; i<4; i++) {
			aPSPage[i].dwSize      = sizeof(PROPSHEETPAGE);
			aPSPage[i].dwFlags     = PSP_DEFAULT;
			aPSPage[i].hInstance   = module.hDllInstance;
		}
		aPSPage[0].pfnDlgProc = ConfigGeneralProc;
		aPSPage[1].pfnDlgProc = ConfigPathProc;
		aPSPage[2].pfnDlgProc = ConfigMixerProc;
		aPSPage[3].pfnDlgProc = ConfigMaskProc;
		aPSPage[0].pszTemplate = MAKEINTRESOURCE(IDD_OPTION);
		aPSPage[1].pszTemplate = MAKEINTRESOURCE(IDD_PATH);
		aPSPage[2].pszTemplate = MAKEINTRESOURCE(IDD_MIXER);
		aPSPage[3].pszTemplate = MAKEINTRESOURCE(IDD_CHANNEL);

		PROPSHEETHEADER oPSHeader;
		::ZeroMemory(&oPSHeader, sizeof(oPSHeader));
		oPSHeader.dwSize      = sizeof(PROPSHEETHEADER);
		oPSHeader.dwFlags     = PSH_NOAPPLYNOW | PSH_PROPSHEETPAGE;
		oPSHeader.hwndParent  = p_parent;
		oPSHeader.hInstance   = module.hDllInstance;
		oPSHeader.pszCaption  = L"in_fmpmd setting";
		oPSHeader.nPages      = sizeof(aPSPage) / sizeof(PROPSHEETPAGE);
		oPSHeader.ppsp        = (LPCPROPSHEETPAGE) &aPSPage;

		::PropertySheet(&oPSHeader);
	}

	/**
	 * Show about dialog
	 *
	 * @param p_parent [in]
	 *     Parent window's handle
	 */
	void AboutDialog(HWND p_parent)
	{
		::MessageBox(p_parent, PLUGIN_ABOUT, L"About this plugin", MB_OK);
	}

	/**
	 * Show info dialog(PMD)
	 *
	 * @param p_parent [in]
	 *     Parent window's handle
	 * @param p_info [in]
	 *
	 */
	void InfoPmdDialog(HWND p_parent, PmdInfo &p_info)
	{
		::DialogBoxParam(module.hDllInstance, MAKEINTRESOURCE(IDD_INFO_PMD_DIALOG), p_parent, info_pmd_proc, (LPARAM)&p_info);
	}

	/**
	 * Show info dialog(FMP)
	 *
	 * @param p_parent [in]
	 *     Parent window's handle
	 * @param p_info [in]
	 *
	 */
	void InfoFmpDialog(HWND p_parent, FmpInfo &p_info)
	{
		::DialogBoxParam(module.hDllInstance, MAKEINTRESOURCE(IDD_INFO_FMP_DIALOG), p_parent, info_fmp_proc, (LPARAM)&p_info);
	}
}

