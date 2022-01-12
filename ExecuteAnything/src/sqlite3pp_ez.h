/*	sqlite3pp_ez.h
	GNU General Public License
	
	Copyright (C) 2021 David Maisonave (www.axter.com)
	The sqlite3pp_ez source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	# Summary
	This repository is a fork of sqlite3pp, and it includes additional code to add UNICODE support, a template Table class and a SQLite class builder.
	The sqlite3pp::Table class along with the sqlite3pp::SQLiteClassBuilder class allows C++ developers to use type safe variables assocaited with the table column types.

	This package contains all the files required to use SQLite3, SQLite3pp, and SQLite3pp_EZ.Only minor modifications have been made to SQLite3 C code and SQLite3pp where needed for UNICODE support.Then bulk of the sqlite3pp_EZ implementation is in sqlite3pp_EZ.h and sqlite3pp_EZ.cpp.

	For usage examples see  https://github.com/David-Maisonave/sqlite3pp_EZ
							or sqlite3pp_ez.h
*/

#ifndef SQLITE3PP_EZ_H
#define SQLITE3PP_EZ_H
#include "sqlite3pp.h"
#include <vector>
#include <string>
#include <iostream>
#include <regex>

// For portability and to avoid conflicts, using T_() macro instead of windows _T().
#ifdef _UNICODE
#define T_(x)      L ## x
#else
#define T_(x)      x
#endif // _UNICODE


namespace sqlite3pp
{
	enum VerbosityLevels {
		VerbosityLevels_NO_OUTPUT = 0
		, VBLV_ERROR = 1	// Prints out unexpected behavior which will cause program to fail
		, VBLV_WARN = 2		// Prints out unexpected behavior, but program can still continue.
		, VBLV_INFO = 3		// Prints out process information
		, VBLV_DEBUG = 4	// Prints out information mainly useful for debugging
		, VBLV_DETAIL = 5	// Prints out expected behavior
	};

	enum ActionIfDatabaseOpen{
		AIO_SkipIfSameFile, 
		AIO_CloseBeforeOpen, 
		AIO_IgnoreOpenRequest, 
		AIO_Throw
	};

	class sql_base
	{
	public:
		// SQLite3 types (Excluding string types)
		using Integer = int;
		using Int = int;
		using Int2 = int;
		using Int8 = int;
		using Tinyint = unsigned char;
		using Smallint = short int;
		using Mediumint = int;
		using Boolean = bool;
		using Bigint = long long int;
		using UBigint = unsigned long long int;
		using Numeric = double;
		using Decimal = double;
		using Real = double;
		using DoublePrcsn = double;
		using Double = double;
		using Float = double;
		using Blob = sqlite3pp::Blob;// Stores binary data
		using Clob = sqlite3pp::Clob;	// Stores strings that can have multiple NULL terminators
		using Date = sqlite3pp::Date;
		using Datetime = sqlite3pp::Datetime;
		using Nchar = std::wstring;
		using Nvarchar = std::wstring;
		using Character = std::string;
		using Varchar = std::string;


		friend database& setGlobalDB( const std::string& db_filename, ActionIfDatabaseOpen actionifopen);
		friend database& setGlobalDB( const std::wstring& db_filename, ActionIfDatabaseOpen actionifopen);
		friend database& getGlobalDB();
		friend int Execute( const std::string& sql );
		friend int Execute( const std::wstring& sql );
		friend int Connect(const char* db_filename, int flags, const char* vfs );
		friend int Connect(const wchar_t* db_filename, int flags, const wchar_t* vfs );
		friend int Attach( const char* db_filename, const char* name );
		friend int Attach( const  wchar_t* db_filename, const  wchar_t* name );
		friend int Detach();
		friend int Backup( const char* db_filename, database& destdb, const char* destdbname, database::backup_handler h, int step_page);
		friend int Backup( const  wchar_t* db_filename, database& destdb, const  wchar_t* destdbname, database::backup_handler h, int step_page );
		friend std::string GetDbErrMsg();
		friend std::wstring GetDbErrMsgW();
		friend int GetDbErrNo();
		friend int GetDbExtErrNo();
		static sqlite3pp::query* CreateQuery(database& db, const std::string& QueryStr);
		static const char TableArg_PreExecuteArg[];
		static const char TableArg_WhereClauseArg[];
		static const char TableArg_DbFileNameArg[];
		static void SetVerbosityLevel(VerbosityLevels v);
		static VerbosityLevels GetVerbosityLevel();

