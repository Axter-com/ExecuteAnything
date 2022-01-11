/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#define INCLUDING_FROM_PROCESSPRGLANGUAGEACTION_CPP
#include "ProcessPrgLanguageAction.h"


ProcessPrgLanguageAction::ProcessPrgLanguageAction(CNppExecPluginMsgSender &npeMsgr, PrgLangProcessType prglangprocesstype) :m_RanATask(false), m_RunTaskSuccess(false), m_RunTaskAndReportedError(false)
{
	ProcessPrgLanguageAction_Handler proc(npeMsgr, prglangprocesstype);
	m_RanATask = proc.get_RanATask();
	m_RunTaskSuccess = proc.get_RunTaskSuccess();
	m_RunTaskAndReportedError = proc.get_RunTaskAndReportedError();
}

// The only ProcessPrgLanguageAction_Handler constructor 
ProcessPrgLanguageAction_Handler::ProcessPrgLanguageAction_Handler(CNppExecPluginMsgSender &npeMsgr, PrgLangProcessType prglangprocesstype) :m_db(ExecuteAnything::GetExecuteAnythingDB())
, m_PrgLangProcessType(prglangprocesstype)
, m_npeMsgr(npeMsgr)
, FILE_EXT(npeMsgr.getCurrentFileExtPart())
, FILE_BASE(npeMsgr.getCurrentFileNamePart())
, FILE_DIR(npeMsgr.getCurrentFileDirectory())
, FILE_DIR_AND_BASE(npeMsgr.getCurrentFileDirectory() + _T("\\") + npeMsgr.getCurrentFileNamePart())
, FQN(npeMsgr.getCurrentFileFullPath())
, Selected_Word(npeMsgr.getCurrentWord())
, FILE_NAME(npeMsgr.getCurrentFileName())
, FileName(Common::ToLower(npeMsgr.getCurrentFileName()))
, m_PlugConfigDir(npeMsgr.getPluginsConfigDir())
, m_WhereThisFileExt(_T("where Ext like '") + FILE_EXT + _T("'"))
, m_RanATask(false)
, m_RunTaskSuccess(false)
, m_RunTaskAndReportedError(false)
, m_ScriptProfileExist(false)
, m_CompileProfileExist(false)
, m_CSciMessager(npeMsgr.getCurrentScintillaWnd())
, m_IsTestAutomation(DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION))
, m_npe_execute_type(eNpeExecuteParam)
{
	Table<sql_view_v_ProgrammingLanguageProfiles> view_ProgrammingLanguageProfiles(m_WhereThisFileExt);
	Set_CompilerAndScript_ExistValues();

	for (auto csProfl : view_ProgrammingLanguageProfiles)
	{
		ResetLoopVar();
		m_PrgLangName = csProfl.get_PrgLangName();
		m_PrgLangProfileName = csProfl.get_ProfileName();
		m_TargetFile = FILE_NAME;

		// ToDo:  Add verification logic here for extensions that are used in multiple programming languages like "pl" which is used for Perl and Prolog
		// For help and google help, start processing it here, because runner and compiler is not needed for that.
		if (m_PrgLangProcessType & TYPE_GET_HELP)
			ProcessHelpRequest();
		else if (DoesHave(m_PrgLangProcessType, TYPE_TEST_AUTOMATION_UPDATE))
		{
			Table<sql_table_TestAutomationSrcCode> TestAutomationSrcCode;
			const int TextLen = m_CSciMessager.getTextLength();
			char * TextData = new char[TextLen + 1]();
			if (m_CSciMessager.getTextRange(0, TextLen, TextData) > 8)
			{
				tstring SourceCode = to_tstring(TextData);
				boost::replace_all(SourceCode, _T("'"), _T("''"));
				tstring ExecutStr = _T("INSERT OR REPLACE INTO TestAutomationSrcCode (PrgLangName, ProfileName, SourceCode, Ext) VALUES ('") + m_PrgLangName + _T("', '") + m_PrgLangProfileName + _T("', '") + SourceCode + _T("', '") + FILE_EXT + _T("')");
				TestAutomationSrcCode.Execute(ExecutStr);
			}
			delete[] TextData;
			m_RanATask = true;
			break;
		}
		else
		{
			WhereClauseArg WherePrgLangAndProfile(_T("Where \"PrgLangName\" like '") + m_PrgLangName + _T("' and \"ProfileName\" like '") + m_PrgLangProfileName + _T("'"));
			sql_table_AdvProfile_Settings AdvProfile = Get_AdvProfileSettings(WherePrgLangAndProfile);
			sql_table_ProgrammingLanguageProfiles ProgrammingLanguageProfile = Get_ProgrammingLanguageProfile(WherePrgLangAndProfile);
			const tstring ClearConsole = m_IsTestAutomation ? _T("") : _T("cls\n");
			m_NppExecScript = ClearConsole + NppExecScriptPrefix;
			if (!AdvProfile.get_DoNotSaveDocBeforeExecute()) // Not Not
				m_NppExecScript += _T("NPP_SAVE\n");
			if (m_IsTestAutomation)
				m_npe_execute_type = eNpeExecuteQueuedParam;
			else
			{
				switch (AdvProfile.get_NpeExecuteType())
				{
				case eNpeExecuteParam:
					m_npe_execute_type = eNpeExecuteParam;
				case eNpeExecuteParam2:
					m_npe_execute_type = eNpeExecuteParam2;
				case eNpeExecuteQueuedParam:
					m_npe_execute_type = eNpeExecuteQueuedParam;
				case eNpeExecuteCollateralParam:
					m_npe_execute_type = eNpeExecuteCollateralParam;
				}
			}
			SetInitialScriptVar();
			if (ProgrammingLanguageProfile.get_Uses_UNIX_PATH())
				Get_UnixPaths();
			PopulateVariablesValue(WherePrgLangAndProfile);
			m_NppExecScript += _T("set FILE_FULL_PATH=$(FQN)\n");
			Get_LangInstallationUsage(WherePrgLangAndProfile);
			Get_ProfileVariables(WherePrgLangAndProfile);
			if (ProgrammingLanguageProfile.get_ClearSysEnvPath())
				m_EnvPath_PostFix = _T("");
			else
				m_EnvPath_PostFix = _T("$(SYS.PATH)");

			if (m_PrgLangProcessType & TYPE_RUN)
			{
				tstring RequiredProgramLanguage = Get_RequiredProgramLanguage();
				tstring RequiredProfileName = Get_RequiredProfileName();
				if (RequiredProgramLanguage.size() && _tcsicmp(m_PrgLangName.c_str(), RequiredProgramLanguage.c_str()) != 0)
				{
					m_VerboseIterationLogging += Get_PrgLangAndProfileSig() + _T("Skipping Prg-Lang '") + m_PrgLangName + _T("', because required language is '") + RequiredProgramLanguage +  _T("'.\n");
					continue;
				}
				if (RequiredProfileName.size() &&  _tcsicmp(m_PrgLangProfileName.c_str(), RequiredProfileName.c_str()) != 0)
				{
					m_VerboseIterationLogging += Get_PrgLangAndProfileSig() + _T("Skipping Prg-Lang-Profile '") + m_PrgLangProfileName + _T("', because required profile is '") + RequiredProfileName + _T("'.\n");
					continue;
				}
				bool bExecuteNppExc = false;
				if (m_ScriptProfileExist && std::stoi(csProfl.get_isScript()) && (!m_CompileProfileExist || ((m_PrgLangProcessType & TYPE_EXECUTE) != TYPE_EXECUTE)))
				{
					if (Get_ScriptProfile())
						bExecuteNppExc = true;
					else
					{
						m_VerboseIterationLogging += Get_PrgLangAndProfileSig() + _T("Could not get Script-Profile.\n");
						if (m_RunTaskAndReportedError)
							break;
						continue;
					}
				}
				else if (m_CompileProfileExist && std::stoi(csProfl.get_isCompiler()))
				{
					if (Get_CompilerProfile(AdvProfile))
						bExecuteNppExc = true;
					else
					{
						m_VerboseIterationLogging += Get_PrgLangAndProfileSig() + _T("Could not get Compiler-Profile.\n");
						if (m_RunTaskAndReportedError)
							break;
						continue;
					}
				}

				if (bExecuteNppExc)
				{
					Setup_Prerequisite(WherePrgLangAndProfile);
					ExecuteNppExc(WherePrgLangAndProfile, AdvProfile);
					break;
				}
			}
		}
	}

	if (!m_RanATask && !m_RunTaskAndReportedError)
	{
		if (DoesHave(m_PrgLangProcessType, TYPE_RUN_IF_YOU_CAN))
		{
			m_npeMsgr.NppMenu_Run();
		}
		else
		{
			tstring CkDbConfigMsg = _T("\n\nCheck DB configuration data.") + (m_VerboseIterationLogging.empty() ? _T("\n") : (_T("\n\nProgram-Lang&Profile iteration logs:\n") + m_VerboseIterationLogging) );
			if (m_ErrorsCreatingScript.empty())
				::MessageBox(Get_Program().getNppWnd(), (_T("Could not find configuration for file extension (") + FILE_EXT + _T(").") + CkDbConfigMsg).c_str(), _T("Missing/Incorrect Database Configuration"), MB_OK | MB_ICONERROR);
			else
				::MessageBox(Get_Program().getNppWnd(), (_T("Could not process file extension (") + FILE_EXT + _T(") due to the following:\n") + m_ErrorsCreatingScript + CkDbConfigMsg).c_str(), (_T("File Extension (") + FILE_EXT + _T(")")).c_str(), MB_OK | MB_ICONERROR);
			m_RunTaskAndReportedError = true;
		}
	}

}

