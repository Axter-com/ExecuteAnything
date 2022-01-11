#include "NppPluginMsgSender.h"
#include <boost/algorithm/string/replace.hpp>

tstring CNppPluginMsgSender::GetTextNppMsg(UINT MsgId)
{
	static const int MaxDataSize = 2048;
	TCHAR data[MaxDataSize + 2] = { 0 };
	if (SendMsg(MsgId, (WPARAM)MaxDataSize, (LPARAM)data))
		return data;
	return tstring();
}

tstring CNppPluginMsgSender::getNppRoaming()
{
	tstring path = getPluginsConfigDir();
	boost::ireplace_last(path, _T("\\plugins\\Config"), _T("\\"));
	return path;
}

BOOL CNppPluginMsgSender::SendMsg(const TCHAR *destModuleName, long internalMsg, void* info)
{
	CommunicationInfo ci = { internalMsg,
		m_srcModuleName.c_str(),
		info };
	return (BOOL) ::SendMessage(m_hNppWnd,
		NPPM_MSGTOPLUGIN,
		(WPARAM)destModuleName,
		(LPARAM)&ci
	);
}

void CNppPluginMsgSender::NppMenu_ViewInBrowser(eVIEWBROWSER_TYPE eviewbrowser_type)
{
	switch (eviewbrowser_type)
	{
	case FIREFOX:
		sendMenuCmd(IDM_VIEW_IN_FIREFOX);
		break;
	case CHROME:
	default:
		sendMenuCmd(IDM_VIEW_IN_CHROME);
		break;
	case EDGE:
		sendMenuCmd(IDM_VIEW_IN_EDGE);
		break;
	case IE:
		sendMenuCmd(IDM_VIEW_IN_IE);
		break;
	}
}
