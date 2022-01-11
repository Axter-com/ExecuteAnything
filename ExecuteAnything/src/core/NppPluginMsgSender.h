#ifndef _npp_plugin_msg_sender_h_
#define _npp_plugin_msg_sender_h_
//---------------------------------------------------------------------------
#include "base.h"
#include "npp_files/PluginInterface.h"
#include "npp_files/menuCmdID.h"
#include <string>

class CNppPluginMsgSender
{
	protected:
		HWND m_hNppWnd;
		typedef std::basic_string<TCHAR> tstring;
		tstring m_srcModuleName;

	public:
		CNppPluginMsgSender(HWND hNppWnd, const TCHAR* srcModuleName) : 
		  m_hNppWnd(hNppWnd), m_srcModuleName(srcModuleName)
		{
		}
		tstring GetTextNppMsg(UINT MsgId);
		BOOL SendMsg(const TCHAR *destModuleName, long internalMsg, void* info);
		inline BOOL SendMsg(const TCHAR *destModuleName, UINT MsgId, WPARAM wparam, LPARAM lparam)	{ return (BOOL) ::SendMessage(m_hNppWnd, MsgId, wparam, lparam);}
		inline int SendMsg(UINT MsgId, WPARAM wparam = 0, LPARAM lparam = 0){	return (int) ::SendMessage(m_hNppWnd, MsgId, wparam, lparam);}
		inline void setSrcModuleName(const TCHAR* srcModuleName){	m_srcModuleName = srcModuleName;}

		// Directories and file path API's
		inline tstring getNppDirectory() { return GetTextNppMsg(NPPM_GETNPPDIRECTORY); }
		inline tstring getPluginsConfigDir() { return GetTextNppMsg(NPPM_GETPLUGINSCONFIGDIR); }
		inline tstring getPluginsHomeDir() { return GetTextNppMsg(NPPM_GETPLUGINHOMEPATH); }
		tstring getNppRoaming();

		// Get details on current document
		inline tstring getCurrentFileDirectory() { return GetTextNppMsg(NPPM_GETCURRENTDIRECTORY); }
		inline tstring getCurrentFileFullPath() { return GetTextNppMsg(NPPM_GETFULLCURRENTPATH); }
		inline tstring getCurrentFileName() { return GetTextNppMsg(NPPM_GETFILENAME); }
		inline tstring getCurrentFileNamePart() { return GetTextNppMsg(NPPM_GETNAMEPART); }
		inline tstring getCurrentFileExtPart() { return GetTextNppMsg(NPPM_GETEXTPART); }
		inline tstring getCurrentWord() { return GetTextNppMsg(NPPM_GETCURRENTWORD); }
		inline int getCurrentLine() { return SendMsg(NPPM_GETCURRENTLINE); }
		inline int getCurrentColumn() { return SendMsg(NPPM_GETCURRENTCOLUMN); }

		// File API's
		inline void switchToFile(const tstring &filename) { SendMsg(NPPM_SWITCHTOFILE, (WPARAM)0, (LPARAM)filename.c_str()); }
		inline void doOpenFile(const tstring &filename) { SendMsg(NPPM_DOOPEN, (WPARAM)0, (LPARAM)filename.c_str()); }
		inline void reloadFile(const tstring &filename, BOOL withAlert = false) { SendMsg(NPPM_RELOADFILE, (WPARAM)withAlert, (LPARAM)filename.c_str()); }
		inline void saveCurrentFileAs(const tstring &filename, BOOL asCopy = FALSE) { SendMsg(NPPM_SAVECURRENTFILEAS, (WPARAM)asCopy, (LPARAM)filename.c_str()); }
		inline void saveFile(const tstring &filename) { SendMsg(NPPM_SAVEFILE, (WPARAM)0, (LPARAM)filename.c_str()); }
		enum eVIEW_OPTION { eALL_OPEN_FILES = 0, ePRIMARY_VIEW = 1, eSECOND_VIEW = 2 };
		inline bool getNumberOfOpenFiles(eVIEW_OPTION type = eALL_OPEN_FILES) { return SendMsg(NPPM_GETNBOPENFILES, (WPARAM)0, (LPARAM)type) > 0 ? true : false; }

		// Set and get line number width mode
		enum eLINENUMBERWIDTHMODE { eLINENUMWIDTH_DYNAMIC = 0, eLINENUMWIDTH_CONSTANT = 1};
		inline bool setLineNoWidthMode(eLINENUMBERWIDTHMODE mode) { SendMsg(NPPM_SETLINENUMBERWIDTHMODE, (WPARAM)0, (LPARAM)mode) > 0 ? true : false; }
		inline int getLineNoWidthMode() { return SendMsg(NPPM_GETLINENUMBERWIDTHMODE); }
		