tstring ProcessPrgLanguageAction_Handler::Get_PrgLangAndProfileSig()
{
	return _T("[") + m_PrgLangName + _T(":") + m_PrgLangProfileName + _T("] : ");
}

tstring ProcessPrgLanguageAction_Handler::Get_RequiredPrgProf(const tstring &Flag)
{
	const size_t MaxRead = 200;
	tstring heystack;
	std::ifstream my_if(FQN.c_str(), std::ios::in);
	if (my_if.is_open())
	{
		my_if.seekg(0, my_if.end);
		std::streamoff length = my_if.tellg();
		if (length > MaxRead)
			length = MaxRead;
		my_if.seekg(0, my_if.beg);
		char *buffer = new char[static_cast<size_t>(length) + 1]();
		if (buffer)
		{
			my_if.read(buffer, length);
			heystack = sqlite3pp::to_tstring(buffer);
			delete[] buffer;
		}
		my_if.close();
	}

	if (heystack.size())
	{
		tregex SearchCriteria = tregex(Flag, std::regex_constants::ECMAScript | std::regex_constants::icase);
		tsmatch match;
		if (std::regex_search(heystack, match, SearchCriteria, regex_compatibility_flag))
		{
			return match[2].str();
		}
	}

	return _T("");
}

tstring ProcessPrgLanguageAction_Handler::Get_RequiredProgramLanguage()
{
	return Get_RequiredPrgProf(_T("(^[^A-Za-z0-9]{0,3}(?:prg-|program-|programming-)?(?:lang|language)[\\s\\=\\:]+)([^\\s\\n\\r/\\\\]+)"));
}

tstring ProcessPrgLanguageAction_Handler::Get_RequiredProfileName()
{
	return Get_RequiredPrgProf(_T("(^[^A-Za-z0-9]{0,3}(?:prg-|program-|programming-|lang-|language-){1,2}profile[\\s\\=\\:]+)([^\\s\\n\\r/\\\\]+)"));
}

void ProcessPrgLanguageAction_Handler::ResetLoopVar() // Keep this function here so as to make it easy to remeber to add variables to it if a new loop variable is created.
{
	m_NppExecScript.clear();
	m_PathsToAddToEnvPath.clear();
	m_RunCmd.clear();
	m_PrgLangName.clear();
	m_PrgLangProfileName.clear();
	PreBuildCmd.clear();
	PostBuildCmd.clear();
	RunBuildBinCmd.clear();
	CleanCmd.clear();
	RebuildCmd.clear();
	DebugCmd.clear();
	m_VariablesValue.clear();
	m_TargetFile.clear();
}

std::vector<std::string> ProcessPrgLanguageAction_Handler::getListOfDrives()
{
	std::vector<std::string> arrayOfDrives;
	char szDrives[MAX_PATH + 2] = { 0 };
	if (GetLogicalDriveStringsA(MAX_PATH, szDrives))
		for (int i = 0; i < 100; i += 4)
			if (szDrives[i] != (char)0)
				arrayOfDrives.push_back(std::string{ szDrives[i], szDrives[i + 1], szDrives[i + 2] });
	return arrayOfDrives;
}

tstring ProcessPrgLanguageAction_Handler::AddPathPrefix(tstring name)
{
	if (name.size())
	{
		if (name[0] != '\\')
			name = _T("\\") + name;
	}
	return name;
}

