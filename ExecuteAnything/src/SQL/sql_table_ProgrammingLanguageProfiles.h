/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_ProgrammingLanguageProfiles> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_PrgLangName() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_Uses_UNIX_PATH() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_PROGRAMMINGLANGUAGEPROFILES_H
#define SQL_TABLE_PROGRAMMINGLANGUAGEPROFILES_H
#include "..\sqlite3pp_ez.h"

class sql_table_ProgrammingLanguageProfiles: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_ProgrammingLanguageProfiles() {} // Default constructor
	template <class T> sql_table_ProgrammingLanguageProfiles(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "ProgrammingLanguageProfiles" ); }
	static StrType getColumnNames() { return T_( "PrgLangName, ProfileName, Launcher, LauncherCmdFormat, Compiler, CompilerCmdFormat, PostBuildCmdFormat, PostBuildCmdBin, BinaryToCompile, BinaryToExecute, FilesToDeleteBeforeBuild, ClearSysEnvPath, Uses_UNIX_PATH"); }
	static StrType getSelecColumnNames() { return T_( "\"PrgLangName\", \"ProfileName\", \"Launcher\", \"LauncherCmdFormat\", \"Compiler\", \"CompilerCmdFormat\", \"PostBuildCmdFormat\", \"PostBuildCmdBin\", \"BinaryToCompile\", \"BinaryToExecute\", \"FilesToDeleteBeforeBuild\", \"ClearSysEnvPath\", \"Uses_UNIX_PATH\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProfileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Launcher) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( LauncherCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Compiler) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CompilerCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PostBuildCmdFormat) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PostBuildCmdBin) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( BinaryToCompile) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( BinaryToExecute) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( FilesToDeleteBeforeBuild) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ClearSysEnvPath) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Uses_UNIX_PATH) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> PrgLangName >> ProfileName >> Launcher >> LauncherCmdFormat >> Compiler >> CompilerCmdFormat >> PostBuildCmdFormat >> PostBuildCmdBin >> BinaryToCompile >> BinaryToExecute >> FilesToDeleteBeforeBuild >> ClearSysEnvPath >> Uses_UNIX_PATH;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		PrgLangName = t.get_PrgLangName();
		ProfileName = t.get_ProfileName();
		Launcher = t.get_Launcher();
		LauncherCmdFormat = t.get_LauncherCmdFormat();
		Compiler = t.get_Compiler();
		CompilerCmdFormat = t.get_CompilerCmdFormat();
		PostBuildCmdFormat = t.get_PostBuildCmdFormat();
		PostBuildCmdBin = t.get_PostBuildCmdBin();
		BinaryToCompile = t.get_BinaryToCompile();
		BinaryToExecute = t.get_BinaryToExecute();
		FilesToDeleteBeforeBuild = t.get_FilesToDeleteBeforeBuild();
		ClearSysEnvPath = t.get_ClearSysEnvPath();
		Uses_UNIX_PATH = t.get_Uses_UNIX_PATH();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 13; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_ProfileName() const {return ProfileName;}
	const Text& get_Launcher() const {return Launcher;}
	const Text& get_LauncherCmdFormat() const {return LauncherCmdFormat;}
	const Text& get_Compiler() const {return Compiler;}
	const Text& get_CompilerCmdFormat() const {return CompilerCmdFormat;}
	const Text& get_PostBuildCmdFormat() const {return PostBuildCmdFormat;}
	const Text& get_PostBuildCmdBin() const {return PostBuildCmdBin;}
	const Text& get_BinaryToCompile() const {return BinaryToCompile;}
	const Text& get_BinaryToExecute() const {return BinaryToExecute;}
	const Text& get_FilesToDeleteBeforeBuild() const {return FilesToDeleteBeforeBuild;}
	const Boolean& get_ClearSysEnvPath() const {return ClearSysEnvPath;}
	const Boolean& get_Uses_UNIX_PATH() const {return Uses_UNIX_PATH;}

	// A set_ function for each field in the table.
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_ProfileName(const Text& data__) {ProfileName = data__;}
	void set_Launcher(const Text& data__) {Launcher = data__;}
	void set_LauncherCmdFormat(const Text& data__) {LauncherCmdFormat = data__;}
	void set_Compiler(const Text& data__) {Compiler = data__;}
	void set_CompilerCmdFormat(const Text& data__) {CompilerCmdFormat = data__;}
	void set_PostBuildCmdFormat(const Text& data__) {PostBuildCmdFormat = data__;}
	void set_PostBuildCmdBin(const Text& data__) {PostBuildCmdBin = data__;}
	void set_BinaryToCompile(const Text& data__) {BinaryToCompile = data__;}
	void set_BinaryToExecute(const Text& data__) {BinaryToExecute = data__;}
	void set_FilesToDeleteBeforeBuild(const Text& data__) {FilesToDeleteBeforeBuild = data__;}
	void set_ClearSysEnvPath(const Boolean& data__) {ClearSysEnvPath = data__;}
	void set_Uses_UNIX_PATH(const Boolean& data__) {Uses_UNIX_PATH = data__;}

	// A member variable for each field in the table
protected:
	Text PrgLangName;
	Text ProfileName;
	Text Launcher;
	Text LauncherCmdFormat;
	Text Compiler;
	Text CompilerCmdFormat;
	Text PostBuildCmdFormat;
	Text PostBuildCmdBin;
	Text BinaryToCompile;
	Text BinaryToExecute;
	Text FilesToDeleteBeforeBuild;
	Boolean ClearSysEnvPath;
	Boolean Uses_UNIX_PATH;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_ProgrammingLanguageProfiles> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << t.str(PrgLangName) << t.d << t.str(ProfileName) << t.d << t.str(Launcher) << t.d << t.str(LauncherCmdFormat) << t.d << t.str(Compiler) << t.d << t.str(CompilerCmdFormat) << t.d << t.str(PostBuildCmdFormat) << t.d << t.str(PostBuildCmdBin) << t.d << t.str(BinaryToCompile) << t.d << t.str(BinaryToExecute) << t.d << t.str(FilesToDeleteBeforeBuild) << t.d << ClearSysEnvPath << t.d << Uses_UNIX_PATH;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_ProgrammingLanguageProfiles& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_ProgrammingLanguageProfiles& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_ProgrammingLanguageProfiles& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_ProgrammingLanguageProfiles& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_PROGRAMMINGLANGUAGEPROFILES_H