		// Misc API's
		inline void loadSession(const tstring &filename) { SendMsg(NPPM_LOADSESSION, (WPARAM)0, (LPARAM)filename.c_str()); }
		inline void launchFindInFilesDlg(const tstring &dir2Search, const tstring &filtre) { SendMsg(NPPM_LAUNCHFINDINFILESDLG, (WPARAM)dir2Search.c_str(), (LPARAM)filtre.c_str()); }
		inline void triggerTabbarContextMenu(int view, int index2Activate) { SendMsg(NPPM_TRIGGERTABBARCONTEXTMENU, (WPARAM)view, (LPARAM)index2Activate); }
		inline void disableAutoUpdate() { SendMsg(NPPM_DISABLEAUTOUPDATE); }
		inline void activateDoc(int view, int index2Activate) { SendMsg(NPPM_ACTIVATEDOC, (WPARAM)view, (LPARAM)index2Activate); }

		// ShortCut and Menu API's
		// Might need to call NPPM_INTERNAL_PLUGINSHORTCUTMOTIFIED (0x0400 + 26), to modify internal shortcuts.  Also look at NPPN_SHORTCUTREMAPPED, and see what the plugin has to do to handle the change.
		inline bool removeShortCutByCmdId(int cmdID) { return SendMsg(NPPM_REMOVESHORTCUTBYCMDID, cmdID) > 0 ? true : false; }
		inline bool getShortCutByCmdId(int cmdID, ShortcutKey &sk) { return SendMsg(NPPM_GETSHORTCUTBYCMDID, (WPARAM)cmdID, (LPARAM)&sk) > 0 ? true : false; }
		inline void setMenuCheck(int cmdID, bool isCheck) { SendMsg(NPPM_SETMENUITEMCHECK, (WPARAM)cmdID, (LPARAM)isCheck); }
		inline void sendMenuCmd(int menuItemID) { SendMsg(NPPM_MENUCOMMAND, (WPARAM)0, (LPARAM)menuItemID); } // Example usage: sendMenuCmd(IDM_FILE_CLOSE);  See "src\core\npp_files\menuCmdID.h" for other commands.
		inline void NppMenu_Exit() { sendMenuCmd(IDM_FILE_EXIT); }
		inline void NppMenu_FileClose() { sendMenuCmd(IDM_FILE_CLOSE); }
		inline void NppMenu_FileCloseAll() { sendMenuCmd(IDM_FILE_CLOSEALL); }
		inline void NppMenu_FileCloseAllToLeft() { sendMenuCmd(IDM_FILE_CLOSEALL_TOLEFT); }
		inline void NppMenu_FileCloseAllToRight() { sendMenuCmd(IDM_FILE_CLOSEALL_TORIGHT); }
		inline void NppMenu_FileCloseAllUnchanged() { sendMenuCmd(IDM_FILE_CLOSEALL_UNCHANGED); }
		inline void NppMenu_FileCloseAllButCurrent() { sendMenuCmd(IDM_FILE_CLOSEALL_BUT_CURRENT); }
		inline void NppMenu_FileSave() { sendMenuCmd(IDM_FILE_SAVE); }
		inline void NppMenu_FileSaveAll() { sendMenuCmd(IDM_FILE_SAVEALL); }
		inline void NppMenu_FileReload() { sendMenuCmd(IDM_FILE_RELOAD); }
		inline void NppMenu_FileRestoreLastClosed() { sendMenuCmd(IDM_FILE_RESTORELASTCLOSEDFILE); }
		inline void NppMenu_FileContainingFolderAsWorkspace() { sendMenuCmd(IDM_FILE_CONTAININGFOLDERASWORKSPACE); }
		inline void NppMenu_FileSaveSession() { sendMenuCmd(IDM_FILE_SAVESESSION); }
		inline void NppMenu_FileOpenAllRecentFiles() { sendMenuCmd(IDM_OPEN_ALL_RECENT_FILE); }
		inline void NppMenu_EditOpenAsFile() { sendMenuCmd(IDM_EDIT_OPENASFILE); }
		inline void NppMenu_SearchInternet() { sendMenuCmd(IDM_EDIT_SEARCHONINTERNET); }
		enum eVIEWBROWSER_TYPE{ FIREFOX , CHROME, EDGE, IE};
		void NppMenu_ViewInBrowser(eVIEWBROWSER_TYPE eviewbrowser_type = CHROME);
		inline void NppMenu_SettingsShortCutMapper() { sendMenuCmd(IDM_SETTING_SHORTCUT_MAPPER); }
		inline void NppMenu_SettingsShortCutMapperMacro() { sendMenuCmd(IDM_SETTING_SHORTCUT_MAPPER_MACRO); }
		inline void NppMenu_SettingsShortCutMapperRun() { sendMenuCmd(IDM_SETTING_SHORTCUT_MAPPER_RUN); }
		inline void NppMenu_SettingsEditContentMenu() { sendMenuCmd(IDM_SETTING_EDITCONTEXTMENU); }
		inline void NppMenu_SettingsOpenPluginDir() { sendMenuCmd(IDM_SETTING_OPENPLUGINSDIR); }
		inline void NppMenu_SettingsImportPlugin() { sendMenuCmd(IDM_SETTING_IMPORTPLUGIN); }

		inline void NppMenu_Run() { sendMenuCmd(IDM_EXECUTE); }
};

//---------------------------------------------------------------------------
#endif
