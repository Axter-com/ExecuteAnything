/* This file was automatically generated using [Sqlite3pp_EZ].
Sqlite3pp_EZ Copyright (C) 2021 David Maisonave (http::\www.axter.com)
Example Usage:		(Using sqlite3pp::Table container)
	// Exampel #1
		sqlite3pp::setGlobalDB("mydatabase.db");
		sqlite3pp::Table<sql_table_InstallationPaths> my_tbl;
		for (auto row : my_tbl)
			std::wcout << row << std::endl;

	// Exampel #2
		for (int i = 0; i < my_tbl.size(); ++i)
			std::wcout << my_tbl[i].get_PrgLangName() << std::endl;

	// Exampel #3
		for (auto r = my_tbl.begin(); r != my_tbl.end(); ++r)
			std::wcout << r->get_Is_UserDef() << std::endl;

For more details see  https://github.com/David-Maisonave/sqlite3pp_EZ
*/
#ifndef SQL_TABLE_INSTALLATIONPATHS_H
#define SQL_TABLE_INSTALLATIONPATHS_H
#include "..\sqlite3pp_ez.h"

class sql_table_InstallationPaths: public sqlite3pp::sql_base
{
public:
	using StrType = sqlite3pp::tstring;
	using Text = StrType;

	// Constructors
	sql_table_InstallationPaths() {} // Default constructor
	template <class T> sql_table_InstallationPaths(const T &t) { setData(t); } // Allows data input from different (or same) tables/views having the same data types and column names

	// getTableName, getColumnNames, getSelecColumnNames, and getStreamData are required for sqlite3pp::Table template class
	static StrType getTableName() { return T_( "InstallationPaths" ); }
	static StrType getColumnNames() { return T_( "PrgLangName, ProfileName, Reference, DefaultInstallPath, SrcBinUrlOrUnc, SrcHomeWebsite, InstalledPath, Info, ValidationBinFileName, ValidationTextFileName, ValidationFileContent, PathType, InstallInstructions, Notes, Is_x64, Is_x86, Is_UserDef"); }
	static StrType getSelecColumnNames() { return T_( "\"PrgLangName\", \"ProfileName\", \"Reference\", \"DefaultInstallPath\", \"SrcBinUrlOrUnc\", \"SrcHomeWebsite\", \"InstalledPath\", \"Info\", \"ValidationBinFileName\", \"ValidationTextFileName\", \"ValidationFileContent\", \"PathType\", \"InstallInstructions\", \"Notes\", \"Is_x64\", \"Is_x86\", \"Is_UserDef\""); }
	StrType GetValues() const
	{
		StrType strtype;
		strtype +=  T_("'") + sqlite3pp::to_tstring( PrgLangName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ProfileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Reference) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( DefaultInstallPath) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SrcBinUrlOrUnc) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( SrcHomeWebsite) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( InstalledPath) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Info) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ValidationBinFileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ValidationTextFileName) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( ValidationFileContent) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( PathType) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( InstallInstructions) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Notes) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Is_x64) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Is_x86) + T_("'");
		strtype +=  T_(",'") + sqlite3pp::to_tstring( Is_UserDef) + T_("'");
		return strtype;
	}
	template<class T> void getStreamData( T q ) { q.getter()  >> PrgLangName >> ProfileName >> Reference >> DefaultInstallPath >> SrcBinUrlOrUnc >> SrcHomeWebsite >> InstalledPath >> Info >> ValidationBinFileName >> ValidationTextFileName >> ValidationFileContent >> PathType >> InstallInstructions >> Notes >> Is_x64 >> Is_x86 >> Is_UserDef;}
	template <class T> void setData(const T &t) // Used to transfer data from different tables/views having same data types and column names
	{
		PrgLangName = t.get_PrgLangName();
		ProfileName = t.get_ProfileName();
		Reference = t.get_Reference();
		DefaultInstallPath = t.get_DefaultInstallPath();
		SrcBinUrlOrUnc = t.get_SrcBinUrlOrUnc();
		SrcHomeWebsite = t.get_SrcHomeWebsite();
		InstalledPath = t.get_InstalledPath();
		Info = t.get_Info();
		ValidationBinFileName = t.get_ValidationBinFileName();
		ValidationTextFileName = t.get_ValidationTextFileName();
		ValidationFileContent = t.get_ValidationFileContent();
		PathType = t.get_PathType();
		InstallInstructions = t.get_InstallInstructions();
		Notes = t.get_Notes();
		Is_x64 = t.get_Is_x64();
		Is_x86 = t.get_Is_x86();
		Is_UserDef = t.get_Is_UserDef();
	}

	// Miscellaneous functions
	static int getColumnCount() { return 17; }

	// A get_ function for each field in the table, which allows read-only access to protected member variables.
	const Text& get_PrgLangName() const {return PrgLangName;}
	const Text& get_ProfileName() const {return ProfileName;}
	const Text& get_Reference() const {return Reference;}
	const Text& get_DefaultInstallPath() const {return DefaultInstallPath;}
	const Text& get_SrcBinUrlOrUnc() const {return SrcBinUrlOrUnc;}
	const Text& get_SrcHomeWebsite() const {return SrcHomeWebsite;}
	const Text& get_InstalledPath() const {return InstalledPath;}
	const Text& get_Info() const {return Info;}
	const Text& get_ValidationBinFileName() const {return ValidationBinFileName;}
	const Text& get_ValidationTextFileName() const {return ValidationTextFileName;}
	const Text& get_ValidationFileContent() const {return ValidationFileContent;}
	const Text& get_PathType() const {return PathType;}
	const Text& get_InstallInstructions() const {return InstallInstructions;}
	const Text& get_Notes() const {return Notes;}
	const Boolean& get_Is_x64() const {return Is_x64;}
	const Boolean& get_Is_x86() const {return Is_x86;}
	const Boolean& get_Is_UserDef() const {return Is_UserDef;}

	// A set_ function for each field in the table.
	void set_PrgLangName(const Text& data__) {PrgLangName = data__;}
	void set_ProfileName(const Text& data__) {ProfileName = data__;}
	void set_Reference(const Text& data__) {Reference = data__;}
	void set_DefaultInstallPath(const Text& data__) {DefaultInstallPath = data__;}
	void set_SrcBinUrlOrUnc(const Text& data__) {SrcBinUrlOrUnc = data__;}
	void set_SrcHomeWebsite(const Text& data__) {SrcHomeWebsite = data__;}
	void set_InstalledPath(const Text& data__) {InstalledPath = data__;}
	void set_Info(const Text& data__) {Info = data__;}
	void set_ValidationBinFileName(const Text& data__) {ValidationBinFileName = data__;}
	void set_ValidationTextFileName(const Text& data__) {ValidationTextFileName = data__;}
	void set_ValidationFileContent(const Text& data__) {ValidationFileContent = data__;}
	void set_PathType(const Text& data__) {PathType = data__;}
	void set_InstallInstructions(const Text& data__) {InstallInstructions = data__;}
	void set_Notes(const Text& data__) {Notes = data__;}
	void set_Is_x64(const Boolean& data__) {Is_x64 = data__;}
	void set_Is_x86(const Boolean& data__) {Is_x86 = data__;}
	void set_Is_UserDef(const Boolean& data__) {Is_UserDef = data__;}

	// A member variable for each field in the table
