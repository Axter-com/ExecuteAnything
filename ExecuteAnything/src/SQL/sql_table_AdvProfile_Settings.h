/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_AdvProfile_Settings> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_PrgLangName() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_NpeExecuteType() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_ADVPROFILE_SETTINGS_H
#define SQL_TABLE_ADVPROFILE_SETTINGS_H
#include "..\sqlite3pp_ez.h"

class sql_table_AdvProfile_Settings: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_AdvProfile_Settings() {} // Default constructor
	template <class T> sql_table_AdvProfile_Settings(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "AdvProfile_Settings" ); }
	static StrType getColumnNames() { return T_( "PrgLangName, ProfileName, Notes, PreBuildCmdBin, PreBuildCmdFormat, RebuildCmdBin, RebuildCmdFormat, CleanCmdBin, CleanCmdFormat, DebuggerCmdBin, DebuggerCmdFormat, SourceBasePath, NppExecConsoleLogging, EnableConsoleLogging, ChangeFocusToConsleBeforeExecute, DoNotSaveDocBeforeExecute, NpeExecuteType"); }
	static StrType getSelecColumnNames() { return T_( "\"PrgLangName\", \"ProfileName\", \"Notes\", \"PreBuildCmdBin\", \"PreBuildCmdFormat\", \"RebuildCmdBin\", \"RebuildCmdFormat\", \"CleanCmdBin\", \"CleanCmdFormat\", \"DebuggerCmdBin\", \"DebuggerCmdFormat\", \"SourceBasePath\", \"NppExecConsoleLogging\", \"EnableConsoleLogging\", \"ChangeFocusToConsleBeforeExecute\", \"DoNotSaveDocBeforeExecute\", \"NpeExecuteType\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProfileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Notes) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PreBuildCmdBin) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PreBuildCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( RebuildCmdBin) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( RebuildCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CleanCmdBin) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CleanCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DebuggerCmdBin) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DebuggerCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SourceBasePath) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NppExecConsoleLogging) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( EnableConsoleLogging) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ChangeFocusToConsleBeforeExecute) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DoNotSaveDocBeforeExecute) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NpeExecuteType) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> PrgLangName >> ProfileName >> Notes >> PreBuildCmdBin >> PreBuildCmdFormat >> RebuildCmdBin >> RebuildCmdFormat >> CleanCmdBin >> CleanCmdFormat >> DebuggerCmdBin >> DebuggerCmdFormat >> SourceBasePath >> NppExecConsoleLogging >> EnableConsoleLogging >> ChangeFocusToConsleBeforeExecute >> DoNotSaveDocBeforeExecute >> NpeExecuteType;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		PrgLangName = t.get_PrgLangName();
		ProfileName = t.get_ProfileName();
		Notes = t.get_Notes();
		PreBuildCmdBin = t.get_PreBuildCmdBin();
		PreBuildCmdFormat = t.get_PreBuildCmdFormat();
		RebuildCmdBin = t.get_RebuildCmdBin();
		RebuildCmdFormat = t.get_RebuildCmdFormat();
		CleanCmdBin = t.get_CleanCmdBin();
		CleanCmdFormat = t.get_CleanCmdFormat();
		DebuggerCmdBin = t.get_DebuggerCmdBin();
		DebuggerCmdFormat = t.get_DebuggerCmdFormat();
		SourceBasePath = t.get_SourceBasePath();
		NppExecConsoleLogging = t.get_NppExecConsoleLogging();
		EnableConsoleLogging = t.get_EnableConsoleLogging();
		ChangeFocusToConsleBeforeExecute = t.get_ChangeFocusToConsleBeforeExecute();
		DoNotSaveDocBeforeExecute = t.get_DoNotSaveDocBeforeExecute();
		NpeExecuteType = t.get_NpeExecuteType();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 17; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_ProfileName() const {return ProfileName;}
	const Text& get_Notes() const {return Notes;}
	const Text& get_PreBuildCmdBin() const {return PreBuildCmdBin;}
	const Text& get_PreBuildCmdFormat() const {return PreBuildCmdFormat;}
	const Text& get_RebuildCmdBin() const {return RebuildCmdBin;}
	const Text& get_RebuildCmdFormat() const {return RebuildCmdFormat;}
	const Text& get_CleanCmdBin() const {return CleanCmdBin;}
	const Text& get_CleanCmdFormat() const {return CleanCmdFormat;}
	const Text& get_DebuggerCmdBin() const {return DebuggerCmdBin;}
	const Text& get_DebuggerCmdFormat() const {return DebuggerCmdFormat;}
	const Text& get_SourceBasePath() const {return SourceBasePath;}
	const Boolean& get_NppExecConsoleLogging() const {return NppExecConsoleLogging;}
	const Boolean& get_EnableConsoleLogging() const {return EnableConsoleLogging;}
	const Boolean& get_ChangeFocusToConsleBeforeExecute() const {return ChangeFocusToConsleBeforeExecute;}
	const Boolean& get_DoNotSaveDocBeforeExecute() const {return DoNotSaveDocBeforeExecute;}
	const Integer& get_NpeExecuteType() const {return NpeExecuteType;}

	// A set_ function for each field in the table.
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_ProfileName(const Text& data__) {ProfileName = data__;}
	void set_Notes(const Text& data__) {Notes = data__;}
	void set_PreBuildCmdBin(const Text& data__) {PreBuildCmdBin = data__;}
	void set_PreBuildCmdFormat(const Text& data__) {PreBuildCmdFormat = data__;}
	void set_RebuildCmdBin(const Text& data__) {RebuildCmdBin = data__;}
	void set_RebuildCmdFormat(const Text& data__) {RebuildCmdFormat = data__;}
	void set_CleanCmdBin(const Text& data__) {CleanCmdBin = data__;}
	void set_CleanCmdFormat(const Text& data__) {CleanCmdFormat = data__;}
	void set_DebuggerCmdBin(const Text& data__) {DebuggerCmdBin = data__;}
	void set_DebuggerCmdFormat(const Text& data__) {DebuggerCmdFormat = data__;}
	void set_SourceBasePath(const Text& data__) {SourceBasePath = data__;}
	void set_NppExecConsoleLogging(const Boolean& data__) {NppExecConsoleLogging = data__;}
	void set_EnableConsoleLogging(const Boolean& data__) {EnableConsoleLogging = data__;}
	void set_ChangeFocusToConsleBeforeExecute(const Boolean& data__) {ChangeFocusToConsleBeforeExecute = data__;}
	void set_DoNotSaveDocBeforeExecute(const Boolean& data__) {DoNotSaveDocBeforeExecute = data__;}
	void set_NpeExecuteType(const Integer& data__) {NpeExecuteType = data__;}

	// A member variable for each field in the table
protected:
	Text PrgLangName;
	Text ProfileName;
	Text Notes;
	Text PreBuildCmdBin;
	Text PreBuildCmdFormat;
	Text RebuildCmdBin;
	Text RebuildCmdFormat;
	Text CleanCmdBin;
	Text CleanCmdFormat;
	Text DebuggerCmdBin;
	Text DebuggerCmdFormat;
	Text SourceBasePath;
	Boolean NppExecConsoleLogging;
	Boolean EnableConsoleLogging;
	Boolean ChangeFocusToConsleBeforeExecute;
	Boolean DoNotSaveDocBeforeExecute;
	Integer NpeExecuteType;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_AdvProfile_Settings> tbl(DbFileNameArg("mydatabase.db"));
			tbl.setDelimit("|"); // Change delimiter
			std::cout << tbl; // Send data to screen with the changed delimiter

			std::ofstream ofs ("data.csv", std::ofstream::out);
			tbl.setDelimit(","); // Change delimiter
			ofs << tbl; // Write data to a CSV file using the changed "," delimiter.

			tbl.out(std::cout); // Send data to screen using out() member function.
	To exclude TableOStream interface, set exclude_ostream_operator to true when creating this class using SQLiteClassBuilder.
	*/
	template<class T> T& OStream(T& t) const
	{
		t.os << t.str(PrgLangName) << t.d << t.str(ProfileName) << t.d << t.str(Notes) << t.d << t.str(PreBuildCmdBin) << t.d << t.str(PreBuildCmdFormat) << t.d << t.str(RebuildCmdBin) << t.d << t.str(RebuildCmdFormat) << t.d << t.str(CleanCmdBin) << t.d << t.str(CleanCmdFormat) << t.d << t.str(DebuggerCmdBin) << t.d << t.str(DebuggerCmdFormat) << t.d << t.str(SourceBasePath) << t.d << NppExecConsoleLogging << t.d << EnableConsoleLogging << t.d << ChangeFocusToConsleBeforeExecute << t.d << DoNotSaveDocBeforeExecute << t.d << NpeExecuteType;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_AdvProfile_Settings& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_AdvProfile_Settings& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_AdvProfile_Settings& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_AdvProfile_Settings& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_ADVPROFILE_SETTINGS_H
