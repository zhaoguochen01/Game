
// GameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Game.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "Human.h"
#include "Scenery.h"
#include <Windows.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
static bool cmp(CScenery a, CScenery b)
{
	return a.y < b.y;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CGameDlg �Ի���



CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGameDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGameDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON1, &CGameDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGameDlg ��Ϣ�������
BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	dc = new CClientDC(this);
	hDCBitmap = CreateCompatibleDC(*dc);
	buffer_hdc = CreateCompatibleDC(NULL);
	h_temp_bitmap = CreateCompatibleBitmap(*dc, 640, 480);
	SelectObject(buffer_hdc, h_temp_bitmap);
	scenery_number = 0;

	for (int i = 0; i < 800; i++)
	{
		CString path = "ͼƬ/��/c";
		char temp[100];
		sprintf_s(temp,100, "%05d", i);
		path += temp;
		path += ".bmp";
		h_person[i] = (HBITMAP)LoadImage(AfxGetInstanceHandle(), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}
	srand(time(NULL));
	for (int i = 0; i < 97; i++)
	{
		CString path = "ͼƬ/��/c";
		char temp[100];
		sprintf_s(temp,100, "%05d", i);
		path += temp;
		path += ".bmp";
		int x = rand() % 640;
		int y = rand() % 480;
		scenery[i].x = x;
		scenery[i].y = y;
		scenery[i].pic = (HBITMAP)LoadImage(AfxGetInstanceHandle(), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	}

	h_land = (HBITMAP)LoadImage(AfxGetInstanceHandle(), "ͼƬ/����A.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	human = CHuman(0,0,2);
	human.dis_x = 0;
	human.dis_y = 0;
	human.state = 8;
	sort(scenery,scenery+96,cmp);
	for (int i = 0; i < 96; i++)
	{
		scenery_chose[i] = rand() % 96;
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGameDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetTimer(10, 25, NULL);
}


void CGameDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

int pic_num[8] = { 40,50,60,70,80,90,100,110 };
int stand_pic_num[8] = { 0,5,10,15,20,25,30,35 };
int last_state = 0;
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DIBSECTION ds;
	BITMAPINFOHEADER &bm = ds.dsBmih;
	GetObject(h_land, sizeof(ds), &ds);
	SelectObject(hDCBitmap, h_land);
	int iWidth = bm.biWidth;
	int iHeight = bm.biHeight;
	BitBlt(buffer_hdc, 0, 0, iWidth, iHeight, hDCBitmap, 0, 0, SRCCOPY);
	human.Move();
	if (human.state == 8)
		human_stand(last_state, ds);
	else
		human_move(human.state, ds);
	BitBlt(*dc, 0, 0, iWidth, iHeight, buffer_hdc, 0, 0, SRCCOPY);
	CDialogEx::OnTimer(nIDEvent);
}

void CGameDlg::TransparentBlt(HDC hDesDC,
	int iDesX, int iDesY,
	int iWide, int iHigh,
	HDC hSurDC,
	int iSurX, int iSurY,
	UINT iMaskCol
	)
{
	HBITMAP hTmpBmp = CreateCompatibleBitmap(hDesDC, iWide, iHigh);
	HBITMAP hMaskBmp = CreateBitmap(iWide, iHigh, 1, 1, NULL);
	HDC		hTmpDC = CreateCompatibleDC(hDesDC);
	HDC		hMaskDC = CreateCompatibleDC(hDesDC);
	HBITMAP hOldTmpBmp = (HBITMAP)SelectObject(hTmpDC, hTmpBmp);
	HBITMAP hOldMaskBmp = (HBITMAP)SelectObject(hMaskDC, hMaskBmp);

	BitBlt(hTmpDC, 0, 0, iWide, iHigh, hSurDC, iSurX, iSurY, SRCCOPY);

	COLORREF colOld = SetBkColor(hTmpDC, iMaskCol);
	BitBlt(hMaskDC, 0, 0, iWide, iHigh, hTmpDC, 0, 0, SRCCOPY);
	SetBkColor(hTmpDC, colOld);

	BitBlt(hMaskDC, 0, 0, iWide, iHigh, hMaskDC, 0, 0, NOTSRCCOPY);

	BitBlt(hTmpDC, 0, 0, iWide, iHigh, hMaskDC, 0, 0, SRCAND);


	BitBlt(hMaskDC, 0, 0, iWide, iHigh, hMaskDC, 0, 0, NOTSRCCOPY);
	BitBlt(hDesDC, iDesX, iDesY, iWide, iHigh, hMaskDC, 0, 0, SRCAND);
	BitBlt(hDesDC, iDesX, iDesY, iWide, iHigh, hTmpDC, 0, 0, SRCPAINT);

	SelectObject(hTmpDC, hOldTmpBmp);
	DeleteDC(hTmpDC);
	SelectObject(hMaskDC, hOldMaskBmp);
	DeleteDC(hMaskDC);

	DeleteObject(hTmpBmp);
	DeleteObject(hMaskBmp);
}

void CGameDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	human.dis_x = point.x;
	human.dis_y = point.y;
	CDialogEx::OnRButtonDown(nFlags, point);
}

void CGameDlg::Clear_Window(HDC hdc) 
{ BitBlt(hdc, 0, 0, 640, 480, NULL, 0, 0, WHITENESS); }

void CGameDlg::human_move(int status, DIBSECTION ds)
{
	paint_behind_human();
	BITMAPINFOHEADER &bm = ds.dsBmih;
	GetObject(h_person[pic_num[status]], sizeof(ds), &ds);
	SelectObject(hDCBitmap, h_person[pic_num[status]]);
	int iWidth = bm.biWidth;
	int iHeight = bm.biHeight;
	TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
	pic_num[status]++;
	if (pic_num[status] == 50+status*10)
	{
		pic_num[status] = 40+status*10;
	}
	last_state = status;
	paint_before_human();
}
void CGameDlg::human_stand(int last_state, DIBSECTION ds)
{
	paint_behind_human();
	BITMAPINFOHEADER &bm = ds.dsBmih;
	GetObject(h_person[ stand_pic_num[last_state]], sizeof(ds), &ds);
	SelectObject(hDCBitmap, h_person[ stand_pic_num[last_state]]);
	int iWidth = bm.biWidth;
	int iHeight = bm.biHeight;
	TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
	 stand_pic_num[last_state]++;
	if ( stand_pic_num[last_state] == last_state * 5+5)
	{
		 stand_pic_num[last_state] =  last_state * 5;
	}
	paint_before_human();
}

void CGameDlg::paint_behind_human()
{
	for (int i = 0; i < scenery_number; i++)
	{
		DIBSECTION ds_temp;
		BITMAPINFOHEADER &bm = ds_temp.dsBmih;
		GetObject(scenery[scenery_chose[i]].pic, sizeof(ds_temp), &ds_temp);
		SelectObject(hDCBitmap, scenery[scenery_chose[i]].pic);
		int iWidth = bm.biWidth;
		int iHeight = bm.biHeight;
		if (scenery[scenery_chose[i]].y + iHeight<human.now_y + 42)
			TransparentBlt(buffer_hdc, scenery[scenery_chose[i]].x, scenery[scenery_chose[i]].y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
	}
}

void CGameDlg::paint_before_human()
{
	for (int i = 0; i < scenery_number; i++)
	{
		DIBSECTION ds_temp;
		BITMAPINFOHEADER &bm = ds_temp.dsBmih;
		GetObject(scenery[scenery_chose[i]].pic, sizeof(ds_temp), &ds_temp);
		SelectObject(hDCBitmap, scenery[scenery_chose[i]].pic);
		int iWidth = bm.biWidth;
		int iHeight = bm.biHeight;
		if (scenery[scenery_chose[i]].y + iHeight >= human.now_y + 42)
			TransparentBlt(buffer_hdc, scenery[scenery_chose[i]].x, scenery[scenery_chose[i]].y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
	}
}

void CGameDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	scenery_number++;
}
