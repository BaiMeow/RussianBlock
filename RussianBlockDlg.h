
// RussianBlockDlg.h: 头文件
//

#pragma once
#include "Game.h"

// CRussianBlockDlg 对话框
class CRussianBlockDlg : public CDialogEx
{
// 构造
public:
	CRussianBlockDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RUSSIANBLOCK_DIALOG };
#endif
protected:
	virtual void  DoDataExchange(CDataExchange* pDX);
private:
	Game _game;
	void DrawBigNet(CDC &dc);
	void DrawSmallNet(CDC &dc);
	void OnKeyDown(UINT nChar);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnClickedBstart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedPause();
	afx_msg void OnClickedHelp();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CStatic score;
	afx_msg void OnStnClickedScore();
	CStatic speed;
	afx_msg void OnStnClickedSpeed();
};
