/*
	GNU General Public License

	Copyright (C) 2021 David Maisonave (www.axter.com) 
	The sqlite3pp_ez source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This source code is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

	For usage examples see  https://github.com/David-Maisonave/sqlite3pp_EZ
							or sqlite3pp_ez.h
*/
#include <windows.h>
#include <stringapiset.h>

#include "sqlite3pp_ez.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <direct.h>
#include <cassert>
#include <sys/types.h> 
#include <sys/stat.h>


#define V_COUT(VB, V)		{if (sqlite3pp::sql_base::GetVerbosityLevel() >= sqlite3pp::VBLV_##VB) {std::cout << __FUNCTION__ << ":" << #VB << ": " << V << std::endl;} }

namespace sqlite3pp
{
	///////////////////////////////////////////////////////////////////////////
	// Added functions for UNICODE support
	///////////////////////////////////////////////////////////////////////////
	std::wstring to_wstring( const char* src )
	{
		int nchars = MultiByteToWideChar( CP_ACP, 0, src, -1, NULL, 0 );
		wchar_t* wsource = new wchar_t[nchars + 2]();
		if ( wsource == NULL )
			return std::wstring();
		MultiByteToWideChar( CP_ACP, 0, src, -1, wsource, nchars );
		std::wstring retrnVal = wsource;
		delete[] wsource;
		return retrnVal;
	}

	std::string to_string( const wchar_t* src )
	{
		int nchars = WideCharToMultiByte( CP_ACP, 0, src, -1, NULL, 0, NULL, NULL );
		char* source = new char[nchars + 2]();
		if ( source == NULL )
			return std::string();
		WideCharToMultiByte( CP_ACP, 0, src, -1, source, nchars, NULL, NULL );
		std::string retrnVal = source;
		delete[] source;
		return retrnVal;
	}

	std::string to_string(const std::wstring &src)
	{
		return sqlite3pp::to_string(src.c_str());
	}

	std::string to_string(const std::string &src)
	{
		return src;
	}

	std::wstring to_wstring(const std::string &src)
	{
		return sqlite3pp::to_wstring(src.c_str());
	}

	std::wstring to_wstring(const std::wstring &src)
	{
		return src;
	}

	// Convertion types
	std::string to_string(const Clob& src)
	{
		if (src)
			return std::string(src->data(), src->data() + src->size());
		return "";
	}

	std::wstring to_wstring(const Clob& src)
	{
		if (src)
			return std::wstring(src->data(), src->data() + src->size());
		return L"";
	}

	std::string to_string(const Blob& src)
	{
		if (src)
			return std::string(src->data(), src->data() + src->size());
		return "";
	}

	std::wstring to_wstring(const Blob& src)
	{
		if (src)
			return std::wstring(src->data(), src->data() + src->size());
		return L"";
	}

	std::string to_string(int src)
	{
		return std::to_string(src);
	}

	std::wstring to_wstring(int src)
	{
		return to_wstring(std::to_string(src));
	}

	std::string to_string(long long int src)
	{
		return std::to_string(src);
	}

	std::wstring to_wstring(long long int src)
	{
		return to_wstring(std::to_string(src));
	}

	std::string to_string(unsigned long long int src)
	{
		return std::to_string(src);
	}

	std::wstring to_wstring(unsigned long long int src)
	{
		return to_wstring(sqlite3pp::to_string(src));
	}

	std::string to_string(double src)
	{
		return std::to_string(src);
	}

	std::wstring to_wstring(double src)
	{
		return to_wstring(sqlite3pp::to_string(src));
	}

	bool isValidDate(const Date& t)
	{
		if (t.t < 1)
			return false;
		return true;
	}

	bool isValidDate(const Datetime& t)
	{
		if (t.tm_struct.tm_sec < 0 || t.tm_struct.tm_sec > 60)
			return false;
		if (t.tm_struct.tm_min < 0 || t.tm_struct.tm_min > 59)
			return false;
		if (t.tm_struct.tm_hour < 0 || t.tm_struct.tm_hour > 23)
			return false;
		if (t.tm_struct.tm_mday < 1 || t.tm_struct.tm_mday > 31)
			return false;
		if (t.tm_struct.tm_mon < 0 || t.tm_struct.tm_mon > 11)
			return false;
		if (t.tm_struct.tm_year < 0 || t.tm_struct.tm_year > 11000) // No more than 10,000 years into the future :-)
			return false;
		if (t.tm_struct.tm_wday < 0 || t.tm_struct.tm_wday > 6)
			return false;
		if (t.tm_struct.tm_yday < 0 || t.tm_struct.tm_yday > 365)
			return false;
		if (t.tm_struct.tm_isdst < 0 || t.tm_struct.tm_isdst > 24)  // ToDo: Check what are valid values for this variable.  Should be just 0 & 1.
			return false;
		return true;
	}

	std::string to_string(const Date& t)
	{
		if (isValidDate(t))
		{
			char buf[256] = { 0 };
			std::tm tm_struct = { 0 };
			gmtime_s(&tm_struct, &t.t);
			strftime(buf, sizeof(buf), "%Y-%m-%db", &tm_struct);
			return buf;
		}
		return "0000-00-00";
	}

	std::wstring to_wstring(const Date& src)
	{
		return to_wstring(sqlite3pp::to_string(src));
	}

