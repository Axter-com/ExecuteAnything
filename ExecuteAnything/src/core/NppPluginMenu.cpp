#include "NppPluginMenu.h"

CNppMessager CNppPluginMenu::m_nppMsgr;

CNppPluginMenu::CNppPluginMenu()
{
}

CNppPluginMenu::~CNppPluginMenu()
{
}

HMENU CNppPluginMenu::GetSubMenu( HMENU hMenu, const tstring & SubMenuName )
{
	if ( !hMenu ) return hMenu;
	const int MaxLen = 256;
	const UINT QtyItems = GetMenuItemCount( hMenu );
	for ( UINT i = 0; i < QtyItems; ++i )
	{
		TCHAR MenuName[MaxLen + 1] = {0};
		GetMenuString( hMenu, i, MenuName, MaxLen, MF_BYPOSITION );
		if ( lstrcmpi( MenuName, SubMenuName.c_str() ) == 0 )
			return ::GetSubMenu( hMenu, i );
	}
	return NULL;
}

void CNppPluginMenu::setNppData( CNppMessager &cnppmessager, const NppData & nppd, const tstring &PlugName )
{
	// ToDo: Add logic to get Npp main menu handle
	//		Try by using GetMenu()
	MENUBARINFO mbi = {sizeof( MENUBARINFO )};
	GetMenuBarInfo( nppd._nppHandle, OBJID_MENU, 0, &mbi );
	HMENU nppMainMenu = mbi.hMenu;
	if ( !nppMainMenu )
	{
		nppMainMenu = GetMenu( nppd._scintillaMainHandle );
		if ( !nppMainMenu )
		{
			HWND parent = GetParent( nppd._scintillaMainHandle );
			if ( parent )
				nppMainMenu = GetMenu( parent );
			else
			{
				parent = GetParent( nppd._nppHandle );
				nppMainMenu = GetMenu( parent );
			}
		}
	}
	HMENU nppPluginMenu = GetMenu( nppd._nppHandle );
	HMENU thisPluginMenu = GetSubMenu( nppPluginMenu,  PlugName);
	cnppmessager.setNppData( nppd, nppMainMenu, nppPluginMenu, thisPluginMenu );
}

void CNppPluginMenu::setNppData(const NppData& nppd, const tstring &PlugName )
{
	setNppData( m_nppMsgr, nppd, PlugName);
}
