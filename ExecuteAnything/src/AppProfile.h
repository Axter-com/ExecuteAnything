#ifndef __APPPROFILE_H__
#define __APPPROFILE_H__
#include <afxwin.h>
#include <string>
typedef std::basic_string<TCHAR> tstring;
class AppProfile
{
protected:
	static tstring		DefaultAppName;
	static const int	MaxBufferSize;
	tstring				m_AppName;
	tstring				m_KeyName;
	DWORD				m_LastError;
public:
	inline tstring GetKeyName()	{return m_KeyName;}
	inline tstring GetAppName(){return m_AppName;}
	AppProfile( tstring KeyName, tstring AppName = DefaultAppName );
	~AppProfile();
	void SetKeyName( tstring KeyName);
	void SetAppName( const tstring &AppName );
	void SetDefaultAppName( const tstring &AppName );
	bool RenameKeyName( const tstring &strKey_OldName, const tstring &strKey_NewName );


	// data reading functions
	bool Read( COleDateTime &dtDefault );
	bool Read( double &fDefault );
	bool Read( tstring &strDefault );
	bool Read( int &nDefault );
	bool Read( bool &bDefault );
	bool Read( DWORD &dwDefault );  // Can be used with COLORREF
	bool Read( CFont &pFont );
	bool Read( CPoint &pPoint );
	bool Read( CSize &pSize );
	bool Read( CRect &pRect );
	int Read( byte* BinaryData, int MaxRead ); //Returns amount read
	// data reading functions
	AppProfile& operator>>( tstring &strValue );
	AppProfile& operator>>( CFont &pFont );
	AppProfile& operator>>( CPoint &pPoint );
	AppProfile& operator>>( CSize &pSize );
	AppProfile& operator>>( CRect &pRect );
	AppProfile& operator>>( double &fValue );
	AppProfile& operator>>( int &nValue );
	AppProfile& operator>>( DWORD &dwValue ); // Can be used with COLORREF
	AppProfile& operator>>( bool &bValue );
	// data reading functions
	bool Read( const tstring &KeyName, COleDateTime &dtDefault );
	bool Read( const tstring &KeyName, double &fDefault );
	bool Read( const tstring &KeyName, tstring &strDefault );
	bool Read( const tstring &KeyName, int &nDefault );
	bool Read( const tstring &KeyName, bool &bDefault );
	bool Read( const tstring &KeyName, DWORD &dwDefault );  // Can be used with COLORREF
	bool Read( const tstring &KeyName, CFont &pFont );
	bool Read( const tstring &KeyName, CPoint &pPoint );
	bool Read( const tstring &KeyName, CSize &pSize );
	bool Read( const tstring &KeyName, CRect &pRect );
	int Read( const tstring &KeyName, byte* BinaryData, int MaxRead ); //Returns amount read

	// data writing functions
	bool Write( const COleDateTime &dtValue );
	bool Write( const tstring &strValue );
	bool Write( const CFont &pFont );
	bool Write( const CPoint &pPoint );
	bool Write( const CSize &pSize );
	bool Write( const CRect &pRect );
	bool Write( double fValue );
	bool Write( int nValue );
	bool Write( const DWORD &dwValue );  // Can be used with COLORREF
	bool Write( const bool &bValue );
	bool Write( byte* BinaryData, int &SizeOfBinaryData );
	// data writing functions
	AppProfile& operator<<(const COleDateTime &dtValue );
	AppProfile& operator<<(const tstring &strValue );
	AppProfile& operator<<(const CFont &pFont );
	AppProfile& operator<<(const CPoint &pPoint );
	AppProfile& operator<<(const CSize &pSize );
	AppProfile& operator<<(const CRect &pRect );
	AppProfile& operator<<(double fValue );
	AppProfile& operator<<(int nValue );
	AppProfile& operator<<(const DWORD &dwValue ); // Can be used with COLORREF
	AppProfile& operator<<(const bool &bValue );
	// data writing functions
	bool Write( const tstring &KeyName, const COleDateTime &dtValue );
	bool Write( const tstring &KeyName, const tstring &strValue );
	bool Write( const tstring &KeyName, const CFont &pFont );
	bool Write( const tstring &KeyName, const CPoint &pPoint );
	bool Write( const tstring &KeyName, const CSize &pSize );
	bool Write( const tstring &KeyName, const CRect &pRect );
	bool Write( const tstring &KeyName, double fValue );
	bool Write( const tstring &KeyName, int nValue );
	bool Write( const tstring &KeyName, const DWORD &dwValue );  // Can be used with COLORREF
	bool Write( const tstring &KeyName, const bool &bValue );
	bool Write( const tstring &KeyName, byte* BinaryData, int &SizeOfBinaryData );

};

#endif //__APPPROFILE_H__