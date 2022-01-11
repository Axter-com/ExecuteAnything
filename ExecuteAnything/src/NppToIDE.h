/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#ifndef _NPPTOIDE_H_
#define _NPPTOIDE_H_
#include "ExecuteAnything.h"

class NppToIDE
{
	HWND m_hwndTip;
	bool m_AddSmartRun_To_RunMenu;
	bool m_AddExecuteAnythingMenu_To_RunMenu;
	bool m_ReplaceNppRunShortcutWithExecuteAnythingRun;
	bool m_AddRestart_To_RunMenu;
	CNppPlugin &m_CNppPlugin;
	CNppExecPluginMsgSender m_npeMsgr;
	CNppMessager m_nppMsgr;
public:
	NppToIDE(CNppPlugin &cnppplugin);
private:
	void Update_Shortcuts_XML();
	void SetRunMenu();
	void InitMenu(HINSTANCE hInst, HMENU hMainMenu, const TCHAR *translation);
	void GetListOfUserDefinedLanguages();
};

#endif // !_NPPTOIDE_H_