#if !defined(__DIALOG_HPP__)
#define __DIALOG_HPP__
/**
 *
 * @file dialog.hpp
 * @brief in_fmpmd dialog
 * @author RuRuRu
 *
 */

#include <string>

namespace FmPmd
{
	struct FmpInfo {
		std::wstring comment[3];
		std::wstring pcmname;
		std::wstring ppzname;
	};
	struct PmdInfo {
		std::wstring memo[7];
	};

	void ConfigDialog(HWND);
	void AboutDialog(HWND);
	void InfoPmdDialog(HWND, PmdInfo&);
	void InfoFmpDialog(HWND, FmpInfo&);
}

#endif /* __DIALOG_HPP__ */
