/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "NppToIDE.h"
#include "PrgLanguagesHelp.h"
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include "sqlite3pp_ez.h"
#include "SQL\sql_Master_Header.h"
#include <windows.h>
#include <CommCtrl.h>

NppToIDE::NppToIDE(CNppPlugin &cnppplugin)
	:m_CNppPlugin(cnppplugin)
	, m_npeMsgr(m_CNppPlugin.getNppWnd()
	, m_CNppPlugin.getDllFileName())
	, m_hwndTip(NULL)
	, m_AddSmartRun_To_RunMenu(true)
	, m_AddExecuteAnythingMenu_To_RunMenu(true)
	, m_ReplaceNppRunShortcutWithExecuteAnythingRun(true)
	, m_AddRestart_To_RunMenu(true)
{
	// ToDo: Add logic to fetch option settings (m_AddRestartToRunMenu, etc...) from xml file in config folder.
	Update_Shortcuts_XML();

	PrgLanguagesHelp prglanguageshelp(m_CNppPlugin);
	SetRunMenu();
}

UINT GetMenuID(HMENU hmenu, tstring FindMenuItem, int32_t &pos_found)
{
	tstring MenuItemName;
	int maxCount = GetMenuItemCount(hmenu);
	for (int32_t pos = 0; pos < maxCount; ++pos)
	{
		const size_t MaxBuf = 256;
		TCHAR buffer[MaxBuf] = { 0 };
		GetMenuString(hmenu, pos, buffer, MaxBuf, MF_BYPOSITION);
		MenuItemName = buffer;
		if (MenuItemName.find(FindMenuItem) == 0)
		{
			pos_found = pos;
			return GetMenuItemID(hmenu, pos);
		}
	}

	return (UINT)-1;
}

// ToDo: When time permits, continue to work on below issue.
// Started working on below function because sometimes files don't open with the correct associated language.
// This seems to be related to opening file extension types before the language has been added to notepad++.
// Even when NPP fails to associate corret language, the NppParameters::getUserDefinedLangNameFromExt function
// is able to get the correct language associated with the file extension.
void NppToIDE::GetListOfUserDefinedLanguages()
{
	HMENU hMainMenu = m_nppMsgr.getNppMainMenu();
	const int MENUINDEX_LANGUAGE = 5;
	HMENU hLangMenu = ::GetSubMenu(hMainMenu, MENUINDEX_LANGUAGE);
	const int32_t numLangs = ::GetMenuItemCount(hLangMenu);
	for (int32_t pos = 0; pos < numLangs; ++pos)
	{
		const size_t MaxBuf = 256;
		TCHAR buffer[MaxBuf] = { 0 };
		GetMenuString(hLangMenu, pos, buffer, MaxBuf, MF_BYPOSITION);
		ExecuteAnything::AddExternalLanguage(buffer, pos);
	}
	// Try to use NppParameters to get supported file extensions
}

