/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "ExecuteAnything_Menu.h"
#include "ProcessPrgLanguageAction.h"
#include "TestAutomation.h"
#include "ExecuteAnything.h"
#include "core\NppMessager.h"
#include "CommonFunctions.h"
#include <sstream>


const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_Cmd = _T("NPEM_EXECUTE_COLLATERAL - cmd");
const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoAttention = _T("NPEM_EXECUTE_COLLATERAL - echo Attention");
const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_CloseConsole = _T("NPEM_EXECUTE_COLLATERAL - close Console");
const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoInNSecs[10] = {
	_T("NPEM_EXECUTE_COLLATERAL - echo In 1 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 2 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 3 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 4 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 5 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 6 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 7 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 8 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 9 second(s)"),
	_T("NPEM_EXECUTE_COLLATERAL - echo In 10 second(s)")
};

const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteQueued_SetA = _T("NPEM_EXECUTE_QUEUED ExecuteAnything - Set A");
const TCHAR* const ExecuteAnything_Menu::cszUniqueIdForExecuteQueued_SetB = _T("NPEM_EXECUTE_QUEUED ExecuteAnything - Set B");

ShortcutKey ExecuteAnything_Menu::MenuItemsShortcutKeys[N_NBFUNCITEMS] = {
//ctrl, alt, shift, KEY
{ false, false, false, VK_F10 }, // Run (Favors script over compiler)
{ true, false, false, VK_F5 }, // Execute command (Favors compiler over script)
{ false, false, false, 0 }, // Run if you can						
{ false, false, false, 0 }, // Rebuild & Execute" ),					
{ false, false, false, 0 }, // Seperator					
//{true, false, false, VK_F10}, // Debug" ),								
//{true, false, true, VK_F10}, // Build & Debug" ),						
//{true, true, false, VK_F10}, // Rebuild & Debug" ),					
//{false, false, false, 0}, // " ),									
{ false, true, false, 0 }, // Rebuild_Only" ),						
{ false, true, false, 0 }, // Build_Only" ),						
{ true, false, true, 0 }, // Clean_Only" ),						
{ false, false, false, 0 }, // " ),									
{ true, true, false, VK_NUMPAD4 }, // menuFunc_IDE_Start_TestAutomation),									
{ true, true, false, VK_NUMPAD5 }, // menuFunc_IDE_CreateFiles_TestAutomation),									
{ true, true, false, VK_NUMPAD6 }, // menuFunc_IDE_Cancel_TestAutomation),									
{ true, true, false, VK_NUMPAD7 }, // menuFunc_IDE_Update_TestAutomation),									
{ false, false, false, 0 }, // " ),									
{ true, false, false, VK_F1 }, // (ctrl-F1) Help_on_Programming_Language" ),		
{ true, true, false, VK_F1 }, // (Carl-alt-F1) Google_Help_on_Programming_Language" )
//{false, false, false, 0}, // " ),									
//{ false, false, false, 0 }, //{true, true, false, VK_F1}, // Regex Assistant" ),					
//{false, false, false, 0}, // " ),									
//{false, false, false, 0}, // " ),									
};

