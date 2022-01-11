/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
#ifndef PROCESSPRGLANGUAGEACTION_
#define PROCESSPRGLANGUAGEACTION_

#include "ExecuteAnything_Menu.h"
#include "NppExecPluginMsgSender.h"
class ProcessPrgLanguageAction : protected ExecuteAnything_Menu
{
	bool m_RanATask;
	bool m_RunTaskSuccess;
	bool m_RunTaskAndReportedError;
public:
	ProcessPrgLanguageAction(CNppExecPluginMsgSender &npeMsgr, PrgLangProcessType prglangprocesstype);
	bool get_RanATask() const { return m_RanATask; }
	bool get_RunTaskSuccess() const { return m_RunTaskSuccess; }
	bool get_RunTaskAndReportedError() const { return m_RunTaskAndReportedError; }
	static const tstring &GetExecuteAnythingSuccessMsgID();
};

#ifdef INCLUDING_FROM_PROCESSPRGLANGUAGEACTION_CPP
#include "ProcessPrgLanguageAction.hpp"  // Contains required headers and constants for ProcessPrgLanguageAction_Handler
using namespace sqlite3pp;
class  ProcessPrgLanguageAction_Handler : protected ExecuteAnything_Menu
{
public:
	ProcessPrgLanguageAction_Handler(CNppExecPluginMsgSender &npeMsgr, PrgLangProcessType prglangprocesstype);
	bool get_RanATask() const { return m_RanATask; }
	bool get_RunTaskSuccess() const { return m_RunTaskSuccess; }
	bool get_RunTaskAndReportedError() const { return m_RunTaskAndReportedError; }
private:
	CNppExecPluginMsgSender &m_npeMsgr;
	const PrgLangProcessType m_PrgLangProcessType;
	CSciMessager m_CSciMessager;
	////////////////////////////////////////////////////////////////////////////////////////////
	// Loop associated variables for -- for (auto csProfl : ProgrammingLanguageProfiles)
	// Make sure to reset these variables to empty string on each loop -- for (auto csProfl : ProgrammingLanguageProfiles)
	tstring m_NppExecScript;
	tstring m_Before_PreExecute;
	tstring m_PathsToAddToEnvPath;
	tstring m_RunCmd;
	tstring m_PrgLangName;
	tstring m_PrgLangProfileName;
	tstring m_TargetFile;
	tstring PreBuildCmd; // Keep variable name as-is so as to match the variable name in the script
	tstring PostBuildCmd;// Keep variable name as-is so as to match the variable name in the script
	tstring RunBuildBinCmd;// Keep variable name as-is so as to match the variable name in the script
	tstring CleanCmd; // Keep variable name as-is so as to match the variable name in the script
	tstring RebuildCmd; // Keep variable name as-is so as to match the variable name in the script
	tstring DebugCmd; // Keep variable name as-is so as to match the variable name in the script
	std::map<tstring, tstring> m_VariablesValue;
	void ResetLoopVar();
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	// Note: Leave these variable names as-is so they match the script's initial variable names
	const tstring FILE_EXT;
	const tstring FILE_BASE;
	const tstring FILE_DIR;
	const tstring FILE_DIR_AND_BASE;
	const tstring FQN;
	const tstring Selected_Word;
	const tstring FILE_NAME;
	const tstring FileName;
	////////////////////////////////////////////////////////////////////////////////////////////
	const tstring m_PlugConfigDir;
	const WhereClauseArg m_WhereThisFileExt;
	bool m_RanATask;
	bool m_RunTaskSuccess;
	bool m_RunTaskAndReportedError;
	bool m_ScriptProfileExist;
	bool m_CompileProfileExist;
	tstring m_EnvPath_PostFix;
	tstring m_ErrorsCreatingScript;
	bool m_IsTestAutomation;
	NPE_EXECUTE_TYPE m_npe_execute_type;
	tstring m_VerboseIterationLogging;
protected:
	std::vector<std::string> getListOfDrives();
	tstring AddPathPrefix(tstring name);
	sqlite3pp::database &m_db;
	void ExecuteNppExc(const WhereClauseArg &WherePrgLangAndProfile, const sql_table_AdvProfile_Settings &Profile);
	tstring GetShowAfterExecuteResults();
	tstring GetValidatedInstalledPath(Table<sql_table_InstallationPaths> &InstallationPaths, const sql_table_InstallationPaths &InstallationPath, bool AddToEnvPath, bool AddWarningIfCannotFindPath = true, bool AddSetVarToScript = true);
	bool TestIfValidInstallation(const tstring &TestPath
		, const tstring &ValidationBinFileName
		, const tstring &ValidationTextFileName
		, const tstring &ValidationFileContent
	);
	tstring GetValidatedInstalledPath(tstring DefaultPath
		, const tstring &ValidationBinFileName
		, const tstring &ValidationTextFileName
		, const tstring &ValidationFileContent
	);
	sql_table_ProgrammingLanguageProfiles Get_ProgrammingLanguageProfile(const WhereClauseArg &WherePrgLangAndProfile);
	sql_table_AdvProfile_Settings Get_AdvProfileSettings(const WhereClauseArg &WherePrgLangAndProfile);
	void Get_LangInstallationUsage(const WhereClauseArg &WherePrgLangAndProfile);
	void PopulateVariablesValue(const WhereClauseArg &WherePrgLangAndProfile);
	void CreateFilesAndFolders(const sql_table_AdvProfile_Variables &pv, tstring AdvVarFileName);
	void Get_ProfileVariables(const WhereClauseArg &WherePrgLangAndProfile);
	void SetInitialScriptVar();
	void Get_UnixPaths();
	bool CheckInstallationPathReference(Table<sql_table_InstallationPaths> &InstallationPaths, bool AddToEnvPath, bool AddSetVarToScript = true);
	bool CheckInstallationPathReference(const tstring &Reference, bool AddToEnvPath, bool AddSetVarToScript = true);  // Find the best match
	void CheckPathVariableNames(const tstring &strHaystack, bool AddToEnvPath, bool AddSetVarToScript = true);
	bool GetDirAndExecuter(const tstring &src, tstring &Runner);
	void Remov_File(tstring filename);
	void CleanCompilerBuildEnvironment(const sql_table_ProgrammingLanguageProfiles &CompilerProfile, const sql_table_AdvProfile_Settings &AdvProfile, const tstring &Compiler, const tstring &BinaryToExecute, tstring &/*HelpAlias*/);
	void ReplaceVariablesWithValue(tstring &target);
	tstring GetReplaceVariablesWithValue(tstring target);
	void Setup_Prerequisite(const WhereClauseArg &WherePrgLangAndProfile);
	bool Get_ScriptProfile();
	bool Get_CompilerProfile(const sql_table_AdvProfile_Settings &AdvProfile);
	void Set_CompilerAndScript_ExistValues();
	void ProcessHelpRequest();
	tstring Get_RequiredProgramLanguage();
	tstring Get_RequiredProfileName();
	tstring Get_RequiredPrgProf(const tstring &Flag);
	tstring Get_PrgLangAndProfileSig();
};
#endif //INCLUDING_FROM_PROCESSPRGLANGUAGEACTION_CPP

#endif // !PROCESSPRGLANGUAGEACTION_
