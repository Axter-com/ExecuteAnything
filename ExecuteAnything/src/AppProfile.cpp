#include "AppProfile.h"

tstring		AppProfile::DefaultAppName	= _T( "FooFoo31" );
const int	AppProfile::MaxBufferSize	= 32000;

AppProfile::AppProfile( tstring KeyName, tstring AppName ) : m_AppName( AppName ), m_KeyName( KeyName )
{
}

AppProfile::~AppProfile()
{
}

void AppProfile::SetKeyName( tstring KeyName )
{
	m_KeyName = KeyName;
}

void AppProfile::SetAppName( const tstring & AppName )
{
	m_AppName = AppName;
}

void AppProfile::SetDefaultAppName( const tstring & AppName )
{
	DefaultAppName = AppName;
}

bool AppProfile::RenameKeyName( const tstring & strKey_OldName, const tstring & strKey_NewName )
{
	return false;
}

bool AppProfile::Read( COleDateTime & value )
{
	return false;
}

bool AppProfile::Read( double & value )
{
	return false;
}

bool AppProfile::Read( tstring & value )
{
	TCHAR buffer[MaxBufferSize] = {0};
	bool ReturnValue = GetProfileString( m_AppName.c_str(), m_KeyName.c_str(), value.c_str(), buffer, MaxBufferSize ) == TRUE;
	value = buffer;
	return ReturnValue;
}

bool AppProfile::Read( int & value )
{
	return false;
}

bool AppProfile::Read( bool & value )
{
	return false;
}

bool AppProfile::Read( DWORD & value )
{
	return false;
}

bool AppProfile::Read( CFont & value )
{
	return false;
}

bool AppProfile::Read( CPoint & value )
{
	return false;
}

bool AppProfile::Read( CSize & value )
{
	return false;
}

bool AppProfile::Read( CRect & value )
{
	return false;
}

int AppProfile::Read( byte * BinaryData, int MaxRead )
{
	return 0;
}


AppProfile & AppProfile::operator>>( tstring & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( CFont & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( CPoint & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( CSize & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( CRect & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( double &value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( int &value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( DWORD & value )
{
	Read( value );
	return *this;
}

AppProfile & AppProfile::operator>>( bool & value )
{
	Read( value );
	return *this;
}

bool AppProfile::Read( const tstring & KeyName, COleDateTime & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, double & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, tstring & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, int & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, bool & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, DWORD & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, CFont & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, CPoint & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, CSize & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

bool AppProfile::Read( const tstring & KeyName, CRect & value )
{
	m_KeyName = KeyName;
	return Read( value );
}

int AppProfile::Read( const tstring & KeyName, byte * BinaryData, int MaxRead )
{
	return 0;
}

bool AppProfile::Write( const COleDateTime & value )
{
	return false;
}

bool AppProfile::Write( const tstring & value )
{
	m_LastError = 0;
	bool RtrnValue = WriteProfileString( m_AppName.c_str(), m_KeyName.c_str(), value.c_str() ) == TRUE;
	if ( !RtrnValue )
		m_LastError = GetLastError();
	return RtrnValue;
}

bool AppProfile::Write( const CFont & value )
{
	return false;
}

bool AppProfile::Write( const CPoint & value )
{
	return false;
}

bool AppProfile::Write( const CSize & value )
{
	return false;
}

bool AppProfile::Write( const CRect & value )
{
	return false;
}

bool AppProfile::Write( double value )
{
	return false;
}

bool AppProfile::Write( int value )
{
	return false;
}

bool AppProfile::Write( const DWORD & value )
{
	return false;
}

bool AppProfile::Write( const bool & value )
{
	return false;
}

bool AppProfile::Write( byte * BinaryData, int & SizeOfBinaryData )
{
	return false;
}

AppProfile & AppProfile::operator<<( const COleDateTime & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const tstring & value )
{
	bool results = Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const CFont & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const CPoint & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const CSize & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const CRect & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( double value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( int value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const DWORD & value )
{
	Write( value );
	return *this;
}

AppProfile & AppProfile::operator<<( const bool & value )
{
	Write( value );
	return *this;
}

bool AppProfile::Write( const tstring & KeyName, const COleDateTime & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const tstring & value )
{
	m_KeyName = KeyName;
	return Write(value);
}

bool AppProfile::Write( const tstring & KeyName, const CFont & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const CPoint & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const CSize & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const CRect & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, double value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, int value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const DWORD & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, const bool & value )
{
	m_KeyName = KeyName;
	return Write( value );
}

bool AppProfile::Write( const tstring & KeyName, byte * BinaryData, int & SizeOfBinaryData )
{
	return false;
}
