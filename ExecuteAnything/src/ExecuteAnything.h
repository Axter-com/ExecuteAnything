/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#ifndef _npp_exec_plugin_msg_tester_h_
#define _npp_exec_plugin_msg_tester_h_
//----------------------------------------------------------------------------
#include "core/base.h"
#include "core/NppPlugin.h"
#include "ExecuteAnything_Menu.h"
#include "nppexec_msgs.h"
#include "NppExecPluginMsgSender.h"

#include "sqlite3pp_ez.h"
#include <map>

inline bool DoesHave(int i, int e)
{
	return ((i&e) == e);
}

class ExecuteAnything: public CNppPlugin
{
    public:
		ExecuteAnything();
        static const TCHAR* const PLUGIN_NAME;
		static CNppPlugin& Get_Program();
		static DWORD WINAPI ProcessNotificationFromNpeExecuteCollateralThreadFunc( LPVOID lpParam );

		static sqlite3pp::database &GetExecuteAnythingDB();
		static sqlite3pp::DbFileNameArg &GetExecuteAnything_DbFileNameArg();
		static const TCHAR* const EXECUTEANYTHING_CALLBACK_ID;
		static const TCHAR* const EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION;
		static void ResetNotificationFlag();
		static bool ReceivedNotification();
		static tstring GetLastConsoleContent();
		static void ResetLastConsoleContent();
		static void CancelTestAutomation();
		static void ResetCancelTestAutomation();
		static const std::map<tstring, int>& GetExternalLanguages();
		static void AddExternalLanguage(tstring LangName, int LangID);

protected:
		static tstring GetExecuteAnythingDBPath();
		static bool ValidateDestPath(const tstring &OriginalDBPath, const tstring &WorkingDBPath);
		static bool processNotificationFromNpeExecuteCollateral( tPluginResultStruct* pResult );
		static bool processNotificationFromNpeExecuteQueued( tPluginResultStruct* pResult );
		static const unsigned int nSecondsTillCloseConsole;
		static bool FoundSourceDb;
		static tstring FoundSourceDbPath;
		static tstring FirstDestFailedToCopyDbPath;
		ExecuteAnything_Menu m_nppPluginMenu;
		static bool NotificationFlag;
		static bool TestAutomationCanceled;
		static tstring LastConsoleContent;
		static std::map<tstring, int> m_ExternalLanguages;

    public:
        // standard n++ plugin functions
        virtual void         nppBeNotified(SCNotification* pscn);
        virtual FuncItem*    nppGetFuncsArray(int* pnbFuncItems);
        virtual const TCHAR* nppGetName();
        virtual LRESULT      nppMessageProc(UINT uMessage, WPARAM wParam, LPARAM lParam);

        // common n++ notification
        virtual void OnNppSetInfo(const NppData& nppd);

        // custom n++ notifications
        void OnNppReady();
        void OnNppShutdown();

};
//----------------------------------------------------------------------------
#endif // !_npp_exec_plugin_msg_tester_h_