void NppToIDE::SetRunMenu()
{
	HMENU hMainMenu = m_nppMsgr.getNppMainMenu();
	HMENU hRunMenu = GetSubMenu(hMainMenu, 9);
	UINT RunSubMenuID = GetMenuItemID(hRunMenu, 0);
	HMENU hPluginMenu = GetSubMenu(hMainMenu, 10);

	int32_t pos_found = -1;
	UINT RunAnythingRunID = GetMenuID(hPluginMenu, _T("Execute Anything "), pos_found);
	UINT RunAnythingRunID_static = GetMenuItemID(hPluginMenu, 3);
	HMENU hExecuteAnythingSubMenu = (pos_found == -1) ? NULL : GetSubMenu(hPluginMenu, pos_found);
	UINT RunAnythingSmartRunID = (hExecuteAnythingSubMenu  == NULL) ? (UINT)-1 : GetMenuItemID(hExecuteAnythingSubMenu, 2);

	int InsertPos = 1;
	
	if (m_AddSmartRun_To_RunMenu && RunAnythingSmartRunID != (UINT)-1)
	{
		MENUITEMINFO mii = { sizeof(mii), MIIM_STRING | MIIM_ID };
		const size_t MaxBuf = 256;
		TCHAR buffer[MaxBuf] = { 0 };
		GetMenuString(hExecuteAnythingSubMenu, 2, buffer, MaxBuf, MF_BYPOSITION);
		mii.dwTypeData = buffer;
		mii.fMask = MIIM_STRING | MIIM_ID;
		mii.wID = RunAnythingSmartRunID;
		InsertMenuItem(hRunMenu, InsertPos++, TRUE, &mii);

		m_hwndTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			m_nppMsgr.getNppWnd(), NULL, NULL, NULL);

		//RECT rect = { 0 };
		//GetMenuItemRect(m_nppMsgr.getNppWnd(), hExecuteAnythingSubMenu, 2, &rect);
		////ClientToScreen(&rect);
		//tstring SmartRunToolTipItem = _T("Run source code via ExecuteAnything if file type supported. Else call Npp Run...");
		//TOOLINFO toolinfo = { sizeof(TOOLINFO), TTF_IDISHWND | TTF_SUBCLASS, m_nppMsgr.getNppWnd(), 
		//	reinterpret_cast<UINT_PTR>(m_nppMsgr.getNppWnd()), 
		//	rect, NULL, const_cast<TCHAR*>(SmartRunToolTipItem.c_str()) };
		//::SendMessage(m_hwndTip, TTM_TRACKPOSITION, 0, static_cast<LPARAM>(MAKELONG(rect.left, rect.top)));
		//::SendMessage(m_hwndTip, TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&toolinfo));
		//::SendMessage(m_hwndTip, TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&toolinfo));
	}

	if (m_AddExecuteAnythingMenu_To_RunMenu && hExecuteAnythingSubMenu != NULL)
	{
		MENUITEMINFO mii = { sizeof(mii), MIIM_STRING | MIIM_SUBMENU };
		mii.dwTypeData = _T("Execute Anything...");
		mii.hSubMenu = hExecuteAnythingSubMenu;
		InsertMenuItem(hRunMenu, InsertPos++, TRUE, &mii);
	}
}

