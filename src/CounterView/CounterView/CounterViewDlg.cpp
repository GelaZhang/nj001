// CounterViewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CounterView.h"
#include "CounterViewDlg.h"

#include "CounterViewFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_APP 10
// CCounterViewDlg 对话框

CCounterViewDlg::CCounterViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCounterViewDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCounterViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCounterViewDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CCounterViewDlg 消息处理程序

BOOL CCounterViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(START_APP, 50, NULL);
	CounterViewApp *app = CounterViewApp::getApp();
	app->init( m_hWnd, GetDC()->GetSafeHdc() );
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CCounterViewDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_COUNTERVIEW_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_COUNTERVIEW_DIALOG));
	}
}
#endif


void CCounterViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if ( START_APP == nIDEvent )
	{
		KillTimer(START_APP);
		CounterViewApp *app = CounterViewApp::getApp();
		
		app->start();
	}
	CDialog::OnTimer(nIDEvent);
}

void CCounterViewDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CDialog::OnLButtonDblClk(nFlags, point);
	//if ( point.x < 200 && point.y < 200 )

}

void CCounterViewDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CDialog::OnClose();
}

void CCounterViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( point.x < 50 && point.y < 50 )
	{
		CounterViewApp *app = CounterViewApp::getApp();
		app->stop();
		app->uninit();
		CDialog::OnCancel();
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CCounterViewDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnRButtonDown(nFlags, point);
}
