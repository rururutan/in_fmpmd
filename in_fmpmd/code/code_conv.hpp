#pragma once
/**
 * sjis <-> utf8 convert
 * @file code
 *
 */
#include <string>
#include <windows.h>

namespace CharacterCode
{
	/// ShifJIS to UTF-16
	class SjisToUtf16 {
	  public:
		SjisToUtf16(std::string&);
		SjisToUtf16(const LPSTR);
		virtual ~SjisToUtf16();
		LPCWSTR GetData();
		bool GetData(std::wstring&);

	  protected:
		SjisToUtf16();
		bool SetData(UINT, std::string&);

	  private:
		LPWSTR m_out_buffer;
	};

	/// Multibyte to UTF-16
	class MbToUtf16 : public SjisToUtf16 {
	  public:
		MbToUtf16(const LPSTR);
	};

	/// UTF-16 to ShiftJIS
	class Utf16ToSjis {
	  public:
		Utf16ToSjis(std::wstring&);
		Utf16ToSjis(const LPWSTR);
		virtual ~Utf16ToSjis();
		bool GetData(std::string&);
		LPCSTR GetData();

	  protected:
		Utf16ToSjis();
		bool SetData(UINT, std::wstring&);

	  private:
		LPSTR m_out_buffer;
	};

	/// UTF-16 to Multibyte
	class Utf16ToMb : public Utf16ToSjis {
	  public:
		Utf16ToMb(const LPWSTR);
	};
}