void ProcessPrgLanguageAction_Handler::ExecuteNppExc(const WhereClauseArg &WherePrgLangAndProfile, const sql_table_AdvProfile_Settings &Profile)
{
	Table<sql_table_InstallationPaths> AdditionalInstallPathsForThisProfile(WherePrgLangAndProfile);
	for (auto t : AdditionalInstallPathsForThisProfile)
		GetValidatedInstalledPath(AdditionalInstallPathsForThisProfile, t, true);

	// Setup values before consolidating it into m_NppExecScript
	const tstring SetFocusStr = Profile.get_ChangeFocusToConsleBeforeExecute() ? _T("npp_setfocus con\n") : _T("");
	const tstring NppExecConsoleLogging = Profile.get_NppExecConsoleLogging() ? NPPEXECCONSOLELOGGING : _T("");
	tstring sShowAfterExecuteResults = GetShowAfterExecuteResults();
	boost::ireplace_all(sShowAfterExecuteResults, _T("$(FileName)"), m_TargetFile);

	m_NppExecScript += SetFocusStr + _T("env_set PATH =") + m_PathsToAddToEnvPath + m_EnvPath_PostFix + _T("\n") + CleanCmd + m_Before_PreExecute + PreBuildCmd + m_RunCmd + PostBuildCmd + RunBuildBinCmd + sShowAfterExecuteResults + NppExecConsoleLogging;

	// Having problems with npes logging, so for now continue logging the script content before execute.
#define DO_EXECUTEANYTHING_LOGGING_ON_SCRIPT_CONTENT
#ifdef DO_EXECUTEANYTHING_LOGGING_ON_SCRIPT_CONTENT
		//The following logging SHOULD no longer be needed because now NppExec is supposed to safe the last run script by default.  But I'm not seeing that work consistently yet
	tstring ExecuteAnythingLogFile = m_PlugConfigDir + _T("\\ExecuteAnything_Execute.log");
	tofstream myfile(ExecuteAnythingLogFile.c_str(), std::ios::out);
	if (myfile.is_open())
	{
		myfile.write(m_NppExecScript.c_str(), m_NppExecScript.size());
		myfile.close();
	}
#endif //DO_EXECUTEANYTHING_LOGGING_ON_SCRIPT_CONTENT
	NpeExecuteBase nep;
	nep.szScriptBody = m_NppExecScript.c_str();
	nep.szID = m_IsTestAutomation ? ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID_TESTAUTOMATION : ExecuteAnything::EXECUTEANYTHING_CALLBACK_ID;
	nep.dwResult = 1; // If true, sends a notification
	ExecuteAnything::ResetNotificationFlag();

	if (m_npeMsgr.NpeExecute(&nep, m_npe_execute_type) != NPE_RESULT_OK)
	{
		::MessageBox(Get_Program().getNppWnd(), _T("Operation failed"), _T("NPEM_EXECUTE"), MB_OK | MB_ICONERROR);
		m_RunTaskAndReportedError = true;
	}
	else
		m_RunTaskSuccess = true;

	m_RanATask = true;
}

tstring ProcessPrgLanguageAction_Handler::GetShowAfterExecuteResults()
{
	tstring sShowAfterExecuteResults = ShowAfterExecuteResults;
	if (m_IsTestAutomation)
	{
		Table<sql_table_MiscData> MiscData(WhereClauseArg(_T("Where DataName like '") + TestAutomationSuccessActionDbName + _T("';")));
		if (MiscData.size())
		{
			tstring ReplacementValue(MiscData[0].get_DataValue()->begin(), MiscData[0].get_DataValue()->end());
			boost::replace_all(ReplacementValue, _T("\\n"), _T("\n"));
			boost::replace_all(sShowAfterExecuteResults, _T(ExecuteAnythingSuccessKeywordReplacement), ReplacementValue);
		}
		else
			boost::replace_all(sShowAfterExecuteResults, _T(ExecuteAnythingSuccessKeywordReplacement), _T(""));
	}
	else
		boost::replace_all(sShowAfterExecuteResults, _T(ExecuteAnythingSuccessKeywordReplacement), _T(""));
	return sShowAfterExecuteResults;
}

tstring ProcessPrgLanguageAction_Handler::GetValidatedInstalledPath(Table<sql_table_InstallationPaths> &InstallationPaths, const sql_table_InstallationPaths &InstallationPath, bool AddToEnvPath, bool AddWarningIfCannotFindPath , bool AddSetVarToScript )
{
	tstring InstalledPath = InstallationPath.get_InstalledPath();
	if (InstalledPath.empty())
	{
		InstalledPath = GetValidatedInstalledPath(InstallationPath.get_DefaultInstallPath()
			, AddPathPrefix(InstallationPath.get_ValidationBinFileName())
			, AddPathPrefix(InstallationPath.get_ValidationTextFileName())
			, InstallationPath.get_ValidationFileContent()
		);

		if (InstalledPath.empty())
		{
			if (AddWarningIfCannotFindPath && AddSetVarToScript)
				m_NppExecScript += _T("// Warning: InstallationPaths empty for reference '") + InstallationPath.get_Reference() + _T("'\n");
			return _T("");
		}

		InstallationPaths.Execute(_T("UPDATE InstallationPaths SET InstalledPath = '") + InstalledPath +
			_T("' WHERE PrgLangName like '") + InstallationPath.get_PrgLangName() +
			_T("' and  ProfileName like '") + InstallationPath.get_ProfileName() +
			_T("' and  SrcBinUrlOrUnc like '") + InstallationPath.get_SrcBinUrlOrUnc() +
			_T("' and  DefaultInstallPath like '") + InstallationPath.get_DefaultInstallPath() +
			_T("' and  Reference like '") + InstallationPath.get_Reference() + _T("';"));
	}

	if (m_VariablesValue.find(InstallationPath.get_Reference()) == m_VariablesValue.end())
		m_VariablesValue[InstallationPath.get_Reference()] = InstalledPath;

	tstring PathToSet = _T("set ") + InstallationPath.get_Reference() + _T(" =") + InstalledPath + _T("\n");
	if (AddSetVarToScript && m_NppExecScript.find(PathToSet) == tstring::npos)
		m_NppExecScript += PathToSet;

	tstring PathToAdd = InstalledPath + _T(";");
	if (AddToEnvPath && m_PathsToAddToEnvPath.find(PathToAdd) != 0 && m_PathsToAddToEnvPath.find(_T(";") + PathToAdd) == tstring::npos)
		m_PathsToAddToEnvPath += PathToAdd;

	return InstalledPath;
}

bool ProcessPrgLanguageAction_Handler::TestIfValidInstallation(const tstring &TestPath
	, const tstring &ValidationBinFileName
	, const tstring &ValidationTextFileName
	, const tstring &ValidationFileContent
)
{
	if (sqlite3pp::dir_exists(TestPath))
	{
		if (ValidationBinFileName.size())
		{
			if (!sqlite3pp::file_exists(TestPath + _T("\\") + ValidationBinFileName))
				return false;
		}

		if (ValidationTextFileName.size())
		{
			if (!sqlite3pp::file_exists(TestPath + _T("\\") + ValidationTextFileName))
				return false;

			if (ValidationFileContent.size())
			{
				// ToDo: Add logic to read file content and confirm
			}
		}

		return true;
	}

	return false;
}

