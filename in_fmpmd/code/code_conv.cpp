/**
 *
 * @file code_convert.cxx
 * @brief character code covert class
 * @author RuRuRu
 *
 */

#include "code_conv.hpp"

namespace CharacterCode
{

MbToUtf16::MbToUtf16(LPSTR p_src)
{
	std::string src(p_src);
	SetData(CP_ACP, src);
}

/**
 * Constructor
 *
 */
SjisToUtf16::SjisToUtf16(LPSTR p_src) :
	m_out_buffer(NULL)
{
	std::string src(p_src);
	SetData(932, src);
}

/**
 * Constructor
 *
 */
SjisToUtf16::SjisToUtf16() :
	m_out_buffer(NULL)
{
}

/**
 * Constructor
 *
 */
SjisToUtf16::SjisToUtf16(std::string &p_src) :
	m_out_buffer(NULL)
{
	SetData(932, p_src);
}

/**
 * Destructor
 *
 */
SjisToUtf16::~SjisToUtf16()
{
	if (m_out_buffer) delete [] m_out_buffer;
}

/**
 * Set src data
 *
 * @param p_code [in]
 *
 * @param p_src [in]
 *
 */
bool SjisToUtf16::SetData(UINT p_code, std::string &p_src)
{
	// sjis -> unicode
	const char *ssrc = p_src.c_str();
    int ires = ::MultiByteToWideChar(p_code, 0, ssrc, -1, NULL, 0);

	if (ires == 0) {
		return false;
	}

	LPWSTR wbuf = new WCHAR[ires+1];
	ires = ::MultiByteToWideChar(p_code, 0, ssrc, -1, wbuf, ires);

	m_out_buffer = wbuf;

	return true;
}

/**
 * Get converted data.
 *
 * @return
 */
LPCWSTR SjisToUtf16::GetData()
{
	return m_out_buffer;
};

/**
 * Get converted data.
 *
 * @return
 */
bool SjisToUtf16::GetData(std::wstring &p_dst)
{
	p_dst.assign(m_out_buffer);
	return true;
};

Utf16ToMb::Utf16ToMb(const LPWSTR p_src)
{
	std::wstring src(p_src);
	SetData(CP_ACP, src);
}

/**
 * Constructor
 *
 */
Utf16ToSjis::Utf16ToSjis() :
	m_out_buffer(NULL)
{
}

/**
 * Constructor
 *
 */
Utf16ToSjis::Utf16ToSjis(const LPWSTR p_src) :
	m_out_buffer(NULL)
{
	std::wstring src(p_src);
	SetData(932, src);
}

/**
 * Constructor
 *
 */
Utf16ToSjis::Utf16ToSjis(std::wstring &p_src) :
	m_out_buffer(NULL)
{
	SetData(932, p_src);
}

/**
 * Destructor
 *
 */
Utf16ToSjis::~Utf16ToSjis()
{
	if (m_out_buffer) delete [] m_out_buffer;
}

/**
 * Set src data
 *
 * @param p_code [in]
 *
 * @param p_src [in]
 *
 */
bool Utf16ToSjis::SetData(UINT p_code, std::wstring &p_src)
{
	// unicode -> sjis
	const wchar_t *ssrc = p_src.c_str();
	int ires = ::WideCharToMultiByte(p_code, 0, ssrc, -1, NULL, 0, NULL, NULL);

	if (ires == 0) {
		return false;
	}

	LPSTR mbuf = new char[ires+1];
	ires = ::WideCharToMultiByte(p_code, 0, ssrc, -1, mbuf, ires, NULL, NULL);

	m_out_buffer = mbuf;

	return true;
}

/**
 * Get converted data.
 *
 * @return
 */
LPCSTR Utf16ToSjis::GetData()
{
	return m_out_buffer;
};

/**
 * Get converted data.
 *
 * @return
 */
bool Utf16ToSjis::GetData(std::string &p_dst)
{
	p_dst.assign(m_out_buffer);
	return true;
};

}
