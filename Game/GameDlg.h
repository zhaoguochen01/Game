
// GameDlg.h : 头文件
//

#pragma once
#include"Human.h"
#include"Scenery.h"

// CGameDlg 对话框
class CGameDlg : public CDialogEx
{
public:
	int scenery_chose[7];
	CClientDC *dc;
	HBITMAP h_person[800];
	HBITMAP h_land;
	HBITMAP h_temp_bitmap;
	HDC hDCBitmap;
	HDC t_hDc;
	HDC buffer_hdc;
	CHuman human;
	CScenery scenery[97];
// 构造
public:
	CGameDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void TransparentBlt(HDC hDesDC,
		int iDesX, int iDesY,
		int iWide, int iHigh,
		HDC hSurDC,
		int iSurX, int iSurY,
		UINT iMaskCol
		);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void Clear_Window(HDC hdc);
	void human_move(int status, DIBSECTION ds);
	void human_stand(int last_state, DIBSECTION ds);
	void paint_behind_human();
	void paint_before_human();
};
