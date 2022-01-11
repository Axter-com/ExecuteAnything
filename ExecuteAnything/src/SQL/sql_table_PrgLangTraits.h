/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_PrgLangTraits> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_ID() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_Notes() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_PRGLANGTRAITS_H
#define SQL_TABLE_PRGLANGTRAITS_H
#include "..\sqlite3pp_ez.h"

class sql_table_PrgLangTraits: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_PrgLangTraits() {} // Default constructor
	template <class T> sql_table_PrgLangTraits(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "PrgLangTraits" ); }
	static StrType getColumnNames() { return T_( "ID, PrgLangName, CommentLineOpen, CommentOpen, CommentClose, Notes"); }
	static StrType getSelecColumnNames() { return T_( "\"ID\", \"PrgLangName\", \"CommentLineOpen\", \"CommentOpen\", \"CommentClose\", \"Notes\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( ID) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CommentLineOpen) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CommentOpen) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( CommentClose) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Notes) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> ID >> PrgLangName >> CommentLineOpen >> CommentOpen >> CommentClose >> Notes;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		ID = t.get_ID();
		PrgLangName = t.get_PrgLangName();
		CommentLineOpen = t.get_CommentLineOpen();
		CommentOpen = t.get_CommentOpen();
		CommentClose = t.get_CommentClose();
		Notes = t.get_Notes();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 6; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Integer& get_ID() const {return ID;}
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_CommentLineOpen() const {return CommentLineOpen;}
	const Text& get_CommentOpen() const {return CommentOpen;}
	const Text& get_CommentClose() const {return CommentClose;}
	const Text& get_Notes() const {return Notes;}

	// A set_ function for each field in the table.
	void set_ID(const Integer& data__) {ID = data__;}
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_CommentLineOpen(const Text& data__) {CommentLineOpen = data__;}
	void set_CommentOpen(const Text& data__) {CommentOpen = data__;}
	void set_CommentClose(const Text& data__) {CommentClose = data__;}
	void set_Notes(const Text& data__) {Notes = data__;}

	// A member variable for each field in the table
protected:
	Integer ID;
	Text PrgLangName;
	Text CommentLineOpen;
	Text CommentOpen;
	Text CommentClose;
	Text Notes;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_PrgLangTraits> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << ID << t.d << t.str(PrgLangName) << t.d << t.str(CommentLineOpen) << t.d << t.str(CommentOpen) << t.d << t.str(CommentClose) << t.d << t.str(Notes);
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_PrgLangTraits& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_PrgLangTraits& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_PrgLangTraits& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_PrgLangTraits& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_PRGLANGTRAITS_H
