/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

Description:
  Find files or directories using simple C++ interface. Can optionally be used with call back functions.
  For example usage with call back functions, see example usage at the bottom of this file.
  Example Usage #1:
		find_files foundFiles(MyFolder.c_str(), _T("*.xml"));
		for (auto filename : foundFiles)
			cout << "Found file " << filename << endl;

  Example Usage #2: Get all folders recursively
		find_folders MyFoundDir(MyPath.c_str(), true);
		for (auto dirname : MyFoundDir)
			ProcessDir(dirname);

  Example Usage #3: Get directories in reverse sorted order which are directly listed in given path. (not recursive)
		find_folders_rsort FoundDir(MyPath.c_str(), false);
		for (auto dirname : FoundDir)
			ProcessDir(dirname);
*/
#ifndef FINDFILES_WITH_CALLBACK_H_INCLUDED_HEADER_GUARD_
#define FINDFILES_WITH_CALLBACK_H_INCLUDED_HEADER_GUARD_

#ifdef WIN32
#pragma warning(disable : 4786)
#endif//WIN32

#include <string>
#include <vector>
#include <set>

class find_files_base
{
 public:
#ifdef _UNICODE
	 using tstring = std::wstring;
#else
	 using tstring = std::string;
#endif // _UNICODE
	typedef std::vector<tstring> container_t;
	typedef bool (*FUNC_PTR_1_arg)(const tstring &FullFileName);
	typedef bool (*FUNC_PTR_2_arg)(const tstring& Path, const tstring &FileName);
	template<typename T, typename F>bool IterateListToCallBackFunc(T &SrcObj, F func){IterateListToCallBackFuncMain(SrcObj, func, NULL, NULL, 0);}
protected:
	find_files_base(tstring StartingPath, const tstring &FileTypes, bool SearchSubDir, bool FindDir, bool FindFiles, FUNC_PTR_1_arg fptr1, FUNC_PTR_2_arg fptr2, bool AppendSep);
	virtual bool FindInDir(const tstring &StartingPath, int iterate = -1 );
	static const tstring PathSep;
	static const tstring Dot;
	static const tstring DotDot;
	static const tstring DirectorySearchWild;
private:
	template<typename T, typename F>
		bool IterateListToCallBackFuncMain(T &SrcObj, F func, FUNC_PTR_1_arg fptr1, FUNC_PTR_2_arg fptr2, int Sw)
	{
		typedef container_t::iterator itr;
		for (itr itpth = m_StartingPathList.begin(), itfl = m_FileList.begin(), end = m_StartingPathList.end();itpth != end;++itpth, ++itfl)
			switch(Sw){
			case 0:
				if (!(SrcObj.*func)(*itpth, *itfl))
					return false;
				break;
			case 1:
				if (!fptr1(*itpth + *itfl))
					return false;
				break;
			case 2:
				if (!fptr2(*itpth, *itfl))
					return false;
				break;
			}

		return true;
	}
	FUNC_PTR_1_arg m_FuncPtr1;
	FUNC_PTR_2_arg m_FuncPtr2;
	const bool m_SearchSubDir;
	const bool m_FindDir;
	const bool m_FindFiles;
	container_t m_PathTree;
	container_t m_StartingPathList;
	container_t m_FileList;
	container_t m_fqn;
	const tstring m_FileTypes;
	const tstring m_StartingPath;
	const tstring m_ParentStartPath;
	bool dummyfunc(const tstring&,const tstring&){}
	typedef bool (find_files_base::*funct_ptr_t)(const tstring &,const tstring &);
	tstring setStartingPath(tstring scr, bool AppendSep = true);
	tstring appendToPath(const tstring &StartingPath, const tstring &WildFileType, bool AlwaysAppend = false);
	tstring getParentPath(const tstring &scr, bool PopulatePathTree = false);
public:
	const tstring& operator[](int i) { return m_fqn[i]; }
	auto begin() { return m_fqn.begin(); }
	auto end() { return m_fqn.end(); }
	size_t size() const { return m_fqn.size(); }
	bool empty() const { return !m_fqn.size(); }
	const container_t &c_StartingPathList;
	const container_t &c_FileList;
	const container_t& GetDirPaths() { return c_StartingPathList; }
	const container_t& GetFilePaths() { return c_FileList; }
};

class find_files : public find_files_base
{
public:
	find_files(tstring StartingPath, const tstring &FileTypes, bool SearchSubDir = true, bool FindDir = false, bool FindFiles = true);
};

class find_files_callback : public find_files_base
{
public:
	find_files_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_1_arg fptr1, bool SearchSubDir = true, bool FindDir = false, bool FindFiles = true);
	find_files_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_2_arg fptr2, bool SearchSubDir = true, bool FindDir = false, bool FindFiles = true);
};

class find_folders_callback : public find_files_callback
{
public:
	find_folders_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_1_arg fptr1, bool SearchSubDir = true);
	find_folders_callback(tstring StartingPath, const tstring &FileTypes, FUNC_PTR_2_arg fptr2, bool SearchSubDir = true);
};

class find_folders : public find_files_base
{
protected:
	std::set<tstring> m_paths;
public:
	find_folders(tstring StartingPath, bool SearchSubDir = true);
	const std::set<tstring>& GetSortedPaths() const { return m_paths; }
};

class find_folders_rsort : public find_folders
{
	container_t m_rev_order_paths;
public:
	find_folders_rsort(tstring StartingPath, bool SearchSubDir = true);
	const container_t& GetRevSortedPaths() const { return m_rev_order_paths; }
};



#endif // !FINDFILES_WITH_CALLBACK_H_INCLUDED_HEADER_GUARD_


/*	*** Example usage with call back functions ***
//Example Usage #4
void MyFunction1(const tstring &FullFileName)
{
	std::cout << FullFileName << std::endl;
}
void main(void)
{
	find_files_callback My_FindFilesInPath(_T("C:\\Program Files\\"), _T("*.htm."), MyFunction1);
}

//Example Usage #5
void MyFunction2(const tstring &Path, const tstring &FileName)
{
	const std::string FullFileName = Path + FileName;
	std::cout << FullFileName << std::endl;
}
void main(void)
{
	find_files_callback My_FindFilesInPath(_T("C:\\Program Files\\"), _T("*.htm."), NULL,MyFunction2);
}
*/