FuncItem ExecuteAnything_Menu::arrNMenuItems[N_NBFUNCITEMS] = {
{ _T("Run (Favor Script)" ),							menuFunc_IDE_Run,									0},
{ _T("Execute (Favor Build)" ),							menuFunc_IDE_Execute,								0}, // { _T("Add/Edit Programming Language Handlers" ),		menuFunc_IDE_ManageProgrammingLanguageHandlers,				0, false, &MenuItemsShortcutKeys[1] },
{ _T("Run... (Smart Run)" ),							menuFunc_IDE_RunIfYouCan,							0},
{ _T("Rebuild && Execute" ),							menuFunc_IDE_Rebuild_And_Execute,					0},
{ _T("" ),												0,													0},
//{ _T("Debug (Execute in Debug Mode)" ),				menuFunc_IDE_Debug,								0},
//{ _T("Build && Debug" ),								menuFunc_IDE_Build_And_Debug,						0},
//{ _T("Rebuild && Debug" ),							menuFunc_IDE_Rebuild_And_Debug,					0},
//{ _T("" ),											0,												0},
{ _T("Rebuild_Only" ),									menuFunc_IDE_Rebuild_Only,							0},
{ _T("Build_Only" ),									menuFunc_IDE_Build_Only,							0},
{ _T("Clean_Only" ),									menuFunc_IDE_Clean_Only,							0},
{ _T("" ),												0,													0},
{ _T("Start_Test_Automation_Programming_Language" ),	menuFunc_IDE_Start_TestAutomation,					0},
{ _T("Create_And_Open_Test_Automation_Files" ),			menuFunc_IDE_CreateFiles_TestAutomation,			0},
{ _T("Cancel_Test_Automation_Programming_Language" ),	menuFunc_IDE_Cancel_TestAutomation,					0},
{ _T("Update_Test_Automation_Programming_Language" ),	menuFunc_IDE_Update_TestAutomation,					0},
{ _T("" ),												0,													0},
{ _T("Help_on_Programming_Language" ),					menuFunc_IDE_Help_On_Programming_Language,			0},
{ _T("Google_Help_on_Programming_Language" ),			menuFunc_IDE_Google_Help_On_Programming_Language,	0},
//{ _T("" ),												0,															0},
//{ _T("" ),												0,															0}, // { _T("Regex Assistant" ),								menuFunc_IDE_Regex_Assistant,								0, false, &MenuItemsShortcutKeys[3] },
//{ _T("" ),												0,															0},
//{ _T("" ),												0,															0},
};

FuncItem* ExecuteAnything_Menu::get_arrNMenuItems()
{
	for (int i = 0;i < N_NBFUNCITEMS;++i)
		arrNMenuItems[i]._pShKey = &MenuItemsShortcutKeys[i];
	return &arrNMenuItems[0];
}


int ExecuteAnything_Menu::GetSecondsTillCloseConsole()
{
	return 6;
}

CNppPlugin& ExecuteAnything_Menu::Get_Program()
{
	return ExecuteAnything::Get_Program();
}

bool ExecuteAnything_Menu::CreateNewThread(LPTHREAD_START_ROUTINE lpFunc, LPVOID lpParam, HANDLE* lphThread /* = NULL */)
{
	DWORD dwThreadId = 0;
	HANDLE hThread = ::CreateThread(
		NULL,
		0,
		lpFunc,
		lpParam,
		0,
		&dwThreadId);

	if ( hThread == NULL )
		return false;
  
	if ( !lphThread )
	{
		::CloseHandle(hThread);
	}
	else
	{
		*lphThread = hThread;
	}
	return true;
}


DWORD WINAPI ExecuteAnything_Menu::NpeExecuteCollateralThreadFunc(LPVOID lpParam)
{
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	NpeExecuteCollateralParam necp;
		
	::Sleep(1000); // let's have some pause to not make everything too fast
		
	// starting "cmd"
	necp.szScriptBody = _T("echo Starting cmd.exe...\n") \
						_T("cmd");
	necp.szID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_Cmd;
	necp.dwResult = 1; // send a notification when executed
	if ( npeMsgr.NpeExecuteCollateral(&necp) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), necp.szID, MB_OK | MB_ICONERROR );
		return 1;
	}

	::Sleep(2000); // just to ensure cmd.exe has enough time to be started
		
	// Printing a message
	necp.szScriptBody = _T("echo Attention!");
	necp.szID = ExecuteAnything_Menu::cszUniqueIdForExecuteCollateral_EchoAttention;
	necp.dwResult = 1; // send a notification when executed
	if ( npeMsgr.NpeExecuteCollateral(&necp) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), necp.szID, MB_OK | MB_ICONERROR );
		return 1;
	}

	return 0;
}


void ExecuteAnything_Menu::funcNpeGetVerDword()
{
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	DWORD dwVersion = npeMsgr.NpeGetVerDword();

	TCHAR str[20];
	::wsprintf(str, _T("0x%04X"), dwVersion);
	::MessageBox( Get_Program().getNppWnd(), str, _T("NppExec version (as dword)"), MB_OK );
}

void ExecuteAnything_Menu::funcNpeGetVerStr()
{
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	TCHAR str[NPE_MAXVERSTR];
	npeMsgr.NpeGetVerStr(str);

	::MessageBox( Get_Program().getNppWnd(), str, _T("NppExec version (as string)"), MB_OK );
}

