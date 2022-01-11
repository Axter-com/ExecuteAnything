/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_MiscData> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_DataName() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_DataValue() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_MISCDATA_H
#define SQL_TABLE_MISCDATA_H
#include "..\sqlite3pp_ez.h"

class sql_table_MiscData: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_MiscData() {} // Default constructor
	template <class T> sql_table_MiscData(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "MiscData" ); }
	static StrType getColumnNames() { return T_( "DataName, DataValue"); }
	static StrType getSelecColumnNames() { return T_( "\"DataName\", \"DataValue\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( DataName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DataValue) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> DataName >> DataValue;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		DataName = t.get_DataName();
		DataValue = t.get_DataValue();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 2; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_DataName() const {return DataName;}
	const Clob& get_DataValue() const {return DataValue;}

	// A set_ function for each field in the table.
	void set_DataName(const Text& data__) {DataName = data__;}
	void set_DataValue(const Clob& data__) {DataValue = data__;}

	// A member variable for each field in the table
protected:
	Text DataName;
	Clob DataValue;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_MiscData> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << t.str(DataName) << t.d << DataValue;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_MiscData& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_MiscData& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_MiscData& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_MiscData& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_MISCDATA_H