	std::string to_string(const Datetime& t)
	{
		if (isValidDate(t))
		{
			char buf[256] = { 0 };
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t.tm_struct);
			return buf;
		}
		return "0000-00-00 00:00:00";
	}

	std::wstring to_wstring(const Datetime& src)
	{
		return to_wstring(sqlite3pp::to_string(src));
	}

	VerbosityLevels sql_base::m_VerbosityLevels = VBLV_ERROR;
	void sql_base::SetVerbosityLevel(VerbosityLevels v)
	{
		m_VerbosityLevels = v;
	}

	VerbosityLevels sql_base::GetVerbosityLevel()
	{
		return m_VerbosityLevels;
	}

	int database::connect( const  wchar_t* db_filename, int flags, const wchar_t * vfs )
	{
		if ( !borrowing_ )
		{
			disconnect();
		}
		return sqlite3_open16( db_filename, &db_ );
	}

	database::database(const wchar_t * db_filename, int flags, const wchar_t * vfs ) : db_( nullptr ), borrowing_( false )
	{
		if ( db_filename )
		{
			auto rc = connect( db_filename, flags, vfs );
			if ( rc != SQLITE_OK )
				throw database_error( "can't connect database" );
		}
	}

	int database::execute( const std::string& sql )
	{
		return execute( sql.c_str() );
	}
	
	int database::execute( const std::wstring& sql )
	{
		return execute(to_string(sql) );
	}

	int database::attach(const wchar_t* db_filename, const wchar_t* name)
	{
		return executef("ATTACH '%q' AS '%q'", to_string(db_filename).c_str(), to_string(name).c_str());
	}

	int database::detach(const wchar_t* name)
	{
		return executef("DETACH '%q'", to_string(name).c_str());
	}

	int database::backup(const  wchar_t* db_filename, database& destdb, const  wchar_t* destdbname, backup_handler h, int step_page)
	{
		return backup(to_string(db_filename).c_str(), destdb, to_string(destdbname).c_str(), h, step_page);
	}
	statement::statement( database& db, wchar_t const* stmt ) : db_( db ), stmt_( 0 ), tail_( 0 )
	{
		if ( stmt )
		{
			auto rc = prepare( stmt );
			if ( rc != SQLITE_OK )
				throw database_error( db_ );
		}
	}

	int statement::prepare( wchar_t const* stmt )
	{
		auto rc = finish();
		if ( rc != SQLITE_OK )
			return rc;

		return prepare_impl( stmt );
	}

	int statement::prepare_impl( wchar_t const* stmt )
	{
		union
		{
			char const** a;
			void const ** v;
		}myw2v = {&tail_ };
		int rc = sqlite3_prepare16_v2( db_.db_, stmt, static_cast<int>(std::wcslen( stmt )), &stmt_, myw2v.v );
		return rc;
	}

	query::query( database& db, wchar_t const* stmt ) : statement( db, stmt )
	{
	}

	///////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////
	// Added Global functions for global_db usage
	///////////////////////////////////////////////////////////////////////////
	database sql_base::global_db;
	tstring sql_base::filenameOfOpenDB;
	bool sql_base::bIsGlblDbOpen = false;
	int sql_base::last_rc = 0;
	const char sql_base::TableArg_PreExecuteArg[] = "PreExecuteArg";
	const char sql_base::TableArg_WhereClauseArg[] = "WhereClauseArg";
	const char sql_base::TableArg_DbFileNameArg[] = "DbFileNameArg";
	const char sql_base::TableArg_ValueArg[] = "ValueArg";

	void sql_base::set(tstring db_filename)
	{
		filenameOfOpenDB = db_filename;
		bIsGlblDbOpen = true;
	}

	void sql_base::unset()
	{
		filenameOfOpenDB = tstring();
		bIsGlblDbOpen = false;
	}

	database& setGlobalDB( const std::string& db_filename, ActionIfDatabaseOpen actionifopen)
	{
		return setGlobalDB(to_wstring(db_filename), actionifopen);
	}

	database& setGlobalDB( const std::wstring& db_filename_i, ActionIfDatabaseOpen actionifopen)
	{
		const std::wstring& db_filename = Get_UpdatedPathCopy(db_filename_i);
		if (sql_base::bIsGlblDbOpen)
		{
			if (actionifopen == AIO_IgnoreOpenRequest ||
				(actionifopen == AIO_SkipIfSameFile && sql_base::filenameOfOpenDB == to_tstring(db_filename)))
				return sql_base::global_db;
			if (actionifopen == AIO_CloseBeforeOpen)
				Detach();
		}

		if (sql_base::bIsGlblDbOpen)
			throw database_error("Trying to set Global DB with '" + to_string(db_filename.c_str()) + "' before detaching previous connection.");
		
		sql_base::global_db = database(db_filename.c_str());
		sql_base::set(to_tstring(db_filename));
		return sql_base::global_db;
	}

	database& getGlobalDB( )
	{
		if (!sql_base::bIsGlblDbOpen)
			throw database_error("Trying to get Global DB before it is opened.");
		return sql_base::global_db;
	}

	int Execute( const std::string& sql )
	{
		return Execute(to_wstring(sql));
	}

	int Execute( const std::wstring& sql )
	{
		if (!sql_base::bIsGlblDbOpen)
			throw database_error("Trying to Execute Global DB before it is opened. Query='" + to_string(sql.c_str()) + "'");
		int rc = sql_base::last_rc = sql_base::global_db.execute( sql.c_str() );
		// ToDo: Add logic here to repart error if rc!=SQLITE_OK
		return rc;
	}

	int Connect( const char* cdb_filename, int flags, const char * vfs )
	{
		std::string db_filename = Get_UpdatedPathCopy(cdb_filename);
		if (sql_base::bIsGlblDbOpen)
			throw database_error("Trying to connect to '" + db_filename + "' after Global DB is open.");
		int rc = sql_base::last_rc = sql_base::global_db.connect(db_filename.c_str(), flags, vfs);
		if (SQLITE_OK == rc)
			sql_base::set(to_tstring(db_filename));
		else
			throw database_error("Connect failed for '" + db_filename + "'.");
		return rc;
	}

	int Connect(const  wchar_t* cdb_filename, int flags, const wchar_t * vfs )
	{
		std::wstring db_filename = Get_UpdatedPathCopy(cdb_filename);
		if (sql_base::bIsGlblDbOpen)
			throw database_error("Trying to connect to '" + to_string(db_filename) + "' after Global DB is open.");
		int rc = sql_base::last_rc = sql_base::global_db.connect(db_filename.c_str(), flags, vfs);;
		if (SQLITE_OK == rc)
			sql_base::set(to_tstring(db_filename));
		else
			throw database_error("Connect failed for '" + to_string(db_filename) + "'.");
		return rc;
	}

	int Attach( const char* cdb_filename, const char* name )
	{
		std::string db_filename = Get_UpdatedPathCopy(cdb_filename);
		if (sql_base::bIsGlblDbOpen)
			throw database_error("Trying to attach to '" + db_filename + "' after Global DB is open.");
		int rc = sql_base::last_rc = sql_base::global_db.attach(db_filename.c_str(), name);
		if (SQLITE_OK == rc)
			sql_base::set(to_tstring(db_filename));
		else
			throw database_error("Attach failed for '" + db_filename + "'.");
		return rc;
	}

	int Attach( const  wchar_t* cdb_filename, const  wchar_t* name )
	{
		std::wstring db_filename = Get_UpdatedPathCopy(cdb_filename);
		if (sql_base::bIsGlblDbOpen)
			throw database_error("Trying to attach to '" + to_string(db_filename) + "' after Global DB is open.");
		int rc = sql_base::last_rc = sql_base::global_db.attach(  db_filename.c_str(), name );
		if (SQLITE_OK == rc)
			sql_base::set(to_tstring(db_filename));
		else
			throw database_error("Attach failed for '" + to_string(db_filename) + "'.");
		return rc;
	}

	int Detach()
	{
		if (!sql_base::bIsGlblDbOpen)
			throw database_error("Trying to detached before Global DB is open.");
		int rc = sql_base::last_rc = sql_base::global_db.detach(sql_base::filenameOfOpenDB.c_str());
		if (SQLITE_OK == rc)
			sql_base::unset();
		else
			throw database_error("Detach failed for '" + to_string(sql_base::filenameOfOpenDB) + "'.");
		return rc;
	}

	int Backup( const char* cdb_filename, database & destdb, const char* cdestdbname, database::backup_handler h, int step_page )
	{
		std::string db_filename = Get_UpdatedPathCopy(cdb_filename);
		std::string destdbname = Get_UpdatedPathCopy(cdestdbname);
		if (!sql_base::bIsGlblDbOpen)
			throw database_error("Trying to use Global DB before it has been opened.");
		return sql_base::global_db.backup(db_filename.c_str(),destdb,destdbname.c_str(),h,step_page);
	}
	int Backup( const  wchar_t* cdb_filename, database & destdb, const  wchar_t* cdestdbname, database::backup_handler h, int step_page )
	{
		std::wstring db_filename = Get_UpdatedPathCopy(cdb_filename);
		std::wstring destdbname = Get_UpdatedPathCopy(cdestdbname);
		if (!sql_base::bIsGlblDbOpen)
			throw database_error("Trying to use Global DB before it has been opened.");
		return sql_base::global_db.backup(db_filename.c_str(), destdb, destdbname.c_str(), h, step_page );
	}
	std::string GetDbErrMsg()
	{
		if (!sql_base::bIsGlblDbOpen)
			return "Error: Failed to open global database before using it";
		return sql_base::global_db.error_msg();
	}
	std::wstring GetDbErrMsgW()
	{
		if (!sql_base::bIsGlblDbOpen)
			return L"Error: Failed to open global database before using it";
		return to_wstring(sql_base::global_db.error_msg());
	}
	int GetDbErrNo()
	{
		if (!sql_base::bIsGlblDbOpen)
			return -1;
		return sql_base::global_db.error_code();
	}
	int GetDbExtErrNo()
	{
		if (!sql_base::bIsGlblDbOpen)
			return -1;
		return sql_base::global_db.extended_error_code();
	}

	bool dir_exists(std::string foldername, bool RepaceEnvVar)
	{
		if (RepaceEnvVar)
			foldername = Get_UpdatedPathCopy(foldername);
		struct stat st = { 0 };
		stat(foldername.c_str(), &st);
		return st.st_mode & S_IFDIR;
	}

	bool dir_exists(std::wstring foldername, bool RepaceEnvVar)
	{
		if (RepaceEnvVar)
			foldername = Get_UpdatedPathCopy(foldername);
		struct _stat st = { 0 };
		_wstat(foldername.c_str(), &st);
		return st.st_mode & S_IFDIR;
	}

	bool file_exists(std::string filename, bool RepaceEnvVar)
	{
		if (RepaceEnvVar)
			filename = Get_UpdatedPathCopy(filename);
		struct stat st = { 0 };
		return (stat(filename.c_str(), &st) == 0);
	}

	bool file_exists(std::wstring filename, bool RepaceEnvVar)
	{
		if (RepaceEnvVar)
			filename = Get_UpdatedPathCopy(filename);
		struct _stat st = { 0 };
		return (_wstat(filename.c_str(), &st) == 0);
	}

	bool copy_file(std::wstring Src, std::wstring Dest, bool OverWriteIfExist)
	{
		Src = Get_UpdatedPathCopy(Src);
		Dest = Get_UpdatedPathCopy(Dest);
		if (file_exists(Dest))
		{
			if (OverWriteIfExist)
			{
				if (!DeleteFileW(Dest.c_str()))
					return false;
			}
			else
				return false;
		}

		bool RetrnVal = false;
		std::ofstream outfile(Dest.c_str(), std::ios::out | std::ios::binary);
		if (outfile.is_open())
		{
			std::ifstream is(Src.c_str(), std::ios::in | std::ios::binary);
			if (is.is_open())
			{
				is.seekg(0, is.end);
				const std::streamoff length = is.tellg();
				is.seekg(0, is.beg);
				char *buffer = new char[static_cast<size_t>(length) + 1]();
				if (buffer)
				{
					is.read(buffer, length);
					outfile.write(buffer, length);
					delete[] buffer;
					RetrnVal = true;
				}
				is.close();
			}
			outfile.close();
		}

		return RetrnVal;
	}

	bool copy_file(std::string Src, std::string Dest, bool OverWriteIfExist)
	{
		return copy_file(to_wstring(Src), to_wstring(Dest), OverWriteIfExist);
	}

	void replace_all(std::wstring & data, const std::wstring &toSearch, const std::wstring &replaceStr)
	{
		// Get the first occurrence
		size_t pos = data.find(toSearch);
		// Repeat till end is reached
		while (pos != std::string::npos)
		{
			// Replace this occurrence of Sub String
			data.replace(pos, toSearch.size(), replaceStr);
			// Get the next occurrence from the current position
			pos = data.find(toSearch, pos + replaceStr.size());
		}
	}

	void CheckEnv(std::wstring &src, const std::string &VarName, const std::string VarNamePrefix, const std::string VarNamePostfix)
	{
		char env_p[_MAX_PATH] = { 0 };
		size_t Sz = 0;
		if (!getenv_s(&Sz, env_p, sizeof(env_p), VarName.c_str()))
		{
			const std::wstring EncVarName = to_wstring(VarNamePrefix + VarName + VarNamePostfix);
			replace_all(src, EncVarName, to_tstring(env_p));
		}
	}
	std::string GetUpdatedSrcPath(std::string &src, int EnvVarToFetch, const std::string VarNamePrefix, const std::string VarNamePostfix)
	{
		src = to_string(Get_UpdatedPathCopy(to_wstring(src), EnvVarToFetch, VarNamePrefix, VarNamePostfix));
		return src;
	}
	std::string Get_UpdatedPathCopy(std::string src, int EnvVarToFetch, const std::string VarNamePrefix, const std::string VarNamePostfix)
	{
		return to_string(Get_UpdatedPathCopy(to_wstring(src), EnvVarToFetch, VarNamePrefix, VarNamePostfix));
	}

	std::wstring GetUpdatedSrcPath(std::wstring &src, int EnvVarToFetch, const std::string VarNamePrefix, const std::string VarNamePostfix)
	{ // EnvVarToFetch BIT Settings: 1=Get User Var, 2=Get System Paths, 4=Get Misc Var
		if (EnvVarToFetch & 1)
		{
			// User level environmental variables
			CheckEnv(src, "APPDATA", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "USERNAME", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "HOMEDRIVE", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "HOMEPATH", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "USERPROFILE", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "APPDATA", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "LOCALAPPDATA", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "TEMP", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "TMP", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "ONEDRIVE", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "USERDOMAIN", VarNamePrefix, VarNamePostfix);
		}

		if (EnvVarToFetch & 2)
		{
			// System path level environmental variables
			CheckEnv(src, "SYSTEMDRIVE", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "COMMONPROGRAMFILES", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "PROGRAMFILES", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "PROGRAMDATA", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "COMMONPROGRAMFILES(x86)", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "WINDIR", VarNamePrefix, VarNamePostfix);
		}

		if (EnvVarToFetch & 4)
		{
			// Miscellaneous environmental variables
			CheckEnv(src, "COMPUTERNAME", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "DATE", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "TIME", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "RANDOM", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "OS", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "CD", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "PUBLIC", VarNamePrefix, VarNamePostfix);
			CheckEnv(src, "ALLUSERSPROFILE", VarNamePrefix, VarNamePostfix);
		}

		return src;
	}

	std::wstring Get_UpdatedPathCopy(std::wstring src, int EnvVarToFetch, const std::string VarNamePrefix, const std::string VarNamePostfix)
	{
		return GetUpdatedSrcPath(src, EnvVarToFetch, VarNamePrefix, VarNamePostfix);
	}


	// Additional implementation for SQLite3pp
