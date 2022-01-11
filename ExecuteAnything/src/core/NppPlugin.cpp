#include "NppPlugin.h"
#include <string.h>



CNppPlugin::CNppPlugin()
{
	m_hDllModule = NULL;
	m_szDllFileName[0] = 0;
	m_szIniFileName[0] = 0;
	m_szPluginName[0] = 0;
}

CNppPlugin::~CNppPlugin()
{
}

HINSTANCE CNppPlugin::getHinstance()
{
	return (HINSTANCE)m_hDllModule;
}

void CNppPlugin::OnDllProcessAttach(HINSTANCE hDLLInstance)
{
	int   nLen;
	TCHAR szPath[2*MAX_PATH + 1];
	
	m_hDllModule = (HMODULE) hDLLInstance;
	nLen = (int) ::GetModuleFileName(m_hDllModule, szPath, 2*MAX_PATH);
	while ( nLen-- > 0 )
	{
		if ( (szPath[nLen] == _T('\\')) || (szPath[nLen] == _T('/')) )
		{
			lstrcpy(m_szDllFileName, szPath + nLen + 1);
			lstrcpy(m_szIniFileName, m_szDllFileName);
			break;
		}
	}

	nLen = lstrlen(m_szIniFileName);
	while ( nLen-- > 0 )
	{
		if ( m_szIniFileName[nLen] == _T('.') )
		{
			m_szIniFileName[nLen] = 0;
			lstrcpy( m_szPluginName, m_szIniFileName );
			lstrcat(m_szIniFileName, _T(".ini"));
			break;
		}
	}
}

void CNppPlugin::OnDllProcessDetach()
{
}

void CNppPlugin::nppSetInfo(const NppData& nppd, const tstring &PlugName )
{
	OnNppSetInfo(nppd, PlugName);
}

void CNppPlugin::OnNppSetInfo( const NppData& nppd, const tstring &PlugName )
{
	CNppPluginMenu::setNppData( m_nppMsgr, nppd, PlugName );
}



/*
	// Menu API examples
	//	DeleteMenu(GetMenu(g_nppData._nppHandle),funcItem[findmenuitem(pfgethtmltidyweb)]._cmdID, MF_BYCOMMAND);
	//	EnableMenuItem(GetMenu(g_nppData._nppHandle), funcItem[findmenuitem(pfhtmltidy)]._cmdID, MF_BYCOMMAND);
	//	CheckMenuItem(GetMenu(g_nppData._nppHandle), funcItem[itemno]._cmdID, MF_BYCOMMAND | ((funcItem[itemno]._init2Check)?MF_CHECKED:MF_UNCHECKED));
	//	DrawMenuBar(g_nppData._nppHandle);

	// Save/Load menu option example
	//	WritePrivateProfileString("Settings","SeparateQuickMenus"       ,(funcItem[g_miSeparateQuickMenus       ]._init2Check)?"1":"0",szINIPath);
	//	altercheck(g_miSeparateQuickMenus,GetPrivateProfileInt("Settings", "SeparateQuickMenus", 0, szINIPath)?'1':'0');
Example to build menu:
EXTERNC void mpxbuildmenu(char *buf,unsigned idx,WORD ic) {
  MENUITEMINFO mi;
		freeIDLOOKUP(idx);
  mallocIDLOOKUP(idx,ic);
  g_wIDlookup[idx].appdata1=buf;
  //DeleteMenu(g_wIDlookup[idx].hSubMenu,funcItem[g_wIDlookup[idx].mi]._cmdID,MF_BYCOMMAND);
  char *c=g_wIDlookup[idx].appdata1; USHORT *cs=(USHORT *)c;
  WORD i; for(i=0; i<ic; i++) {
	//MessageBoxFree(g_nppData._nppHandle,smprintf("i:%u str:(%x + %u)",i,c,cs[i]),PLUGIN_NAME, MB_OK|MB_ICONWARNING);
	mi.cbSize=sizeof(mi); mi.fMask=MIIM_ID|MIIM_TYPE; mi.wID=g_wIDlookup[idx].wParamLo+i; mi.dwTypeData=c+cs[i]; mi.cch=strlen(mi.dwTypeData); mi.fType=MFT_STRING; InsertMenuItem(g_wIDlookup[idx].hSubMenu,GetMenuItemCount(g_wIDlookup[idx].hSubMenu),TRUE,&mi);
  }
}

*/