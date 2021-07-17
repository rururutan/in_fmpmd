/**
 * @file config.cpp
 * @brief
 * @author RuRuRu
 *
 */

#include "config.hpp"
#include "version.hpp"
#include "PMDWinImport.h"

namespace FmPmd
{
	static BOOL WritePrivateProfileInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int dwValue, LPCTSTR lpszFile)
	{
		wchar_t buf[64+1];	// 
		::_snwprintf(buf, 64, L"%d", dwValue);
		return(WritePrivateProfileString(lpszSection, lpszKey, buf, lpszFile));
	}

	bool PluginSetting::Load(in_char *p_file)
	{
		// create ini path
		IniPath = p_file;
		size_t ofs = IniPath.find_last_of(L".");
		if (ofs != 0) {
			IniPath.resize(ofs);
		}
		IniPath.append(L".ini");

		Frequency = GetPrivateProfileInt(APP_NAME, L"Frequency", DEFAULT_FREQUENCY, IniPath.c_str());
		PpzFrequency = GetPrivateProfileInt(APP_NAME, L"PpzFrequency", DEFAULT_FREQUENCY, IniPath.c_str());
		EnablePpzFrequency = GetPrivateProfileInt(APP_NAME, L"EnablePpzFrequency", 0, IniPath.c_str())?true:false;
		FmCalc55k = GetPrivateProfileInt(APP_NAME, L"FmCalc55K", 1, IniPath.c_str())?true:false;
		LoopLimit = GetPrivateProfileInt(APP_NAME, L"LoopLimit", DEFAULT_LOOPL_LIMIT, IniPath.c_str());
		FadeLength = GetPrivateProfileInt(APP_NAME, L"FadeTime", DEFAULT_FADE_LENGTH, IniPath.c_str());
		InfiniteLoop = GetPrivateProfileInt(APP_NAME, L"InfiniteLoop", 0, IniPath.c_str())?true:false;

		PcmInterpolation = GetPrivateProfileInt(APP_NAME, L"PcmInterpolation", 1, IniPath.c_str())?true:false;
		EnablePps = GetPrivateProfileInt(APP_NAME, L"EnablePPS", 1, IniPath.c_str())?true:false;
		RhythmWithPps = GetPrivateProfileInt(APP_NAME, L"RhythmWithPPS", 0, IniPath.c_str())?true:false;
		Pmdb2PcmMode = GetPrivateProfileInt(APP_NAME, L"Pmdb2PcmMode", 0, IniPath.c_str())?true:false;
		EmulateAdpcmAtPpz8 = GetPrivateProfileInt(APP_NAME, L"EmulateAdpcmAtPpz8", 0, IniPath.c_str())?true:false;

		FmpVolume[0] = GetPrivateProfileInt(APP_NAME, L"FmpVolumeFm", DEFAULT_VOL, IniPath.c_str());
		FmpVolume[1] = GetPrivateProfileInt(APP_NAME, L"FmpVolumeSsg", DEFAULT_VOL, IniPath.c_str());
		FmpVolume[2] = GetPrivateProfileInt(APP_NAME, L"FmpVolumeRhythm", DEFAULT_VOL, IniPath.c_str());
		FmpVolume[3] = GetPrivateProfileInt(APP_NAME, L"FmpVolumeAdpcm", DEFAULT_VOL, IniPath.c_str());
		PmdVolume[0] = GetPrivateProfileInt(APP_NAME, L"PmdVolumeFm", DEFAULT_PMD_VOL_FM, IniPath.c_str());
		PmdVolume[1] = GetPrivateProfileInt(APP_NAME, L"PmdVolumeSsg", DEFAULT_VOL, IniPath.c_str());
		PmdVolume[2] = GetPrivateProfileInt(APP_NAME, L"PmdVolumeRhythm", DEFAULT_VOL, IniPath.c_str());
		PmdVolume[3] = GetPrivateProfileInt(APP_NAME, L"PmdVolumeAdpcm", DEFAULT_VOL, IniPath.c_str());
		PmdVolume[4] = GetPrivateProfileInt(APP_NAME, L"PmdVolumePpz", DEFAULT_VOL, IniPath.c_str());

		wchar_t buffer[MAX_PATH] = {0};
		GetPrivateProfileString(APP_NAME, L"RhythmPath", L"C:\\Program Files\\Winamp\\Plugins\\fmpmd", buffer, MAX_PATH, IniPath.c_str());
		RhythmPath.assign(buffer);

		int PathMax = GetPrivateProfileInt(APP_NAME, L"PcmSearchPathNum", 0, IniPath.c_str());
		if (PathMax > MAX_PCMDIR) PathMax = MAX_PCMDIR;

		wchar_t ConvBUf[4] = {0};
		for (int i=0; i < PathMax; i++) {
			std::wstring KeyString(L"PcmSearchPath");
			_itow(i, ConvBUf, 4);
			KeyString.append(ConvBUf);
			if (GetPrivateProfileString(APP_NAME, KeyString.c_str(), NULL, buffer, MAX_PATH, IniPath.c_str()) <= 0)
				break;
			PcmSearchPath.push_back(buffer);
		}

		int mask = GetPrivateProfileInt(APP_NAME, L"ChannelMask", 0, IniPath.c_str());
		for (int i=0; i<MAX_CH_MASK; i++) {
			m_ch_mask[i] = (mask >> i) & 0x01;
		}
		return true;
	}

	bool PluginSetting::Save()
	{
		WritePrivateProfileInt(APP_NAME, L"Frequency", Frequency, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PpzFrequency", PpzFrequency, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"EnablePpzFrequency", EnablePpzFrequency, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"FmCalc55K", FmCalc55k, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"LoopLimit", LoopLimit, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"FadeTime", FadeLength, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PcmInterpolation", PcmInterpolation, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"EnablePPS", EnablePps, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"RhythmWithPPS", RhythmWithPps, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"Pmdb2PcmMode", Pmdb2PcmMode, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"EmulateAdpcmAtPpz8", EmulateAdpcmAtPpz8, IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"InfiniteLoop", InfiniteLoop, IniPath.c_str());

		WritePrivateProfileInt(APP_NAME, L"FmpVolumeFm", FmpVolume[0], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"FmpVolumeSsg", FmpVolume[1], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"FmpVolumeRhythm", FmpVolume[2], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"FmpVolumeAdpcm", FmpVolume[3], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PmdVolumeFm", PmdVolume[0], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PmdVolumeSsg", PmdVolume[1], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PmdVolumeRhythm", PmdVolume[2], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PmdVolumeAdpcm", PmdVolume[3], IniPath.c_str());
		WritePrivateProfileInt(APP_NAME, L"PmdVolumePpz", PmdVolume[4], IniPath.c_str());

		WritePrivateProfileString(APP_NAME, L"RhythmPath", RhythmPath.c_str(), IniPath.c_str());

		int j=0;
		wchar_t ConvBUf[4];
		for(std::vector<std::wstring>::iterator i=PcmSearchPath.begin(); i!=PcmSearchPath.end(); i++,j++) {
			if (j == MAX_PCMDIR) break;
			std::wstring KeyString(L"PcmSearchPath");
			_itow(j, ConvBUf, 4);
			KeyString.append(ConvBUf);
			WritePrivateProfileString(APP_NAME, KeyString.c_str(), (*i).c_str(), IniPath.c_str());
		}
		WritePrivateProfileInt(APP_NAME, L"PcmSearchPathNum", PcmSearchPath.size(), IniPath.c_str());
		int mask(0);
		for (int i=0; i<MAX_CH_MASK; i++) {
			mask |= ((m_ch_mask[i] & 0x01) << i);
		}
		WritePrivateProfileInt(APP_NAME, L"ChannelMask", mask, IniPath.c_str());
		return true;
	}

}