#ifndef SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY

#ifndef SQLITE3PP_NO_UNICODE
	wchar_t const* query::rows::get(int idx, wchar_t const*) const
	{
		return reinterpret_cast<wchar_t const*>(sqlite3_column_text16(stmt_, idx));
	}

	std::wstring query::rows::get(int idx, const std::wstring&) const
	{
#ifdef SQLITE3PP_ALLOW_NULL_STRING_RETURN
		bool AllowNullStringReturn = true;
#else
		bool AllowNullStringReturn = false;
#endif  // !SQLITE3PP_ALLOW_NULL_STRING_RETURN
		std::wstring value;
		const char * strtype = sqlite3_column_decltype(stmt_, idx);
		if (!strtype)
		{
			V_COUT(WARN, "Received NULL value when getting column type for idx " << idx << ". Treating type as ASCII or UTF8.");
		}

		bool GetUnicodeString = false;
		if (!strtype || strcmp(strtype, "TEXT") == 0 || strncmp("CHARACTER", strtype, 9) == 0 || strncmp("VARYING CHARACTER", strtype, 17) == 0 || strncmp("VARCHAR", strtype, 7) == 0)
			GetUnicodeString = false;
		else if ( strncmp("NCHAR", strtype, 5) == 0 || strncmp("NVARCHAR", strtype, 8) == 0 || strncmp("NATIVE CHARACTER", strtype, 16) == 0)
			GetUnicodeString = true;
		else
		{
			V_COUT(WARN, "Could not find a string type to process. Treating type as ASCII or UTF8.");
			assert(0);// Code should NOT get here.  If it does something went wrong.
			GetUnicodeString = false;
		}

		union
		{ // This union is here for debugging purposes.
			wchar_t const* Val_w;
			char const* Val_a;
			const void* Val;
		};

		if (GetUnicodeString)
		{
			Val_w = get(idx, (wchar_t const*)0);
			if (Val_w || AllowNullStringReturn)
				value = Val_w;
		}
		else
		{
			Val_a = get(idx, (char const*)0);
			if (Val_a || AllowNullStringReturn)
				value = to_wstring(Val_a);
		}

		return value; 
	}

	database_error::database_error(const std::string& msg) : std::runtime_error(msg.c_str())
	{
	}

