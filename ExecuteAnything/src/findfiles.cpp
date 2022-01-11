/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
/*
Description:
  *** For example usage, see comments at end of header file ***
*/
#include "findfiles.h"
#include <windows.h>
#include <tchar.h>
#include <regex>

typedef std::basic_regex<TCHAR> tregex;
typedef std::match_results<find_files_base::tstring::const_iterator> tsmatch;

find_files_base::tstring find_files_base::setStartingPath(tstring StartingPath, bool AppendSep)
{
	size_t endIndex = StartingPath.find_last_not_of(_T("\\/"));
	if (endIndex != tstring::npos)
	{
		StartingPath = StartingPath.substr(0, endIndex + 1);
	}

	if (AppendSep)
		StartingPath += PathSep;
	return StartingPath;
}

// Main constructor
find_files_base::find_files_base(tstring StartingPath, const tstring &FileTypes, bool SearchSubDir, bool FindDir, bool FindFiles, FUNC_PTR_1_arg fptr1, FUNC_PTR_2_arg fptr2, bool AppendSep)
	: m_FileTypes(FileTypes)
	, m_FuncPtr1(fptr1)
	, m_FuncPtr2(fptr2)
	, c_StartingPathList(m_StartingPathList)
	, c_FileList(m_FileList)
	, m_StartingPath(setStartingPath(StartingPath, AppendSep))
	, m_SearchSubDir(SearchSubDir)
	, m_FindDir(FindDir)
	, m_FindFiles(FindFiles)
	, m_ParentStartPath(getParentPath(StartingPath, true))
{
	if (m_PathTree.size())
	{
		FindInDir(m_PathTree[0], 1);
	}
	else
		FindInDir(m_StartingPath);
}


find_files_base::tstring find_files_base::appendToPath(const tstring &StartingPath, const tstring &WildFileType, bool AlwaysAppend)
{
	if (!AlwaysAppend && (StartingPath.find(_T("*")) != tstring::npos || StartingPath.find(_T("?")) != tstring::npos))
		return StartingPath;
	if (!StartingPath.size() || StartingPath[StartingPath.size()-1] == '\\')
		return StartingPath + WildFileType;
	return StartingPath + _T("\\") + WildFileType;
}

const std::regex_constants::match_flag_type regex_compatibility_flag = std::regex_constants::format_sed;

find_files_base::tstring find_files_base::getParentPath(const tstring &StartingPath, bool PopulatePathTree)
{
	if (StartingPath.find(_T("*")) == tstring::npos && StartingPath.find(_T("?")) == tstring::npos)
		return StartingPath;

	//(^[^\n\r]*\\)([^/\\\n\r]+)$
	//(^[^\\n\\r]*\\\\)([^/\\\\\\n\\r]+)$
	static const tregex SearchParentPath = tregex(_T("(^[^\\n\\r]*\\\\)([^/\\\\\\n\\r]+)"), std::regex_constants::ECMAScript | std::regex_constants::icase);
	tsmatch match;
	if (std::regex_search(StartingPath, match, SearchParentPath))
	{
		tstring m0 = match[0].str();
		tstring m1 = match[1].str();
		if (m1.find(_T("*")) == tstring::npos && m1.find(_T("?")) == tstring::npos)
			return m1;

		// If m1 has wild char, then parsed out the path
		if (PopulatePathTree)
		{
			// \\[^\n\r?\*\\]*[\*?]+[^\n\r\\]*
			static const tregex SearchWildPaths = tregex(_T("\\\\[^\\n\\r?\\*\\\\]*[\\*?]+[^\\n\\r\\\\]*"), std::regex_constants::ECMAScript | std::regex_constants::icase);
			tstring last_suffix;
			tstring::const_iterator start = StartingPath.begin(), end = StartingPath.end();
			while (std::regex_search(start, end, match, SearchWildPaths, regex_compatibility_flag))
			{
				tstring Parent = match.prefix();
				tstring child = match[0].str();
				last_suffix = match.suffix();
				m_PathTree.push_back(Parent + child);
				start = match.suffix().first;
			}
			m_PathTree.push_back(last_suffix);
		}
	}
	return StartingPath;
}

const find_files_base::tstring find_files_base::PathSep = _T("\\");
const find_files_base::tstring find_files_base::Dot = _T(".");
const find_files_base::tstring find_files_base::DotDot = _T("..");
const find_files_base::tstring find_files_base::DirectorySearchWild = _T("*.*");