	protected:
		static sqlite3pp::database global_db; // To be used as global DB
		static tstring filenameOfOpenDB; // The file name of the DB currently opened by global_db
		static bool bIsGlblDbOpen; // To be used as global DB
		static int last_rc;
		static const char TableArg_ValueArg[];
		static VerbosityLevels m_VerbosityLevels; // Default value VBLV_ERROR
	private:
		static void set(tstring db_filename);
		static void unset();
	};
	

	std::string to_string(const std::wstring &src);
	std::string to_string(const std::string &src);// For template usage
	std::wstring to_wstring(const std::string &src);
	std::wstring to_wstring(const std::wstring &src);// For template usage
	std::string to_string(const wchar_t* src);
	std::wstring to_wstring(const char* src);
	std::string to_string(const Clob& src);
	std::wstring to_wstring(const Clob& src);
	std::string to_string(const Blob& src);
	std::wstring to_wstring(const Blob& src);
	std::string to_string(int src);
	std::wstring to_wstring(int src);
	std::wstring to_wstring(long long int src);
	std::string to_string(long long int src);
	std::wstring to_wstring(unsigned long long int src);
	std::string to_string(unsigned long long int src);
	std::string to_string(double src);
	std::wstring to_wstring(double src);
	std::string to_string(const Date& src);
	std::wstring to_wstring(const Date& src);
	std::string to_string(const Datetime& src);
	std::wstring to_wstring(const Datetime& src);
#ifdef _UNICODE
#define to_tstring to_wstring
	typedef std::basic_regex<wchar_t> tregex;
	typedef std::basic_ofstream<wchar_t, std::char_traits<wchar_t> > tofstream;
#else
#define to_tstring to_string
	typedef std::basic_regex<char> tregex;
	typedef std::basic_ofstream<char, std::char_traits<char> > tofstream;
#endif // _UNICODE

	typedef std::match_results<tstring::const_iterator> tsmatch;
	std::string GetUpdatedSrcPath(std::string &src, int EnvVarToFetch = 3, const std::string VarNamePrefix = "%", const std::string VarNamePostfix = "%");// EnvVarToFetch BIT Settings: 1=Get User Var, 2=Get System Paths, 4=Get Misc Var
	std::string Get_UpdatedPathCopy(std::string src, int EnvVarToFetch = 3, const std::string VarNamePrefix = "%", const std::string VarNamePostfix = "%");
	std::wstring GetUpdatedSrcPath(std::wstring &src, int EnvVarToFetch = 3, const std::string VarNamePrefix = "%", const std::string VarNamePostfix = "%");
	std::wstring Get_UpdatedPathCopy(std::wstring src, int EnvVarToFetch = 3, const std::string VarNamePrefix = "%", const std::string VarNamePostfix = "%");


	////////////////////////////////////////////////////////////////////////////////////////////
	// ostream_w and ostream_a are used to help out stream different charater types.
	struct ostream_w
	{
		std::wostream &os;
		const std::wstring d; // Delimiter
		ostream_w(std::wostream &o, const std::string& sep) :os(o), d(to_wstring(sep.c_str())) {}
		ostream_w(std::wostream &o, const std::wstring& sep) :os(o), d(sep) {}
		std::wstring str(const std::string& str) const { return to_wstring(str.c_str()); }
		inline std::wstring str(const std::wstring& str) const { return str; }
	};
	struct ostream_a
	{
		std::ostream &os;
		const std::string d; // Delimiter
		ostream_a(std::ostream &o, const std::string& sep) :os(o), d(sep) {}
		ostream_a(std::ostream &o, const std::wstring& sep) :os(o), d(to_string(sep.c_str())) {}
		inline std::string str(const std::string& str) const { return str; }
		std::string str(const std::wstring& str) const { return to_string(str.c_str()); }
	};

