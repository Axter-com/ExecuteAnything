/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_TestAllTypes> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_INTEGER_T() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_BLOB_T() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_TESTALLTYPES_H
#define SQL_TABLE_TESTALLTYPES_H
#include "..\sqlite3pp_ez.h"

class sql_table_TestAllTypes: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_TestAllTypes() {} // Default constructor
	template <class T> sql_table_TestAllTypes(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "TestAllTypes" ); }
	static StrType getColumnNames() { return T_( "INTEGER_T, Int1, TinyInt1, SmalllInt1, MedInt1, BigInt1, UBigInt1, Int2_1, Int8_1, TEXT_T, Char1, Varchar1, Varchar2, NcharType1, NcharType2, NVarCharType1, Clob1, REAL_T, Double1, DoublePrec1, Float1, NUMERIC_T, Decimal1, Boolean1, Date1, Datetime1, BLOB_T"); }
	static StrType getSelecColumnNames() { return T_( "\"INTEGER_T\", \"Int1\", \"TinyInt1\", \"SmalllInt1\", \"MedInt1\", \"BigInt1\", \"UBigInt1\", \"Int2_1\", \"Int8_1\", \"TEXT_T\", \"Char1\", \"Varchar1\", \"Varchar2\", \"NcharType1\", \"NcharType2\", \"NVarCharType1\", \"Clob1\", \"REAL_T\", \"Double1\", \"DoublePrec1\", \"Float1\", \"NUMERIC_T\", \"Decimal1\", \"Boolean1\", \"Date1\", \"Datetime1\", \"BLOB_T\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( INTEGER_T) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Int1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( TinyInt1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SmalllInt1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( MedInt1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( BigInt1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( UBigInt1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Int2_1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Int8_1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( TEXT_T) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Char1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Varchar1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Varchar2) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NcharType1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NcharType2) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NVarCharType1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Clob1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( REAL_T) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Double1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DoublePrec1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Float1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( NUMERIC_T) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Decimal1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Boolean1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Date1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Datetime1) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( BLOB_T) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> INTEGER_T >> Int1 >> TinyInt1 >> SmalllInt1 >> MedInt1 >> BigInt1 >> UBigInt1 >> Int2_1 >> Int8_1 >> TEXT_T >> Char1 >> Varchar1 >> Varchar2 >> NcharType1 >> NcharType2 >> NVarCharType1 >> Clob1 >> REAL_T >> Double1 >> DoublePrec1 >> Float1 >> NUMERIC_T >> Decimal1 >> Boolean1 >> Date1 >> Datetime1 >> BLOB_T;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		INTEGER_T = t.get_INTEGER_T();
		Int1 = t.get_Int1();
		TinyInt1 = t.get_TinyInt1();
		SmalllInt1 = t.get_SmalllInt1();
		MedInt1 = t.get_MedInt1();
		BigInt1 = t.get_BigInt1();
		UBigInt1 = t.get_UBigInt1();
		Int2_1 = t.get_Int2_1();
		Int8_1 = t.get_Int8_1();
		TEXT_T = t.get_TEXT_T();
		Char1 = t.get_Char1();
		Varchar1 = t.get_Varchar1();
		Varchar2 = t.get_Varchar2();
		NcharType1 = t.get_NcharType1();
		NcharType2 = t.get_NcharType2();
		NVarCharType1 = t.get_NVarCharType1();
		Clob1 = t.get_Clob1();
		REAL_T = t.get_REAL_T();
		Double1 = t.get_Double1();
		DoublePrec1 = t.get_DoublePrec1();
		Float1 = t.get_Float1();
		NUMERIC_T = t.get_NUMERIC_T();
		Decimal1 = t.get_Decimal1();
		Boolean1 = t.get_Boolean1();
		Date1 = t.get_Date1();
		Datetime1 = t.get_Datetime1();
		BLOB_T = t.get_BLOB_T();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 27; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Integer& get_INTEGER_T() const {return INTEGER_T;}
	const Int& get_Int1() const {return Int1;}
	const Tinyint& get_TinyInt1() const {return TinyInt1;}
	const Smallint& get_SmalllInt1() const {return SmalllInt1;}
	const Mediumint& get_MedInt1() const {return MedInt1;}
	const Bigint& get_BigInt1() const {return BigInt1;}
	const UBigint& get_UBigInt1() const {return UBigInt1;}
	const Int2& get_Int2_1() const {return Int2_1;}
	const Int8& get_Int8_1() const {return Int8_1;}
	const Text& get_TEXT_T() const {return TEXT_T;}
	const Character& get_Char1() const {return Char1;}
	const Varchar& get_Varchar1() const {return Varchar1;}
	const Varchar& get_Varchar2() const {return Varchar2;}
	const Nchar& get_NcharType1() const {return NcharType1;}
	const Nchar& get_NcharType2() const {return NcharType2;}
	const Nvarchar& get_NVarCharType1() const {return NVarCharType1;}
	const Clob& get_Clob1() const {return Clob1;}
	const Real& get_REAL_T() const {return REAL_T;}
	const Double& get_Double1() const {return Double1;}
	const DoublePrcsn& get_DoublePrec1() const {return DoublePrec1;}
	const Float& get_Float1() const {return Float1;}
	const Numeric& get_NUMERIC_T() const {return NUMERIC_T;}
	const Decimal& get_Decimal1() const {return Decimal1;}
	const Boolean& get_Boolean1() const {return Boolean1;}
	const Date& get_Date1() const {return Date1;}
	const Datetime& get_Datetime1() const {return Datetime1;}
	const Blob& get_BLOB_T() const {return BLOB_T;}

	// A set_ function for each field in the table.
	void set_INTEGER_T(const Integer& data__) {INTEGER_T = data__;}
	void set_Int1(const Int& data__) {Int1 = data__;}
	void set_TinyInt1(const Tinyint& data__) {TinyInt1 = data__;}
	void set_SmalllInt1(const Smallint& data__) {SmalllInt1 = data__;}
	void set_MedInt1(const Mediumint& data__) {MedInt1 = data__;}
	void set_BigInt1(const Bigint& data__) {BigInt1 = data__;}
	void set_UBigInt1(const UBigint& data__) {UBigInt1 = data__;}
	void set_Int2_1(const Int2& data__) {Int2_1 = data__;}
	void set_Int8_1(const Int8& data__) {Int8_1 = data__;}
	void set_TEXT_T(const Text& data__) {TEXT_T = data__;}
	void set_Char1(const Character& data__) {Char1 = data__;}
	void set_Varchar1(const Varchar& data__) {Varchar1 = data__;}
	void set_Varchar2(const Varchar& data__) {Varchar2 = data__;}
	void set_NcharType1(const Nchar& data__) {NcharType1 = data__;}
	void set_NcharType2(const Nchar& data__) {NcharType2 = data__;}
	void set_NVarCharType1(const Nvarchar& data__) {NVarCharType1 = data__;}
	void set_Clob1(const Clob& data__) {Clob1 = data__;}
	void set_REAL_T(const Real& data__) {REAL_T = data__;}
	void set_Double1(const Double& data__) {Double1 = data__;}
	void set_DoublePrec1(const DoublePrcsn& data__) {DoublePrec1 = data__;}
	void set_Float1(const Float& data__) {Float1 = data__;}
	void set_NUMERIC_T(const Numeric& data__) {NUMERIC_T = data__;}
	void set_Decimal1(const Decimal& data__) {Decimal1 = data__;}
	void set_Boolean1(const Boolean& data__) {Boolean1 = data__;}
	void set_Date1(const Date& data__) {Date1 = data__;}
	void set_Datetime1(const Datetime& data__) {Datetime1 = data__;}
	void set_BLOB_T(const Blob& data__) {BLOB_T = data__;}

	// A member variable for each field in the table
protected:
	Integer INTEGER_T;
	Int Int1;
	Tinyint TinyInt1;
	Smallint SmalllInt1;
	Mediumint MedInt1;
	Bigint BigInt1;
	UBigint UBigInt1;
	Int2 Int2_1;
	Int8 Int8_1;
	Text TEXT_T;
	Character Char1;
	Varchar Varchar1;
	Varchar Varchar2;
	Nchar NcharType1;
	Nchar NcharType2;
	Nvarchar NVarCharType1;
	Clob Clob1;
	Real REAL_T;
	Double Double1;
	DoublePrcsn DoublePrec1;
	Float Float1;
	Numeric NUMERIC_T;
	Decimal Decimal1;
	Boolean Boolean1;
	Date Date1;
	Datetime Datetime1;
	Blob BLOB_T;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_TestAllTypes> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << INTEGER_T << t.d << Int1 << t.d << TinyInt1 << t.d << SmalllInt1 << t.d << MedInt1 << t.d << BigInt1 << t.d << UBigInt1 << t.d << Int2_1 << t.d << Int8_1 << t.d << t.str(TEXT_T) << t.d << t.str(Char1) << t.d << t.str(Varchar1) << t.d << t.str(Varchar2) << t.d << t.str(NcharType1) << t.d << t.str(NcharType2) << t.d << t.str(NVarCharType1) << t.d << Clob1 << t.d << REAL_T << t.d << Double1 << t.d << DoublePrec1 << t.d << Float1 << t.d << NUMERIC_T << t.d << Decimal1 << t.d << Boolean1 << t.d << Date1 << t.d << Datetime1 << t.d << BLOB_T;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_TestAllTypes& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_TestAllTypes& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_TestAllTypes& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_TestAllTypes& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_TESTALLTYPES_H
