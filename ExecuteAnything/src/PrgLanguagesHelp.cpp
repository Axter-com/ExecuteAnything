/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "PrgLanguagesHelp.h"
#include "findfiles.h"

#include<boost/tokenizer.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include<iostream> 
#include<string> 
#include<fstream>
#include <sstream>
#include <iomanip>

using namespace sqlite3pp;

const tstring PrgLanguagesHelp::INITIATED = _T("Initiated");
const tstring PrgLanguagesHelp::PRGLANGKEYWORDSINITIATED_BASEKEY = _T("PrgLangKeywordsInitiated_");
const int PrgLanguagesHelp::MinKeywordLen = 2;
const int PrgLanguagesHelp::PRGLANGID_STARTINDEX = 5000;
const tstring PrgLanguagesHelp::userDefinedLanguagesUDLs[] = { _T("userDefineLangs"), _T("userDefinedLanguages-master\\UDLs"), _T("UDLs"), _T("userDefinedLanguages-master"), _T("userDefinedLanguages\\UDLs"), _T("plugins\\ExecuteAnything\\userDefineLangs") };
const int PrgLanguagesHelp::Qty_userDefinedLanguagesUDLs = sizeof(PrgLanguagesHelp::userDefinedLanguagesUDLs) / sizeof(PrgLanguagesHelp::userDefinedLanguagesUDLs[0]);
const tstring PrgLanguagesHelp::InternalPrgLangKeywordsInitiated	= PRGLANGKEYWORDSINITIATED_BASEKEY + _T("Internal");
const tstring PrgLanguagesHelp::ExternalPrgLangKeywordsInitiated	= PRGLANGKEYWORDSINITIATED_BASEKEY + _T("External");
const tstring PrgLanguagesHelp::ExternalPrgLangKeywordsModDate		= PRGLANGKEYWORDSINITIATED_BASEKEY + _T("External_ModDate");
const std::string PrgLanguagesHelp::UserLanguageNamePrefix = "<UserLang name=\"";
const std::string PrgLanguagesHelp::KeywordsKeywordsNamePrefix = "<Keywords name=\"Keywords";
const std::string PrgLanguagesHelp::KeywordsWordsNamePrefix = "<Keywords name=\"Words";
const std::string PrgLanguagesHelp::KeywordsCommentsPrefix = "<Keywords name=\"Comments\">";
const std::string PrgLanguagesHelp::KeywordsPost = "</Keywords>";
const std::string PrgLanguagesHelp::UserLanguageNamePost = "</UserLang>";
const std::string PrgLanguagesHelp::InlineEndAncher = "/>";
const std::regex PrgLanguagesHelp::SearchLangName = std::regex("<UserLang name=\"([^\"]+)\"\\s*ext=\"([^\"]*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
const std::regex PrgLanguagesHelp::SearchLangKeywordsSingleLine = std::regex("<Keywords name=\"(?:Keywords|Words)[0-9]+\"\\s*>([^<]+)</Keywords>", std::regex_constants::ECMAScript | std::regex_constants::icase);
const std::regex PrgLanguagesHelp::SearchLangKeywordsMultipleLines = std::regex("<Keywords name=\"(?:Keywords|Words)[0-9]+\"\\s*>([^<]+)", std::regex_constants::ECMAScript | std::regex_constants::icase);
const std::regex PrgLanguagesHelp::SearchLangComments = std::regex("<Keywords name=\"Comments\">00([^0]*)01([^0]*)02([^0]*)03([^0]*)04([^<]*)", std::regex_constants::ECMAScript | std::regex_constants::icase);

PrgLanguagesHelp::PrgLanguagesHelp(CNppPlugin &cnppplugin)
	:m_CNppPlugin(cnppplugin)
	, m_npeMsgr(m_CNppPlugin.getNppWnd()
	, m_CNppPlugin.getDllFileName())
	, m_hwndTip(NULL)
	, m_InternalPrgLangKeywordsInitiated(false)
	, m_ExternalPrgLangKeywordsInitiated(false)
	, m_db(ExecuteAnything::GetExecuteAnythingDB())
{
	UpdateDB();
}

void PrgLanguagesHelp::UpdateDB()
{
	// This function is used to Initiate ExecuteAnything DB with program language keywords and language traits for both NPP internal and external program languages.
	// NPP internal program languages are not scanned if the database file is distribute with the NPP internal program languages prepoulated.
	// NPP external languages located in sub-folders do not get scanned if the database file is distribute with the NPP external program languages prepoulated.
	// Initially, the external languages found in userDefineLang.xml 
	// After DB has initial keywords, this function is used to update the db if the file modification date has changed for file userDefineLang.xml 
	Table <sql_table_MiscData> sql_table_miscdata(WhereClauseArg(_T("where DataName like '") + PRGLANGKEYWORDSINITIATED_BASEKEY + _T("%'")));
	int QtyTypesInitiated = 0;
	for (auto miscdata : sql_table_miscdata)
	{
		if (to_tstring(miscdata.get_DataValue()) == INITIATED)
			++QtyTypesInitiated;
		if (miscdata.get_DataName() == InternalPrgLangKeywordsInitiated)
		{
			if (to_tstring(miscdata.get_DataValue()) == INITIATED)
				m_InternalPrgLangKeywordsInitiated = true;
		}
		else if (miscdata.get_DataName() == ExternalPrgLangKeywordsInitiated)
		{
			if (to_tstring(miscdata.get_DataValue()) == INITIATED)
				m_ExternalPrgLangKeywordsInitiated = true;
		}
		else if (miscdata.get_DataName() == ExternalPrgLangKeywordsModDate)
		{
			ModDateOf_userDefineLangXmlFile = to_tstring(miscdata.get_DataValue());
		}
	}

	if (!m_InternalPrgLangKeywordsInitiated)
	{
		if (InitiateInternalPrgLangKeywordsIntoDB())
		{
			m_InternalPrgLangKeywordsInitiated = true;
			sql_table_miscdata.Execute(_T("INSERT INTO MiscData (DataName, DataValue) VALUES ('") + InternalPrgLangKeywordsInitiated + _T("', '") + INITIATED + _T("')"));
		}
	}

	if (InitiateExternalPrgLangKeywordsIntoDB(Qty_userDefinedLanguagesUDLs + 2 > QtyTypesInitiated) && !m_ExternalPrgLangKeywordsInitiated)
	{
		m_ExternalPrgLangKeywordsInitiated = true;
		sql_table_miscdata.Execute(_T("INSERT INTO MiscData (DataName, DataValue) VALUES ('") + ExternalPrgLangKeywordsInitiated + _T("', '") + INITIATED + _T("')"));
	}
}

std::string to_string(FILETIME ftime) // ISO format, time zone designator Z == zero (UTC)
{
	SYSTEMTIME utc;
	::FileTimeToSystemTime(std::addressof(ftime), std::addressof(utc));

	std::ostringstream stm;
	const auto w2 = std::setw(2);
	stm << std::setfill('0') << std::setw(4) << utc.wYear << '-' << w2 << utc.wMonth
		<< '-' << w2 << utc.wDay << ' ' << w2 << utc.wYear << ' ' << w2 << utc.wHour
		<< ':' << w2 << utc.wMinute << ':' << w2 << utc.wSecond << 'Z';

	return stm.str();
}

std::wstring to_wstring(FILETIME ftime)
{
	return to_wstring(to_string(ftime));
}


bool PrgLanguagesHelp::InitiateExternalPrgLangKeywordsIntoDB(bool Proccess_SubPaths)
{
	int PrgLangID_StartIndex = PRGLANGID_STARTINDEX;
	Table <sql_view_v_LastPrgLangTraitsID> LastPrgLangTraitsID;
	if (LastPrgLangTraitsID.size() && LastPrgLangTraitsID[0].get_MaxID().size())
	{
		int MaxID = std::stoi(LastPrgLangTraitsID[0].get_MaxID()) + 1;
		if (MaxID > PrgLangID_StartIndex)
			PrgLangID_StartIndex = MaxID;
	}
	const tstring NppMainXmlPath = m_npeMsgr.getNppRoaming();
	WIN32_FILE_ATTRIBUTE_DATA attrData = { 0 };
	const tstring userDefineLangXmlFileName = NppMainXmlPath + _T("userDefineLang.xml");
	GetFileAttributesEx(userDefineLangXmlFileName.c_str(), GetFileExInfoStandard, &attrData);
	tstring DateStamp = to_tstring(attrData.ftLastWriteTime);
	if (ModDateOf_userDefineLangXmlFile != DateStamp)
	{
		if (InitiateExternalPrgLangKeywordsIntoDB(userDefineLangXmlFileName, PrgLangID_StartIndex))
		{
			Table <sql_table_MiscData> sql_table_miscdata;
			sql_table_miscdata.Execute(_T("INSERT INTO MiscData (DataName, DataValue) VALUES ('") + ExternalPrgLangKeywordsModDate + _T("', '") + DateStamp + _T("')"));
		}
	}

	if (Proccess_SubPaths)
	{
		for (auto userDefinedLanguagesUDL : userDefinedLanguagesUDLs)
		{
			Table <sql_table_MiscData> sql_table_miscdata(WhereClauseArg(_T("where DataName like '") + PRGLANGKEYWORDSINITIATED_BASEKEY + userDefinedLanguagesUDL + _T("'")));
			if (sql_table_miscdata.size() && to_tstring(sql_table_miscdata[0].get_DataValue()) == INITIATED)
				continue;
			find_files findfiles(NppMainXmlPath + userDefinedLanguagesUDL, _T("*.xml"));
			for (auto file : findfiles)
				InitiateExternalPrgLangKeywordsIntoDB(file, PrgLangID_StartIndex);
			sql_table_miscdata.Execute(_T("INSERT INTO MiscData (DataName, DataValue) VALUES ('") + PRGLANGKEYWORDSINITIATED_BASEKEY + userDefinedLanguagesUDL + _T("', '") + INITIATED + _T("')"));
		}
	}

	return PrgLangID_StartIndex > PRGLANGID_STARTINDEX;
}

bool PrgLanguagesHelp::InitiateExternalPrgLangKeywordsIntoDB(const tstring &LangsXml_filename, int &PrgLangID_StartIndex)
{
	const tstring NL_Tab = _T("\n\t");
	const tstring TopNodeNL_Tab = _T("\n    ");

	std::string line;
	std::ifstream my_if(LangsXml_filename.c_str(), std::ios::in);
	if (!my_if.is_open())
		return false;
	while (std::getline(my_if, line))
	{
		if (line.find(UserLanguageNamePrefix) == std::string::npos || line.find(InlineEndAncher) != std::string::npos)
			continue;
		std::smatch match;
		if (!std::regex_search(line, match, SearchLangName))
			continue;
		const std::string LangName = match[1];
		if (LangName.empty())
			continue;
		const std::string Exts = match[2];
		std::string commentLine;
		std::string commentStart;
		std::string commentEnd;
		std::vector<std::string> Keywords;
		while (std::getline(my_if, line))
		{
			if (line.find(UserLanguageNamePost) != std::string::npos)
				break;

			if (line.find(KeywordsKeywordsNamePrefix) != std::string::npos || line.find(KeywordsWordsNamePrefix) != std::string::npos)
			{
				if (line.find(KeywordsPost) == std::string::npos)
				{
					std::string more_line;
					while (std::getline(my_if, more_line))
					{
						line += " " + more_line;
						if(line.find(KeywordsPost) != std::string::npos)
							break;
					}
					boost::replace_all(line, "\t", " ");
					boost::replace_all(line, "\n", " ");
					boost::replace_all(line, "\r", " ");
					while (line.find("  ") != std::string::npos)
						boost::replace_all(line, "  ", " ");
				}

				if (!std::regex_search(line, match, SearchLangKeywordsSingleLine))
					continue;
				std::string KeywordsInSpaceSeperatedList = match[1];

				if (KeywordsInSpaceSeperatedList.empty())
					continue;
				boost::tokenizer<> tok(KeywordsInSpaceSeperatedList);
				for (auto beg : tok)
				{
					if (beg.size() >= MinKeywordLen)
						Keywords.push_back(beg);
				}
			}
			else if (line.find(KeywordsCommentsPrefix) != std::string::npos)
			{
				if (!std::regex_search(line, match, SearchLangComments))
					continue;
				commentLine = match[1];
				if (match.size() > 4)
					commentStart = match[4];
				if (match.size() > 5)
					commentEnd = match[5];
			}
		}
		AddToDatabase(LangName, Exts, commentLine, commentStart, commentEnd, Keywords, PrgLangID_StartIndex, LangsXml_filename);
	}
	
	my_if.close();
	return true;
}

void PrgLanguagesHelp::AddToDatabase(std::string LangName, std::string Exts, std::string &commentLine, std::string &commentStart, std::string &commentEnd, 
	const std::vector<std::string> &Keywords, int &PrgLangID_StartIndex, const tstring &SrcFileName)
{
	boost::algorithm::trim(LangName);
	boost::algorithm::trim(Exts);
	boost::algorithm::trim(commentLine);
	boost::algorithm::trim(commentStart);
	boost::algorithm::trim(commentEnd);
	sql_table_PrgLangTraits PrgLangTrait;
	PrgLangTrait.set_PrgLangName(to_tstring(LangName));
	PrgLangTrait.set_CommentLineOpen(to_tstring(commentLine));
	PrgLangTrait.set_CommentOpen(to_tstring(commentStart));
	PrgLangTrait.set_CommentClose(to_tstring(commentEnd));
	PrgLangTrait.set_ID(PrgLangID_StartIndex++);
	tble_PrgLangTraits.Insert(PrgLangTrait);
	Table <sql_table_PrgLangTraits> tble_PrgLangTraitsForCurrentLang(WhereClauseArg(_T("where PrgLangName like '") + to_tstring(LangName) + _T("'")));
	if (tble_PrgLangTraitsForCurrentLang.empty())
		return;
	int PrgLangID = tble_PrgLangTraitsForCurrentLang[0].get_ID();

	for (auto keyword : Keywords)
	{
		sql_table_KeyWords keyword_row;
		keyword_row.set_PrgLangTraitsID(PrgLangID);
		keyword_row.set_Keyword(to_tstring(keyword));
		tble_KeyWords.Insert(keyword_row);
	}

	Table <sql_table_PrgLang> tble_PrgLangForCurrentLang(WhereClauseArg(_T("where PrgLangName like '") + to_tstring(LangName) + _T("'")));
	if (tble_PrgLangForCurrentLang.empty())
	{
		sql_table_PrgLang PrgLang_row;
		PrgLang_row.set_PrgLangName(to_tstring(LangName));
		PrgLang_row.set_CommentLineOpen(to_tstring(commentLine));
		PrgLang_row.set_CommentOpen(to_tstring(commentStart));
		PrgLang_row.set_CommentClose(to_tstring(commentEnd));
		PrgLang_row.set_Notes(_T("From Npp file ") + SrcFileName);
		tble_PrgLangForCurrentLang.Insert(PrgLang_row);
	}
	else
	{
		sql_table_PrgLang PrgLang_row = tble_PrgLangForCurrentLang[0];
		bool UpdateDb = false;
		if (PrgLang_row.get_CommentLineOpen().empty() && commentLine.size())
		{
			PrgLang_row.set_CommentLineOpen(to_tstring(commentLine));
			UpdateDb = true;
		}
		if (PrgLang_row.get_CommentOpen().empty() && commentStart.size())
		{
			PrgLang_row.set_CommentOpen(to_tstring(commentStart));
			UpdateDb = true;
		}
		if (PrgLang_row.get_CommentClose().empty() && commentEnd.size())
		{
			PrgLang_row.set_CommentClose(to_tstring(commentEnd));
			UpdateDb = true;
		}

		if (UpdateDb)
		{
			PrgLang_row.set_Notes(PrgLang_row.get_Notes() + _T(";Updated from Npp file ") + SrcFileName);
			tble_PrgLangForCurrentLang.UpdateDb(PrgLang_row);
		}
	}

	boost::tokenizer<> tok(Exts);
	for (auto beg : tok)
	{
		if (beg.size())
		{
			const tstring Ext = _T(".") + to_tstring(beg);
			Table <sql_table_FileExt> tble_FileExt(WhereClauseArg(_T("where PrgLangName like '") + to_tstring(LangName) + _T("' and Ext like '") + Ext + _T("'")));
			if (tble_FileExt.empty())
				tble_FileExt.Execute(_T("INSERT INTO FileExt (PrgLangName, Ext, Notes) VALUES ('") + to_tstring(LangName) + _T("', '") + Ext + _T("'', 'From Npp')"));
		}
	}

}

bool PrgLanguagesHelp::InitiateInternalPrgLangKeywordsIntoDB()
{
	// DoSelftDiagnostics();
	const tstring NppMainXmlPath = m_npeMsgr.getNppRoaming();
	const tstring LangsXml_filename = NppMainXmlPath + _T("langs.xml");
	const tstring NL_Tab = _T("\n\t");
	const tstring TopNodeNL_Tab = _T("\n    ");

	std::string line;
	std::ifstream my_if(LangsXml_filename.c_str(), std::ios::in);
	if (!my_if.is_open())
		return false;
	const int PRGLANGID_STARTINDEX = 1000;
	int PrgLangID_StartIndex = PRGLANGID_STARTINDEX;
	const std::string LanguageNamePrefix = "<Language name=\"";
	const std::string KeywordsNamePrefix = "<Keywords name=\"";
	const std::string LanguageNamePost = "</Language>";
	const std::string InlineEndAncher = "/>";
	const std::string commentEnd = "commentEnd=";
	while (std::getline(my_if, line))
	{
		if (line.find(LanguageNamePrefix) == std::string::npos || line.find(InlineEndAncher) != std::string::npos)
			continue;
		std::smatch match;
		std::regex SearchLangName = std::regex("<Language name=\"([^\"]+)\"\\s*ext=\"([^\"]*)\"\\s+commentLine=\"([^\"]*)", std::regex_constants::ECMAScript | std::regex_constants::icase);
		if (!std::regex_search(line, match, SearchLangName))
			continue;

		const std::string LangName = match[1];
		if (LangName.empty())
			continue;
		const std::string Exts = match[2];
		std::string commentLine;
		std::string commentStart;
		std::string commentEnd;
		if (match.size() > 3)
			commentLine = match[3];
		SearchLangName = std::regex("<Language name=\"([^\"]+)\"\\s*ext=\"([^\"]*)\"\\s+commentLine=\"([^\"]*)\"\\s+commentStart=\"([^\"]*)\"\\s+commentEnd=\"([^\"]*)\"\\s*>", std::regex_constants::ECMAScript | std::regex_constants::icase);
		if (line.find(commentEnd) != std::string::npos && std::regex_search(line, match, SearchLangName))
		{
			if (match.size() > 4)
			{
				commentStart = match[4];
				if (match.size() > 5)
				{
					commentEnd = match[5];
				}
			}
		}

		std::vector<std::string> Keywords;
		while (std::getline(my_if, line))
		{
			if (line.find(LanguageNamePost) != std::string::npos)
				break;
			if (line.find(KeywordsNamePrefix) == std::string::npos)
				continue;
			std::regex SearchLangKeywords = std::regex("<Keywords name=\"[^\"]+\"\\s*>([^<]+)</Keywords>", std::regex_constants::ECMAScript | std::regex_constants::icase);
			if (!std::regex_search(line, match, SearchLangKeywords))
				continue;
			std::string KeywordsInSpaceSeperatedList = match[1];
			if (KeywordsInSpaceSeperatedList.empty())
				continue;
			boost::tokenizer<> tok(KeywordsInSpaceSeperatedList);
			for (auto beg : tok)
			{
				if (beg.size() >= MinKeywordLen)
					Keywords.push_back(beg);
			}
		}

		AddToDatabase(LangName, Exts, commentLine, commentStart, commentEnd, Keywords, PrgLangID_StartIndex, LangsXml_filename);
	}

	my_if.close();

	return PrgLangID_StartIndex > PRGLANGID_STARTINDEX;
}

void PrgLanguagesHelp::DoSelftDiagnostics() // Should only be used for trouble shooting purposes
{
	Table <sql_table_TestAllTypes> tble_TestAllTypes;
	sql_table_TestAllTypes TestAllTypes;
	TestAllTypes.set_TEXT_T(_T("Test with only TEXT type set."));
	tble_TestAllTypes.Insert(TestAllTypes);  // Test with only TEXT type set.

	TestAllTypes.set_TEXT_T(_T("Test for TEXT type"));
	TestAllTypes.set_NcharType1(_T("Test for NcharType1 type"));
	TestAllTypes.set_NcharType2(_T("Test for NcharType2 type"));
	TestAllTypes.set_NVarCharType1(_T("Test for NVarCharType1 type"));
	TestAllTypes.set_Char1("Test for Char1 type");
	TestAllTypes.set_Varchar1("Test for Varchar1 type");
	TestAllTypes.set_Varchar2("Test for Varchar2 type");
	int Val = 100;
	TestAllTypes.set_INTEGER_T(Val++);
	TestAllTypes.set_Int1(Val++);
	TestAllTypes.set_TinyInt1(Val++);
	TestAllTypes.set_SmalllInt1(Val++);
	TestAllTypes.set_MedInt1(Val++);
	TestAllTypes.set_BigInt1(Val++);
	TestAllTypes.set_UBigInt1(Val++);
	TestAllTypes.set_Int2_1(Val++);
	TestAllTypes.set_Int8_1(Val++);
	Val = 500;
	TestAllTypes.set_REAL_T(0.654321 + Val++);
	TestAllTypes.set_Double1(0.654321 + Val++);
	TestAllTypes.set_DoublePrec1(0.654321 + Val++);
	TestAllTypes.set_Float1(0.654321 + Val++);
	TestAllTypes.set_NUMERIC_T(0.654321 + Val++);
	TestAllTypes.set_Decimal1(0.654321 + Val++);
	TestAllTypes.set_Boolean1(false);
	tble_TestAllTypes.Insert(TestAllTypes); // Test with out setting Clob and Blob

	Val = 200;
	TestAllTypes.set_INTEGER_T(Val++);
	TestAllTypes.set_Int1(Val++);
	TestAllTypes.set_TinyInt1(Val++);
	TestAllTypes.set_SmalllInt1(Val++);
	TestAllTypes.set_MedInt1(Val++);
	TestAllTypes.set_BigInt1(Val++);
	TestAllTypes.set_UBigInt1(Val++);
	TestAllTypes.set_Int2_1(Val++);
	TestAllTypes.set_Int8_1(Val++);
	Val = 9000;
	TestAllTypes.set_REAL_T(0.654321 + Val++);
	TestAllTypes.set_Double1(0.654321 + Val++);
	TestAllTypes.set_DoublePrec1(0.654321 + Val++);
	TestAllTypes.set_Float1(0.654321 + Val++);
	TestAllTypes.set_NUMERIC_T(0.654321 + Val++);
	TestAllTypes.set_Decimal1(0.654321 + Val++);
	TestAllTypes.set_Boolean1(true);
	std::string clob1_content = "Test for Clob type";
	Clob clob1 = std::shared_ptr< std::vector<char> >(new std::vector<char>(clob1_content.begin(), clob1_content.end()));
	TestAllTypes.set_Clob1(clob1);
	std::string blob1_content = "Test for Blob type";
	Blob blob1 = std::shared_ptr<std::vector<unsigned char> >(new std::vector<unsigned char>(blob1_content.begin(), blob1_content.end()));
	TestAllTypes.set_BLOB_T(blob1);
	tble_TestAllTypes.Insert(TestAllTypes); // Test with all types set.
}




