void ExecuteAnything_Menu::funcNpeGetState()
{
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	DWORD dwState = npeMsgr.NpeGetState();
	if ( dwState == NPE_STATEREADY )
		::MessageBox( Get_Program().getNppWnd(), _T("Ready"), _T("NppExec state"), MB_OK );
	else if ( dwState & NPE_STATEBUSY )
		::MessageBox( Get_Program().getNppWnd(), _T("Busy"), _T("NppExec state"), MB_OK );
	else
		::MessageBox( Get_Program().getNppWnd(), _T("UNKNOWN"), _T("NppExec state"), MB_OK );
}

void ExecuteAnything_Menu::funcNpePrint()
{
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	const TCHAR* cszText = _T("Hello!\nThis is a test message.\r\nAre you happy to see it? ;-)");
	if ( npeMsgr.NpePrint(cszText) != NPE_STATEREADY )
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_PRINT"), MB_OK | MB_ICONERROR );
}

void ExecuteAnything_Menu::funcNpeExecute()
{
	NpeExecuteParam nep;
	nep.szScriptBody = _T("cd c:\\ \n dir\r\ncd $(NPP_DIRECTORY)\ndir \ndir *.txt");
	nep.dwResult = 1; // send a notification when executed

	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeExecute(&nep) != NPE_RESULT_OK )
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR );
}

void ExecuteAnything_Menu::funcNpeNppExec()
{
	NpeNppExecParam npep;
	npep.szScriptName = _T("script");
	// npep.szScriptName = _T("C:\\Program Files\\My NppExec Scripts\\test script.txt");
	npep.szScriptArguments = _T("\"arg 1\" \"arg 2\"");
	npep.dwResult = 1; // send a notification when executed

	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeExecute(&npep) != NPE_RESULT_OK )
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_NPPEXEC"), MB_OK | MB_ICONERROR );
}

void ExecuteAnything_Menu::funcNpeExecuteCollateral()
{
	// Whatever NppExec is currently doing, the following will be executed collaterally:

	NpeExecuteCollateralParam necp;
	necp.szScriptBody = _T("echo NPEM_EXECUTE_COLLATERAL - starting\n") \
						_T("echo This text will be printed even when NppExec is \"busy\"");
	necp.szID = NULL;  // no id - no notification
	necp.dwResult = 0; // don't send a notification when executed
	
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeExecuteCollateral(&necp) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE_COLLATERAL - starting"), MB_OK | MB_ICONERROR );
		return;
	}

	// The rest of the test is in a separate thread to not block Notepad++'s GUI
	if ( !CreateNewThread(NpeExecuteCollateralThreadFunc, NULL) )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Failed to create a thread"), _T("NPEM_EXECUTE_COLLATERAL - thread"), MB_OK | MB_ICONERROR );
		return;
	}
}

void ExecuteAnything_Menu::funcNpeExecuteQueued()
{
	// Whatever NppExec is currently doing, the following will be queued:

	NpeExecuteQueuedParam neqp;
	neqp.szScriptBody = _T("npe_console a+ -- // Console: append on, silent\n") \
						_T("echo Setting A = 10...\n") \
						_T("set A = 10\n") \
						_T("echo A = $(A)");
	neqp.szID = cszUniqueIdForExecuteQueued_SetA;
	neqp.dwResult = 1; // send a notification when executed

	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeExecuteQue(&neqp) != NPE_RESULT_OK )
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), neqp.szID, MB_OK | MB_ICONERROR );

	neqp.szScriptBody = _T("echo Setting B = A + 20...\n") \
						_T("set B ~ $(A) + 20\n") \
						_T("echo B = $(B)");
	neqp.szID = cszUniqueIdForExecuteQueued_SetB;
	neqp.dwResult = 1; // send a notification when executed

	if ( npeMsgr.NpeExecuteQue(&neqp) != NPE_RESULT_OK )
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), neqp.szID, MB_OK | MB_ICONERROR );
}

