#ifndef _npp_exec_plugin_msg_sender_h_
#define _npp_exec_plugin_msg_sender_h_
//---------------------------------------------------------------------------
#include "core/NppPluginMsgSender.h"
#include "nppexec_msgs.h"

enum NPE_EXECUTE_TYPE{eNpeExecuteParam = 0, eNpeExecuteParam2 = 1, eNpeExecuteQueuedParam = 2, eNpeExecuteCollateralParam = 3};

class CNppExecPluginMsgSender : public CNppPluginMsgSender
{
	protected:
		tstring m_destModuleName;
		tstring GetTextNppExecMsg(UINT MsgId);
	public:
		CNppExecPluginMsgSender(HWND hNppWnd, const TCHAR* srcModuleName,  const TCHAR* destModuleName = _T("NppExec.dll")) : 
			CNppPluginMsgSender(hNppWnd, srcModuleName), m_destModuleName(destModuleName)
		{
		}

		HWND    getCurrentScintillaWnd();
		void setDestModuleName(const TCHAR* destModuleName);
		BOOL SendNpeMsg(long internalMsg, void* info);
		LRESULT SendNppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

		BOOL NpeGetVerStr(TCHAR pszVersion[NPE_MAXVERSTR]);
		DWORD NpeGetVerDword();
		DWORD NpeGetState();
		DWORD NpeGetScriptNames(NpeGetScriptNamesParam* nsn);
		DWORD NpeGetScriptByName(NpeGetScriptByNameParam* nsn);
		DWORD NpePrint(const TCHAR* szText);


		// DM: Change functions to overloading instead of having a different name for each function. 
		DWORD NpeExecute(NpeNppExecParam* npep);
		DWORD NpeExecute(NpeNppExecParam2* npep);
		DWORD NpeExecute(NpeExecuteParam* nep);
		DWORD NpeExecute(NpeExecuteParam2* nep, NPE_EXECUTE_TYPE npe_execute_type = eNpeExecuteParam2);

		DWORD NpeExecuteQue(NpeExecuteQueuedParam* nep);
		DWORD NpeExecuteCollateral(NpeExecuteCollateralParam* necp);

		void NpeFreePtr(void* ptr);
};

//---------------------------------------------------------------------------
#endif