protected:
	Text PrgLangName;
	Text ProfileName;
	Text Reference;
	Text DefaultInstallPath;
	Text SrcBinUrlOrUnc;
	Text SrcHomeWebsite;
	Text InstalledPath;
	Text Info;
	Text ValidationBinFileName;
	Text ValidationTextFileName;
	Text ValidationFileContent;
	Text PathType;
	Text InstallInstructions;
	Text Notes;
	Boolean Is_x64;
	Boolean Is_x86;
	Boolean Is_UserDef;

	////////////////////////////////////////////////////////////////////////////////////////////
	/* sqlite3pp::TableOStream container interface.
	Functions OStream(), operator<<(), and Delimiter() are required when using the sqlite3pp::TableOStream container.
	Example Usage:		(Using sqlite3pp::TableOStream container)
			TableOStream<sql_table_InstallationPaths> tbl(DbFileNameArg("mydatabase.db"));
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
		t.os << t.str(PrgLangName) << t.d << t.str(ProfileName) << t.d << t.str(Reference) << t.d << t.str(DefaultInstallPath) << t.d << t.str(SrcBinUrlOrUnc) << t.d << t.str(SrcHomeWebsite) << t.d << t.str(InstalledPath) << t.d << t.str(Info) << t.d << t.str(ValidationBinFileName) << t.d << t.str(ValidationTextFileName) << t.d << t.str(ValidationFileContent) << t.d << t.str(PathType) << t.d << t.str(InstallInstructions) << t.d << t.str(Notes) << t.d << Is_x64 << t.d << Is_x86 << t.d << Is_UserDef;
		return t;
	}
	friend std::ostream& operator<<(std::ostream& os, const sql_table_InstallationPaths& t);
	friend std::wostream& operator<<(std::wostream& os, const sql_table_InstallationPaths& t);
	static StrType Delimiter() { return T_( "," ); }
	////////////////////////////////////////////////////////////////////////////////////////////
};
// sqlite3pp::TableOStream container interface.
inline std::ostream& operator<<(std::ostream& os, const sql_table_InstallationPaths& t) { sqlite3pp::ostream_a o(os, t.Delimiter()); return t.OStream(o).os; }
inline std::wostream& operator<<(std::wostream& os, const sql_table_InstallationPaths& t) { sqlite3pp::ostream_w o(os, t.Delimiter());  return t.OStream(o).os; }

#endif // !SQL_TABLE_INSTALLATIONPATHS_H