void ExecuteAnything_Menu::funcNpeGetScriptNames()
{
	NpeGetScriptNamesParam nsn;
	nsn.pScriptNames = NULL;
	nsn.dwResult = 0;
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeGetScriptNames(&nsn) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_GETSCRIPTNAMES"), MB_OK | MB_ICONERROR );
		return;
	}

	// OK, let's get the script names
	std::list< std::basic_string<TCHAR> > scriptNames;
	{
		std::basic_string<TCHAR> scriptName;
		const TCHAR* pszNames = nsn.pScriptNames;
		TCHAR ch;
		while ( (ch = *pszNames) != 0 )
		{
			if ( ch == _T('\n') )
			{
				scriptNames.push_back(scriptName);
				scriptName.clear();
			}
			else
			{
				scriptName += ch;
			}
			++pszNames;
		}
		if ( !scriptName.empty() )
		{
			scriptNames.push_back(scriptName);
		}
	}

	// free the memory allocated by NppExec
	npeMsgr.NpeFreePtr( nsn.pScriptNames );

	// show the script names
	std::basic_ostringstream<TCHAR> oss;
	oss << _T("There are ") << scriptNames.size() << _T(" scripts in NppExec:\n");
	size_t n = 0;
	std::list< std::basic_string<TCHAR> >::const_iterator itr;
	for ( itr = scriptNames.begin(); itr != scriptNames.end(); ++itr )
	{
		oss << ++n << _T(". ") << *itr << _T("\n");
	}
	::MessageBox( Get_Program().getNppWnd(), oss.str().c_str(), _T("NPEM_GETSCRIPTNAMES"), MB_OK );
}

void ExecuteAnything_Menu::funcNpeGetScriptByName()
{
	// 1.1. Let's get the available script names first
	NpeGetScriptNamesParam nsn0;
	nsn0.pScriptNames = NULL;
	nsn0.dwResult = 0;
	CNppExecPluginMsgSender npeMsgr( Get_Program().getNppWnd(), Get_Program().getDllFileName() );
	if ( npeMsgr.NpeGetScriptNames(&nsn0) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed (get script name)"), _T("NPEM_GETSCRIPTBYNAME"), MB_OK | MB_ICONERROR );
		return;
	}
	// 1.2. Get the first script name
	std::basic_string<TCHAR> firstScriptName;
	{
		std::basic_string<TCHAR> scriptName;
		const TCHAR* pszNames = nsn0.pScriptNames;
		TCHAR ch;
		while ( (ch = *pszNames) != 0 )
		{
			if ( ch == _T('\n') )
			{
				firstScriptName = scriptName;
				break;
			}
			scriptName += ch;
			++pszNames;
		}
		if ( firstScriptName.empty() && !scriptName.empty() )
		{
			firstScriptName = scriptName;
		}
	}
	// 1.3. Free the memory allocated by NppExec
	npeMsgr.NpeFreePtr( nsn0.pScriptNames );
	// 1.4. Check if the first script name is empty
	if ( firstScriptName.empty() )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("(No scripts available)"), _T("NPEM_GETSCRIPTBYNAME"), MB_OK );
		return;
	}

	// 2.1. Get the script by name
	NpeGetScriptByNameParam nsn;
	nsn.szScriptName = firstScriptName.c_str();
	nsn.pScriptBody = NULL;
	nsn.dwResult = 0;
	if ( npeMsgr.NpeGetScriptByName(&nsn) != NPE_RESULT_OK )
	{
		::MessageBox( Get_Program().getNppWnd(), _T("Operation failed (get script body)"), _T("NPEM_GETSCRIPTBYNAME"), MB_OK | MB_ICONERROR );
		return;
	}
	// 2.2. Get the script lines
	std::list< std::basic_string<TCHAR> > scriptLines;
	{
		std::basic_string<TCHAR> scriptLine;
		const TCHAR* pszScriptBody = nsn.pScriptBody;
		TCHAR ch;
		while ( (ch = *pszScriptBody) != 0 )
		{
			if ( ch == _T('\n') )
			{
				scriptLines.push_back(scriptLine);
				scriptLine.clear();
			}
			else
			{
				scriptLine += ch;
			}
			++pszScriptBody;
		}
		if ( !scriptLine.empty() )
		{
			scriptLines.push_back(scriptLine);
		}
	}
	// 2.3. Free the memory allocated by NppExec
	npeMsgr.NpeFreePtr( nsn.pScriptBody );
	// 2.4. Show the script lines
	std::basic_ostringstream<TCHAR> oss;
	oss << _T("The script \"") << firstScriptName << _T("\" contains ") << scriptLines.size() << _T(" lines:\n");
	size_t n = 0;
	std::list< std::basic_string<TCHAR> >::const_iterator itr;
	for ( itr = scriptLines.begin(); itr != scriptLines.end(); ++itr )
	{
		oss << _T("(") << ++n << _T(") ") << *itr << _T("\n");
	}
	::MessageBox( Get_Program().getNppWnd(), oss.str().c_str(), _T("NPEM_GETSCRIPTBYNAME"), MB_OK );
}

