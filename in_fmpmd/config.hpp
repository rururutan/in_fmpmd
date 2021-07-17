#if !defined(__CONFIG_HPP__)
#define __CONFIG_HPP__
/**
 *
 * @file config.hpp
 * @brief in_fmpmd configuration
 * @author RuRuRu
 * @todo èâä˙ílÇÃdefine or static constâª
 *
 */

#include <string>
#include <vector>
#include <windows.h>
#include "winamp/in2.h"

namespace FmPmd
{
	const int DEFAULT_FREQUENCY = 48000;
	const int DEFAULT_LOOPL_LIMIT = 3;
	const int DEFAULT_FADE_LENGTH = 3000;
	const int DEFAULT_VOL = 0;
	const int DEFAULT_PMD_VOL_FM = 16;

	const int FMP_VOL_NUM = 4;
	const int PMD_VOL_NUM = 5;
	const int MAX_CH_MASK = 22;
	const int MAX_PMD_VOL = 255;
	const int MAX_FMP_VOL_FM = 99;
	const int MAX_FMP_VOL_SSG = 15;
	const int MAX_FMP_VOL_RHYTHM = 63;
	const int MAX_FMP_VOL_ADPCM = 99;

	/* Setting struct */
	struct PluginSetting {

	  public:

		enum volume {
			vol_fm,
			vol_ssg,
			vol_ppz,
			vol_adpcm,
			vol_rhythm
		};

		std::wstring RhythmPath;					/**< OPNA rhythm wav files path */
		std::wstring IniPath;						/**< ini file path */
		std::vector<std::wstring> PcmSearchPath;	/**< PCM search path list */

		int Frequency;								/**< mixing frequency */
		int PpzFrequency;							/**< PPZ8 mixing frequency */
		bool EnablePpzFrequency;					/**< enable PPZ8 frequency */
		bool FmCalc55k;								/**< calc 55KHz */
		int LoopLimit;								/**< max loop count */
		int FadeLength;								/**< fadeout time(ms) */
		bool InfiniteLoop;							/**< enable infinite loop */

		bool PcmInterpolation;						/**< pcm/ppz/pps interpolation */
		bool EnablePps;								/**< PMD:Enable PPS */
		bool RhythmWithPps;							/**< PMD:RHYTHM with PPS */
		bool Pmdb2PcmMode;							/**< PMD:Compatible PMDB2 PCM */
		bool EmulateAdpcmAtPpz8;					/**< FMP:Emulate ADPCM in PPZ8 */

		int FmpVolume[FMP_VOL_NUM];					/**< Array of volume(0~255) */
		int PmdVolume[PMD_VOL_NUM];					/**< Array of volume(0~255) */
		int m_ch_mask[MAX_CH_MASK];					/**< Channel mask */

		// ñ¢égóp
		int m_thread_priority;						/**< play thread priority */

	  public:

		PluginSetting() :
			Frequency(DEFAULT_FREQUENCY),
			PpzFrequency(DEFAULT_FREQUENCY),
			EnablePpzFrequency(false),
			FmCalc55k(1),
			LoopLimit(DEFAULT_LOOPL_LIMIT),
			InfiniteLoop(false),
			FadeLength(DEFAULT_FADE_LENGTH),
			PcmInterpolation(true),
			EnablePps(true),
			RhythmWithPps(false),
			Pmdb2PcmMode(false),
			EmulateAdpcmAtPpz8(false),
			RhythmPath(L"C:\\Program Files\\Winamp\\Plugins\\fmpmd")
		{
			for (int i=0; i < FMP_VOL_NUM; i++)
				FmpVolume[i] = DEFAULT_VOL;
			for (int i=1; i < PMD_VOL_NUM; i++)
				PmdVolume[i] = DEFAULT_VOL;
			PmdVolume[0] = DEFAULT_PMD_VOL_FM;
		}
		bool Load(in_char*);
		bool Save();
	};
}

#endif /* __CONFIG_HPP__ */

