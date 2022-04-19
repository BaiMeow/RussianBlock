
// RussianBlockDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RussianBlock.h"
#include "RussianBlockDlg.h"
#include "afxdialogex.h"
#include <mutex>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRussianBlockDlg 对话框



CRussianBlockDlg::CRussianBlockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RUSSIANBLOCK_DIALOG, pParent) ,_game(23,15)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRussianBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_SCORE, score);
	DDX_Control(pDX, IDC_SPEED, speed);
}

void CRussianBlockDlg::DrawBigNet(CDC &dc2)
{
	CRect rect;
	CWnd* wnd = GetDlgItem(IDC_PIC_MAIN);
	CPaintDC dc(wnd);
	wnd->GetClientRect(&rect);

	CRect windowRect;
	wnd->GetWindowRect(&windowRect);
	ScreenToClient(windowRect);

	CDC dcMem;
	CBitmap bitmap;
	dcMem.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	dcMem.SelectObject(&bitmap);
	dcMem.BitBlt(0, 0, rect.Width(), rect.Height(), &dc2, windowRect.left, windowRect.top, SRCCOPY);
	
	//辅助数组绘图
	int* pAuxBigNet = _game.AddToolToAux(new int[_game._netHeight * _game._netWidth], _game._iLocX[0], _game._iLocY[0], _game._tool[0]);
	_game.AddToolToAux(pAuxBigNet, _game._iLocX[1], _game._iLocY[1], _game._tool[1],false);
	
	const COLORREF colorTableA[] = { RGB(236, 204, 104),RGB(255, 127, 80),RGB(255, 107, 129),RGB(164, 176, 190),RGB(123, 237, 159), RGB(112, 161, 255),RGB(83, 82, 237) };
	for (int i = 0; i < _game._netHeight; ++i) {
		for (int j = 0; j < _game._netWidth; ++j) {
			int iType = pAuxBigNet[i * _game._netWidth + j];
			if (iType > 0 && iType <= 7) {
				//CClientDC dcc(wnd);
				CBrush brush(colorTableA[iType - 1]);
				dcMem.FillRect(CRect(j * rect.Width() / _game._netWidth,
					i*rect.Height()/_game._netHeight,
					(j+1) * rect.Width() / _game._netWidth,
					(i+1) * rect.Height() / _game._netHeight
					),&brush);
				dcMem.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
				dcMem.Rectangle(j * rect.Width() / _game._netWidth,
					i * rect.Height() / _game._netHeight,
					(j + 1) * rect.Width() / _game._netWidth,
					(i + 1) * rect.Height() / _game._netHeight
				);
			}
		}
	}
	//wnd->RedrawWindow();	
	dcMem.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));	
	dcMem.Rectangle(0, 0, rect.Width(), rect.Height());

	CPen pen(PS_SOLID, 2, RGB(127, 143, 166));
	CPen *oldpen = dcMem.SelectObject(&pen);
	dcMem.MoveTo(rect.Width() / 2, 0);
	dcMem.LineTo(rect.Width() / 2, rect.Height() / _game._netHeight * 4);
	dcMem.SelectObject(oldpen);

	dc2.BitBlt(windowRect.left, windowRect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	//销毁辅助数组
	delete[] pAuxBigNet;
}

void CRussianBlockDlg::DrawSmallNet(CDC &dc2)
{
	for (int i = 0; i < 2; ++i) {
		Tool& next_tool = _game._nextTool[i];
		int type = next_tool.GetType();
		if (type == 0) {
			return;
		}

		CRect rect;
		CWnd* wnd;
		if (i == 0) {
			wnd = GetDlgItem(IDC_PIC_LEFT);
		}
		else {
			wnd = GetDlgItem(IDC_PIC_RIGHT);
		}
		CPaintDC dc(wnd);
		wnd->GetClientRect(&rect);
		//CClientDC dcc(wnd);

		CRect windowRect;
		wnd->GetWindowRect(&windowRect);
		ScreenToClient(windowRect);

		CDC dcMem;
		CBitmap bitmap;
		dcMem.CreateCompatibleDC(&dc);
		bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		dcMem.SelectObject(&bitmap);
		dcMem.BitBlt(0, 0, rect.Width(), rect.Height(), &dc2, windowRect.left, windowRect.top, SRCCOPY);

		const COLORREF colorTableA[] = { RGB(236, 204, 104),RGB(255, 127, 80),RGB(255, 107, 129),RGB(164, 176, 190),RGB(123, 237, 159), RGB(112, 161, 255),RGB(83, 82, 237) };
		CBrush brush(colorTableA[type - 1]);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (next_tool.ElementAt(i, j) != 0) {
					dcMem.FillRect(CRect(j * rect.Width() / 4,
						i * rect.Height() / 4,
						(j + 1) * rect.Width() / 4,
						(i + 1) * rect.Height() / 4
					), &brush);
					dcMem.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
					dcMem.Rectangle(j * rect.Width() / 4,
						i * rect.Height() / 4,
						(j + 1) * rect.Width() / 4,
						(i + 1) * rect.Height() / 4
					);
				}
			}
		}

		//wnd->RedrawWindow();
		dcMem.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
		dcMem.Rectangle(0, 0, rect.Width(), rect.Height());
		dc2.BitBlt(windowRect.left, windowRect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	}
}