#endif// !SQLITE3PP_NO_UNICODE

	Blob query::rows::get(int idx, const Blob&) const
	{
		const int data_len = column_bytes(idx);
		const unsigned char* ptr = static_cast<const unsigned char*>(sqlite3_column_blob(stmt_, idx));
		Blob data(new std::vector<unsigned char>(ptr, ptr + data_len));
		return data;
	}

	Clob query::rows::get(int idx, const Clob&) const
	{
		const int data_len = column_bytes(idx);
		const char* ptr = static_cast<const char*>(sqlite3_column_blob(stmt_, idx));
		Clob data(new std::vector<char>(ptr, ptr + data_len));
		return data;
	}

	unsigned char query::rows::get(int idx, const unsigned char&) const
	{
		return static_cast<unsigned char>(get(idx, int()));
	}

	short int query::rows::get(int idx, const short int&) const
	{
		return static_cast<short int>(get(idx, int()));
	}

	bool query::rows::get(int idx, const bool&) const
	{
		return static_cast<bool>(get(idx, int()));
	}

	unsigned long long int query::rows::get(int idx, const unsigned long long int&) const
	{
		return static_cast<unsigned long long int>(sqlite3_column_int64(stmt_, idx));
	}

	Date query::rows::get(int idx, const Date&) const
	{
		const char* s = reinterpret_cast<const char*>(sqlite3_column_text(stmt_, idx));
		std::tm d = { 0 };
		Date data = { 0 };
		int rc = sscanf_s(s, "%d-%d-%d", &d.tm_year, &d.tm_mon, &d.tm_mday);
		if (rc < 1 || !d.tm_mday)
		{
			if (s) V_COUT(WARN, "Could not parse value '" << s << "' into a date.");
			if (rc < 1) V_COUT(WARN, "sscanf_s returned value less than 1 indicating invalid data.");
			if (!d.tm_mday) V_COUT(WARN, "tm_mday == 0. Invalid value.");
			return data;
		}
		if (d.tm_year > 1900)
		{
			d.tm_year -= 1900;
			d.tm_mon -= 1;
		}
		data.t = mktime(&d);
		if (data.t == -1)
		{
			V_COUT(WARN, "Received -1 from mktime.");
			return Date();
		}
		return data;
	}

	Datetime query::rows::get(int idx, const Datetime&) const
	{
		Datetime data = { 0 };
		const char* s = reinterpret_cast<const char*>(sqlite3_column_text(stmt_, idx));
		int rc = sscanf_s(s, "%d-%d-%d %d:%d:%d", &data.tm_struct.tm_year, &data.tm_struct.tm_mon, &data.tm_struct.tm_mday, &data.tm_struct.tm_hour, &data.tm_struct.tm_min, &data.tm_struct.tm_sec);
		if (rc < 1 || !data.tm_struct.tm_mday)
		{
			if (s) V_COUT(WARN, "Could not parse value '" << s << "' into a date.");
			if (rc < 1) V_COUT(WARN, "sscanf_s returned value less than 1 indicating invalid data.");
			if (!data.tm_struct.tm_mday) V_COUT(WARN, "tm_mday == 0. Invalid value.");
			return Datetime();
		}

		if (data.tm_struct.tm_year > 1900)
		{
			data.tm_struct.tm_year -= 1900;
			data.tm_struct.tm_mon -= 1;
		}
		return data;
	}

	std::wostream& operator<<(std::wostream& os, const sql_base::Character& t)
	{
		os << to_wstring(t);
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const sql_base::Nchar& t)
	{
		os << to_string(t);
		return os;
	}

	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Blob& t)
	{
		std::string data(t->data(), t->data() + t->size());
		std::wstring wdata = to_wstring(data);
		os.write(wdata.c_str(), wdata.size());
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Blob& t)
	{
		const char* ptr = (const char*)t->data();
		os.write(ptr, t->size());
		return os;
	}

	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Clob& t)
	{
		std::string data(t->data(), t->data() + t->size());
		std::wstring wdata = to_wstring(data);
		os.write(wdata.c_str(), wdata.size());
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Clob& t)
	{
		const char* ptr = (const char*)t->data();
		os.write(ptr, t->size());
		return os;
	}

	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Datetime& t)
	{
		if (isValidDate(t))
		{
			wchar_t buf[256] = { 0 };
			wcsftime(buf, sizeof(buf), L"%Y-%m-%d %H:%M:%S", &t.tm_struct);
			os << buf;
		}
		else
		{
			V_COUT(WARN, "tm_mday == 0. Invalid value.");
			os << L"0000-00-00 00:00:00";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Datetime& t)
	{
		if (isValidDate(t))
		{
			char buf[256] = { 0 };
			strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t.tm_struct);
			os << buf;
		}
		else
		{
			V_COUT(WARN, "tm_mday == 0. Invalid value.");
			os << "0000-00-00 00:00:00";
		}
		return os;
	}

	std::wostream& operator<<(std::wostream& os, const sqlite3pp::Date& t)
	{
		if (isValidDate(t))
		{
			wchar_t buf[256] = { 0 };
			std::tm tm_struct = { 0 };
			gmtime_s(&tm_struct, &t.t);
			wcsftime(buf, sizeof(buf), L"%Y-%m-%d", &tm_struct);
			os << buf;
		}
		else
		{
			V_COUT(WARN, "t.t = Invalid Falue.");
			os << L"0000-00-00";
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const sqlite3pp::Date& t)
	{
		if (isValidDate(t))
		{
			char buf[256] = { 0 };
			std::tm tm_struct = { 0 };
			gmtime_s(&tm_struct, &t.t);
			strftime(buf, sizeof(buf), "%Y-%m-%db", &tm_struct);
			os << buf;
		}
		else
		{
			V_COUT(WARN, "t.t = Invalid Falue.");
			os << "0000-00-00";
		}
		return os;
	}
#endif	// !SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start of implementation for SQLiteClassBuilder
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////////////
  // sqlite_master is used when querying all the tables in a SQLite DB
	class sqlite_master
	{
	public:
		using StrType = std::string;
		static StrType getTableName() { return  "sqlite_master"; }
		static StrType getColumnNames() { return  "type, name, tbl_name, rootpage, sql"; }
		static StrType getSelecColumnNames() { return  "type, name, tbl_name, rootpage, sql"; }
		template<class T> void getStreamData(T q) { q.getter() >> type >> name >> tbl_name >> rootpage >> sql; }
		static int getColumnCount() { return 5; }
	public:
		StrType type;
		StrType name;
		StrType tbl_name;
		StrType rootpage;
		StrType sql;
	};
  ////////////////////////////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////////////////////
  // SQLiteClassBuilder Predefines.
	// Predefined string options
	const StrOptions SQLiteClassBuilder::strOpt_std_string = { "std::string", "sqlite3pp::to_string", "", "", "#include <string>" };
	const StrOptions SQLiteClassBuilder::strOpt_std_wstring = { "std::wstring", "sqlite3pp::to_wstring", "L", "", "#include <string>" };
	const StrOptions SQLiteClassBuilder::strOpt_sql_tstring = { "sqlite3pp::tstring", "sqlite3pp::to_tstring", "T_(", ")", "#include \"sqlite3pp_ez.h\"" };
	const StrOptions SQLiteClassBuilder::strOpt_sql_tstring_T = { "sqlite3pp::tstring", "sqlite3pp::to_tstring", "_T(", ")", "#include \"sqlite3pp_ez.h\"" };
	// Predefined MiscOptions for common settings
	const MiscOptions SQLiteClassBuilder::MiscOpt_max = { ",", false, false, false, false, false, false, false, false, false };
	const MiscOptions SQLiteClassBuilder::MiscOpt_min = { ",", true, true, true, true, true, false, false, true, true };
	const MiscOptions SQLiteClassBuilder::MiscOpt_var = { ",", true, true, true, true, true, true, false, true, true };
	// Default settings for HeaderOpt
	const HeaderOpt SQLiteClassBuilder::HeadersCreatedSqlDir = { "SQL\\", "sql_", "", "h", "..\\sqlite3pp_ez.h" };
	const HeaderOpt SQLiteClassBuilder::HeadersCreadedBaseDir = { "", "sql_", "", "h", "sqlite3pp_ez.h" };

	const char *SQLiteClassBuilder::Nill = "#NILL#";
	const char *SQLiteClassBuilder::CreateHeaderForAllTables = "%_CreateHeaderForAllTables_%";
  ////////////////////////////////////////////////////////////////////////////////////////////

	std::string SQLiteClassBuilder::GetType(const std::string &tblVw, const std::string &colName, const char* str_org)
	{
		std::string strtype = GetType_s(tblVw, colName, str_org);
		V_COUT(DETAIL, "Using type '" << strtype << "' for column '" << colName << "' in table/view '" << tblVw << "'.");
		return strtype;
	}

	std::string SQLiteClassBuilder::GetType_s(const std::string &tblVw, const std::string &colName, const char* str_org)
	{
		const std::string DefaultType = "Text";
		if (!str_org)
		{
			// Logging out as DEBUG because some views do return NULL value for the column type.
			V_COUT(DEBUG, "Entered with DB type NULL value for column '" << colName << "' in table/view '" << tblVw << "'.\nGracefully continuing by returning default type '" << DefaultType << "'.");
			return DefaultType;
		}
		char str[99] = { 0 };
		strcpy_s(str, str_org);
		_strupr_s(str);
		// There's no practical method for handling blob or clob other than the Blob and Clob type, so don't even include them  in an option to declare them any other way.
		if (strcmp("BLOB", str) == 0)
			return "Blob";
		if (strcmp("CLOB", str) == 0)
			return "Clob";

#ifdef SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY // If defined convert types to Resulting Affinity (int, double, or StrType)
		bool UseBaseTypes = true;
#else  // Use SQLite3 sub types and use type names
		bool UseBaseTypes = false;
#endif //SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY

		if (UseBaseTypes || m_options.m.use_basic_types_only)
		{
			// Only output detail once
			static bool HasLoggedDetails = false;
			if (!HasLoggedDetails)
			{
				HasLoggedDetails = true;
				V_COUT(DETAIL, "Only setting DB types to basic types due to compiler #define SQLITE3PP_CONVERT_TO_RESULTING_AFFINITY(" << UseBaseTypes << ") or input option use_basic_types_only(" << m_options.m.use_basic_types_only << ")");
			}

			if (strcmp("INTEGER", str) == 0 || strcmp("INT", str) == 0 || strcmp("TINYINT", str) == 0 || strcmp("SMALLINT", str) == 0 || strcmp("MEDIUMINTSMALLINT", str) == 0 || strcmp("BIGINT", str) == 0 || strcmp("UNSIGNED BIG INT", str) == 0 || strcmp("INT2", str) == 0 || strcmp("INT8", str) == 0)
				return "int";
			if (strcmp("REAL", str) == 0 || strcmp("DOUBLE", str) == 0 || strcmp("DOUBLE PRECISION", str) == 0 || strcmp("FLOAT", str) == 0 || strncmp("DECIMAL", str, 7) == 0 || strcmp("BOOLEANL", str) == 0 || strcmp("BOOLEAN", str) == 0 || strcmp("DATE", str) == 0 || strcmp("DATETIME", str) == 0 || strcmp("NUMERIC", str) == 0)
				return "double";

			if (m_options.m.use_basic_types_only)
			{
				// String types
				if (strcmp("TEXT", str) == 0 || strncmp("CHARACTER", str, 9) == 0 || strncmp("VARCHAR", str, 7) == 0 || strncmp("VARYING CHARACTER", str, 17) == 0)
					return "std::string";
				if (strncmp("NVARCHAR", str, 8) == 0 || strncmp("NATIVE CHARACTER", str, 16) == 0 || strncmp("NCHAR", str, 5) == 0)
					return "std::wstring";
			}
		}
		else
		{
			if (strcmp("INTEGER", str) == 0)
				return "Integer";
			if (strcmp("INT", str) == 0)
				return "Int";
			if (strcmp("INT2", str) == 0)
				return "Int2";
			if (strcmp("INT8", str) == 0)
				return "Int8";
			if (strcmp("TINYINT", str) == 0)
				return "Tinyint";
			if (strcmp("SMALLINT", str) == 0)
				return "Smallint";
			if (strcmp("MEDIUMINTSMALLINT", str) == 0)
				return "Mediumint";
			if (strcmp("BOOLEAN", str) == 0 || strcmp("BOOLEANL", str) == 0)
				return "Boolean";
			if (strcmp("BIGINT", str) == 0)
				return "Bigint";
			if (strcmp("UNSIGNED BIG INT", str) == 0)
				return "UBigint";
			if (strcmp("DATE", str) == 0)
				return "Date";
			if (strcmp("DATETIME", str) == 0)
				return "Datetime";
			if (strcmp("NUMERIC", str) == 0)
				return "Numeric";
			if (strncmp("DECIMAL", str, 7) == 0)
				return "Decimal";
			if (strcmp("REAL", str) == 0)
				return "Real";
			if (strcmp("DOUBLE PRECISION", str) == 0)
				return "DoublePrcsn";
			if (strcmp("DOUBLE", str) == 0)
				return "Double";
			if (strcmp("FLOAT", str) == 0)
				return "Float";

			// String types
			if (strcmp("TEXT", str) == 0)
				return "Text";
			if (strncmp("CHARACTER", str, 9) == 0)
				return "Character";
			if (strncmp("VARYING CHARACTER", str, 17) == 0
				|| strncmp("VARCHAR", str, 7) == 0)
				return "Varchar";
			if (strncmp("NATIVE CHARACTER", str, 16) == 0
				|| strncmp("NCHAR", str, 5) == 0)
				return "Nchar";
			if (strncmp("NVARCHAR", str, 8) == 0)
				return "Nvarchar";
		}

		V_COUT(WARN, "Received unknown type ('" << str_org << "') from DB  for column '" << colName << "' in table/view '" << tblVw << "'.");
		assert(0); // Always assert, because the code should not reach this point.
		V_COUT(WARN, "Gracefully continuing by returning default type '" << DefaultType << "'.");
		return DefaultType; // Handle it gracefully for release mode.
	}

	TblClassOptions SQLiteClassBuilder::Init(const StrOptions & stroptions, const MiscOptions & miscoptions, const HeaderOpt & headeropt)
	{
		TblClassOptions option = { stroptions, miscoptions, headeropt };
		return option;
	}

	void SQLiteClassBuilder::Init(const std::string & TableOrView_name, const std::string & AndWhereClause)
	{
		if (m_options.h.dest_folder.size())
		{
			if (m_options.h.dest_folder[m_options.h.dest_folder.size() - 1] == '/')
				m_options.h.dest_folder[m_options.h.dest_folder.size() - 1] = '\\';
			else if (m_options.h.dest_folder[m_options.h.dest_folder.size() - 1] != '\\')
				m_options.h.dest_folder = +"\\";
			V_COUT(DEBUG, "Using destination path '" << m_options.h.dest_folder << "'.");
		}

		if (TableOrView_name == CreateHeaderForAllTables)
		{
			V_COUT(DEBUG, "Calling CreateAllHeaders with Where Clause '" << AndWhereClause << "'.");
			CreateAllHeaders(m_options, AndWhereClause);
		}
		else if (!TableOrView_name.empty() && TableOrView_name != Nill)
		{
			V_COUT(DEBUG, "Calling CreateHeader with name '" << TableOrView_name << "'.");
			CreateHeader(TableOrView_name);
		}
	}

	SQLiteClassBuilder::~SQLiteClassBuilder()
	{
		V_COUT(DEBUG, "Disconnecting database.");
		m_db.disconnect();
	}

	bool SQLiteClassBuilder::CreateAllHeaders(const std::string &AndWhereClause)
	{
		V_COUT(DEBUG, "Calling CreateAllHeaders with Where Clause '" << AndWhereClause << "'.");
		return CreateAllHeaders(m_options, AndWhereClause);
	}
	static const char TopHeaderCommnetsPrt1[] = "/* This file was automatically generated using [Sqlite3pp_EZ].\nSqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\\www.axter.com)";
	static const char TopHeaderCommnetsPrt2[] = "For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ\n*/";

	bool SQLiteClassBuilder::CreateHeaderPrefix(const std::string& TableName, std::ofstream &myfile, std::string& ClassName, std::string& HeaderUpper, std::string FirstColumnName, std::string LastColumnName, bool AppendToVect)
	{
		V_COUT(DEBUG, "Entering with arguments: '" << TableName << "', ofstream, '" << ClassName << "', '" << HeaderUpper << "', '" << FirstColumnName << "', '" << LastColumnName << "', " << AppendToVect);
		std::ios_base::openmode openMode = m_AppendTableToHeader ? std::ios_base::out | std::ios_base::app : std::ios_base::out;
		ClassName = m_options.h.header_prefix + TableName + m_options.h.header_postfix;
		const std::string HeaderFileName = ClassName + "." + m_options.h.file_type;
		const std::string HeaderFileNameWithFolder = m_options.h.dest_folder + HeaderFileName;
		myfile.open(HeaderFileNameWithFolder.c_str(), openMode);
		if (!myfile.is_open())
		{
			V_COUT(WARN, "Failed to open file '" << HeaderFileNameWithFolder << "'");
			return false;
		}
		if (AppendToVect)
			m_HeadersCreated.push_back(HeaderFileName);
		char headerupper[256] = { 0 };
		strcpy_s(headerupper, (ClassName + "_H").c_str());
		_strupr_s(headerupper);
		HeaderUpper = headerupper;
		if (!m_options.m.exclude_comments && AppendToVect == true)
		{
			myfile << TopHeaderCommnetsPrt1 << std::endl;
			if (FirstColumnName.empty())
				FirstColumnName = "ColumnFoo";
			if (LastColumnName.empty())
				LastColumnName = "ColumnWiget";
			myfile << "Example Usage:\t\t(Using sqlite3pp::Table container)" << std::endl;
			myfile << "\t// Exampel #1\n\t\tsqlite3pp::setGlobalDB(\"mydatabase.db\");" << std::endl;
			myfile << "\t\tsqlite3pp::Table<" << ClassName << "> my_tbl;\n\t\tfor (auto row : my_tbl)\n\t\t\tstd::wcout << row << std::endl;\n" << std::endl;

			myfile << "\t// Exampel #2\n\t\tfor (int i = 0; i < my_tbl.size(); ++i)\n\t\t\tstd::wcout << my_tbl[i].get_" << FirstColumnName << "() << std::endl;\n" << std::endl;

			myfile << "\t// Exampel #3\n\t\tfor (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)\n\t\t\tstd::wcout << r->get_" << LastColumnName << "() << std::endl;\n" << std::endl;
			myfile << TopHeaderCommnetsPrt2 << std::endl;
		}
		// Add includes needed to support specified m_options.str_type
		myfile << "#ifndef " << HeaderUpper << std::endl;
		myfile << "#define " << HeaderUpper << std::endl;
		const std::string AdditionalInclude = "#include \"" + m_options.h.header_include + "\"";
		if (m_options.s.str_include.size() && m_options.s.str_include != AdditionalInclude &&
			(m_options.s.str_include != strOpt_sql_tstring.str_include || (m_options.h.header_include != HeadersCreatedSqlDir.header_include && m_options.h.header_include != HeadersCreadedBaseDir.header_include) ))
			myfile << m_options.s.str_include << std::endl;

		if (m_options.h.header_include.size())
			myfile << AdditionalInclude << std::endl;

		V_COUT(DETAIL, "Created prefix data for class '" << ClassName << "' in file '" << HeaderFileNameWithFolder << "' for table '" << TableName << "'");
		return true;
	}

	bool SQLiteClassBuilder::CreateAllHeaders(const TblClassOptions &strtype, const std::string &AndWhereClause)
	{
		m_HeadersCreated.clear();
		m_ClassNames.clear();
		m_options = strtype;
		const std::string OrgPrefix = m_options.h.header_prefix;
		using SQLiteMaster = Table<sqlite_master>;
		SQLiteMaster tbl(m_db, WhereClauseArg(T_("where (type = 'table' or type = 'view') ") + to_tstring(AndWhereClause)));

		for (auto t : tbl)
		{
			m_options.h.header_prefix = OrgPrefix + t.type + "_";
			ProcessClassCreation(t.tbl_name);
		}
		m_options.h.header_prefix = OrgPrefix;
		std::ofstream myfile;
		std::string ClassName, HeaderUpper;
		if (CreateHeaderPrefix("Master_Header", myfile, ClassName, HeaderUpper, "", "", false))
		{
			for (auto s : m_HeadersCreated)
				myfile << "#include \"" << s << "\"" << std::endl;
			if (!m_options.m.exclude_main_hdr_example)
			{
				myfile << "\n" << std::endl;
				if (!m_options.m.exclude_comment_out_exampl)
					myfile << "/*" << std::endl;
				if (!m_options.m.exclude_comments)
				{
					myfile << "// This example code can be used to test and validate all tables." << std::endl;
					myfile << "// Example Usage:" << std::endl;
					myfile << "// \t\tqlite3pp::setGlobalDB(\"mydatabase.db\")" << std::endl;
					myfile << "// \t\tsqlite3pp::testAllTables();" << std::endl;
					myfile << "// Warning: testPopulatingAllTables and testAllTables may take a very long time on a database with a lot of content." << std::endl;
				}
				myfile << "#include <map>\n#include <memory>\nnamespace sqlite3pp\n{" << std::endl;

				// Function to create all table instances
				if (!m_options.m.exclude_comments)
					myfile << "\t// Function to test populating all tables & views." << std::endl;
				myfile << "\tstd::map< std::string, std::shared_ptr<sqlite3pp::TableBase> > testPopulatingAllTables()\n\t{" << std::endl;
				myfile << "\t\tstd::map< std::string, std::shared_ptr < sqlite3pp::TableBase>> Tables;" << std::endl;
				for (auto s : m_ClassNames)
					myfile << "\t\tTables[\"" << s << "\"] = std::shared_ptr<sqlite3pp::TableBase>(new sqlite3pp::Table<" << s << ">());" << std::endl;
				myfile << "\t\treturn Tables;\n\t}" << std::endl;

				// Function to test all tables
				myfile << std::endl;
				if (!m_options.m.exclude_comments)
					myfile << "\t// Function to test displaying content of all tables & views." << std::endl;
				myfile << "\tvoid testAllTables()\n\t{" << std::endl;
				myfile << "\t\tstd::map< std::string, std::shared_ptr < sqlite3pp::TableBase>> myTables = sqlite3pp::testPopulatingAllTables();" << std::endl;
				myfile << "\t\tfor (auto t : myTables)\n\t\t\tt.second->out(std::wcout);" << std::endl;
				myfile << "\t}" << std::endl;

				myfile << "}" << std::endl;
				if (!m_options.m.exclude_comment_out_exampl)
					myfile << "*/" << std::endl;
				myfile << "\n" << std::endl;
			}
			myfile << "\n#endif // !" << HeaderUpper << std::endl;
			myfile.close();
			V_COUT(DETAIL, "Finish creating Master_Header file.");
		}

		return true;
	}

	bool IsStrType(const char* str)
	{
		if (strcmp("Text", str) == 0 || strcmp("StrType", str) == 0 || strncmp("Character", str, 9) == 0 || strncmp("Varchar", str, 7) == 0)
			return true;
		if (strncmp("Nvarchar", str, 8) == 0 || strncmp("Nchar", str, 5) == 0)
			return true;
		return false;
	}

	bool SQLiteClassBuilder::CreateHeader(const std::string& TableName, const TblClassOptions *strtype, std::string QueryStr)
	{
		m_HeadersCreated.clear();
		m_ClassNames.clear();
		TblClassOptions options;
		if (strtype)
		{
			options = m_options;
			m_options = *strtype;
		}
		return ProcessClassCreation(TableName, QueryStr);
	}
	sqlite3pp::query* sql_base::CreateQuery(database& db, const std::string& QueryStr)
	{
		try
		{
			return new sqlite3pp::query(db, QueryStr.c_str());
		}
		catch (...)
		{
			return NULL;
		}
		return NULL;
	}

	bool SQLiteClassBuilder::ProcessClassCreation(const std::string& TableName, std::string QueryStr)
	{
		// ToDo: Change this code to use PRAGMA.  Example: PRAGMA table_info('v_PrgLangProfile');
		//		The pragma should return columns (cid, name, type, notnull, dflt_value, pk)
		const char* CommentSection = "////////////////////////////////////////////////////////////////////////////////////////////";
		if (QueryStr.empty())
			QueryStr = "SELECT * FROM \"" + TableName + "\"";
		std::shared_ptr < sqlite3pp::query> qry(sql_base::CreateQuery(m_db, QueryStr));
		if (!qry)
			return false;
		std::vector<std::pair<std::string, std::string> > columns;
		std::vector<std::pair<std::string, std::string> > columns_with_comma;
		std::string FirstColumnName;
		std::string LastColumnName = "get_MyColumnFoo()";
		for (int i = 0; i < qry->column_count(); ++i)
		{
			if (strstr(qry->column_name(i), ":") != NULL) continue;

			columns.push_back(std::pair<std::string, std::string>(qry->column_name(i), GetType(TableName, qry->column_name(i), qry->column_decltype(i))));
			columns_with_comma.push_back(std::pair<std::string, std::string>(qry->column_name(i), i ? ", " : ""));
			if (FirstColumnName.empty())
				FirstColumnName = qry->column_name(i);
			else
				LastColumnName = qry->column_name(i);
		}
		std::ofstream myfile;
		std::string ClassName, HeaderUpper;
		if (!CreateHeaderPrefix(TableName, myfile, ClassName, HeaderUpper, FirstColumnName, LastColumnName))
			return false;
		m_ClassNames.push_back(ClassName);
		////////////////////////////////////////////////////////////////////////////////////////////
		// Create Table/View class, and create a define type for strings
		myfile << "\nclass " << ClassName << ": public sqlite3pp::sql_base\n{\npublic:" << std::endl;
		myfile << "\tusing StrType = " << m_options.s.str_type << ";\n\tusing Text = StrType;" << std::endl;

		if (!m_options.m.exclude_table_interface)
		{
			if (!m_options.m.exclude_comments)
				myfile << "\n\t// Constructors" << std::endl;
			// These constructors are only useful if method setData is created.
			myfile << "\t" << ClassName << "() {}";  // Allow default constructor to still work
			if (!m_options.m.exclude_comments)
				myfile << " // Default constructor";
			myfile << std::endl;
			myfile << "\ttemplate <class T> " << ClassName << "(const T &t) { setData(t); }"; // This constructor allows data transfer from different tables/views having same data types and column names
			if (!m_options.m.exclude_comments)
				myfile << " // Allows data input from different (or same) tables/views having the same data types and column names";
			myfile << std::endl;

			if (!m_options.m.exclude_comments)
				myfile << "\n\t// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class" << std::endl;

			// Create getTableName member function. Needed for sqlite3pp::Table template class
			myfile << "\tstatic StrType getTableName() { return " << m_options.s.str_pre << " \"" << TableName << "\" " << m_options.s.str_post << "; }" << std::endl;

			// Create getColumnNames member function. Needed for sqlite3pp::Table template class
			myfile << "\tstatic StrType getColumnNames() { return " << m_options.s.str_pre << " \"";
			for (auto c : columns_with_comma)
				myfile << c.second << c.first;
			myfile << "\"" << m_options.s.str_post << "; }" << std::endl;

			// Create getSelecColumnNames member function. Needed for sqlite3pp::Table template class
			myfile << "\tstatic StrType getSelecColumnNames() { return " << m_options.s.str_pre << " \"";
			for (auto c : columns_with_comma)
				myfile << c.second << "\\\"" << c.first << "\\\"";
			myfile << "\"" << m_options.s.str_post << "; }" << std::endl;

			// Create GetValues member function. Needed for sqlite3pp::Table template class
			myfile << "\tStrType GetValues() const\n\t{\n\t\tStrType strtype;";
			std::string commaDel;
			for (auto c : columns)
			{
				myfile << "\n\t\tstrtype +=  " << m_options.s.str_pre << "\"" << commaDel << "'\"" <<  m_options.s.str_post 
					<< " + " << m_options.s.str_tostr << "( " << c.first << ") + " << m_options.s.str_pre << "\"'\""  << m_options.s.str_post << ";";
				commaDel = ",";
			}
			myfile << "\n\t\treturn strtype;\n\t}" << std::endl;

			// Create getStreamData member function. Needed for sqlite3pp::Table template class
			myfile << "\ttemplate<class T> void getStreamData( T q ) { q.getter() ";
			for (auto c : columns)
				myfile << " >> " << c.first;
			myfile << ";}" << std::endl;

			// Create setData member function. Used to transfer data from different tables/views having same data types and column names
			myfile << "\ttemplate <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names\n\t{" << std::endl;
			for (auto c : columns)
				myfile << "\t\t" << c.first << " = t.get_" << c.first << "();" << std::endl;
			myfile << "\t}" << std::endl;


			// Miscellaneous functions
			if (!m_options.m.exclude_comments)
				myfile << "\n\t// Miscellaneous functions" << std::endl;
			myfile << "\tstatic int getColumnCount() { return " << qry->column_count() << "; }" << std::endl;
		}

		// Define get function for each data member variable. Always create these functions if member variables are protected.
		if (m_options.m.exclude_get_functions != true || m_options.m.is_public_var_members != true)
		{
			if (!m_options.m.exclude_comments)
				myfile << "\n\t// A get_ function for each field in the table";
			if (m_options.m.is_public_var_members != true)
				myfile << ", which allows read-only access to protected member variables";
			myfile << "." << std::endl;
			for (auto c : columns)
				myfile << "\tconst " << c.second << "& get_" << c.first << "() const {return " << c.first << ";}" << std::endl;
		}

		// Define set function for each data member variable.
		if (m_options.m.exclude_set_functions != true)
		{
			if (!m_options.m.exclude_comments)
				myfile << "\n\t// A set_ function for each field in the table." << std::endl;
			for (auto c : columns)
				myfile << "\tvoid set_" << c.first << "(const " << c.second << "& data__) {" << c.first << " = data__;}" << std::endl;
		}

		if (!m_options.m.exclude_comments)
			myfile << "\n\t// A member variable for each field in the table" << std::endl;
		// Define if data member variables are protected or public
		const char* publicOrPrivate = m_options.m.is_public_var_members ? "public" : "protected";
		myfile << publicOrPrivate << ":" << std::endl;
		// Define data member variables associated with the table/view
		for (auto c : columns)
			myfile << "\t" << c.second << " " << c.first << ";" << std::endl;

		const std::string OperatorStreamComment1 = "/* sqlite3pp::TableOStream container interface.\n\tFunctions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.\n\tExample Usage:\t\t(Using sqlite3pp::TableOStream container)\n\t\t\tTableOStream<" + ClassName + "> tbl(DbFileNameArg(\"mydatabase.db\"));\n\t\t\ttbl.setDelimit(\"|\"); // Change delimiter\n\t\t\tstd::cout << tbl; // Send data to screen with the changed delimiter\n\n\t\t\tstd::ofstream ofs (\"data.csv\", std::ofstream::out);\n\t\t\ttbl.setDelimit(\",\"); // Change delimiter\n\t\t\tofs << tbl; // Write data to a CSV file using the changed \",\" delimiter.\n\n\t\t\ttbl.out(std::cout); // Send data to screen using out() member function.\n\tTo exclude TableOStream interface, set exclude_ostream_operator to true when creating this class using SQLiteClassBuilder.\n\t*/\n";
		const char* OperatorStreamComment2 = "// sqlite3pp::TableOStream container interface.\n";
		if (m_options.m.exclude_ostream_operator != true)
		{
			if (!m_options.m.exclude_comments)
				myfile << "\n\t" << CommentSection << "\n\t" << OperatorStreamComment1;
			////////////////////////////////////////////////////////////////////////////////////////////
			// Create function OStream
			myfile << "\ttemplate<class T> T& OStream(T& t) const\n\t{\n\t\tt.os";
			std::string delimiter_tmp;
			for (auto c : columns)
			{
				if (IsStrType(c.second.c_str()))
					myfile << delimiter_tmp << " << t.str(" << c.first << ")";
				else
					myfile << delimiter_tmp << " << " << c.first;
				if (delimiter_tmp.empty())
					delimiter_tmp = " << t.d";
			}
			myfile << ";\n\t\treturn t;\n\t}" << std::endl;
			////////////////////////////////////////////////////////////////////////////////////////////
			// Declare operator<< friends
			myfile << "\tfriend std::ostream& operator<<(std::ostream& os, const " << ClassName << "& t);" << std::endl;
			myfile << "\tfriend std::wostream& operator<<(std::wostream& os, const " << ClassName << "& t);" << std::endl;
			// Create Delimit member function. It's needed for operator<<
			myfile << "\tstatic StrType Delimiter() { return " << m_options.s.str_pre << " \"" << m_options.m.delimiter << "\" " << m_options.s.str_post << "; }" << std::endl;
			if (!m_options.m.exclude_comments)
				myfile << "\t" << CommentSection << std::endl;
		}


		// End of Table/View class
		myfile << "};" << std::endl;
		////////////////////////////////////////////////////////////////////////////////////////////

		if (m_options.m.exclude_ostream_operator != true)
		{
			if (!m_options.m.exclude_comments)
				myfile << OperatorStreamComment2;
			myfile << "inline std::ostream& operator<<(std::ostream& os, const " << ClassName << "& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }" << std::endl;
			myfile << "inline std::wostream& operator<<(std::wostream& os, const " << ClassName << "& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }" << std::endl;
		}

		myfile << "\n#endif // !" << HeaderUpper << std::endl;

		//Done
		myfile.close();
		V_COUT(DETAIL, "Finish creating class '" << ClassName << "' for table '" << TableName << "'");
		return true;
	}
};
