/*
	Copyright (C) 2022 David Maisonave (https://www.axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "ExecuteAnything.h"
#include "NppExecPluginMsgSender.h"
#include "ClipboardXX.hpp"
#include "sqlite3pp_ez.h"
#include "NppToIDE.h"

const TCHAR* const ExecuteAnything::PLUGIN_NAME = _T("Execute Anything (Universal IDE)");
const TCHAR* const ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID = _T("EXECUTEANYTHING_CALLBACK_ID");
const TCHAR* const ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION = _T("EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION");

bool ExecuteAnything::NotificationFlag = false;
tstring ExecuteAnything::LastConsoleContent;
bool ExecuteAnything::TestAutomationCanceled = false;
bool ExecuteAnything::FoundSourceDb = false;
tstring ExecuteAnything::FoundSourceDbPath;
tstring ExecuteAnything::FirstDestFailedToCopyDbPath;

bool ExecuteAnything::ValidateDestPath(const tstring &OrgSourceDBPath, const tstring &ActiveDbPath)
{
	if (!FoundSourceDb && sqlite3pp::file_exists(OrgSourceDBPath, true))
	{// Only set FoundSourceDbPath to the first DB found
		FoundSourceDb = true;
		FoundSourceDbPath = OrgSourceDBPath;
	}

	if (sqlite3pp::file_exists(ActiveDbPath, true))
		return true;
	if (!sqlite3pp::file_exists(OrgSourceDBPath, true))
		return false;
	if (sqlite3pp::copy_file(OrgSourceDBPath, ActiveDbPath))
		return true;
	else if (FirstDestFailedToCopyDbPath.empty())
		FirstDestFailedToCopyDbPath = ActiveDbPath;
	return false;
}

sqlite3pp::database &ExecuteAnything::GetExecuteAnythingDB()
{
	static sqlite3pp::database &ExecuteAnythingDB(sqlite3pp::setGlobalDB(ExecuteAnything::GetExecuteAnythingDBPath()));
	return ExecuteAnythingDB;
}

sqlite3pp::DbFileNameArg &ExecuteAnything::GetExecuteAnything_DbFileNameArg()
{
	static sqlite3pp::DbFileNameArg dbfilenamearg(ExecuteAnything::GetExecuteAnythingDBPath());
	return dbfilenamearg;
}

tstring ExecuteAnything::GetExecuteAnythingDBPath()
{
	CNppExecPluginMsgSender npeMsgr(Get_Program().getNppWnd(), Get_Program().getDllFileName());
	FoundSourceDb = false;
	FoundSourceDbPath.clear();
	FirstDestFailedToCopyDbPath.clear();
	static const tstring DbFileName = _T("\\ExecuteAnything_active.db");
	static const tstring DbFileNameOrg = _T("\\ExecuteAnything.db");
	static const tstring ActiveDbPaths[] = {
		npeMsgr.getPluginsConfigDir() + DbFileName  // If default destination path doesn't work, try other likely paths
		, npeMsgr.getNppDirectory() + _T("\\plugins\\Config") + DbFileName
		, npeMsgr.getNppDirectory() + DbFileName
		, npeMsgr.getPluginsConfigDir() + DbFileName
		, _T("%APPDATA%\\Notepad++\\plugins\\Config") + DbFileName  // This should evaluate to the same path as the first, but try checking explicitly anyway.
		, _T("%HOMEDRIVE%%HOMEPATH%\\Documents\\Notepad++") + DbFileName
		, _T("%HOMEDRIVE%%HOMEPATH%\\Documents") + DbFileName
		, _T("%HOMEDRIVE%%HOMEPATH%\\Notepad++") + DbFileName
		, _T("%HOMEDRIVE%%HOMEPATH%") + DbFileName
		, _T("%HOMEDRIVE%%HOMEPATH%\\.npp") + DbFileName
	};

	static const tstring OrgSourceDBPaths[] = {
		npeMsgr.getPluginsHomeDir() + _T("\\ExecuteAnything") + DbFileNameOrg  // If default source path doesn't work, try other likely paths
		, _T("%APPDATA%\\Notepad++\\plugins\\ExecuteAnything") + DbFileNameOrg // This should evaluate to the same path as the first, but try checking explicitly anyway.
		, npeMsgr.getNppDirectory() + _T("\\plugins\\ExecuteAnything") + DbFileNameOrg
		, npeMsgr.getNppDirectory() + DbFileNameOrg
		, npeMsgr.getPluginsConfigDir() + DbFileNameOrg
	};

	static tstring LastFoundActiveDbPathDurringThisSession; // Once active DB path is found, only perform search once per session
	if (LastFoundActiveDbPathDurringThisSession.size())
		return LastFoundActiveDbPathDurringThisSession;

	for (auto ActiveDbPath : ActiveDbPaths)
	{
		for (auto OrgSourceDBPath : OrgSourceDBPaths)
		{
			if (ValidateDestPath(OrgSourceDBPath, ActiveDbPath))
			{
				LastFoundActiveDbPathDurringThisSession = ActiveDbPath;
				// ToDo: Add logic to check if FoundSourceDbPath has a newer DB version, and if so, migrate it's content to LastFoundActiveDbPathDurringThisSession
				return LastFoundActiveDbPathDurringThisSession;
			}
		}
	}

	if (FoundSourceDb)
	{
		tstring szText = _T("Found source database at '") + FoundSourceDbPath + _T("'\n");
		if (FirstDestFailedToCopyDbPath.size())
			szText += _T("Could not copy source database to '") + FirstDestFailedToCopyDbPath + _T("'!\n");
		else
			szText += _T("Could not copy source database!\n");
		szText += _T("Check if destination path has write access for current user.\n");
		::MessageBox(Get_Program().getNppWnd(), szText.c_str(), _T("Could not copy ExecuteAnything database!"), MB_OK | MB_ICONERROR);
	}
	else
	{
		const tstring Header = _T("Could not find ExecuteAnything database!");
		tstring szText = Header + _T("\n\nFailed to find '") + DbFileNameOrg + _T("' in any of the following paths:\n");
		for (auto OrgSourceDBPath : OrgSourceDBPaths)
			szText += OrgSourceDBPath + _T("\n");

		szText += _T("\nConsider downloading the database from https://github.com/David-Maisonave/ExecuteAnything/blob/master/ExecuteAnything/src/SQL/db/ExecuteAnything.db\n");
		szText += _T("And saving it to the following path:") + OrgSourceDBPaths[0] + _T("\n");
		szText += _T("Or reinstall ExecuteAnything.\n");

		::MessageBox(Get_Program().getNppWnd(), szText.c_str(), Header.c_str(), MB_OK | MB_ICONERROR);
	}
	return _T("");// ToDo: Consider adding excetion here.
}

tstring ExecuteAnything::GetLastConsoleContent()
{
	return LastConsoleContent;
}

void ExecuteAnything::ResetLastConsoleContent()
{
	LastConsoleContent.clear();
}

void ExecuteAnything::ResetCancelTestAutomation()
{
	TestAutomationCanceled = false;
}

void ExecuteAnything::CancelTestAutomation()
{
	TestAutomationCanceled = true;
	ResetNotificationFlag();
	ResetLastConsoleContent();
}
void ExecuteAnything::ResetNotificationFlag()
{
	NotificationFlag = false;
}

bool ExecuteAnything::ReceivedNotification()
{
	return NotificationFlag;
}

FuncItem* ExecuteAnything::nppGetFuncsArray( int* pnbFuncItems )
{
	*pnbFuncItems = ExecuteAnything_Menu::N_NBFUNCITEMS;
	return ExecuteAnything_Menu::get_arrNMenuItems();
}

const TCHAR* ExecuteAnything::nppGetName()
{
	return ExecuteAnything::PLUGIN_NAME;
}

std::map<tstring, int> ExecuteAnything::m_ExternalLanguages;
const std::map<tstring, int>& ExecuteAnything::GetExternalLanguages()
{
	return m_ExternalLanguages;
}

void ExecuteAnything::AddExternalLanguage(tstring LangName, int LangID)
{
	m_ExternalLanguages[LangName] = LangID;
}

void ExecuteAnything::nppBeNotified( SCNotification* pscn )
{
	if ( pscn->nmhdr.hwndFrom == m_nppMsgr.getNppWnd() )
	{
		static CNppExecPluginMsgSender npeMsgr(Get_Program().getNppWnd(), Get_Program().getDllFileName());
		// >>> notifications from Notepad++
		switch ( pscn->nmhdr.code )
		{
		case NPPN_READY:
			OnNppReady();
			break;

		case NPPN_SHUTDOWN:
			OnNppShutdown();
			break;

		case NPPN_LANGCHANGED:
		{
			uptr_t currentBufferID = pscn->nmhdr.idFrom;
			HWND hwndNpp = (HWND)pscn->nmhdr.hwndFrom;
			tstring Ext = npeMsgr.getCurrentFileExtPart();
			int LangID = (int)npeMsgr.SendNppMsg(NPPM_GETBUFFERLANGTYPE, currentBufferID, 0);
			if (!LangID || LangID == IDM_LANG_TEXT)
			{
				// ToDo: Need to add logic in NppToIDE to get all user defined lanaguages
				//	Can use ExecuteAnything DB to find associated file extensions to programming languages
				if (Ext == _T(".ahk"))
						npeMsgr.SendNppMsg(NPPM_SETCURRENTLANGTYPE, currentBufferID, 46185);
						//npeMsgr.SendNppMsg(NPPM_SETBUFFERLANGTYPE, currentBufferID, 46185);
			}
		}
			break;

		case IDM_EXECUTE:
			ExecuteAnything_Menu::menuFunc_IDE_Execute();
			break;

		default:
			break;
		}
		// <<< notifications from Notepad++
	}
}


LRESULT ExecuteAnything::nppMessageProc( UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	if ( uMessage == NPPM_MSGTOPLUGIN )
	{
		CommunicationInfo* pci = (CommunicationInfo *)lParam;
		if ( pci )
		{
			if ( pci->internalMsg == NPEN_RESULT )
			{
				// NPEN_RESULT notification...
				if ( lstrcmpi( pci->srcModuleName, _T( "NppExec.dll" ) ) == 0 )
				{
					// ...from NppExec plugin
					DWORD dwResult = *((DWORD *)pci->info); // <-- the old way (NppExec version >= 0x02F5)
					tPluginResultStruct* pResult = (tPluginResultStruct *)pci->info; // <-- the new way (NppExec version >= 0x06A0)

					if ( !processNotificationFromNpeExecuteCollateral( pResult ) &&
						 !processNotificationFromNpeExecuteQueued( pResult ) )
					{
						TCHAR szText[128];

						if ( dwResult == NPE_RESULT_OK )
						{
							const tstring strID = pResult->szID;
							if (strID == EXECUTEANYTHING_CALLBACK_ID)
							{
								NotificationFlag = true;
							}
							//else if (strID == EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION)
							//{
							//	//if (!TestAutomationCanceled)
							//	//{
							//	//	NotificationFlag = true;
							//	//	// Move to next tab, and execute
							//	//	SendKeys sendkeys(_T(""), _T("Notepad++"));
							//	//	sendkeys.SendString(_T("^%{NUMPAD5}"));
							//	//}
							//}
							else
							{
								// OK, the script has been executed
								::wsprintf(szText, _T("OK\n(id = %s)"), pResult->szID);
								::MessageBox(getNppWnd(), szText, _T("NPEN_RESULT notification"), MB_OK);
							}
						} else if ( dwResult == NPE_RESULT_ABORTED )
						{
							// aborted, maybe because NppExec's Console was closed
							::wsprintf( szText, _T( "ABORTED\n(id = %s)" ), pResult->szID );
							::MessageBox( getNppWnd(), szText, _T( "NPEN_RESULT notification" ), MB_OK | MB_ICONWARNING );
						} else
						{
							// failed, maybe internal error in NppExec
							::wsprintf( szText, _T( "FAILED\n(id = %s)" ), pResult->szID );
							::MessageBox( getNppWnd(), szText, _T( "NPEN_RESULT notification" ), MB_OK | MB_ICONERROR );
						}
					}
				}
			}
		}
	}
	return 1;
}

void ExecuteAnything::OnNppSetInfo( const NppData& nppd )
{
	m_nppPluginMenu.setNppData( nppd, ExecuteAnything::PLUGIN_NAME );
}

void ExecuteAnything::OnNppReady()
{
	// TODO:  add your code here :)
	NppToIDE npptoide(Get_Program());
}

void ExecuteAnything::OnNppShutdown()
{
	// TODO:  add your code here :)
}

ExecuteAnything::ExecuteAnything()
{
	OnNppSetInfo( m_nppMsgr.getNppData() );

	//ShortcutKey sk = { 0 };
	//sk._key = VK_F5;
	//SCNotification scnN = { 0 };
	//scnN.nmhdr.code = NPPN_SHORTCUTREMAPPED;
	//scnN.nmhdr.hwndFrom = reinterpret_cast<void *>(&sk); // ShortcutKey structure
	//scnN.nmhdr.idFrom = (uptr_t)0; // cmdID
	//nppBeNotified(&scnN);
}

CNppPlugin& ExecuteAnything::Get_Program()
{
	static ExecuteAnything theMsgTester;
	// Try to replace F5 key
	CNppExecPluginMsgSender npeMsgr(theMsgTester.getNppWnd(), theMsgTester.getDllFileName());
	ShortcutKey sk = { 0 };
	if (npeMsgr.getShortCutByCmdId(IDM_EXECUTE, sk))
	{
		if (sk._key == VK_F5 && !sk._isAlt && !sk._isCtrl && !sk._isShift)
		{
			npeMsgr.removeShortCutByCmdId(IDM_EXECUTE);
		}
	}

	return theMsgTester;
}


const unsigned int ExecuteAnything::nSecondsTillCloseConsole = 5;

DWORD WINAPI ExecuteAnything::ProcessNotificationFromNpeExecuteCollateralThreadFunc( LPVOID lpParam )
{
	const TCHAR* pszID = (const TCHAR *)lpParam;
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	NpeExecuteCollateralParam necp;
	TCHAR szScriptBody[128];

	if ( lstrcmp( pszID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_Cmd ) == 0 )
	{
		// Done!
		// (This notification is sent right after the "cmd" has exited.)

		necp.szScriptBody = _T( "npp_console ?\n" ) \
			_T( "echo Waiting for 2.5 seconds..." ); // "npp_console ?" does not open the closed Console
		necp.szID = NULL;  // no id - no notification
		necp.dwResult = 0; // don't send a notification when executed
		npeMsgr.NpeExecuteCollateral( &necp );

		::Sleep( 2500 ); // give the user time to realize the Console has been closed :)

		necp.szScriptBody = _T( "echo Reopening the Console...\n" ) \
			_T( "npp_console on // show the Console\n" ) \
			_T( "echo NPEM_EXECUTE_COLLATERAL - done!\n" ) \
			_T( "echo ================ READY ================" ); // collateral scripts do not print "==== READY ====" at the end
		necp.szID = NULL;  // no id - no notification
		necp.dwResult = 0; // don't send a notification when executed
		npeMsgr.NpeExecuteCollateral( &necp );
	} else if ( lstrcmp( pszID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoAttention ) == 0 )
	{
		::Sleep( 1500 ); // let's have some pause to not make everything too fast

		// Printing a message
		::wsprintf( szScriptBody, _T( "echo NppExec's Console will be closed in %u second(s)..." ), nSecondsTillCloseConsole );
		necp.szScriptBody = szScriptBody;
		necp.szID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[nSecondsTillCloseConsole - 1];
		necp.dwResult = 1; // send a notification when executed
		if ( npeMsgr.NpeExecuteCollateral( &necp ) != NPE_RESULT_OK )
		{
			::MessageBox( Get_Program().getNppWnd(), _T( "Operation failed" ), necp.szID, MB_OK | MB_ICONERROR );
			return 1;
		}
	} else if ( lstrcmp( pszID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_CloseConsole ) == 0 )
	{
		// Nothing to do here - we process cszUniqueIdForExecuteCollateral_Cmd instead
	} else
	{
		for ( int i = 0; i < nSecondsTillCloseConsole; ++i )
		{
			if ( lstrcmp( pszID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[i] ) == 0 )
			{
				::Sleep( 1000 ); // wait for 1 second (repeating nSecondsTillCloseConsole times)

				if ( i == 0 ) // <-- if it was the last second left, close the Console
				{
					// Closing NppExec's Console & exiting "cmd"
					necp.szScriptBody = _T( "set local @exit_cmd_silent = exit // the exit command\n" ) \
						_T( "npp_console off // close the Console" );
					// NOTE: a local variable in a collateral script is not the same as in a regular script.
					// A local variable of a collateral script becomes its parent script's local variable
					// when the collateral script ends.
					// A local variable of a regular script dies (disappears) at the end of the regular script.
					necp.szID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_CloseConsole;
					necp.dwResult = 1; // send a notification when executed
					if ( npeMsgr.NpeExecuteCollateral( &necp ) != NPE_RESULT_OK )
					{
						::MessageBox( Get_Program().getNppWnd(), _T( "Operation failed" ), necp.szID, MB_OK | MB_ICONERROR );
						return 1;
					}
				} else
				{
					// Printing the next message (5... 4... 3... 2... 1 second(s))
					::wsprintf( szScriptBody, _T( "echo NppExec's Console will be closed in %u second(s)..." ), i );
					necp.szScriptBody = szScriptBody;
					necp.szID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[i - 1];
					necp.dwResult = 1; // send a notification when executed
					if ( npeMsgr.NpeExecuteCollateral( &necp ) != NPE_RESULT_OK )
					{
						::MessageBox( Get_Program().getNppWnd(), _T( "Operation failed" ), necp.szID, MB_OK | MB_ICONERROR );
						return 1;
					}
				}

				break;
			}
		}
	}

	return 0;
}

bool ExecuteAnything::processNotificationFromNpeExecuteCollateral( tPluginResultStruct* pResult )
{
	const TCHAR* pszID = NULL;

	// Checking the pResult->szID ...

	for ( int i = 0; i < nSecondsTillCloseConsole; ++i )
	{
		if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[i] ) == 0 )
		{
			pszID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[i];
			break;
		}
	}

	if ( pszID == NULL )
	{
		if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_Cmd ) == 0 )
		{
			pszID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_Cmd;
		} else if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoAttention ) == 0 )
		{
			pszID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoAttention;
		} else if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_CloseConsole ) == 0 )
		{
			pszID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_CloseConsole;
		}
	}

	if ( pszID != NULL )
	{
		// The actual processing is in a separate thread to not block Notepad++'s GUI
		if ( !ExecuteAnything_Menu::CreateNewThread( ProcessNotificationFromNpeExecuteCollateralThreadFunc, (LPVOID)pszID ) )
		{
			::MessageBox( Get_Program().getNppWnd(), _T( "Failed to create a thread" ), pszID, MB_OK | MB_ICONERROR );
		}
		return true;
	}

	return false;
}

bool ExecuteAnything::processNotificationFromNpeExecuteQueued( tPluginResultStruct* pResult )
{
	TCHAR szScriptBody[128] = { 0 };
	NpeExecuteQueuedParam neqp;

	if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteQueued_SetA ) == 0 )
	{
		::wsprintf( szScriptBody, _T( "echo NPEM_EXECUTE_QUEUED - Set A - %s" ), pResult->dwResult == NPE_RESULT_OK ? _T( "OK" ) : _T( "FAILED" ) );
		neqp.szScriptBody = szScriptBody;
		neqp.szID = NULL;  // no id - no notification
		neqp.dwResult = 0; // don't send a notification when executed

		CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
		npeMsgr.NpeExecuteQue( &neqp );

		return true;
	}
	else if ( lstrcmp( pResult->szID, ExecuteAnything_Menu::cszUniqueIdForExecuteQueued_SetB ) == 0 )
	{
		::wsprintf( szScriptBody, _T( "echo NPEM_EXECUTE_QUEUED - Set B - %s\nnpe_console a- -- // Console: append off, silent" ), pResult->dwResult == NPE_RESULT_OK ? _T( "OK" ) : _T( "FAILED" ) );
		neqp.szScriptBody = szScriptBody;
		neqp.szID = NULL;  // no id - no notification
		neqp.dwResult = 0; // don't send a notification when executed

		CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
		npeMsgr.NpeExecuteQue( &neqp );

		return true;
	}
	else if (lstrcmp(pResult->szID, ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION) == 0)
	{

		return true;
	}
	else if (lstrcmp(pResult->szID, ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID) == 0)
	{

		return true;
	}
	return false;
}

