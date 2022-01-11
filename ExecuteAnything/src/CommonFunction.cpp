/*
	Copyright (C) 2021  David Maisonave
	The RegexAssistant source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "pch.h"
#include <windows.h>
#include "CommonFunctions.h"
#include <algorithm>
#include <cwctype>

#ifndef EXCLUDE_BOOST_CODE
#include <boost\algorithm\string\replace.hpp>
#endif //!EXCLUDE_BOOST_CODE


namespace Common
{
	std::wstring ToWString(std::string str )	
	{
		if ( str.empty() ) return std::wstring();
		int size_needed = MultiByteToWideChar( CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0 );
		std::wstring wstrTo( size_needed, 0 );
		MultiByteToWideChar( CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed );
		return wstrTo;
	}

	std::string ToString(std::wstring wstr )	
	{
		if ( wstr.empty() ) return std::string();
		int size_needed = WideCharToMultiByte( CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL );
		std::string strTo( size_needed, 0 );
		WideCharToMultiByte( CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL );
		return strTo;
	}

	std::string ToLower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),[](unsigned char c) { return (unsigned char)std::tolower(c); } );
		return s;
	}

	std::wstring ToLower(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](wchar_t c) { return std::towlower(c); });
		return s;
	}


	std::string ToUpper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (unsigned char)std::toupper(c); });
		return s;
	}


	std::wstring ToUpper(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](wchar_t c) { return std::towupper(c); });
		return s;
	}




#ifndef EXCLUDE_BOOST_CODE
	void ReplaceAll(std::string &src, const std::string &searchStr, const std::string &ReplacementStr)
	{
		boost::replace_all(src, searchStr, ReplacementStr);
	}

	std::string Replace(std::string &src, const std::string &searchStr, const std::string &ReplacementStr)
	{
		ReplaceAll(src, searchStr, ReplacementStr);
		return src;
	}

	std::string ReplaceInCopy(const std::string &org, const std::string &searchStr, const std::string &ReplacementStr)
	{
		std::string str = org;
		ReplaceAll(str, searchStr, ReplacementStr);
		return str;
	}
	void ReplaceAll(std::wstring &src, const std::wstring &searchStr, const std::wstring &ReplacementStr)
	{
		boost::replace_all(src, searchStr, ReplacementStr);
	}

	std::wstring Replace(std::wstring &src, const std::wstring &searchStr, const std::wstring &ReplacementStr)
	{
		ReplaceAll(src, searchStr, ReplacementStr);
		return src;
	}

	std::wstring ReplaceInCopy(const std::wstring &org, const std::wstring &searchStr, const std::wstring &ReplacementStr)
	{
		std::wstring str = org;
		ReplaceAll(str, searchStr, ReplacementStr);
		return str;
	}
#endif //!EXCLUDE_BOOST_CODE

#ifdef _AFX
	void TraceDiagnosticInfo( std::string msg )
	{// AfxTrace has a max buffer size of 512 bytes, which is 256 wchar_t's.  Need to make sure the diagnostic info doesn't overrun the buffer by sending it MaxSectionSize characters at a time
		const int MaxSectionSize = 100;
		const int strsize = static_cast<int>(msg.size());
		if ( strsize > MaxSectionSize )
		{
#ifndef EXCLUDE_BOOST_CODE
			Common::Replace( msg, "\n", " " );
			Common::Replace( msg, "\r", " " );
#endif //!EXCLUDE_BOOST_CODE
		}
		for ( int i = 0; (i * MaxSectionSize) < strsize; ++i )
			AfxTrace( i ? L"\t\t\t\t%S\n" : L"Exception Info:\t%S\n", msg.substr( i * MaxSectionSize, (i + 1) * MaxSectionSize ).c_str() );
	}
#endif //_AFX
}

