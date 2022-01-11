/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_PrgLangAlias> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_PrgLangTraitsID() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_Notes() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_PRGLANGALIAS_H
#define SQL_TABLE_PRGLANGALIAS_H
#include "..\sqlite3pp_ez.h"

class sql_table_PrgLangAlias: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_PrgLangAlias() {} // Default constructor
	template <class T> sql_table_PrgLangAlias(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "PrgLangAlias" ); }
	static StrType getColumnNames() { return T_( "PrgLangTraitsID, Alias, Notes"); }
	static StrType getSelecColumnNames() { return T_( "\"PrgLangTraitsID\", \"Alias\", \"Notes\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( PrgLangTraitsID) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Alias) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Notes) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> PrgLangTraitsID >> Alias >> Notes;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		PrgLangTraitsID = t.get_PrgLangTraitsID();
		Alias = t.get_Alias();
		Notes = t.get_Notes();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 3; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Integer& get_PrgLangTraitsID() const {return PrgLangTraitsID;}
	const Text& get_Alias() const {return Alias;}
	const Text& get_Notes() const {return Notes;}

	// A set_ function for each field in the table.
	void set_PrgLangTraitsID(const Integer& data__) {PrgLangTraitsID = data__;}
	void set_Alias(const Text& data__) {Alias = data__;}
	void set_Notes(const Text& data__) {Notes = data__;}

	// A member variable for each field in the table
protected:
	Integer PrgLangTraitsID;
	Text Alias;
	Text Notes;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_PrgLangAlias> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << PrgLangTraitsID << t.d << t.str(Alias) << t.d << t.str(Notes);
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_PrgLangAlias& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_PrgLangAlias& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_PrgLangAlias& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_PrgLangAlias& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_PRGLANGALIAS_H
