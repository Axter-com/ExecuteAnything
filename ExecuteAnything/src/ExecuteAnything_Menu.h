/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#ifndef _ExecuteAnything_Menu_h_
#define _ExecuteAnything_Menu_h_
//----------------------------------------------------------------------------
#include "core/NppPluginMenu.h"
#include "core/NppPlugin.h"


class ExecuteAnything_Menu : public CNppPluginMenu
{
	public:
		static bool CreateNewThread( LPTHREAD_START_ROUTINE lpFunc, LPVOID lpParam, HANDLE* lphThread = NULL );
		static DWORD WINAPI NpeExecuteCollateralThreadFunc( LPVOID lpParam );
		static CNppPlugin& Get_Program();
		enum NMenuItems {
			N_IDE_Run,
			N_IDE_EXECUTE,
			N_IDE_REBUILD_AND_EXECUTE,
			N_IDE_Run_IF_YOU_CAN,
			N_IDE_SeperatorOrSubmenu2,
			//N_IDE_DEBUG,
			//N_IDE_BUILD_AND_DEBUG,
			//N_IDE_REBUILD_AND_DEBUG,
			//N_IDE_SeperatorOrSubmenu3,
			N_IDE_REBUILD_ONLY,
			N_IDE_BUILD_ONLY,
			N_IDE_CLEAN_ONLY,
			N_IDE_SeperatorOrSubmenu4,
			N_IDE_START_TEST_AUTOMATION_ON_ALL_PROGRAMMING_LANGUAGE,
			N_IDE_CONTINUE_TEST_AUTOMATION_ON_ALL_PROGRAMMING_LANGUAGE,
			N_IDE_CANCEL_TEST_AUTOMATION_ON_ALL_PROGRAMMING_LANGUAGE,
			N_IDE_UPDATE_TEST_AUTOMATION_ON_ALL_PROGRAMMING_LANGUAGE,
			N_IDE_SeperatorOrSubmenu6,
			N_IDE_HELP_ON_PROGRAMMING_LANGUAGE,
			N_IDE_GOOGLE_HELP_ON_PROGRAMMING_LANGUAGE,
			//N_IDE_SeperatorOrSubmenu0,
			//N_IDE_REGEX_ASSISTANT,
			//N_IDE_SeperatorOrSubmenu1,
			//N_IDE_ManageProgrammingLanguages,
			//N_IDE_BUILD_AND_EXECUTE,

			N_NBFUNCITEMS
		};

		// ExecuteAnything enums and functions
		enum PrgLangProcessType
		{
			  TYPE_RUN = 1
			, TYPE_EXECUTE = 2 | TYPE_RUN
			, TYPE_DEBUG = 4 | TYPE_EXECUTE
			, TYPE_CLEAN = 8
			, TYPE_BUILD = 16 | TYPE_EXECUTE
			, TYPE_REBUILD = TYPE_CLEAN | TYPE_BUILD
			, TYPE_BUILD_AND_EXECUTE = TYPE_BUILD | TYPE_EXECUTE
			, TYPE_BUILD_AND_DEBUG = TYPE_BUILD | TYPE_DEBUG
			, TYPE_REBUILD_AND_EXECUTE = TYPE_REBUILD | TYPE_EXECUTE
			, TYPE_REBUILD_AND_DEBUG = TYPE_REBUILD | TYPE_DEBUG
			, TYPE_GET_HELP = 32
			, TYPE_HELP_ON_PRG_LANG = 64 | TYPE_GET_HELP
			, TYPE_GOOGLE_ON_PRG_LANG = 128 | TYPE_GET_HELP
			, TYPE_SEND_MSG_NOTIFICATION = 256
			, TYPE_TEST_AUTOMATION = 512 | TYPE_EXECUTE | TYPE_BUILD
			//, TYPE_TEST_AUTOMATION_ALL_OPENED_FILES = TYPE_TEST_AUTOMATION | 512
			//, TYPE_TEST_AUTOMATION_ALL_DB_SRC_FILES = TYPE_TEST_AUTOMATION | 1024
			//, TYPE_TEST_AUTOMATION_ALL_OPENED_FILES_GET_NOTIFICATION = TYPE_TEST_AUTOMATION_ALL_OPENED_FILES | TYPE_SEND_MSG_NOTIFICATION
			, TYPE_TEST_AUTOMATION_CREATEFILES = TYPE_TEST_AUTOMATION | 1024
			, TYPE_TEST_AUTOMATION_START = TYPE_TEST_AUTOMATION_CREATEFILES | 2048
			, TYPE_TEST_AUTOMATION_CANCEL = TYPE_TEST_AUTOMATION | 4096
			, TYPE_TEST_AUTOMATION_UPDATE = TYPE_TEST_AUTOMATION | 8192
			, TYPE_RUN_IF_YOU_CAN = TYPE_RUN | 16384  // If ExecuteAnything doesn't support running the file type, call internal Notepad++ run command (id=49000)
		};

		static FuncItem*  get_arrNMenuItems();
		virtual int GetSecondsTillCloseConsole();

		static const TCHAR* const cszUniqueIdForExecuteCollateral_Cmd;
		static const TCHAR* const cszUniqueIdForExecuteCollateral_EchoAttention;
		static const TCHAR* const cszUniqueIdForExecuteCollateral_EchoInNSecs[10];
		static const TCHAR* const cszUniqueIdForExecuteCollateral_CloseConsole;

		static const TCHAR* const cszUniqueIdForExecuteQueued_SetA;
		static const TCHAR* const cszUniqueIdForExecuteQueued_SetB;

		static void ProcessPrgLangAction( PrgLangProcessType prglangprocesstype );

		// ExecuteAnything Menu functions
		static void menuFunc_IDE_Run();
		static void menuFunc_IDE_Execute();
		static void menuFunc_IDE_ManageProgrammingLanguageHandlers();
		static void menuFunc_IDE_Build_And_Execute();
		static void menuFunc_IDE_Build_And_Debug();
		static void menuFunc_IDE_Rebuild_And_Execute();
		static void menuFunc_IDE_Rebuild_And_Debug();
		static void menuFunc_IDE_Debug();
		static void menuFunc_IDE_Rebuild_Only();
		static void menuFunc_IDE_Build_Only();
		static void menuFunc_IDE_Clean_Only();
		static void menuFunc_IDE_Help_On_Programming_Language();
		static void menuFunc_IDE_Google_Help_On_Programming_Language();
		static void menuFunc_IDE_Regex_Assistant();
		static void menuFunc_IDE_Start_TestAutomation();
		static void menuFunc_IDE_CreateFiles_TestAutomation();
		static void menuFunc_IDE_Cancel_TestAutomation();
		static void menuFunc_IDE_Update_TestAutomation();
		static void menuFunc_IDE_RunIfYouCan();
protected:
		static FuncItem arrNMenuItems[N_NBFUNCITEMS];
		static ShortcutKey MenuItemsShortcutKeys[N_NBFUNCITEMS];
		// Old functions from original Msg plugin
		static void funcNpeGetVerDword();
		static void funcNpeGetVerStr();
		static void funcNpeGetState();
		static void funcNpePrint();
		static void funcNpeExecute();
		static void funcNpeNppExec();
		static void funcNpeExecuteCollateral();
		static void funcNpeExecuteQueued();
		static void funcNpeGetScriptNames();
		static void funcNpeGetScriptByName();



};


//----------------------------------------------------------------------------
#endif
