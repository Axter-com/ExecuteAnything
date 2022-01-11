/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

// This file contains headers and constants only for ProcessPrgLanguageAction.cpp
#ifdef INCLUDING_FROM_PROCESSPRGLANGUAGEACTION_CPP
#include "ExecuteAnything.h"
#include "core\SciMessager.h"
#include "CommonFunctions.h"
#include "findfiles.h"
#include "sqlite3pp_ez.h"
#include "SQL\sql_Master_Header.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string.hpp>

#include <map>

static const tstring NppExecScriptPrefix = _T("\
npp_console -\n\
env_unset  PATH\n\
env_unset  LIB\n\
env_unset  INCLUDE\n\
npp_console +\n\
set local HelpAliasPefix=Console-shortcut keys (type key and hit enter):\n\
set local __next_line_c ~ strunescape \\n\n\
set __next_line=$(__next_line_c)\n\
set HelpAliasPostfix=$(__next_line)s=shows all local variables$(__next_line)h=show NppExec console commands$(__next_line)help all=Show all console commands in detail$(__next_line)\n\
set local Highlight10 =Console-shortcut*\n\
set local Include5 = *\n\
con_filter +i5 +h10\n\
set __tab ~ chr 0x09\n\
npe_cmdalias h =help\n\
npe_cmdalias q = help$(__next_line_c)set$(__next_line_c)\n\
npe_cmdalias s = set\n\
npe_cmdalias x = NPP_EXEC ExecuteTaskBasedOnFileExtension\n\
");

static const tstring AliasHelpPrefixStr = _T("npe_cmdalias ? =echo $(HelpAliasPefix)");
static const tstring AliasHelpPostfixStr = _T("$(HelpAliasPostfix)");

const tstring &ProcessPrgLanguageAction::GetExecuteAnythingSuccessMsgID()
{
	static const tstring returnValue = _T("ExecuteAnythingSuccessMsgID:");
	return returnValue;
}

#define  ExecuteAnythingSuccessKeywordReplacement	"//SuccesSKeyWordReplacemenT"

static const tstring TestAutomationSuccessActionStr = _T("npp_close");
static const tstring TestAutomationSuccessActionDbName = _T("TestAutomationSuccessAction");

static const TCHAR ShowAfterExecuteResults[] = _T("\n\
if $(EXITCODE) == 0\n\
	echo =============== SUCCESS =============== ===============\n\
	echo **********  Success running $(FileName) **********\n\
	echo =============== SUCCESS =============== ===============\n\
	" ExecuteAnythingSuccessKeywordReplacement "\n\
else\n\
	echo ============ Run ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: $(FileName) failed with error: $(EXITCODE) !!!!\n\
	echo $(FQN) : 1\n\
endif\n\
:QuitIt\n\
");
static const TCHAR NPPEXECCONSOLELOGGING[] = _T("\n\
con_save $(FILE_DIR)\\LoggingNppExecuteConsole.tmp\n\
cmd /c echo ************************************************************************************************************************************************************************************>>$(FILE_DIR)\\LoggingNppExecuteConsole.log\n\
cmd /c echo ***>>$(FILE_DIR)\\LoggingNppExecuteConsole.log\n\
cmd /c type $(FILE_DIR)\\LoggingNppExecuteConsole.tmp >> $(FILE_DIR)\\LoggingNppExecuteConsole.log\n\
echo ===================================== ===============\n\
\n\
");

static const TCHAR PostBuildCmdStr[] = _T("\
Echo Executing Linker/Post-Build command:$(__next_line)$(PostBuildCmd)\n\
$(PostBuildCmd)\n\
if $(EXITCODE) != 0\n\
	echo =========== Post-Build/Link ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: Post-Build/Link error $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo Post-Build/Link Command=$(PostBuildCmd)\n\
	goto :QuitIt\n\
endif\n\
");

static const TCHAR PreBuildCmdStr[] = _T("\
Echo Executing Pre-Build command:$(__next_line)$(PreBuildCmd)\n\
$(PreBuildCmd)\n\
if $(EXITCODE) != 0\n\
	echo =========== Pre-Build ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: Pre-Build error $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo Pre-Build Command=$(PreBuildCmd)\n\
	goto :QuitIt\n\
endif\n\
");

static const TCHAR CleanCmdStr[] = _T("\
Echo Executing Clean command:$(__next_line)$(CleanCmd)\n\
$(CleanCmd)\n\
if $(EXITCODE) != 0\n\
	echo =========== CleanCmd ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: CleanCmd error $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo Clean Command=$(CleanCmd)\n\
	goto :QuitIt\n\
endif\n\
");

static const TCHAR RunBuildBinCmdStr[] = _T("\
Echo Executing binary:$(BinaryToExecute)\n\
$(BinaryToExecute)\n\
if $(EXITCODE) != 0\n\
	echo =========== Binary Execute ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: Error fo $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo BinaryToExecute Command=$(BinaryToExecute)\n\
	goto :QuitIt\n\
endif\n\
");

static const TCHAR CompileCmdStr[] = _T("\
Echo Executing build command:$(__next_line)$(BuildCmd)\n\
$(BuildCmd)\n\
if $(EXITCODE) != 0\n\
	echo =========== BuildCmd ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: BuildCmd error $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo Build Command=$(BuildCmd)\n\
	goto :QuitIt\n\
endif\n\
");

static const TCHAR ScriptCmdStr[] = _T("\
Echo Running script $(FileName) using command:$(__next_line)$(ExecuteCMD)\n\
$(ExecuteCMD)\n\
if $(EXITCODE) != 0\n\
	echo =========== Script Run ERROR! =============== ===============\n\
	echo $(__tab) !!!! Error: Script run error $(FILE_FQN) with error: $(EXITCODE) !!!!\n\
	echo ExecuteCMD=$(ExecuteCMD)\n\
	goto :QuitIt\n\
endif\n\
");


static const tregex SearchFileName = tregex(_T("[\\\\/:]([^\\\\\\n/:]+)$"), std::regex_constants::ECMAScript | std::regex_constants::icase);
static const tregex SearchDirSeperator = tregex(_T("[\\\\:]"), std::regex_constants::ECMAScript | std::regex_constants::icase);
static const tregex SearchVariables = tregex(_T("\\$\\(([\\w\\.-]+)\\)"), std::regex_constants::ECMAScript | std::regex_constants::icase);
static const std::regex_constants::match_flag_type regex_compatibility_flag = std::regex_constants::format_sed;


#endif //INCLUDING_FROM_PROCESSPRGLANGUAGEACTION_CPP