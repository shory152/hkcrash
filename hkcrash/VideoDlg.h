// VideoDlg.h : CVideoDlg 的声明

#pragma once

#include "resource.h"       // 主符号

#include <atlhost.h>
#include "HCNetSDK.h"

using namespace ATL;

static void errexit(LPWSTR msg)
{
	::MessageBox(NULL, msg, L"", 0);
	exit(1);
}
// CVideoDlg

class CVideoDlg : 
	public CAxDialogImpl<CVideoDlg>
{
public:
	CVideoDlg()
	{
		
	}

	~CVideoDlg()
	{
		destroyHk();
	}

	void SetDVRParam(LPCTSTR ip, WORD port, LPCTSTR user, LPCTSTR pwd)
	{
		_bstr_t tmp;
		tmp = ip;
		strncpy_s(this->ip, tmp, 128);
		this->port = port;
		tmp = user;
		strncpy_s(this->user, tmp, 128);
		tmp = pwd;
		strncpy_s(this->pwd, tmp, 128);
	}

	enum { IDD = IDD_VIDEODLG };

BEGIN_MSG_MAP(CVideoDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedCancel)
	CHAIN_MSG_MAP(CAxDialogImpl<CVideoDlg>)
END_MSG_MAP()

// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CAxDialogImpl<CVideoDlg>::OnInitDialog(uMsg, wParam, lParam, bHandled);

		initHk();

		bHandled = TRUE;
		return 1;  // 使系统设置焦点
	}

	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

private:
	typedef BOOL (__stdcall *NET_DVR_Init_P)();
	typedef LONG (__stdcall *NET_DVR_Login_V40_P)(LPNET_DVR_USER_LOGIN_INFO pLoginInfo, LPNET_DVR_DEVICEINFO_V40 lpDeviceInfo);
	typedef BOOL (__stdcall *NET_DVR_Logout_P)(LONG lUserID);
	typedef LONG (__stdcall *NET_DVR_RealPlay_P)(LONG lUserID, LPNET_DVR_CLIENTINFO lpClientInfo);
	typedef LONG (__stdcall *NET_DVR_RealPlay_V40_P)(LONG lUserID, LPNET_DVR_PREVIEWINFO lpPreviewInfo, REALDATACALLBACK fRealDataCallBack_V30, void* pUser);
	typedef BOOL (__stdcall *NET_DVR_StopRealPlay_P)(LONG lRealHandle);
	typedef BOOL (__stdcall *NET_DVR_Cleanup_P)();

	HMODULE hmod = NULL;
	NET_DVR_Init_P NET_DVR_Init = NULL;
	NET_DVR_Login_V40_P NET_DVR_Login_V40 = NULL;
	NET_DVR_Logout_P NET_DVR_Logout = NULL;
	NET_DVR_RealPlay_P NET_DVR_RealPlay = NULL;
	NET_DVR_RealPlay_V40_P NET_DVR_RealPlay_V40 = NULL;
	NET_DVR_Cleanup_P NET_DVR_Cleanup = NULL;
	NET_DVR_StopRealPlay_P NET_DVR_StopRealPlay = NULL;

	NET_DVR_DEVICEINFO_V40  stuDvrDevV40 = { 0 };		// 设备参数
	LONG lUserId = -1;
	LONG lHandlePlay = -1;

	CHAR ip[128] = { 0 };
	WORD port = 8000;
	CHAR user[128] = { 0 };
	CHAR pwd[128] = { 0 };

	void initHk()
	{
		// load library, get function address
		hmod = LoadLibrary(L"HCNetSDK.dll");
		if (hmod == NULL)
			errexit(L"can not load HCNetSDK.dll");

		NET_DVR_Init = GetProcAddress(hmod, "NET_DVR_Init");
		if (NET_DVR_Init == NULL)
			errexit(L"can not get function");
		
		NET_DVR_Login_V40 = (NET_DVR_Login_V40_P)GetProcAddress(hmod, "NET_DVR_Login_V40");
		if (NET_DVR_Login_V40 == NULL)
			errexit(L"can not get function");
		
		NET_DVR_Logout = (NET_DVR_Logout_P)GetProcAddress(hmod, "NET_DVR_Logout");
		if (NET_DVR_Logout == NULL)
			errexit(L"can not get function");
		
		NET_DVR_RealPlay = (NET_DVR_RealPlay_P)GetProcAddress(hmod, "NET_DVR_RealPlay");
		if (NET_DVR_RealPlay == NULL)
			errexit(L"can not get function");
		
		NET_DVR_RealPlay_V40 = (NET_DVR_RealPlay_V40_P)GetProcAddress(hmod, "NET_DVR_RealPlay_V40");
		if (NET_DVR_RealPlay_V40 == NULL)
			errexit(L"can not get function");
		
		NET_DVR_StopRealPlay = (NET_DVR_StopRealPlay_P)GetProcAddress(hmod, "NET_DVR_StopRealPlay");
		if (NET_DVR_StopRealPlay == NULL)
			errexit(L"can not get function");
		
		NET_DVR_Cleanup = (NET_DVR_Cleanup_P)GetProcAddress(hmod, "NET_DVR_Cleanup");
		if (NET_DVR_Cleanup == NULL)
			errexit(L"can not get function");

		// login
		if (!NET_DVR_Init())
			errexit(L"init sdk fail");
		
		NET_DVR_USER_LOGIN_INFO stuDvrLoginInfo = { 0 };

		strncpy_s(stuDvrLoginInfo.sDeviceAddress, ip, NET_DVR_DEV_ADDRESS_MAX_LEN);
		stuDvrLoginInfo.wPort = port;
		strncpy_s(stuDvrLoginInfo.sUserName, user, NET_DVR_LOGIN_USERNAME_MAX_LEN);
		strncpy_s(stuDvrLoginInfo.sPassword, pwd, NET_DVR_LOGIN_PASSWD_MAX_LEN);
		stuDvrLoginInfo.bUseAsynLogin = FALSE;

		lUserId = NET_DVR_Login_V40(&stuDvrLoginInfo, &stuDvrDevV40);
		if (lUserId < 0)
			errexit(L"login fail");

		// real play
		NET_DVR_PREVIEWINFO stuDvrPreviewInfo = { 0 };
		
		// 设置预览参数
		stuDvrPreviewInfo.hPlayWnd = GetDlgItem(IDC_VIDEO).m_hWnd; // 播放窗口句柄
		stuDvrPreviewInfo.lChannel = stuDvrDevV40.struDeviceV30.byStartChan; // 第几个通道
		stuDvrPreviewInfo.dwStreamType = (DWORD)0; // 主码流，子码流，第三码流, ...
		stuDvrPreviewInfo.bBlocked = 0;        // 不等待取流

		lHandlePlay = NET_DVR_RealPlay_V40(lUserId, &stuDvrPreviewInfo, NULL, NULL);
	}

	void destroyHk()
	{
		if (lHandlePlay >= 0)
			NET_DVR_StopRealPlay(lHandlePlay);
		if (lUserId >= 0)
			NET_DVR_Logout(lUserId);
		NET_DVR_Cleanup();

		FreeLibrary(hmod);
	}
};


