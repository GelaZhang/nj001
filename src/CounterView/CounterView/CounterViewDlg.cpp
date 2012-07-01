// CounterViewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CounterView.h"
#include "CounterViewDlg.h"

#include "CounterViewFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_APP 10
// CCounterViewDlg �Ի���

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


// CCounterViewDlg ��Ϣ�������

BOOL CCounterViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(START_APP, 50, NULL);
	CounterViewApp *app = CounterViewApp::getApp();
	app->init( m_hWnd, GetDC()->GetSafeHdc() );
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CDialog::OnLButtonDblClk(nFlags, point);
	//if ( point.x < 200 && point.y < 200 )

}

void CCounterViewDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CDialog::OnClose();
}

void CCounterViewDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnRButtonDown(nFlags, point);
}
