/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_AdvProfile_Variables> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_PrgLangName() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_Notes() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_ADVPROFILE_VARIABLES_H
#define SQL_TABLE_ADVPROFILE_VARIABLES_H
#include "..\sqlite3pp_ez.h"

class sql_table_AdvProfile_Variables: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_AdvProfile_Variables() {} // Default constructor
	template <class T> sql_table_AdvProfile_Variables(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "AdvProfile_Variables" ); }
	static StrType getColumnNames() { return T_( "PrgLangName, ProfileName, VarName, VarValue, SetVar, SetEnvVar, AddToPath, AddVarValueToEnvVar, AddVarNameToEnvVar, SetLocalVar, GetValueFromSrcFile, Dir, File, FileArg, ProcessOrder, Notes"); }
	static StrType getSelecColumnNames() { return T_( "\"PrgLangName\", \"ProfileName\", \"VarName\", \"VarValue\", \"SetVar\", \"SetEnvVar\", \"AddToPath\", \"AddVarValueToEnvVar\", \"AddVarNameToEnvVar\", \"SetLocalVar\", \"GetValueFromSrcFile\", \"Dir\", \"File\", \"FileArg\", \"ProcessOrder\", \"Notes\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProfileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( VarName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( VarValue) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SetVar) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SetEnvVar) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( AddToPath) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( AddVarValueToEnvVar) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( AddVarNameToEnvVar) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SetLocalVar) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( GetValueFromSrcFile) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Dir) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( File) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( FileArg) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProcessOrder) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Notes) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> PrgLangName >> ProfileName >> VarName >> VarValue >> SetVar >> SetEnvVar >> AddToPath >> AddVarValueToEnvVar >> AddVarNameToEnvVar >> SetLocalVar >> GetValueFromSrcFile >> Dir >> File >> FileArg >> ProcessOrder >> Notes;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		PrgLangName = t.get_PrgLangName();
		ProfileName = t.get_ProfileName();
		VarName = t.get_VarName();
		VarValue = t.get_VarValue();
		SetVar = t.get_SetVar();
		SetEnvVar = t.get_SetEnvVar();
		AddToPath = t.get_AddToPath();
		AddVarValueToEnvVar = t.get_AddVarValueToEnvVar();
		AddVarNameToEnvVar = t.get_AddVarNameToEnvVar();
		SetLocalVar = t.get_SetLocalVar();
		GetValueFromSrcFile = t.get_GetValueFromSrcFile();
		Dir = t.get_Dir();
		File = t.get_File();
		FileArg = t.get_FileArg();
		ProcessOrder = t.get_ProcessOrder();
		Notes = t.get_Notes();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 16; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_ProfileName() const {return ProfileName;}
	const Text& get_VarName() const {return VarName;}
	const Text& get_VarValue() const {return VarValue;}
	const Boolean& get_SetVar() const {return SetVar;}
	const Boolean& get_SetEnvVar() const {return SetEnvVar;}
	const Boolean& get_AddToPath() const {return AddToPath;}
	const Text& get_AddVarValueToEnvVar() const {return AddVarValueToEnvVar;}
	const Text& get_AddVarNameToEnvVar() const {return AddVarNameToEnvVar;}
	const Boolean& get_SetLocalVar() const {return SetLocalVar;}
	const Boolean& get_GetValueFromSrcFile() const {return GetValueFromSrcFile;}
	const Integer& get_Dir() const {return Dir;}
	const Integer& get_File() const {return File;}
	const Text& get_FileArg() const {return FileArg;}
	const Integer& get_ProcessOrder() const {return ProcessOrder;}
	const Text& get_Notes() const {return Notes;}

	// A set_ function for each field in the table.
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_ProfileName(const Text& data__) {ProfileName = data__;}
	void set_VarName(const Text& data__) {VarName = data__;}
	void set_VarValue(const Text& data__) {VarValue = data__;}
	void set_SetVar(const Boolean& data__) {SetVar = data__;}
	void set_SetEnvVar(const Boolean& data__) {SetEnvVar = data__;}
	void set_AddToPath(const Boolean& data__) {AddToPath = data__;}
	void set_AddVarValueToEnvVar(const Text& data__) {AddVarValueToEnvVar = data__;}
	void set_AddVarNameToEnvVar(const Text& data__) {AddVarNameToEnvVar = data__;}
	void set_SetLocalVar(const Boolean& data__) {SetLocalVar = data__;}
	void set_GetValueFromSrcFile(const Boolean& data__) {GetValueFromSrcFile = data__;}
	void set_Dir(const Integer& data__) {Dir = data__;}
	void set_File(const Integer& data__) {File = data__;}
	void set_FileArg(const Text& data__) {FileArg = data__;}
	void set_ProcessOrder(const Integer& data__) {ProcessOrder = data__;}
	void set_Notes(const Text& data__) {Notes = data__;}

	// A member variable for each field in the table
protected:
	Text PrgLangName;
	Text ProfileName;
	Text VarName;
	Text VarValue;
	Boolean SetVar;
	Boolean SetEnvVar;
	Boolean AddToPath;
	Text AddVarValueToEnvVar;
	Text AddVarNameToEnvVar;
	Boolean SetLocalVar;
	Boolean GetValueFromSrcFile;
	Integer Dir;
	Integer File;
	Text FileArg;
	Integer ProcessOrder;
	Text Notes;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_AdvProfile_Variables> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << t.str(PrgLangName) << t.d << t.str(ProfileName) << t.d << t.str(VarName) << t.d << t.str(VarValue) << t.d << SetVar << t.d << SetEnvVar << t.d << AddToPath << t.d << t.str(AddVarValueToEnvVar) << t.d << t.str(AddVarNameToEnvVar) << t.d << SetLocalVar << t.d << GetValueFromSrcFile << t.d << Dir << t.d << File << t.d << t.str(FileArg) << t.d << ProcessOrder << t.d << t.str(Notes);
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_AdvProfile_Variables& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_AdvProfile_Variables& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_AdvProfile_Variables& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_AdvProfile_Variables& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_ADVPROFILE_VARIABLES_H