void CRussianBlockDlg::OnKeyDown(UINT nChar)
{
	if (nChar == VK_SPACE) {
		OnClickedPause();
	}
	Invalidate(true);
}

BEGIN_MESSAGE_MAP(CRussianBlockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BSTART, &CRussianBlockDlg::OnClickedBstart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BPAUSE, &CRussianBlockDlg::OnClickedPause)
	ON_BN_CLICKED(IDC_BHELP, &CRussianBlockDlg::OnClickedHelp)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_STN_CLICKED(IDC_SCORE, &CRussianBlockDlg::OnStnClickedScore)
	ON_STN_CLICKED(IDC_SPEED, &CRussianBlockDlg::OnStnClickedSpeed)
END_MESSAGE_MAP()


// CRussianBlockDlg 消息处理程序

BOOL CRussianBlockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRussianBlockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRussianBlockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		
		CBitmap bmpBackground;
		CDC dcbg;
		dcbg.CreateCompatibleDC(&dc);
		bmpBackground.LoadBitmap(IDB_BITMAP1);
		dcbg.SelectObject(bmpBackground);

		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap dcMemBitMap;
		dcMemBitMap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
		dcMem.SelectObject(&dcMemBitMap);

		dcMem.BitBlt(0, 0, rect.Width(), rect.Height(), &dcbg, 0, 0, SRCCOPY);
		
		DrawSmallNet(dcMem);
		DrawBigNet(dcMem);

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRussianBlockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRussianBlockDlg::OnClickedBstart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (_game._state == GO) {
		KillTimer(1);
		KillTimer(2);
	}
	_game.Start();
	SetTimer(1, _game.GetTickTime(), NULL);
	SetTimer(2, 100, NULL);

}


void CRussianBlockDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		if (!_game.Go()) {
			KillTimer(1);
			KillTimer(2);
			TCHAR* msg = _T("Game Over!");
			MessageBox(msg);
			return;
		}
		char str[10];
		CString score("得分:"), difficulty("难度:");
		_itoa_s(_game.GetScore(), str, 10);
		score.Append(CString(str));
		this->score.SetWindowTextW(score);

		_itoa_s(_game.GetDifficulty(), str, 10);
		difficulty.Append(CString(str));
		this->speed.SetWindowTextW(difficulty);
		Invalidate(true);
		KillTimer(1);
		SetTimer(1, _game.GetTickTime(), NULL);
	}
		break;
	case 2:
	{
		if (_game._state == GO) {
			if (GetAsyncKeyState(VK_UP)) _game.Input(VK_UP);
			if (GetAsyncKeyState(VK_DOWN)) _game.Input(VK_DOWN);
			if (GetAsyncKeyState(VK_LEFT)) _game.Input(VK_LEFT);
			if (GetAsyncKeyState(VK_RIGHT)) _game.Input(VK_RIGHT);
			if (GetAsyncKeyState('A')) _game.Input('A');
			if (GetAsyncKeyState('S')) _game.Input('S');
			if (GetAsyncKeyState('D')) _game.Input('D');
			if (GetAsyncKeyState('W')) _game.Input('W');
		}
		Invalidate(true);
	}
	}
}


void CRussianBlockDlg::OnClickedPause()
{
	_game.PauseOrContinue();
	if (_game._state == PAUSE) {
		KillTimer(1);
		KillTimer(2);
	}
	if (_game._state == GO) {
		SetTimer(1, _game.GetTickTime(), NULL);
		SetTimer(2, 100, NULL);
	}
}


void CRussianBlockDlg::OnClickedHelp()
{
	TCHAR* msg = _T("左：ASDW\n右：方向键");
	MessageBox(msg);
}


BOOL CRussianBlockDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN) {
		OnKeyDown((UINT)pMsg->wParam);
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRussianBlockDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (_game._state == GO) {
		KillTimer(1);
		KillTimer(2);
	}
	CDialogEx::OnClose();
}


BOOL CRussianBlockDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return true;
}



void CRussianBlockDlg::OnStnClickedScore()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CRussianBlockDlg::OnStnClickedSpeed()
{
	// TODO: 在此添加控件通知处理程序代码
}