bool find_files_base::FindInDir(const tstring &StartingPath, int iterate)
{
	bool rv = true;
	const tstring ParentPath = getParentPath(StartingPath);
	if (m_SearchSubDir || iterate != -1)
	{
		const tstring StartingPathAndWild = appendToPath(StartingPath, DirectorySearchWild);
		WIN32_FIND_DATA MyWIN32_FIND_DATA_dir;
		HANDLE fff_subdir = FindFirstFile(StartingPathAndWild.c_str(), &MyWIN32_FIND_DATA_dir);
		if (fff_subdir && fff_subdir != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (Dot == MyWIN32_FIND_DATA_dir.cFileName || DotDot == MyWIN32_FIND_DATA_dir.cFileName)
					continue;
				if (MyWIN32_FIND_DATA_dir.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					if (m_FindDir && iterate == m_PathTree.size())
					{
						m_StartingPathList.push_back(ParentPath);
						m_FileList.push_back(_T(""));
						m_fqn.push_back(ParentPath);
						if (m_FuncPtr1)
							m_FuncPtr1(ParentPath);
						else if (m_FuncPtr2)
							m_FuncPtr2(ParentPath, _T(""));
					}
					if (iterate == -1)
						rv = FindInDir(ParentPath + MyWIN32_FIND_DATA_dir.cFileName + PathSep);
					else
					{
						if (iterate < m_PathTree.size())
							rv = FindInDir(ParentPath + MyWIN32_FIND_DATA_dir.cFileName + m_PathTree[iterate], iterate + 1);
					}
				}
			} while (rv && FindNextFile(fff_subdir, &MyWIN32_FIND_DATA_dir) != FALSE);
		}
		else
			return false;
		FindClose(fff_subdir);
		if (!rv)
			return rv;
	}

	if (iterate == -1)
	{
		WIN32_FIND_DATA MyWIN32_FIND_DATA;
		const tstring FilesToSearch = appendToPath(StartingPath, m_FileTypes, true);
		HANDLE fff_files = FindFirstFile(FilesToSearch.c_str(), &MyWIN32_FIND_DATA);
		if (fff_files && fff_files != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (MyWIN32_FIND_DATA.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && !m_FindDir)
					continue;
				if (!(MyWIN32_FIND_DATA.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && !m_FindFiles)
					continue;
				if (Dot == MyWIN32_FIND_DATA.cFileName || DotDot == MyWIN32_FIND_DATA.cFileName)
					continue;
				tstring FileName = (!m_FindDir || ParentPath[ParentPath.size() - 1] == '\\') ? MyWIN32_FIND_DATA.cFileName : _T("");
				m_StartingPathList.push_back(ParentPath);
				m_FileList.push_back(FileName);
				m_fqn.push_back(appendToPath(ParentPath, FileName, true));
				if (m_FuncPtr1)
					m_FuncPtr1(ParentPath + FileName);
				else if (m_FuncPtr2)
					m_FuncPtr2(ParentPath, FileName);
			} while (FindNextFile(fff_files, &MyWIN32_FIND_DATA) != FALSE);
		}
		else
			rv = false;
		FindClose(fff_files);
	}

	return rv;
}


// Derived constructors
find_files::find_files(tstring StartingPath, const tstring &FileTypes, bool SearchSubDir, bool FindDir, bool FindFiles)
	:find_files_base(StartingPath, FileTypes, SearchSubDir, FindDir, FindFiles, NULL, NULL, false)
{
}

find_files_callback::find_files_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_1_arg fptr1, bool SearchSubDir, bool FindDir, bool FindFiles)
	: find_files_base(StartingPath, FileTypes, SearchSubDir, FindDir, FindFiles, fptr1, NULL, false)
{
}

find_files_callback::find_files_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_2_arg fptr2, bool SearchSubDir, bool FindDir, bool FindFiles)
	: find_files_base(StartingPath, FileTypes, SearchSubDir, FindDir, FindFiles, NULL, fptr2, false)
{
}

find_folders_callback::find_folders_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_1_arg fptr1, bool SearchSubDir)
	: find_files_callback(StartingPath, FileTypes, fptr1, SearchSubDir, true, false)
{
}

find_folders_callback::find_folders_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_2_arg fptr2, bool SearchSubDir)
	: find_files_callback(StartingPath, FileTypes, fptr2, SearchSubDir, true, false)
{
}



find_folders::find_folders(tstring StartingPath, bool SearchSubDir)
	:find_files_base(StartingPath, _T(""), SearchSubDir, true, false, NULL, NULL, false)
{
	for (int i = 0; i < c_FileList.size(); ++i)
	{
		tstring DirName = c_StartingPathList[i];
		tstring FileName = c_FileList[i];
		m_paths.insert(DirName + FileName);
	}
}

find_folders_rsort::find_folders_rsort(tstring StartingPath, bool SearchSubDir)
	: find_folders(StartingPath, SearchSubDir)
{
	for (auto rit = m_paths.rbegin(); rit != m_paths.rend(); rit++)
		m_rev_order_paths.push_back(*rit);
}

