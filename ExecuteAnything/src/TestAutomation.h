/*
	Copyright (C) 2022 David Maisonave (https://axter.com)
	The ExecuteAnything source code is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License.
	This program is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/
/*
Description:
  *** For example usage, see comments at end of header file ***
*/
#ifndef TESTAUTOMATION_H
#define TESTAUTOMATION_H
#include "ExecuteAnything.h"
#include "ExecuteAnything_Menu.h"
#include "NppExecPluginMsgSender.h"

enum TESTAUTOMATIONTESTTYPE {
	eTATT_NONE = 0
	, eTATT_Basic = 1
	, eTATT_AwaitingAutoValidation = 2
	, eTATT_Validated = 4
	, eTATT_Unvalidated = 8
	, eTATT_ScriptModeValidation = 16
	, eTATT_CompileModeValidation = 32
	, eTATT_NppExecScriptOperable = 256
	, eTATT_ScriptMode = 512				// B: 0000000010000000000
	, eTATT_CompileMode = 1024				// B: 0000000100000000000
	, eTATT_AwatingTest = 2048				// B: 0000001000000000000
	, eTATT_WindowsInterface = 4096			// B: 0000010000000000000
	, eTATT_Inoperable = 16384				// B: 0000100000000000000
	, eTATT_AwaitingFix = 32768				// B: 0001000000000000000
	, eTATT_InopSetup = 64536				// B: 0010000000000000000
	, eTATT_InopSourceCode = 131072			// B: 0100000000000000000
	, eTATT_PermanentlyInoperable = 262144  // B: 1000000000000000000
	//, eTATT_Any_InopFlag = 507904			// B: 1111100000000000000 (507904) (7C000)
	// Untill code for working windows interface test automation is added, include eTATT_WindowsInterface as an Inop flag.
	, eTATT_Any_InopFlag = 507904			// B: 1111110000000000000 (516096) (7E000)
};

class TestAutomation : protected ExecuteAnything_Menu
{
public:
	TestAutomation(CNppExecPluginMsgSender &npeMsgr, PrgLangProcessType prglangprocesstype);
};

#endif // !TESTAUTOMATION_H