void NppToIDE::InitMenu(HINSTANCE hInst, HMENU hMainMenu, const TCHAR *translation)
{
}
void NppToIDE::Update_Shortcuts_XML()
{
	const tstring NppMainXmlPath = m_npeMsgr.getNppRoaming();
	const tstring ShortcutsXml_filename = NppMainXmlPath + _T("shortcuts.xml");
	const tstring NL_Tab = _T("\n\t");
	const tstring TopNodeNL_Tab = _T("\n    ");

	tstring ShortcutsXml_Content;
	std::ifstream my_if(ShortcutsXml_filename.c_str(), std::ios::in);
	if (my_if.is_open())
	{
		my_if.seekg(0, my_if.end);
		const std::streamoff length = my_if.tellg();
		my_if.seekg(0, my_if.beg);
		char *buffer = new char[static_cast<size_t>(length) + 1]();
		if (buffer)
		{
			my_if.read(buffer, length);
			ShortcutsXml_Content = sqlite3pp::to_tstring(buffer);
			delete[] buffer;
		}
		my_if.close();

		bool UpdateFile = false;
		bool HasReplaceShortcutKeyForNppRun = false;

		if (m_ReplaceNppRunShortcutWithExecuteAnythingRun)
		{
			// ToDo: Consider adding regex code here to replace string search
			const tstring NewShortcutKey = _T("<Shortcut id=\"49000\" Ctrl=\"no\" Alt=\"yes\" Shift=\"no\" Key=\"116\" />");
			if (ShortcutsXml_Content.find(NewShortcutKey) == tstring::npos)
			{
				const tstring InternalCommands_StartTag = _T("<InternalCommands>");
				const tstring InternalCommands_StartAndEndTag = _T("<InternalCommands />");
				const tstring InternalCommands_EndTag = _T("</InternalCommands>");
				if (ShortcutsXml_Content.find(InternalCommands_StartAndEndTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, InternalCommands_StartAndEndTag, InternalCommands_StartTag + NL_Tab + NewShortcutKey + TopNodeNL_Tab + InternalCommands_EndTag);
					UpdateFile = true;
					HasReplaceShortcutKeyForNppRun = true;
				}
				else if (ShortcutsXml_Content.find(InternalCommands_StartTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, InternalCommands_StartTag, InternalCommands_StartTag + NL_Tab + NewShortcutKey);
					UpdateFile = true;
					HasReplaceShortcutKeyForNppRun = true;
				}
			}
			else
				HasReplaceShortcutKeyForNppRun = true;
		}

		if (HasReplaceShortcutKeyForNppRun)
		{
			const tstring NewShortcutKey = _T("<PluginCommand moduleName=\"ExecuteAnything.dll\" internalID=\"2\" Ctrl=\"no\" Alt=\"no\" Shift=\"no\" Key=\"116\" />");
			if (ShortcutsXml_Content.find(NewShortcutKey) == tstring::npos)
			{
				const tstring PluginCommands_StartTag = _T("<PluginCommands>");
				const tstring PluginCommands_StartAndEndTag = _T("<PluginCommands />");
				const tstring PluginCommands_EndTag = _T("</PluginCommands>");
				if (ShortcutsXml_Content.find(PluginCommands_StartAndEndTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, PluginCommands_StartAndEndTag, PluginCommands_StartTag + NL_Tab + NewShortcutKey + TopNodeNL_Tab + PluginCommands_EndTag);
					UpdateFile = true;
				}
				else if (ShortcutsXml_Content.find(PluginCommands_StartTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, PluginCommands_StartTag, PluginCommands_StartTag + NL_Tab + NewShortcutKey);
					UpdateFile = true;
				}
			}
		}

		if (m_AddRestart_To_RunMenu)
		{
			const tstring NewShortcutKey[] = {
				_T("<Command name=\"Restart\" Ctrl=\"no\" Alt=\"no\" Shift=\"no\" Key=\"0\">cmd /c taskkill /im notepad++.exe &amp;&amp; TIMEOUT /T 1 &amp;&amp; start &quot;&quot; &quot;$(NPP_FULL_FILE_PATH)&quot;</Command>"),
				_T("<Command name=\"Restart (Force)\" Ctrl=\"no\" Alt=\"no\" Shift=\"no\" Key=\"0\">cmd /c taskkill /f /im notepad++.exe &amp;&amp; start &quot;&quot; &quot;$(NPP_FULL_FILE_PATH)&quot;</Command>"),
				_T("<!--  [ExecuteAnything Place-Holder to add Restart as Admin option here] -->"),  // ToDo: Create proper implementation for retart as admin in line below, and comment out this line
				_T("<Command name=\"Restart as Admin (Force)\" Ctrl=\"no\" Alt=\"no\" Shift=\"no\" Key=\"0\">cmd /c taskkill /f /im notepad++.exe &amp;&amp; start &quot;&quot; &quot;$(NPP_DIRECTORY)\\plugins\\ExecuteAnything\\notepad++.exe.lnk&quot;</Command>")
			};
			if (ShortcutsXml_Content.find(_T("\"Restart\"")) == tstring::npos)
			{
				const tstring PluginCommands_StartTag = _T("<UserDefinedCommands>");
				const tstring PluginCommands_StartAndEndTag = _T("<UserDefinedCommands />");
				const tstring PluginCommands_EndTag = _T("</UserDefinedCommands>");
				if (ShortcutsXml_Content.find(PluginCommands_StartAndEndTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, PluginCommands_StartAndEndTag, PluginCommands_StartTag + NL_Tab + NewShortcutKey[0] + NL_Tab + NewShortcutKey[1] + NL_Tab + NewShortcutKey[2] + TopNodeNL_Tab + PluginCommands_EndTag);
					UpdateFile = true;
				}
				else if (ShortcutsXml_Content.find(PluginCommands_StartTag) != tstring::npos)
				{
					boost::ireplace_all(ShortcutsXml_Content, PluginCommands_StartTag, PluginCommands_StartTag + NL_Tab + NewShortcutKey[0] + NL_Tab + NewShortcutKey[1] + NL_Tab + NewShortcutKey[2]);
					UpdateFile = true;
				}
			}
		}

		if (UpdateFile)
		{
			DeleteFile(ShortcutsXml_filename.c_str());
			std::ofstream my_of(ShortcutsXml_filename.c_str(), std::ios::out);
			if (my_of.is_open())
			{
				std::string ShortcutsXml_Content_a = sqlite3pp::to_string(ShortcutsXml_Content);
				my_of.write(ShortcutsXml_Content_a.c_str(), ShortcutsXml_Content_a.size());
				my_of.close();
			}
		}

	}
}

