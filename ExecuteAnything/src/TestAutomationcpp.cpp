/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#include "TestAutomation.h"  // Contains required headers and constants for this source file
#include "ProcessPrgLanguageAction.h"
#include "ExecuteAnything.h"
#include "sqlite3pp_ez.h"
#include "ClipboardXX.hpp"
#include "SendKeys.h"
#include "SQL\sql_Master_Header.h"
#include <boost/algorithm/string/replace.hpp>

#include <string>
#include <set>
#include <vector>
#include <memory>
#include <sstream>

using namespace sqlite3pp;


class TestAutomation_Handler : protected ExecuteAnything_Menu
{
	CNppExecPluginMsgSender &m_npeMsgr;
	const PrgLangProcessType m_PrgLangProcessType;
	static std::set<tstring> m_FilesProccessed;
	static bool m_TestAutomationCancelled;
	int						m_QtyFiles;
	std::set<tstring >		m_SetOfFilesOpened;

public:
	TestAutomation_Handler(CNppExecPluginMsgSender &Msgr, const PrgLangProcessType p):m_npeMsgr(Msgr), m_PrgLangProcessType(p)
	{
		
		if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_CANCEL))
			CancelTestAutomation();
		else if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_UPDATE))
			UpdateDatabaseWithCurrentFile();
		else if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_CANCEL))
			m_TestAutomationCancelled = true;
		else
			RunTestAutomation();
	}

	void CancelTestAutomation()
	{
		ExecuteAnything::CancelTestAutomation();
		size_t QtyFilesTest = m_FilesProccessed.size();
		ExecuteAnything::ResetLastConsoleContent();
		m_FilesProccessed.clear();
		// ToDo: Replace the following 2 lines of code with better solution.
		SendKeys sendkeys(_T(""), _T("Notepad++"));
		sendkeys.SendString(_T("^C"));
		tstring Msg = _T("Canceled Automation test complete.  Tested ") + sqlite3pp::to_tstring(std::to_string(QtyFilesTest)) + _T(" files.");
		::MessageBox(Get_Program().getNppWnd(), Msg.c_str(), _T("Test Automation complete"), MB_OK | MB_ICONINFORMATION);
	}
	
	void UpdateDatabaseWithCurrentFile()
	{
		ExecuteAnything::CancelTestAutomation();
		size_t QtyFilesTest = m_FilesProccessed.size();
		ExecuteAnything::ResetLastConsoleContent();
		m_FilesProccessed.clear();
		ProcessPrgLanguageAction processprglanguageaction(m_npeMsgr, m_PrgLangProcessType);
	}

	size_t GetOpenedFiles()
	{
		static const int	m_MAX_FILENAME_LEN = 1024;
		m_QtyFiles = m_npeMsgr.SendMsg((UINT)NPPM_GETNBOPENFILES, (WPARAM)0, (LPARAM)0);
		std::vector<TCHAR*>	dumbPointers;
		for (int i = 0;i < m_QtyFiles;++i)
			dumbPointers.push_back(new TCHAR[m_MAX_FILENAME_LEN]());

		m_npeMsgr.SendMsg(NPPM_GETOPENFILENAMES,	(WPARAM)dumbPointers.data(), (LPARAM)m_QtyFiles);
		m_SetOfFilesOpened.clear();
		for (auto s : dumbPointers)
		{
			if (s && s[0])
				m_SetOfFilesOpened.insert(s);
			delete[] s;
		}
		size_t QtyInserted = m_SetOfFilesOpened.size();
		return QtyInserted;
	}

	void RunTestAutomation()
	{
		if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_CREATEFILES))
		{
			m_TestAutomationCancelled = false;
			ExecuteAnything::ResetLastConsoleContent();
			ExecuteAnything::ResetCancelTestAutomation();
			m_FilesProccessed.clear();
			using namespace sqlite3pp;
			std::vector<tstring> FilesCreated;

			tstring TestFolder = Get_UpdatedPathCopy(m_npeMsgr.getPluginsHomeDir() + _T("\\Config\\ExecuteAnything_TestAutomation"));
			if (!sqlite3pp::dir_exists(TestFolder))
				_tmkdir(TestFolder.c_str());
			GetOpenedFiles();
			DbFileNameArg &dbfilenamearg(ExecuteAnything::GetExecuteAnything_DbFileNameArg());
			Table<sql_table_TestAutomationSrcCode> TestAutomationSrcCode(dbfilenamearg);
			for (auto t : TestAutomationSrcCode)
			{
				if (m_TestAutomationCancelled)
					return;
				const int TestType = t.get_TestType();
				if ((TestType & eTATT_Basic) && !(TestType & eTATT_Any_InopFlag)) // Only run basic test type and that are not flagged inoperable
				{
					tstring TestFile = TestFolder + _T("\\HelloWorld") + t.get_Ext();
					if (file_exists(TestFile))
					{
						if (m_SetOfFilesOpened.find(TestFile) == m_SetOfFilesOpened.end())
							m_npeMsgr.SendMsg(NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(TestFile.c_str()));
						FilesCreated.push_back(TestFile);
					}
					else
					{
						std::ofstream outfile(TestFile.c_str(), std::ios::out);
						if (outfile.is_open())
						{
							std::string data(t.get_SourceCode()->begin(), t.get_SourceCode()->end());
							boost::replace_all(data, "\r\n", "\n");
							outfile.write(data.c_str(), data.size());
							outfile.close();
							m_npeMsgr.SendMsg(NPPM_DOOPEN, 0, reinterpret_cast<LPARAM>(TestFile.c_str()));
							FilesCreated.push_back(TestFile);
						}
					}
				}
			}

			if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_START))
			{
				for (auto f : FilesCreated)
				{
					if (m_TestAutomationCancelled)
						return;
					m_npeMsgr.SendMsg(NPPM_SWITCHTOFILE, 0, (LPARAM)f.c_str());
					ProcessPrgLanguageAction processprglanguageaction(m_npeMsgr, m_PrgLangProcessType);
				}
			}
		}
	}
};

std::set<tstring> TestAutomation_Handler::m_FilesProccessed;
bool TestAutomation_Handler::m_TestAutomationCancelled = false;


TestAutomation::TestAutomation(CNppExecPluginMsgSender &m_npeMsgr, PrgLangProcessType prglangprocesstype) 
{
	TestAutomation_Handler proc(m_npeMsgr, prglangprocesstype);
}

















