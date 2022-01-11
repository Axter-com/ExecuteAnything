/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#ifndef _PRGLANGUAGESHELP_H_
#define _PRGLANGUAGESHELP_H_
#include "ExecuteAnything.h"
#include "sqlite3pp_ez.h"
#include "SQL\sql_Master_Header.h"

class PrgLanguagesHelp
{
	HWND m_hwndTip;
	CNppPlugin &m_CNppPlugin;
	CNppExecPluginMsgSender m_npeMsgr;
	CNppMessager m_nppMsgr;
	bool m_InternalPrgLangKeywordsInitiated;
	bool m_ExternalPrgLangKeywordsInitiated;
	sqlite3pp::database &m_db;
	tstring ModDateOf_userDefineLangXmlFile;
	sqlite3pp::Table <sql_table_PrgLangTraits> tble_PrgLangTraits;
	sqlite3pp::Table <sql_table_KeyWords> tble_KeyWords;
public:
	PrgLanguagesHelp(CNppPlugin &cnppplugin);
private:
	void UpdateDB();
	bool InitiateInternalPrgLangKeywordsIntoDB();
	bool InitiateExternalPrgLangKeywordsIntoDB(bool Proccess_SubPaths);
	bool InitiateExternalPrgLangKeywordsIntoDB(const tstring &LangsXml_filename, int &PrgLangID_StartIndex);
	void AddToDatabase(std::string LangName, std::string Exts, std::string &commentLine, std::string &commentStart, std::string &commentEnd, const std::vector<std::string> &Keywords, int &PrgLangID_StartIndex, const tstring &SrcFileName);
	void DoSelftDiagnostics(); // Should only be used for trouble shooting purposes
	static const int MinKeywordLen;
	static const int PRGLANGID_STARTINDEX;
	static const tstring INITIATED;
	static const tstring PRGLANGKEYWORDSINITIATED_BASEKEY;
	static const tstring userDefinedLanguagesUDLs[];
	static const int Qty_userDefinedLanguagesUDLs;
	static const tstring InternalPrgLangKeywordsInitiated;
	static const tstring ExternalPrgLangKeywordsInitiated;
	static const tstring ExternalPrgLangKeywordsModDate;
	static const std::string UserLanguageNamePrefix;
	static const std::string KeywordsKeywordsNamePrefix;
	static const std::string KeywordsWordsNamePrefix;
	static const std::string KeywordsCommentsPrefix;
	static const std::string KeywordsPost;
	static const std::string UserLanguageNamePost;
	static const std::string InlineEndAncher;
	static const std::regex SearchLangName;
	static const std::regex SearchLangKeywordsSingleLine;
	static const std::regex SearchLangKeywordsMultipleLines;
	static const std::regex SearchLangComments;
};

#endif // !_PRGLANGUAGESHELP_H_