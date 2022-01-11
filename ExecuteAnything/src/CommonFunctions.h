/*
	Copyright (C) 2021  David Maisonave
	The RegexAssistant source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <algorithm> 
#include <cctype>
#include <locale>

namespace Common
{
	std::wstring ToWString(std::string src);
	inline std::wstring ToWString( const std::wstring &src ) { return src; }
	inline std::wstring ToWString( const wchar_t* src ) { return std::wstring( src ); }

	std::string ToString( std::wstring src);
	inline std::string ToString( const std::string &src ) { return src; }
	inline std::string ToString( const wchar_t* src ) { return ToString( std::wstring(src) ); }

	inline std::string ToString( int src ) { return std::to_string( src ); }
	inline std::string ToString( long src ) { return std::to_string( src ); }
	inline std::string ToString( long long src ) { return std::to_string( src ); }
	inline std::string ToString( unsigned src ) { return std::to_string( src ); }
	inline std::string ToString( unsigned long src ) { return std::to_string( src ); }
	inline std::string ToString( unsigned long long src ) { return std::to_string( src ); }
	inline std::string ToString( float src ) { return std::to_string( src ); }
	inline std::string ToString( double src ) { return std::to_string( src ); }
	inline std::string ToString( long double src ) { return std::to_string( src ); }

	inline std::wstring ToWString( int src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( long src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( long long src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( unsigned src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( unsigned long src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( unsigned long long src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( float src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( double src ) { return std::to_wstring( src ); }
	inline std::wstring ToWString( long double src ) { return std::to_wstring( src ); }
	
	template<class T>
	std::wstring ToStr( std::wstring dest, T src )
	{
		dest = std::to_wstring( src );
		return dest;
	}
	template<class T>
	std::string ToStr( std::string dest, T src )
	{
		dest = std::to_string( src );
		return dest;
	}

	// trim from start (in place)
	inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	inline std::string ltrim_copy(std::string s) {
		ltrim(s);
		return s;
	}

	// trim from end (copying)
	inline std::string rtrim_copy(std::string s) {
		rtrim(s);
		return s;
	}

	// trim from both ends (copying)
	inline std::string trim_copy(std::string s) {
		trim(s);
		return s;
	}

	// trim from end of string (right)
	inline std::string& rtrim(std::string& s, const char* t_str)
	{
		s.erase(s.find_last_not_of(t_str) + 1);
		return s;
	}

	// trim from beginning of string (left)
	inline std::string& ltrim(std::string& s, const char* t_str)
	{
		s.erase(0, s.find_first_not_of(t_str));
		return s;
	}

	// trim from both ends of string (right then left)
	inline std::string& trim(std::string& s, const char* t_str)
	{
		return ltrim(rtrim(s, t_str), t_str);
	}

	std::string ToLower(std::string src);
	std::wstring ToLower(std::wstring src);
	std::string ToUpper(std::string src);
	std::wstring ToUpper(std::wstring src);
#ifdef _AFX
	inline CString ToCString(const char * src) { return ToWString(src).c_str(); }
	inline CString ToCString(const wchar_t * src) { return src; }

	inline CString ToCString( int src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( long src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( long long src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( unsigned src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( unsigned long src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( unsigned long long src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( float src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( double src ) { return std::to_wstring( src ).c_str(); }
	inline CString ToCString( long double src ) { return std::to_wstring( src ).c_str(); }
	inline size_t StrLen(const CString &str) { return static_cast <size_t>(str.GetLength()); }
	inline DWORD StrDwLen( const CString &str ) { return static_cast<DWORD>(str.GetLength()); }
	void TraceDiagnosticInfo( std::string msg );
#endif //_AFX

#ifndef EXCLUDE_BOOST_CODE
	std::string ReplaceInCopy(const std::string &src, const std::string &searchStr, const std::string &ReplacementStr);
	std::string Replace(std::string &src, const std::string &searchStr, const std::string &ReplacementStr);
	void ReplaceAll(std::string &src, const std::string &searchStr, const std::string &ReplacementStr);
	std::wstring ReplaceInCopy(const std::wstring &src, const std::wstring &searchStr, const std::wstring &ReplacementStr);
	std::wstring Replace(std::wstring &src, const std::wstring &searchStr, const std::wstring &ReplacementStr);
	void ReplaceAll(std::wstring &src, const std::wstring &searchStr, const std::wstring &ReplacementStr);
#endif //!EXCLUDE_BOOST_CODE

	template <class strType1, class strType2> bool MatchesNoCase(strType1 str1, strType2 str2)
	{
		std::wstring wstr1 = ToWString(str1);
		std::wstring wstr2 = ToWString(str2);
		return (0 == _wcsicmp(wstr1.c_str(), wstr2.c_str()));
	}

	inline size_t StrLen(const char * str) { return strlen(str); }
	inline size_t StrLen(const wchar_t * str) { return wcslen(str); }

#ifdef _WINBASE_
#define GETLASTERROR__	GetLastError()
#else
#include <cstddef>
#define GETLASTERROR__	GetErrNo()
	int GetErrNo()
	{
		int Err = 0;
		_get_errno(&Err);
		return Err;
	}
	using DWORD = unsigned long;
#endif

	inline DWORD StrDwLen( const char * str ) { return static_cast<DWORD>(strlen( str )); }
	inline DWORD StrDwLen( const wchar_t * str ) { return static_cast<DWORD>(wcslen( str )); }

	class errno_exception
	{
		int m_errno;
		std::string m_what;
		std::exception *m_exception;
		std::string m_name;
	public:
		errno_exception( const char *name, std::exception &Exception, int no = GETLASTERROR__, std::string s = "" ) :m_errno( no ), m_what( s.size() ? s : Exception.what() ), m_exception( &Exception ), m_name( name ) {}
		errno_exception( const char *s = NULL, int no = GETLASTERROR__ ) :m_errno( no ), m_what( (s == NULL) ? "Unknown Exception" : s ), m_exception( NULL ), m_name( (s == NULL) ? "Unknown Exception" : "Exception" ) {}
		errno_exception( std::string &s, int no = GETLASTERROR__ ) :m_errno( no ), m_what( s ), m_exception( NULL ), m_name( "Exception" ) {}
		const char* what() { return (m_exception == NULL) ? m_what.c_str() : m_exception->what(); }
		int code() { return m_errno; }
		const char* name() { return m_name.c_str(); }
	};

	class auto_lock
	{
		std::mutex mtx;
	public:
		auto_lock(){ mtx.lock(); }
		~auto_lock(){ mtx.unlock(); }
	};


}

#define REPORT_ERR_AND_EXIT(exit_err_no, msg_format, ...) {CString msg; msg.Format(_T("Error:  " msg_format "  Performming early exit due to error."),__VA_ARGS__);AfxMessageBox(msg);exit(exit_err_no);}
#define ERR_LOG(msg_format, ...	)	{Common::Logging logging(__FILE__, __LINE__, __FUNCTION__, Common::Verbosity::ERR, msg_format __VA_OPT__(,) __VA_ARGS__);}