void ExecuteAnything_Menu::menuFunc_IDE_Run()
{

	ProcessPrgLangAction(TYPE_RUN);
}

void ExecuteAnything_Menu::menuFunc_IDE_Execute()
{

	ProcessPrgLangAction(TYPE_EXECUTE);
}

void ExecuteAnything_Menu::menuFunc_IDE_ManageProgrammingLanguageHandlers()
{
	funcNpeGetVerStr();
}

void ExecuteAnything_Menu::menuFunc_IDE_Debug()
{
	ProcessPrgLangAction( TYPE_DEBUG );
}

void ExecuteAnything_Menu::menuFunc_IDE_Build_And_Execute()
{
	ProcessPrgLangAction( TYPE_BUILD_AND_EXECUTE );
}

void ExecuteAnything_Menu::menuFunc_IDE_Build_And_Debug()
{
	ProcessPrgLangAction( TYPE_BUILD_AND_DEBUG );
}

void ExecuteAnything_Menu::menuFunc_IDE_Rebuild_And_Execute()
{
	ProcessPrgLangAction( TYPE_REBUILD_AND_EXECUTE );
}

void ExecuteAnything_Menu::menuFunc_IDE_Rebuild_And_Debug()
{
	ProcessPrgLangAction( TYPE_REBUILD_AND_DEBUG );
}

void ExecuteAnything_Menu::menuFunc_IDE_Rebuild_Only()
{
	ProcessPrgLangAction( TYPE_REBUILD );
}

void ExecuteAnything_Menu::menuFunc_IDE_Build_Only()
{
	ProcessPrgLangAction( TYPE_BUILD );
}

void ExecuteAnything_Menu::menuFunc_IDE_Clean_Only()
{
	ProcessPrgLangAction( TYPE_CLEAN );
}

void ExecuteAnything_Menu::menuFunc_IDE_Help_On_Programming_Language()
{
	ProcessPrgLangAction( TYPE_HELP_ON_PRG_LANG );
}

void ExecuteAnything_Menu::menuFunc_IDE_Google_Help_On_Programming_Language()
{
	ProcessPrgLangAction( TYPE_GOOGLE_ON_PRG_LANG );
}
void ExecuteAnything_Menu::menuFunc_IDE_Regex_Assistant()
{
	funcNpeGetVerStr();
}

void ExecuteAnything_Menu::ProcessPrgLangAction( PrgLangProcessType prglangprocesstype )
{
	static CNppExecPluginMsgSender npeMsgr(Get_Program().getNppWnd(), Get_Program().getDllFileName());

	tstring CurrentFileName = npeMsgr.getCurrentFileName();
	if (CurrentFileName.size())
	{
		if (DoesHave(prglangprocesstype,TYPE_TEST_AUTOMATION))
			TestAutomation testautomation(npeMsgr, prglangprocesstype);
		else
			ProcessPrgLanguageAction processprglanguageaction(npeMsgr, prglangprocesstype);
	}
}

void ExecuteAnything_Menu::menuFunc_IDE_Start_TestAutomation()
{
	ProcessPrgLangAction(TYPE_TEST_AUTOMATION_START);
}

void ExecuteAnything_Menu::menuFunc_IDE_CreateFiles_TestAutomation()
{
	ProcessPrgLangAction(TYPE_TEST_AUTOMATION_CREATEFILES);
}

void ExecuteAnything_Menu::menuFunc_IDE_Cancel_TestAutomation()
{
	ProcessPrgLangAction(TYPE_TEST_AUTOMATION_CANCEL);
}

void ExecuteAnything_Menu::menuFunc_IDE_Update_TestAutomation()
{
	ProcessPrgLangAction(TYPE_TEST_AUTOMATION_UPDATE);
}

void ExecuteAnything_Menu::menuFunc_IDE_RunIfYouCan()
{
	ProcessPrgLangAction(TYPE_RUN_IF_YOU_CAN);
}



















