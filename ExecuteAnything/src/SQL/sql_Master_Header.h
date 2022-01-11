#ifndef SQL_MASTER_HEADER_H
#define SQL_MASTER_HEADER_H
#include "..\sqlite3pp_ez.h"
#include "sql_table_FileExt.h"
#include "sql_table_MiscData.h"
#include "sql_table_PrgLang.h"
#include "sql_table_SrcCodeType.h"
#include "sql_table_TestAutomationTestType.h"
#include "sql_table_ProgrammingLanguageProfiles.h"
#include "sql_table_AdvProfile_Settings.h"
#include "sql_table_AdvProfile_Variables.h"
#include "sql_table_InstallationPaths.h"
#include "sql_table_LangInstallationUsage.h"
#include "sql_table_Prerequisites.h"
#include "sql_table_TestAutomationSrcCode.h"
#include "sql_view_v_ProgrammingLanguageProfiles.h"
#include "sql_table_sqlite_sequence.h"
#include "sql_table_SrcCode.h"
#include "sql_table_PrgLangTraits.h"
#include "sql_table_PrgLangAlias.h"
#include "sql_table_KeyWords.h"
#include "sql_table_TestAllTypes.h"
#include "sql_view_v_LastPrgLangTraitsID.h"


/*
// This example code can be used to test and validate all tables.
// Example Usage:
// 		qlite3pp::setGlobalDB("mydatabase.db")
// 		sqlite3pp::testAllTables();
// Warning: testPopulatingAllTables and testAllTables may take a very long time on a database with a lot of content.
#include <map>
#include <memory>
namespace sqlite3pp
{
	// Function to test populating all tables & views.
	std::map< std::string, std::shared_ptr<sqlite3pp::TableBase> > testPopulatingAllTables()
	{
		std::map< std::string, std::shared_ptr < sqlite3pp::TableBase>> Tables;
		Tables["sql_table_FileExt"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_FileExt>());
		Tables["sql_table_MiscData"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_MiscData>());
		Tables["sql_table_PrgLang"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_PrgLang>());
		Tables["sql_table_SrcCodeType"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_SrcCodeType>());
		Tables["sql_table_TestAutomationTestType"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_TestAutomationTestType>());
		Tables["sql_table_ProgrammingLanguageProfiles"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_ProgrammingLanguageProfiles>());
		Tables["sql_table_AdvProfile_Settings"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_AdvProfile_Settings>());
		Tables["sql_table_AdvProfile_Variables"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_AdvProfile_Variables>());
		Tables["sql_table_InstallationPaths"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_InstallationPaths>());
		Tables["sql_table_LangInstallationUsage"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_LangInstallationUsage>());
		Tables["sql_table_Prerequisites"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_Prerequisites>());
		Tables["sql_table_TestAutomationSrcCode"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_TestAutomationSrcCode>());
		Tables["sql_view_v_ProgrammingLanguageProfiles"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_view_v_ProgrammingLanguageProfiles>());
		Tables["sql_table_sqlite_sequence"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_sqlite_sequence>());
		Tables["sql_table_SrcCode"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_SrcCode>());
		Tables["sql_table_PrgLangTraits"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_PrgLangTraits>());
		Tables["sql_table_PrgLangAlias"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_PrgLangAlias>());
		Tables["sql_table_KeyWords"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_KeyWords>());
		Tables["sql_table_TestAllTypes"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_table_TestAllTypes>());
		Tables["sql_view_v_LastPrgLangTraitsID"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<sql_view_v_LastPrgLangTraitsID>());
		return Tables;
	}

	// Function to test displaying content of all tables & views.
	void testAllTables()
	{
		std::map< std::string, std::shared_ptr < sqlite3pp::TableBase>> myTables = sqlite3pp::testPopulatingAllTables();
		for (auto t : myTables)
			t.second->out(std::wcout);
	}
}
*/



#endif // !SQL_MASTER_HEADER_H