	template <char const *TypeName, class T_STR = sqlite3pp::tstring>
	class TableArg
	{
		const T_STR m_Str;
	public:
		TableArg(const T_STR& Str = T_STR()) :m_Str(Str) {}
		const T_STR& get_Str() const { return m_Str; }
		char const * get_TypeName() const { return TypeName; }
	};

	template <char const *TypeName, class T_STR = sqlite3pp::tstring>
	class TableArgEnv : public TableArg<TypeName, T_STR> // This class converts environmental variable names in the input string to the env value. 
	{
	public:
		TableArgEnv(const T_STR& Str = T_STR()) :TableArg<TypeName, T_STR>(Get_UpdatedPathCopy(Str)) {}
	};

	// Constructor aruments
	using PreExecuteArg = TableArg<sql_base::TableArg_PreExecuteArg>;  // Use this argument to perform a SQL Execute before querying the table/view
	using WhereClauseArg = TableArg<sql_base::TableArg_WhereClauseArg>; // Use this argument to add a where clause when querying the table/view
	using DbFileNameArg = TableArgEnv<sql_base::TableArg_DbFileNameArg>; // Use this argument to open the specified database file
	enum DoNothingArg {	DoNotPopulate }; // Used to explicitly state that constructor does not populate it's data (m_VectType)


	class TableBase
	{
	public:
		virtual sqlite3pp::tstring GetTableName() const = 0;
		virtual sqlite3pp::tstring GetColumnNames() const = 0;
		virtual int GetColumnCount() const = 0;
	};

	template <class T
		, class T_PARENT = T // Having ability to change T_PARENT to a base class allows for merging queries (like a union, but faster)
		, class TABLEBASE = TableBase // Used by TableStream class in order to include stream out methods.
	>  
	class Table : public TABLEBASE, sql_base
	{
	public:
		// Defined types to be used
		using T_STR = typename T::StrType; // Allows Table class to work with different string types (std::string, std::wstring, sqlite3pp::tstring)
		using DataType = T_PARENT;
		using VectType = std::vector<DataType>;
		using ValueArg = TableArg<TableArg_ValueArg, T_STR>;


