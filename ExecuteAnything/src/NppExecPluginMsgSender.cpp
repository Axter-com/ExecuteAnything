#include "NppExecPluginMsgSender.h"
#include "core\NppMessager.h"

tstring CNppExecPluginMsgSender::GetTextNppExecMsg(UINT MsgId)
{
	static const int MaxDataSize = 2048;
	TCHAR data[MaxDataSize + 2] = { 0 };
	if (SendMsg(m_destModuleName.c_str(), MsgId, (WPARAM)MaxDataSize, (LPARAM)data))
		return data;
	return tstring();
}

HWND CNppExecPluginMsgSender::getCurrentScintillaWnd()
{
	return CNppMessager::getNppData()._scintillaMainHandle;
}

void CNppExecPluginMsgSender::setDestModuleName( const TCHAR* destModuleName )
{
	m_destModuleName = destModuleName;
}

BOOL CNppExecPluginMsgSender::SendNpeMsg( long internalMsg, void* info )
{
	return SendMsg( m_destModuleName.c_str(), internalMsg, info );
}

LRESULT CNppExecPluginMsgSender::SendNppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return CNppPluginMsgSender::SendMsg(uMsg, wParam, lParam);
}

DWORD CNppExecPluginMsgSender::NpeGetVerDword()
{
	DWORD dwVersion = 0;
	SendNpeMsg( NPEM_GETVERDWORD, (void *)&dwVersion );
	return dwVersion;
}

BOOL CNppExecPluginMsgSender::NpeGetVerStr( TCHAR pszVersion[NPE_MAXVERSTR] )
{
	pszVersion[0] = 0;
	return SendNpeMsg( NPEM_GETVERSTR, (void *)pszVersion );
}

DWORD CNppExecPluginMsgSender::NpeGetState()
{
	DWORD dwState = 0;
	SendNpeMsg( NPEM_GETSTATE, (void *)&dwState );
	return dwState;
}

DWORD CNppExecPluginMsgSender::NpePrint( const TCHAR* szText )
{
	DWORD dwState = NpeGetState();
	if ( dwState == NPE_STATEREADY )
	{
		SendNpeMsg( NPEM_PRINT, (void *)szText );
	}
	return dwState;
}

DWORD CNppExecPluginMsgSender::NpeExecute(NpeExecuteParam* nep)
{
	DWORD dwState = NpeGetState();
	if (dwState == NPE_STATEREADY)
	{
		SendNpeMsg(NPEM_EXECUTE, (void *)nep);
		return nep->dwResult;
	}
	return dwState;
}

DWORD CNppExecPluginMsgSender::NpeExecute(NpeExecuteParam2* nep, NPE_EXECUTE_TYPE npe_execute_type)
{
	switch (npe_execute_type)
	{
	case eNpeExecuteParam:
		NpeExecuteParam nep_dst;
		nep_dst.szScriptBody = nep->szScriptBody;
		nep_dst.dwResult = nep->dwResult;
		return NpeExecute(&nep_dst);
	case eNpeExecuteQueuedParam:
		return NpeExecuteQue(nep);
	case eNpeExecuteCollateralParam:
		return NpeExecuteCollateral(nep);
	case eNpeExecuteParam2:
		DWORD dwState = NpeGetState();
		if (dwState == NPE_STATEREADY)
		{
			SendNpeMsg(NPEM_EXECUTE2, (void *)nep);
			return nep->dwResult;
		}
		return dwState;
	}

	return 0;
}

DWORD CNppExecPluginMsgSender::NpeExecuteQue(NpeExecuteQueuedParam* nep)
{
	SendNpeMsg(NPEM_EXECUTE_QUEUED, (void *)nep);
	return nep->dwResult;
}

DWORD CNppExecPluginMsgSender::NpeExecute(NpeNppExecParam2* npep)
{
	DWORD dwState = NpeGetState();
	if (dwState == NPE_STATEREADY)
	{
		SendNpeMsg(NPEM_NPPEXEC2, (void *)npep);
		return npep->dwResult;
	}
	return dwState;
}

DWORD CNppExecPluginMsgSender::NpeExecute(NpeNppExecParam* npep)
{
	DWORD dwState = NpeGetState();
	if (dwState == NPE_STATEREADY)
	{
		SendNpeMsg(NPEM_NPPEXEC, (void *)npep);
		return npep->dwResult;
	}
	return dwState;
}

DWORD CNppExecPluginMsgSender::NpeExecuteCollateral( NpeExecuteCollateralParam* necp )
{
	SendNpeMsg( NPEM_EXECUTE_COLLATERAL, (void *)necp );
	return necp->dwResult;
}

void CNppExecPluginMsgSender::NpeFreePtr( void* ptr )
{
	SendNpeMsg( NPEM_FREEPTR, ptr );
}

DWORD CNppExecPluginMsgSender::NpeGetScriptNames( NpeGetScriptNamesParam* nsn )
{
	SendNpeMsg( NPEM_GETSCRIPTNAMES, (void *)nsn );
	return nsn->dwResult;
}

DWORD CNppExecPluginMsgSender::NpeGetScriptByName( NpeGetScriptByNameParam* nsn )
{
	SendNpeMsg( NPEM_GETSCRIPTBYNAME, (void *)nsn );
	return nsn->dwResult;
}
