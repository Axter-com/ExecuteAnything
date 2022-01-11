/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_view_v_ProgrammingLanguageProfiles> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_Ext() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_isCompiler() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_VIEW_V_PROGRAMMINGLANGUAGEPROFILES_H
#define SQL_VIEW_V_PROGRAMMINGLANGUAGEPROFILES_H
#include "..\sqlite3pp_ez.h"

class sql_view_v_ProgrammingLanguageProfiles: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_view_v_ProgrammingLanguageProfiles() {} // Default constructor
	template <class T> sql_view_v_ProgrammingLanguageProfiles(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "v_ProgrammingLanguageProfiles" ); }
	static StrType getColumnNames() { return T_( "Ext, PrgLangName, ProfileName, isScript, isCompiler"); }
	static StrType getSelecColumnNames() { return T_( "\"Ext\", \"PrgLangName\", \"ProfileName\", \"isScript\", \"isCompiler\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( Ext) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProfileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( isScript) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( isCompiler) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> Ext >> PrgLangName >> ProfileName >> isScript >> isCompiler;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		Ext = t.get_Ext();
		PrgLangName = t.get_PrgLangName();
		ProfileName = t.get_ProfileName();
		isScript = t.get_isScript();
		isCompiler = t.get_isCompiler();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 5; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_Ext() const {return Ext;}
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_ProfileName() const {return ProfileName;}
	const Text& get_isScript() const {return isScript;}
	const Text& get_isCompiler() const {return isCompiler;}

	// A set_ function for each field in the table.
	void set_Ext(const Text& data__) {Ext = data__;}
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_ProfileName(const Text& data__) {ProfileName = data__;}
	void set_isScript(const Text& data__) {isScript = data__;}
	void set_isCompiler(const Text& data__) {isCompiler = data__;}

	// A member variable for each field in the table
protected:
	Text Ext;
	Text PrgLangName;
	Text ProfileName;
	Text isScript;
	Text isCompiler;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_view_v_ProgrammingLanguageProfiles> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << t.str(Ext) << t.d << t.str(PrgLangName) << t.d << t.str(ProfileName) << t.d << t.str(isScript) << t.d << t.str(isCompiler);
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_view_v_ProgrammingLanguageProfiles& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_view_v_ProgrammingLanguageProfiles& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_view_v_ProgrammingLanguageProfiles& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_view_v_ProgrammingLanguageProfiles& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_VIEW_V_PROGRAMMINGLANGUAGEPROFILES_H