	protected:
		// All member variables
		VectType m_VectType;
		const PreExecuteArg		m_PreExecute;	// Mainly here for debugging purposes
		const WhereClauseArg	m_WhereClause;	// Mainly here for debugging purposes
		const DbFileNameArg		m_DbFileName;	// Mainly here for debugging purposes
		sqlite3pp::database &m_db;
		const T_STR m_InitQuery;					// Mainly here for debugging purposes
		const T_STR m_TableName;					// Mainly here for debugging purposes
		const T_STR m_ColumnNames;					// Mainly here for debugging purposes
		const int m_ColumnCount;					// Mainly here for debugging purposes
	public:
		// There are 2 constructor sets with each having 4 types of constructs. There are 4 types purely for the sake of convenience. Determine which constructors to use by which arguments are needed.
		// Set of constructors needing a sqlite3pp::database instance in constructor argument. These constructors automatically populate the object using data from the database db instance.
		Table(sqlite3pp::database &db, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) 	:m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db( db ), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery( m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(sqlite3pp::database &db, PreExecuteArg preexecutearg, WhereClauseArg whereclausearg = WhereClauseArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery( m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(sqlite3pp::database &db, DbFileNameArg dbfilenamearg, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery(m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(sqlite3pp::database &db, DbFileNameArg dbfilenamearg, PreExecuteArg preexecutearg, WhereClauseArg whereclausearg = WhereClauseArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery(m_db, m_InitQuery, preexecutearg, dbfilenamearg); }

		// Same as above set, but this set uses the single global database instance, and so db does not need to be pass to the constructor. These constructors automatically populate the object using data from the global database instance.
		Table(WhereClauseArg whereclausearg = WhereClauseArg()	, PreExecuteArg preexecutearg = PreExecuteArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()):m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db( global_db ), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery( m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(PreExecuteArg preexecutearg						, WhereClauseArg whereclausearg = WhereClauseArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(global_db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery(  m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(DbFileNameArg dbfilenamearg						, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(global_db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery( m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		Table(DbFileNameArg dbfilenamearg						, PreExecuteArg preexecutearg, WhereClauseArg whereclausearg = WhereClauseArg()) :m_PreExecute(preexecutearg), m_WhereClause(whereclausearg), m_DbFileName(dbfilenamearg), m_db(global_db), m_InitQuery(CreateSelectQueryStr(whereclausearg, T_STR())), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { ExecuteQuery( m_db, m_InitQuery, preexecutearg, dbfilenamearg); }
		
		// Set of constructors which do NOT populate itself using the database.  Instead the constructors takes an argument which is used to automatically populate itself
		Table(sqlite3pp::database &db, const VectType &VectTypes ) :m_db( db ), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { for ( auto v : VectTypes )  m_VectType.push_back( v ); }
		Table( const VectType &VectTypes ) :m_db( global_db ), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { for ( auto v : VectTypes )  m_VectType.push_back( v ); }

		// A constructor which does NOT populate itself at all.
		Table(DoNothingArg) :m_db( global_db ), m_TableName(T::getTableName()), m_ColumnNames(T::getColumnNames()), m_ColumnCount(T::getColumnCount()) { }

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Start Section:  Methods to get or set data in memory
		const VectType& Get() const { return m_VectType; }
		const DataType& operator[]( int i ) { return m_VectType[i]; }
		auto begin() { return m_VectType.begin(); }
		auto end() { return m_VectType.end(); }
		void push_back( const DataType &datatype ) { return m_VectType.push_back( datatype ); }
		void append( const VectType &vecttype ) { return m_VectType.push_back( vecttype ); }
		size_t size() const { return m_VectType.size(); }
		bool empty() const { return !m_VectType.size(); }
		template <class TVECT> void addData(const TVECT &tvect) // Used to transfer data from different tables/views having same data types and column names
		{
			for (auto i : tvect)
			{
				DataType datatype;
				datatype.setData(i);
				m_VectType.push_back(datatype);
			}
		}
		// End Section: Methods to get or set data in memory
		//////////////////////////////////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Start Section: SQL action methods
		void Insert( bool DeleteAllBeforeInsert = false)
		{ 
			if (DeleteAllBeforeInsert)
				DeleteAll();  // ToDo: Add logic to have delete use where clause if the constructor received one
			for (auto d : m_VectType)
				Insert(ValueArg(d.GetValues()), T_STR()); 
		}
		void UpdateDb(bool DeleteAllBeforeUpdate = false)
		{
			if (DeleteAllBeforeUpdate)
				DeleteAll(); // ToDo: Add logic to have delete use where clause if the constructor received one
			for (auto d : m_VectType) 
				UpdateDb(ValueArg(d.GetValues()), T_STR());
		}
		int Execute(const T_STR& strExecute){return m_db.execute(strExecute);}
		void Insert(const DataType &d) { push_back(d); Insert(ValueArg(d.GetValues()), T_STR()); }
		void UpdateDb(const DataType &d) { UpdateDb(ValueArg(d.GetValues()), T_STR()); }
		void DeleteAll(){ DeleteAll(T_STR()); }
		std::string CreateSelectQueryStr(WhereClauseArg whereclausearg, std::string) { return "SELECT " + T::getSelecColumnNames() + " FROM \"" + T::getTableName() + "\" " + to_string(whereclausearg.get_Str().c_str()); }
		std::wstring CreateSelectQueryStr(WhereClauseArg whereclausearg, std::wstring) { return L"SELECT " + T::getSelecColumnNames() + L" FROM \"" + T::getTableName() + L"\" " + to_wstring(whereclausearg.get_Str().c_str()); }
		void ReQuery()
		{
			m_VectType.clear();
			ExecuteQuery(m_db, m_InitQuery, m_PreExecute, m_DbFileName);
		}
		// End Section: SQL action methods
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Use these set of functions with a table instance
		virtual sqlite3pp::tstring GetTableName() const { return to_tstring(m_TableName); }
		virtual sqlite3pp::tstring GetColumnNames() const { return to_tstring(m_ColumnNames); }
		virtual int GetColumnCount() const { return m_ColumnCount; }
		
		// Use these set of functions when there's NO table instance
		static T_STR getTableName() { return T::getTableName(); }
		static T_STR getColumnNames() { return T::getColumnNames(); }
		static int getColumnCount() { return T::getColumnCount(); }
	protected:
		// Protected methods
		void Insert(const ValueArg& valuearg, std::string) { m_db.execute("INSERT INTO " + T::getTableName() + " (" + T::getColumnNames() + ") VALUES (" + valuearg.get_Str() + ")"); }
		void Insert(const ValueArg& valuearg, std::wstring) { m_db.execute(L"INSERT INTO " + T::getTableName() + L" (" + T::getColumnNames() + L") VALUES (" + valuearg.get_Str() + L")"); }
		void UpdateDb(const ValueArg& valuearg, std::string) { m_db.execute("INSERT OR REPLACE INTO " + T::getTableName() + " (" + T::getColumnNames() + ") VALUES (" + valuearg.get_Str() + ")"); }
		void UpdateDb(const ValueArg& valuearg, std::wstring) { m_db.execute(L"INSERT OR REPLACE INTO " + T::getTableName() + L" (" + T::getColumnNames() + L") VALUES (" + valuearg.get_Str() + L")"); }
		void DeleteAll(std::string) { m_db.execute("DELETE FROM  " + T::getTableName()); }
		void DeleteAll(std::wstring) { m_db.execute(L"DELETE FROM  " + T::getTableName()); }
		void PopulateVect(sqlite3pp::database &db, sqlite3pp::query &qry )
		{
			for ( auto q : qry )
			{
				DataType temp_var;
				temp_var.getStreamData( q );
				m_VectType.push_back( temp_var );
			}
		}
		void ExecuteQuery(sqlite3pp::database &db, const T_STR& strQuery, PreExecuteArg preexecutearg = PreExecuteArg(), DbFileNameArg dbfilenamearg = DbFileNameArg())
		{
			if (!dbfilenamearg.get_Str().empty())
				db = database(dbfilenamearg.get_Str().c_str());
			if (!preexecutearg.get_Str().empty())
				m_db.execute(to_string(preexecutearg.get_Str()));
			sqlite3pp::query qry(db, strQuery.c_str() );
			PopulateVect(db, qry );
		}
	};


	class TableOStreamBase : public TableBase
	{
	public:
		virtual std::ostream& out(std::ostream& os) const = 0;
		virtual std::wostream& out(std::wostream& os) const = 0;
	};

	// In order to use TableOStream, the T class must have OStream interface.
	template <class T
		, class T_PARENT = T // Having ability to change T_PARENT to a base class allows for merging queries (like a union, but faster)
		, class TABLEOSTREAMBASE = TableOStreamBase
	>
	class TableOStream : public Table<T, T_PARENT, TABLEOSTREAMBASE>
	{
	public:
		using TABLE = Table<T, T_PARENT, TABLEOSTREAMBASE>;

		// See sqlite3pp::Table class for details on these constructors
		TableOStream(sqlite3pp::database &db, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()): TABLE(db, whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()){}
		TableOStream(sqlite3pp::database &db, PreExecuteArg preexecutearg, WhereClauseArg whereclausearg = WhereClauseArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) : TABLE(db, whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(sqlite3pp::database &db, DbFileNameArg dbfilenamearg, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg()) : TABLE(db, whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(sqlite3pp::database &db, DbFileNameArg dbfilenamearg, PreExecuteArg preexecutearg = PreExecuteArg(), WhereClauseArg whereclausearg = WhereClauseArg()) : TABLE(db, whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) : TABLE(whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(PreExecuteArg preexecutearg, WhereClauseArg whereclausearg = WhereClauseArg(), DbFileNameArg dbfilenamearg = DbFileNameArg()) : TABLE(whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(DbFileNameArg dbfilenamearg, WhereClauseArg whereclausearg = WhereClauseArg(), PreExecuteArg preexecutearg = PreExecuteArg()) : TABLE(whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(DbFileNameArg dbfilenamearg, PreExecuteArg preexecutearg = PreExecuteArg(), WhereClauseArg whereclausearg = WhereClauseArg()) : TABLE(whereclausearg, preexecutearg, dbfilenamearg), Delimit(T::Delimiter()) {}
		TableOStream(sqlite3pp::database &db, const TABLE::VectType &VectTypes) : TABLE(db, VectTypes), Delimit(T::Delimiter()) {}
		TableOStream(const TABLE::VectType &VectTypes) : TABLE(VectTypes), Delimit(T::Delimiter()) {}
		TableOStream(DoNothingArg a) : TABLE(a), Delimit(T::Delimiter()) {}

		void setDelimit(const TABLE::T_STR &delimit) { Delimit = delimit; }
		const TABLE::T_STR & getDelimit() const { return Delimit; }
		virtual std::ostream& out(std::ostream& os) const
		{
			sqlite3pp::ostream_a o(os, Delimit);
			for (auto d : TABLE::m_VectType)
				d.OStream(o);
			return os;
		}
		virtual std::wostream& out(std::wostream& os) const
		{
			sqlite3pp::ostream_w o(os, Delimit);
			for (auto d : TABLE::m_VectType)
				d.OStream(o);
			return os;
		}
	protected:
		TABLE::T_STR Delimit;
	};

	template <class T, class T_PARENT = T, class TABLEOSTREAMBASE = TableOStreamBase>	
	std::ostream& operator<<(std::ostream& os, const TableOStream<T, T_PARENT, TABLEOSTREAMBASE>  & t)
	{
		return t.out(os);
	}

	template <class T, class T_PARENT = T, class TABLEOSTREAMBASE = TableOStreamBase>
	std::wostream& operator<<(std::wostream& os, const TableOStream<T, T_PARENT, TABLEOSTREAMBASE> & t)
	{
		return t.out(os);
	}


	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Blob& t);
	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Blob& t);
	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Clob& t);
	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Clob& t);
	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Datetime& t);
	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Datetime& t);
	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Date& t);
	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Date& t);

	database& setGlobalDB(const std::string& db_filename, ActionIfDatabaseOpen actionifopen = AIO_SkipIfSameFile);
	database& setGlobalDB(const std::wstring& db_filename, ActionIfDatabaseOpen actionifopen = AIO_SkipIfSameFile);
	database& getGlobalDB();
	int Execute( const std::string& sql );
	int Execute( const std::wstring& sql );
	int Connect(const char* db_filename, int flags, const char* vfs = nullptr );
	int Connect(const wchar_t* db_filename, int flags, const wchar_t* vfs = nullptr );
	int Attach(const char* db_filename, const char* name );
	int Attach(const wchar_t* db_filename, const  wchar_t* name );
	int Detach();
	int Backup(const char* db_filename, database& destdb, const char* destdbname, database::backup_handler h, int step_page = 5 );
	int Backup(const wchar_t* db_filename, database& destdb, const  wchar_t* destdbname, database::backup_handler h, int step_page = 5 );
	std::string GetDbErrMsg();
	std::wstring GetDbErrMsgW();
	int GetDbErrNo();
	int GetDbExtErrNo();

	bool dir_exists(std::string foldername, bool RepaceEnvVar = false);
	bool dir_exists(std::wstring foldername, bool RepaceEnvVar = false);
	bool file_exists(std::string filename, bool RepaceEnvVar = false);
	bool file_exists(std::wstring filename, bool RepaceEnvVar = false);
	bool copy_file(std::string Src, std::string Dest, bool OverWriteIfExist = false);
	bool copy_file(std::wstring Src, std::wstring Dest, bool OverWriteIfExist = false);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of implementation for SQLiteClassBuilder: The SQLiteClassBuilder class is used to create C++ header files for tables and views
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// TblClassOptions can be ignored when using the default settings, which can work with sqlite3pp::Table
	struct StrOptions
	{
		std::string str_type;			// "std::string", "std::wstring", "sqlite3pp::tstring"
		std::string str_tostr;			// "sqlite3pp::to_string", "sqlite3pp::to_wstring", "sqlite3pp::to_tstring"
		std::string str_pre;			//  "", "L", "_T("
		std::string str_post;			//  "", "", ")"
		std::string str_include;		//  include needed to support str_type. ie:("#include <string>", "#include \"sqlite3pp_ez.h\"", "#include <string>")
	};
	struct HeaderOpt
	{
		std::string dest_folder;		// Default: "SQL\\"				Destination folder where headers are created. If drive letter is not included in the path, folder is treated as relative path.
		std::string header_prefix;		// Default: "sql_"				Desired prefix for headers created.
		std::string header_postfix;		// Default: ""					Desired postfix for header created.
		std::string file_type;			// Default: "h"					Other options (hpp, hxx, class)
		std::string header_include;		// Default: "..\sqlite3pp_ez.h"	Other options (SQLite3pp_ez.h)
	};
	struct MiscOptions
	{
		std::string delimiter;			// Only used with opereator<<, and can be and desired string value to mark seperation between field output. ie:  ",", ", ", " ", ";", ""
		bool is_public_var_members;		// True to make data members public, and false to make data members protected.
		bool exclude_get_functions;		// If true, no get function. If false, a get function is created for each data member variable.
		bool exclude_set_functions;		// If true, no set function. If false, a set function is created for each data member variable.
		bool exclude_ostream_operator;	// If true, no operator<<. If false, a creates friend declaration, getDelimiter function, and global operator<< for the class
		bool exclude_comments;			// If true, excludes comments and additional spaces.
		bool exclude_table_interface;	// If true, excludes sqlite3pp::Table interface functions ( getTableName, getColumnNames, and getStreamData), and excludes Miscellaneous function(s).
		bool use_basic_types_only;		// If true, only int, double, std::string, and std::wstring are used
		bool exclude_main_hdr_example;	// If true, excludes example code added to sql_Master_Header.h
		bool exclude_comment_out_exampl;// If true, does NOT comment out example code
	}; // Create a custom defined TblClassOptions variable, or used one of the SQLiteClassBuilder predefined types, or use the default type which is automatically set by the SQLiteClassBuilder constructor

	struct TblClassOptions
	{
		StrOptions s;
		MiscOptions m;
		HeaderOpt h;
	};

	class SQLiteClassBuilder // Use SQLiteClassBuilder to create C++ header files for tables and views
	{
		sqlite3pp::database m_db;
		TblClassOptions m_options;
		const TblClassOptions m_options_org;
		bool m_AppendTableToHeader;
		std::vector<std::string> m_HeadersCreated;
		std::vector<std::string> m_ClassNames;
		std::string GetType(const std::string &tblVw, const std::string &colName, const char* str);
		std::string GetType_s(const std::string &tblVw, const std::string &colName, const char* str);
		TblClassOptions Init(const StrOptions & stroptions, const MiscOptions & miscoptions, const HeaderOpt & headeropt);
		void Init(
			  const std::string& TableOrView_name
			, const std::string &AndWhereClause
		);
		bool ProcessClassCreation(const std::string& ClassName, std::string QueryStr = "");
		bool CreateHeaderPrefix(const std::string& TableName, std::ofstream &myfile, std::string& ClassName, std::string& HeaderUpper, std::string FirstColumnName = "", std::string LastColumnName = "", bool AppendToVect = true);
	public:
		// This constructor is best to use when creating a header for all tables in the constructor.  (Headers can also be created by calling CreateHeader or CreateAllHeaders)
		SQLiteClassBuilder(const std::string& Db_filename						
			, const StrOptions &stroptions										// StrOptions is used to define the default string type.  Can be set to a custom define StrOptions, or to one of the predefined common options (strOpt_std_string, strOpt_std_wstring, strOpt_std_tstring, strOpt_sql_tstring)
			, const std::string &AndWhereClause = ""							// Used when creating multiple tables.  Can specify which tables/views to include via where clause
			, const MiscOptions &miscoptions = MiscOpt_max						// MiscOptions is used to define miscellaneous options.  Can be set to a custom define MiscOptions, or to one of the predefined common options (MiscOpt_max, MiscOpt_min, MiscOpt_var)
			, const HeaderOpt &headeropt = HeadersCreatedSqlDir						// HeaderOpt is used to define the naming convention to use when creating the header file(s).
			, const std::string& TableOrView_name = CreateHeaderForAllTables	// If equal to "%_CreateHeaderForAllTables_%", a header for each table and view is created. If equal to table or view name, a single header for associated table or view is created. If empty or equal to "#NILL#", the constructor does not create any headers.
		) :m_db(Db_filename.c_str()), m_options( Init(stroptions, miscoptions, headeropt) ), m_options_org(m_options), m_AppendTableToHeader(false){Init(TableOrView_name, AndWhereClause);}

		// This constructor is best when crating a single header or no headers at all in the contructor. (Headers can also be created by calling CreateHeader or CreateAllHeaders)
		SQLiteClassBuilder(const std::string& Db_filename						// Only Required Field
			, const std::string& TableOrView_name = ""							// If equal to "%_CreateHeaderForAllTables_%", a header for each table and view is created. If equal to table or view name, a single header for associated table or view is created. If empty or equal to "#NILL#", the constructor does not create any headers.
			, const std::string &AndWhereClause = ""							// Used when creating multiple tables.  Can specify which tables/views to include via where clause
			, const StrOptions &stroptions = strOpt_std_string					// StrOptions is used to define the default string type.  Can be set to a custom define StrOptions, or to one of the predefined common options (strOpt_std_string, strOpt_std_wstring, strOpt_std_tstring, strOpt_sql_tstring)
			, const MiscOptions &miscoptions = MiscOpt_max						// MiscOptions is used to define miscellaneous options.  Can be set to a custom define MiscOptions, or to one of the predefined common options (MiscOpt_max, MiscOpt_min, MiscOpt_var)
			, const HeaderOpt &headeropt = HeadersCreatedSqlDir						// HeaderOpt is used to define the naming convention to use when creating the header file(s).
		) :m_db(Db_filename.c_str()), m_options( Init(stroptions, miscoptions, headeropt) ), m_options_org(m_options), m_AppendTableToHeader(false){Init(TableOrView_name, AndWhereClause);}

		~SQLiteClassBuilder();
		bool CreateAllHeaders(const std::string &AndWhereClause = "");
		bool CreateAllHeaders(const TblClassOptions &strtype, const std::string &AndWhereClause = "");
		bool CreateHeader(const std::string& TableName, const TblClassOptions *strtype = NULL, std::string QueryStr = "");

		const std::vector<std::string>& GetHeadersCreated() const { return m_HeadersCreated; }
		const std::vector<std::string>& GetClassNames() const { return m_ClassNames; }
		const std::string& GetDestFolder() const { return m_options.h.dest_folder; }

		// Predefined string options
		static const StrOptions strOpt_std_string ;		// TEXT type defaults to std::string
		static const StrOptions strOpt_std_wstring;		// TEXT type defaults to std::wstring
		static const StrOptions strOpt_sql_tstring;		// TEXT type defaults to sqlite3pp::tstring, and uses T_() macro for portability and to avoid conflicts with tchar.h
		static const StrOptions strOpt_sql_tstring_T;	// Same as strOpt_sql_tstring, but uses windows _T() macro
		// Predefined MiscOptions for common settings
		static const MiscOptions MiscOpt_max;	// sqlite3pp:Table compatible. Includes the full implementation, and it creates protected data members. It includes get_* functions, set_* functions, comments, and operator<<.
		static const MiscOptions MiscOpt_min;	// sqlite3pp:Table compatible. Creates a minimalist class, with bare essentials to interface with sqlite3pp:Table. It creates public member variables, and it excludes get_* functions, set_* functions, comments, and operator<<
		static const MiscOptions MiscOpt_var;	// *NOT* compatible with sqlite3pp:Table. It creates a class with data members only, and it's an option to be used for other interfaces.
		// Default settings for HeaderOpt
		static const HeaderOpt HeadersCreatedSqlDir; // Creates headers in sub folder called SQL.
		static const HeaderOpt HeadersCreadedBaseDir; // Creates headers in the base folder.

		static const char *Nill; // = "#NILL#"
		static const char *CreateHeaderForAllTables; // = "%_CreateHeaderForAllTables_%"
	};

	using SqlBld = SQLiteClassBuilder; // Short alias

} // namespace sqlite3pp

#endif //SQLITE3PP_EZ_H