tstring ProcessPrgLanguageAction_Handler::GetValidatedInstalledPath(tstring DefaultPath
	, const tstring &ValidationBinFileName
	, const tstring &ValidationTextFileName
	, const tstring &ValidationFileContent
)
{
	const tstring PrgFiles = _T("C:\\Program Files");
	const tstring PrgFilesx86 = _T("C:\\Program Files (x86)");

	const tstring M_LngDev = _T("\\M_LngDev");
	const tstring M_LngDev_Long = _T("\\Multiple_Program_Languages_Development_Environment");
	const tstring CheckM_Paths[] = { M_LngDev, M_LngDev_Long };

	const tstring BasePathEmpty = _T("");
	const tstring M_LngDevPrgFiles = _T("\\__\\PrgFiles");
	const tstring M_LngDevPrgFiles64 = _T("\\__\\PrgFiles64");
	const tstring M_LngDevPrgFiles86 = _T("\\__\\PrgFiles86");
	const tstring CheckPaths[] = { BasePathEmpty, M_LngDevPrgFiles, M_LngDevPrgFiles64, M_LngDevPrgFiles86 };

	if (DefaultPath.empty())
		return tstring();

	ReplaceVariablesWithValue(DefaultPath);

	if (DefaultPath.find(_T("*")) != tstring::npos || DefaultPath.find(_T("?")) != tstring::npos)
	{
		find_folders_rsort finddir(DefaultPath.c_str(), false);
		for (auto apath : finddir.GetRevSortedPaths()) // Use reverse iterator to test string ending in higher number first.  IE: C:\FPC\3.2.2 before C:\FPC\2.1.0
			if (TestIfValidInstallation(apath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
				return apath;
		return tstring();
	}

	if (TestIfValidInstallation(DefaultPath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
		return DefaultPath;

	// Remove PrgFiles, drive letter, and ":". Also make sure path starts with "\\"
	if (DefaultPath._Starts_with(PrgFiles))
		DefaultPath = DefaultPath.substr(PrgFiles.size());
	else if (DefaultPath._Starts_with(PrgFilesx86))
		DefaultPath = DefaultPath.substr(PrgFilesx86.size());
	//if (DefaultPath.substr(0, PrgFiles.size()) == PrgFiles)
	//	DefaultPath = DefaultPath.substr(PrgFiles.size());
	//else if (DefaultPath.substr(0, PrgFilesx86.size()) == PrgFilesx86)
	//	DefaultPath = DefaultPath.substr(PrgFilesx86.size());
	else if (DefaultPath.size() > 2 && DefaultPath[1] == ':')
		DefaultPath = DefaultPath.substr(2);
	else if (DefaultPath[0] != '\\')
		DefaultPath = _T("\\") + DefaultPath;

	tstring InstalledPath;
	//Check if installed in other drives
	std::vector<std::string> Drives = getListOfDrives();
	for (auto d : Drives)
	{
		InstalledPath = sqlite3pp::to_tstring(d) + _T(":") + DefaultPath;
		if (TestIfValidInstallation(InstalledPath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
			return InstalledPath;

		for (auto pmLngDev : CheckM_Paths)
		{
			tstring BasePath = sqlite3pp::to_tstring(d) + _T(":") + pmLngDev;
			if (sqlite3pp::dir_exists(BasePath))
			{
				for (auto p : CheckPaths)
				{
					InstalledPath = BasePath + p + DefaultPath;
					if (TestIfValidInstallation(InstalledPath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
						return InstalledPath;
				}
			}
		}
	}

	//Check if installed in Programs Files
	if (DefaultPath.size() > 2)
	{
		InstalledPath = _T("C:\\Program Files") + DefaultPath;
		if (TestIfValidInstallation(InstalledPath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
			return InstalledPath;
		InstalledPath = _T("C:\\Program Files (x86)") + DefaultPath;
		if (TestIfValidInstallation(InstalledPath, ValidationBinFileName, ValidationTextFileName, ValidationFileContent))
			return InstalledPath;
	}


	return tstring();
}

sql_table_ProgrammingLanguageProfiles ProcessPrgLanguageAction_Handler::Get_ProgrammingLanguageProfile(const WhereClauseArg &WherePrgLangAndProfile)
{
	sql_table_ProgrammingLanguageProfiles Profile;
	Table<sql_table_ProgrammingLanguageProfiles> Profiles(WherePrgLangAndProfile);
	if (Profiles.size())
		return Profiles[0];
	return Profile;
}

sql_table_AdvProfile_Settings ProcessPrgLanguageAction_Handler::Get_AdvProfileSettings(const WhereClauseArg &WherePrgLangAndProfile)
{
	sql_table_AdvProfile_Settings Profile;
	Table<sql_table_AdvProfile_Settings> AdvProfile_Settings(WherePrgLangAndProfile);
	if (!AdvProfile_Settings.size())
	{
		AdvProfile_Settings.Execute(_T("INSERT INTO AdvProfile_Settings (PrgLangName, ProfileName) VALUES ('") + m_PrgLangName + _T("', '") + m_PrgLangProfileName + _T("')"));
		AdvProfile_Settings.ReQuery();
	}
	if (AdvProfile_Settings.size())
		Profile = AdvProfile_Settings[0];
	return Profile;
}

void ProcessPrgLanguageAction_Handler::Get_LangInstallationUsage(const WhereClauseArg &WherePrgLangAndProfile)
{
	Table<sql_table_LangInstallationUsage> LangInstallationUsage(WherePrgLangAndProfile);
	for (auto li : LangInstallationUsage)
		CheckInstallationPathReference(li.get_Reference(), true);
}

void ProcessPrgLanguageAction_Handler::PopulateVariablesValue(const WhereClauseArg &WherePrgLangAndProfile)
{
	Table<sql_table_AdvProfile_Variables> AdvProfile_Variables(WherePrgLangAndProfile);
	for (auto pv : AdvProfile_Variables)
	{
		CheckPathVariableNames(pv.get_VarValue(), false, false);
		m_VariablesValue[pv.get_VarName()] = GetReplaceVariablesWithValue(pv.get_VarValue());
	}
	Table<sql_table_LangInstallationUsage> LangInstallationUsage(WherePrgLangAndProfile);
	for (auto li : LangInstallationUsage)
		CheckInstallationPathReference(li.get_Reference(), false, false);

	for (int i = 0; i < 10; ++i) // Try running at most 10 iterations to parse out all the variable values
	{
		bool FoundVariables = false;
		for (auto vv : m_VariablesValue)
		{
			tsmatch match;
			if (std::regex_search(vv.second, match, SearchVariables, regex_compatibility_flag))
			{
				FoundVariables = true;
				tstring parsed_path = GetReplaceVariablesWithValue(vv.second);
				m_VariablesValue[vv.first] = parsed_path;
			}
		}
		if (!FoundVariables)
			break;
	}
}

void ProcessPrgLanguageAction_Handler::CreateFilesAndFolders(const sql_table_AdvProfile_Variables &pv, tstring AdvVarFileName)
{
	if (AdvVarFileName.size() < 3 || AdvVarFileName[1] != ':')
		AdvVarFileName = GetReplaceVariablesWithValue(FILE_DIR + _T("\\") + AdvVarFileName);// If AdvVarFileName has no drive letter, then treat as sub dir under FILE_DIR

	tstring FileArg = GetReplaceVariablesWithValue(pv.get_FileArg());
	if (FileArg.size() < 3 || FileArg[1] != ':')
		FileArg = GetReplaceVariablesWithValue(FILE_DIR + _T("\\") + pv.get_FileArg());// If FileArg has no drive letter, then treat as sub dir under FILE_DIR

	int Action = pv.get_Dir();
	enum { ActionCreate = 1, ActionCreateWithContent = 2, ActionCopy = 4, ActionMove = 8, ActionRename = 16, ActionDelete = 32, ActionChangeDir = 64, ActionIgnoreIfExist = 128 };
	if (Action)
	{// Bit arguments. 128 = Ignore 1, 4, 8, &16 if dest dir exists; 1 = creates dir having dir name VarValue; 4 = copies dir from FileArg to VarValue; 8 = moves dir from FileArg to VarValue path; 16 = renames dir from FileArg to VarValue name; 32 = deletes dir named VarValue;
		if (dir_exists(AdvVarFileName))
		{
			if (Action & ActionDelete)
				_trmdir(AdvVarFileName.c_str());
			else if (!(Action & ActionIgnoreIfExist))
				if (Action & (ActionCreate + ActionCopy + ActionMove + ActionRename))
					_trmdir(AdvVarFileName.c_str());
		}

		if (!dir_exists(AdvVarFileName))
		{
			if (Action & ActionCreate)
				_tmkdir(AdvVarFileName.c_str());
			if ((Action & (ActionRename + ActionMove)) && dir_exists(FileArg)) // ToDo: Move doesn't work accross drives.  Add logic to support moving directory from one drive to another
				_trename(FileArg.c_str(), AdvVarFileName.c_str());
			// ToDo: Add logic to support coping directories
		}

		if (dir_exists(AdvVarFileName) && (Action & ActionChangeDir))  // Need this last, incase path has to be created in above section.
			m_Before_PreExecute += _T("CD \"") + AdvVarFileName + _T("\"\n");
	}

	Action = pv.get_File();
	if (Action)
	{// Bit arguments. 128 = Ignore 1, 2, 4, 8, &16 if dest file exists; 1 = creates empty file having file name VarValue; 2 = creates file with FileArg content having file name VarValue; 4 = copies file from VarValue to FileArg; 8 = moves file from VarValue to FileArg path; 16 = renames file from VarValue to FileArg name; 32 = deletes file named VarValue;
		if (file_exists(AdvVarFileName))
		{
			if (Action & ActionDelete)
				_tremove(AdvVarFileName.c_str());
			else if (!(Action & ActionIgnoreIfExist))
				if (Action & (ActionCreate + ActionCopy + ActionMove + ActionRename))
					_tremove(AdvVarFileName.c_str());
		}

		if (!file_exists(AdvVarFileName))
		{
			if (Action & (ActionCreate + ActionCreateWithContent))
			{
				tofstream myfile(AdvVarFileName.c_str(), std::ios::out);
				if (myfile.is_open())
				{
					if (Action & ActionCreateWithContent)
						myfile.write(FileArg.c_str(), FileArg.size());
					myfile.close();
				}
			}
			else if (file_exists(FileArg))
			{
				if (Action & (ActionRename + ActionMove)) // ToDo: Move doesn't work accross drives.  Add logic to support moving directory from one drive to another
					_trename(FileArg.c_str(), AdvVarFileName.c_str());
				else if (Action & ActionCopy)
					copy_file(FileArg, AdvVarFileName);
			}
		}
	}
}

void ProcessPrgLanguageAction_Handler::Get_ProfileVariables(const WhereClauseArg &WherePrgLangAndProfile)
{
	WhereClauseArg whereclausearg = WherePrgLangAndProfile.get_Str() + _T(" order by ProcessOrder");
	Table<sql_table_AdvProfile_Variables> AdvProfile_Variables(whereclausearg);
	for (auto pv : AdvProfile_Variables)
	{
		tstring VarValue = GetReplaceVariablesWithValue(pv.get_VarValue());
		if (pv.get_AddToPath())
		{
			// Check if the path uses variables, and if so, validate the paths
			CheckPathVariableNames(pv.get_VarValue(), true);

			VarValue = GetReplaceVariablesWithValue(pv.get_VarValue());  // Parse it out again, incase CheckPathVariableNames added additional variables to parse
			m_PathsToAddToEnvPath += VarValue + _T(";");
		}

		if (pv.get_GetValueFromSrcFile())
		{
			const int max_TextData = 2000;
			char TextData[max_TextData + 10] = { 0 };
			int TextLen = m_CSciMessager.getTextLength();
			if (TextLen > max_TextData)
				TextLen = max_TextData;
			if (m_CSciMessager.getTextRange(0, TextLen, TextData) > 8)
			{
				tstring heystack = to_tstring(TextData);
				tregex SearchCriteria = tregex(pv.get_VarValue(), std::regex_constants::ECMAScript | std::regex_constants::icase);
				tsmatch match;
				if (std::regex_search(heystack, match, SearchCriteria, regex_compatibility_flag))
				{
					VarValue = match[1].str();
				}
			}
		}

		if (pv.get_SetVar())
			m_NppExecScript += _T("set ") + pv.get_VarName() + _T(" =") + VarValue + _T("\n");

		if (pv.get_SetLocalVar())
			m_NppExecScript += _T("set local ") + pv.get_VarName() + _T(" =") + VarValue + _T("\n");

		if (pv.get_SetEnvVar())
			m_NppExecScript += _T("env_set ") + pv.get_VarName() + _T(" =") + VarValue + _T("\n");

		if (pv.get_AddVarValueToEnvVar().size())
			m_NppExecScript += _T("env_set ") + pv.get_AddVarValueToEnvVar() + _T(" =") + VarValue + _T(";$(") + pv.get_AddVarValueToEnvVar() + _T(")\n");

		if (pv.get_AddVarNameToEnvVar().size())
			m_NppExecScript += _T("env_set ") + pv.get_AddVarNameToEnvVar() + _T(" =$(") + pv.get_VarName() + _T(");$(") + pv.get_AddVarNameToEnvVar() + _T(")\n");

		if (pv.get_Dir() || pv.get_File())
			CreateFilesAndFolders(pv, VarValue);

		if ((pv.get_SetVar() || pv.get_SetLocalVar() || pv.get_SetEnvVar()) && m_VariablesValue.find(pv.get_VarName()) == m_VariablesValue.end())
			m_VariablesValue[pv.get_VarName()] = VarValue;
	}
}

void ProcessPrgLanguageAction_Handler::SetInitialScriptVar()
{
	m_NppExecScript += _T("set FQN=") + FQN + _T("\n");
	m_NppExecScript += _T("set FILE_EXT=") + FILE_EXT + _T("\n");
	m_NppExecScript += _T("set FILE_BASE=") + FILE_BASE + _T("\n");
	m_NppExecScript += _T("set FILE_DIR=") + FILE_DIR + _T("\n");
	m_NppExecScript += _T("set FILE_DIR_AND_BASE=") + FILE_DIR_AND_BASE + _T("\n");
	m_NppExecScript += _T("SET filename ~ STRLOWER $(FILE_NAME)\n");
	m_NppExecScript += _T("set obj = $(FILE_DIR_AND_BASE)\n");
	m_NppExecScript += _T("SET FILE_NAME_NO_EXT_LOWER ~ STRLOWER $(FILE_BASE)\n");  // ToDo: This is only used by one language so far.  Consider adding this only when needed.
	m_NppExecScript += _T("set PRG_Lang=") + m_PrgLangName + _T("\n");
	m_NppExecScript += _T("set PROFILE=") + m_PrgLangProfileName + _T("\n");
	m_VariablesValue[_T("FQN")] = FQN;
	m_VariablesValue[_T("FILE_EXT")] = FILE_EXT;
	m_VariablesValue[_T("FILE_BASE")] = FILE_BASE;
	m_VariablesValue[_T("FILE_DIR")] = FILE_DIR;
	m_VariablesValue[_T("FILE_DIR_AND_BASE")] = FILE_DIR_AND_BASE;
	m_VariablesValue[_T("FILE_NAME")] = FILE_NAME;
	m_VariablesValue[_T("filename")] = Common::ToLower(FILE_NAME); // ToDo: Make this equal to lower case value
	m_VariablesValue[_T("obj")] = FILE_DIR_AND_BASE;
	m_VariablesValue[_T("FILE_NAME_NO_EXT_LOWER")] = Common::ToLower(FILE_BASE); // ToDo: Make this equal to lower case value
	m_VariablesValue[_T("PRG_Lang")] = m_PrgLangName;
	m_VariablesValue[_T("PROFILE")] = m_PrgLangProfileName;

}

void ProcessPrgLanguageAction_Handler::Get_UnixPaths()
{
	tstring UNIX_DIR = _T("/") + boost::replace_all_copy(FILE_DIR, _T("\\"), _T("/"));
	boost::replace_all(UNIX_DIR, _T(":"), _T(""));
	tstring UNIX_PATH = _T("/") + boost::replace_all_copy(FQN, _T("\\"), _T("/"));
	boost::replace_all(UNIX_PATH, _T(":"), _T(""));
	m_NppExecScript += _T("set UNIX_DIR=") + UNIX_DIR + _T("\n");
	m_NppExecScript += _T("set UNIX_PATH=") + UNIX_PATH + _T("\n");
	m_VariablesValue[_T("UNIX_DIR")] = UNIX_DIR;
	m_VariablesValue[_T("UNIX_PATH")] = UNIX_PATH;
}

bool ProcessPrgLanguageAction_Handler::CheckInstallationPathReference(Table<sql_table_InstallationPaths> &InstallationPaths, bool AddToEnvPath, bool AddSetVarToScript )
{
	for (auto InstallPath : InstallationPaths)
		if (GetValidatedInstalledPath(InstallationPaths, InstallPath, AddToEnvPath, false, AddSetVarToScript).size())
			return true;
	return false;
}

bool ProcessPrgLanguageAction_Handler::CheckInstallationPathReference(const tstring &Reference, bool AddToEnvPath, bool AddSetVarToScript )  // Find the best match
{
	// If available, get InstallationPaths associated with the program language and profile
	Table<sql_table_InstallationPaths> InstallationPathsForPrgLangProfile(WhereClauseArg(_T("where PrgLangName like '") + m_PrgLangName + _T("' and ProfileName like '") + m_PrgLangProfileName + _T("' and Reference like '") + Reference + _T("' ORDER BY DefaultInstallPath DESC")));
	if (CheckInstallationPathReference(InstallationPathsForPrgLangProfile, AddToEnvPath, AddSetVarToScript))
		return true;

	// If available, get InstallationPaths associated with the program language with [any] profile
	Table<sql_table_InstallationPaths> InstallationPathsForPrgLangAnyProfile(WhereClauseArg(_T("where PrgLangName like '") + m_PrgLangName + _T("' and ProfileName like '[any]' and Reference like '") + Reference + _T("' ORDER BY DefaultInstallPath DESC")));
	if (CheckInstallationPathReference(InstallationPathsForPrgLangAnyProfile, AddToEnvPath, AddSetVarToScript))
		return true;

	// If no installations found with the specific program language, then search for generic installations.
	Table<sql_table_InstallationPaths> InstallationPaths(WhereClauseArg(_T("where PrgLangName like '[any]' and ProfileName like '[any]' and Reference like '") + Reference + _T("' ORDER BY DefaultInstallPath DESC")));
	if (CheckInstallationPathReference(InstallationPaths, AddToEnvPath, AddSetVarToScript))
		return true;

	return false;
}

void ProcessPrgLanguageAction_Handler::CheckPathVariableNames(const tstring &strHaystack, bool AddToEnvPath, bool AddSetVarToScript )
{
	tsmatch match;
	tstring::const_iterator start = strHaystack.begin(), end = strHaystack.end();
	while (std::regex_search(start, end, match, SearchVariables, regex_compatibility_flag))
	{
		tstring VariableName = match[1].str();
		tstring SetVarToSearch1 = _T("set ") + VariableName + _T(" =");
		tstring SetVarToSearch2 = _T("set ") + VariableName + _T("=");
		if (!boost::ifind_first(m_NppExecScript, SetVarToSearch1)
			&& !boost::ifind_first(m_NppExecScript, SetVarToSearch2))
		{
			if (CheckInstallationPathReference(VariableName, AddToEnvPath, AddSetVarToScript))
			{
				Table<sql_table_LangInstallationUsage> LangInstallationUsage;
				LangInstallationUsage.Execute(_T("INSERT INTO LangInstallationUsage (PrgLangName, ProfileName, Reference) VALUES ('")
					+ m_PrgLangName + _T("', '") + m_PrgLangProfileName + _T("', '") + VariableName + _T("')"));

			}
		}
		start = match.suffix().first;
	}
}

bool ProcessPrgLanguageAction_Handler::GetDirAndExecuter(const tstring &src, tstring &Runner)
{
	tsmatch match;
	if (!std::regex_search(src, match, SearchDirSeperator))
	{
		Runner = src;
		CheckPathVariableNames(Runner, true);
	}
	else if (std::regex_search(src, match, SearchFileName))
	{
		Runner = match[0].str().substr(1);
		tstring Runner_dir = src.substr(0, src.size() - match[0].str().size());
		m_PathsToAddToEnvPath += Runner_dir + _T(";");
		CheckPathVariableNames(Runner_dir, false);
	}
	else
	{
		::MessageBox(Get_Program().getNppWnd(), _T("Could not find runner in pathsrc"), _T("Runner not found!"), MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

void ProcessPrgLanguageAction_Handler::Remov_File(tstring filename)
{
	if (file_exists(filename.c_str()))
		DeleteFile(filename.c_str());

	if (m_IsTestAutomation)
	{
		CleanCmd += _T("cmd /C if exist \"") + filename + _T("\" del  /Q /F  \"") + filename + _T("\"\n");
	}
}

void ProcessPrgLanguageAction_Handler::CleanCompilerBuildEnvironment(const sql_table_ProgrammingLanguageProfiles &CompilerProfile, const sql_table_AdvProfile_Settings &AdvProfile, const tstring &Compiler, const tstring &BinaryToExecute, tstring &/*HelpAlias*/)
{
	tstring BinFileToDelete = FILE_DIR + _T("\\") + BinaryToExecute;
	ReplaceVariablesWithValue(BinFileToDelete);
	Remov_File(BinFileToDelete);

	// Files that always get deleted before build/compile command
	if (CompilerProfile.get_FilesToDeleteBeforeBuild().size())
	{
		// ToDo:(Low-Priority) Consider adding alias cmd to delete these files from the script
		// HelpAlias += _T("$(__next_line)D=Delete Build files");
		const tstring DefaultFilesToDelete = _T("$(FILE_BASE).exe,$(FILE_BASE).exp,$(FILE_BASE).lib,$(FILE_BASE).o,$(FILE_BASE).obj,$(FILE_BASE).nl,$(FILE_BASE).ilk,$(FILE_BASE).pdb,$(FILE_BASE).ali");
		tstring FilesToDeleteOnClean = CompilerProfile.get_FilesToDeleteBeforeBuild();
		if (FilesToDeleteOnClean == _T("Default"))
			FilesToDeleteOnClean = DefaultFilesToDelete;
		std::vector<tstring> strs;
		boost::split(strs, FilesToDeleteOnClean, boost::is_any_of(","));
		for (auto filename : strs)
		{
			tstring FileToDelete = FILE_DIR + _T("\\") + filename;
			ReplaceVariablesWithValue(FileToDelete);
			Remov_File(FileToDelete);
		}
	}

	// This section only gets executed in a rebuild or clean request
	if (m_PrgLangProcessType & TYPE_CLEAN)
	{
		if (AdvProfile.get_CleanCmdFormat().size())
		{
			tstring Clean;
			if (AdvProfile.get_CleanCmdBin().size() && GetDirAndExecuter(AdvProfile.get_CleanCmdBin(), Clean))
				m_NppExecScript += _T("set Clean=") + Clean + _T("\n");
			if (AdvProfile.get_CleanCmdFormat().size())
			{
				if (Clean.empty())
					m_NppExecScript += _T("set Clean=") + Compiler + _T("\n");
				m_NppExecScript += _T("set CleanCmd=") + AdvProfile.get_CleanCmdFormat() + _T("\n");
				CleanCmd += CleanCmdStr;
			}
			// ToDo:(Low-Priority) Consider adding alias cmd to excecute clean action
			// HelpAlias += _T("$(__next_line)N=Clean Build files");
		}
	}
}

void ProcessPrgLanguageAction_Handler::ReplaceVariablesWithValue(tstring &target)
{
	for (auto vv : m_VariablesValue)
	{
		tstring VariableName = _T("$(") + vv.first + _T(")");
		boost::ireplace_all(target, VariableName, vv.second);
	}
	target = Get_UpdatedPathCopy(target);
}

tstring ProcessPrgLanguageAction_Handler::GetReplaceVariablesWithValue(tstring target)
{
	ReplaceVariablesWithValue(target);
	return target;
}

void ProcessPrgLanguageAction_Handler::Setup_Prerequisite(const WhereClauseArg &WherePrgLangAndProfile)
{
	sqlite3pp::Table<sql_table_Prerequisites> Prerequisites(WherePrgLangAndProfile);
	for (auto Prerequisite : Prerequisites)
	{
		tstring PathName = FILE_DIR + _T("\\") + Prerequisite.get_PathName();
		ReplaceVariablesWithValue(PathName);
		if (Prerequisite.get_IsDir())
		{
			if (!sqlite3pp::dir_exists(PathName))
				_tmkdir(PathName.c_str());
		}
		else
		{
			if (sqlite3pp::file_exists(PathName))
				DeleteFile(PathName.c_str());
			tofstream myfile(PathName.c_str(), std::ios::out);
			if (myfile.is_open())
			{
				tstring FileContent = Prerequisite.get_FileContent();
				ReplaceVariablesWithValue(FileContent);
				myfile.write(FileContent.c_str(), FileContent.size());
				myfile.close();
			}
		}
	}
}

bool ProcessPrgLanguageAction_Handler::Get_ScriptProfile()
{
	WhereClauseArg WherePrgLangAndProfile(_T("Where \"PrgLangName\" like '") + m_PrgLangName + _T("' and \"ProfileName\" like '") + m_PrgLangProfileName + _T("' and length(Launcher) > 0"));
	tstring Runner;
	sqlite3pp::Table<sql_table_ProgrammingLanguageProfiles> ScriptProfiles(WherePrgLangAndProfile);
	if (!ScriptProfiles.size())
	{
		m_ErrorsCreatingScript += _T("Could not find script profile using clause   [") + WherePrgLangAndProfile.get_Str() + _T("]!\n");
		return false;
	}

	const sql_table_ProgrammingLanguageProfiles &ScriptProfile = ScriptProfiles[0];
	if (!GetDirAndExecuter(ScriptProfile.get_Launcher(), Runner))
	{
		::MessageBox(Get_Program().getNppWnd(), _T("Could not find runner in path v_ProfileDetails.Cmd"), _T("Runner not found!"), MB_OK | MB_ICONERROR);
		m_RunTaskAndReportedError = true;
		return false;
	}

	m_NppExecScript += _T("set Runner=") + Runner + _T("\nset ExecuteCMD=") + ScriptProfile.get_LauncherCmdFormat() + _T("\n");
	m_NppExecScript += _T("npe_cmdalias r = $(ExecuteCMD)\n");
	m_NppExecScript += AliasHelpPrefixStr + _T("$(__next_line)R=Run-Script") + AliasHelpPostfixStr + _T("\n");
	m_RunCmd = ScriptCmdStr;
	return true;
}

bool ProcessPrgLanguageAction_Handler::Get_CompilerProfile(const sql_table_AdvProfile_Settings &AdvProfile)
{
	WhereClauseArg WherePrgLangAndProfile(_T("Where \"PrgLangName\" like '") + m_PrgLangName + _T("' and \"ProfileName\" like '") + m_PrgLangProfileName + _T("' and length(Compiler) > 0"));
	sqlite3pp::Table<sql_table_ProgrammingLanguageProfiles> ProgrammingLanguageProfiles(WherePrgLangAndProfile);
	if (!ProgrammingLanguageProfiles.size())
	{
		m_ErrorsCreatingScript += _T("Could not find compiler profile using clause   [") + WherePrgLangAndProfile.get_Str() + _T("]!\n");
		return false;
	}

	const sql_table_ProgrammingLanguageProfiles &CompilerProfile = ProgrammingLanguageProfiles[0];
	tstring BinaryToCompile = CompilerProfile.get_BinaryToCompile();
	if (BinaryToCompile.empty())
		BinaryToCompile = _T("$(FILE_BASE).exe");
	m_NppExecScript += _T("set BinaryToCompile=") + BinaryToCompile + _T("\n");
	tstring BinaryToExecute = CompilerProfile.get_BinaryToExecute();
	// If empty, make BinaryToExecute equal BinaryToCompile
	if (BinaryToExecute.empty())
		BinaryToExecute = BinaryToCompile;
	m_NppExecScript += _T("set BinaryToExecute=") + BinaryToExecute + _T("\n");
	m_TargetFile = BinaryToExecute;
	m_NppExecScript += _T("set DebugBuild=\n");

	tstring Compiler;
	if (!GetDirAndExecuter(CompilerProfile.get_Compiler(), Compiler))
	{
		::MessageBox(Get_Program().getNppWnd(), _T("Could not find Compiler in path CompilerProfile.get_CleanCmdFormat()"), _T("Compiler not found!"), MB_OK | MB_ICONERROR);
		m_RunTaskAndReportedError = true;
		return false;
	}
	m_NppExecScript += _T("set Compiler=") + Compiler + _T("\nset BuildCmd=") + CompilerProfile.get_CompilerCmdFormat() + _T("\n");
	m_NppExecScript += _T("npe_cmdalias c = $(BuildCmd)\n");

	tstring HelpAlias = _T("$(__next_line)C=Compile/Build file");
	CleanCompilerBuildEnvironment(CompilerProfile, AdvProfile, Compiler, BinaryToExecute, HelpAlias);

	if (AdvProfile.get_PreBuildCmdFormat().size())
	{
		tstring PreBuild;
		if (AdvProfile.get_PreBuildCmdBin().size() && GetDirAndExecuter(AdvProfile.get_PreBuildCmdBin(), PreBuild))
			m_NppExecScript += _T("set PreBuild=") + PreBuild + _T("\n");
		if (PreBuild.empty())
			m_NppExecScript += _T("set PreBuild=") + Compiler + _T("\n");
		m_NppExecScript += _T("set PreBuildCmd=") + AdvProfile.get_PreBuildCmdFormat() + _T("\n");
		PreBuildCmd = PreBuildCmdStr;
		m_NppExecScript += _T("npe_cmdalias p = $(PreBuildCmd)\n");
		HelpAlias += _T("$(__next_line)P=Pre-Build-Cmd");
	}

	if (CompilerProfile.get_PostBuildCmdFormat().size())
	{
		tstring PostBuild;
		if (CompilerProfile.get_PostBuildCmdBin().size() && GetDirAndExecuter(CompilerProfile.get_PostBuildCmdBin(), PostBuild))
			m_NppExecScript += _T("set PostBuild=") + PostBuild + _T("\n");
		if (PostBuild.empty())
			m_NppExecScript += _T("set PostBuild=") + Compiler + _T("\n");
		m_NppExecScript += _T("set PostBuildCmd=") + CompilerProfile.get_PostBuildCmdFormat() + _T("\n");
		PostBuildCmd = PostBuildCmdStr;
		m_NppExecScript += _T("npe_cmdalias l = $(PostBuildCmd)\n");
		HelpAlias += _T("$(__next_line)L=Link/PostBuild-Cmd");
	}

	m_NppExecScript += _T("npe_cmdalias r = $(BinaryToExecute)\n");
	m_NppExecScript += AliasHelpPrefixStr + _T("$(__next_line)R=Run-Bin") + HelpAlias + AliasHelpPostfixStr + _T("\n");
	RunBuildBinCmd = RunBuildBinCmdStr;
	m_RunCmd = CompileCmdStr;
	return true;
}

void ProcessPrgLanguageAction_Handler::Set_CompilerAndScript_ExistValues()
{
	WhereClauseArg WhereThisFileExtForScripts(_T("where Ext like '") + FILE_EXT + _T("' and isScript = 1"));
	Table<sql_view_v_ProgrammingLanguageProfiles> ScriptProfile(WhereThisFileExtForScripts);
	WhereClauseArg WhereThisFileExtForCompilers(_T("where Ext like '") + FILE_EXT + _T("' and isCompiler = 1"));
	Table<sql_view_v_ProgrammingLanguageProfiles> CompilerProfile(WhereThisFileExtForCompilers);
	m_ScriptProfileExist = ScriptProfile.size() > 0;
	m_CompileProfileExist = CompilerProfile.size() > 0;

}

void ProcessPrgLanguageAction_Handler::ProcessHelpRequest()
{
	m_NppExecScript += _T("set Selected_Word=") + Selected_Word + _T("\n");
	// ToDo: Add code here to check if a word is selected, and get program language specific help
	// ToDo: Add logic to the database to include multiple help sources
	// ToDo: Add logic to DB to get help on language key words.  Might need an additional database file just for program language specific help details.
}



















