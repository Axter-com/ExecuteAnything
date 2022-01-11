#ifndef _base_h_
#define _base_h_

#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_regex<TCHAR> tregex;
typedef std::basic_ofstream<TCHAR, std::char_traits<TCHAR> > tofstream;
typedef std::basic_ifstream<TCHAR, std::char_traits<TCHAR> > tifstream;
typedef std::match_results<tstring::const_iterator> tsmatch;

#endif // !_base_h_

