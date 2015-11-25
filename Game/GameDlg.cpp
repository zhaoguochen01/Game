
// GameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Game.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "Human.h"
#include "Scenery.h"
#include <Windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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

	for (int i = 0; i < 7; i++)
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
	for (int i = 0; i < 7; i++)
	{
		BITMAPINFOHEADER &bm = ds.dsBmih;
		GetObject(scenery[scenery_chose[i]].pic, sizeof(ds), &ds);
		SelectObject(hDCBitmap, scenery[scenery_chose[i]].pic);
		int iWidth = bm.biWidth;
		int iHeight = bm.biHeight;
		TransparentBlt(buffer_hdc, scenery[scenery_chose[i]].x, scenery[scenery_chose[i]].y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
	}
	human.Move();
	switch(human.state)
	{
		case SOUTH:
		{	
			GetObject(h_person[pic_num[0]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[0]]);
			TransparentBlt(buffer_hdc,human.now_x,human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[0]++;
			if (pic_num[0] == 50)
			{
				pic_num[0] = 40;
			}
			last_state = 0;
			break;
		}
		case SOUTH_WEST:
		{	
			GetObject(h_person[pic_num[1]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[1]]);	
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc,human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[1]++;
			if (pic_num[1] == 60)
			{
				pic_num[1] = 50;
			}
			last_state = 1;
			break;
		}
		case WEST:
		{
			GetObject(h_person[pic_num[2]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[2]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[2]++;
			if (pic_num[2] == 70)
			{
				pic_num[2] = 60;
			}
			last_state = 2;
			break;
		}
		case NORTH_WEST:
		{
			GetObject(h_person[pic_num[3]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[3]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[3]++;
			if (pic_num[3] == 80)
			{
				pic_num[3] = 70;
			}
			last_state = 3;
			break;
		}
		case NORTH:
		{
			GetObject(h_person[pic_num[4]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[4]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[4]++;
			if (pic_num[4] == 90)
			{
				pic_num[4] = 80;
			}
			last_state = 4;
			break;
		}
		case NORTH_EAST:
		{
			GetObject(h_person[pic_num[5]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[5]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[5]++;
			if (pic_num[5] == 100)
			{
				pic_num[5] = 90;
			}
			last_state = 5;
			break;
		}
		case EAST:
		{
			GetObject(h_person[pic_num[6]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[6]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[6]++;
			if (pic_num[6] == 110)
			{
				pic_num[6] = 100;
			}
			last_state = 6;
			break;
		}
		case SOUTH_EAST:
		{
			GetObject(h_person[pic_num[7]], sizeof(ds), &ds);
			SelectObject(hDCBitmap, h_person[pic_num[7]]);
			int iWidth = bm.biWidth;
			int iHeight = bm.biHeight;
			TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
			pic_num[7]++;
			if (pic_num[7] == 120)
			{
				pic_num[7] = 110;
			}
			last_state = 7;
			break;
		}
		case STAND:
		{
			switch (last_state)
			{
				case 0:
			{
				GetObject(h_person[stand_pic_num[0]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[0]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[0]++;
				if (stand_pic_num[0] == 5)
				{
					stand_pic_num[0] = 0;
				}
				break;
			}
			case 1:
			{
				GetObject(h_person[stand_pic_num[1]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[1]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[1]++;
				if (stand_pic_num[1] == 10)
				{
					stand_pic_num[1] = 5;
				}
				break;
			}
			case 2:
			{
				GetObject(h_person[stand_pic_num[2]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[2]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[2]++;
				if (stand_pic_num[2] == 15)
				{
					stand_pic_num[2] = 10;
				}
				break;
			}
			case 3:
			{
				GetObject(h_person[stand_pic_num[3]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[3]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[3]++;
				if (stand_pic_num[3] == 20)
				{
					stand_pic_num[3] = 15;
				}
				break;
			}
			case 4:
			{
				GetObject(h_person[stand_pic_num[4]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[4]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[4]++;
				if (stand_pic_num[4] == 25)
				{
					stand_pic_num[4] = 20;
				}
				break;
			}
			case 5:
			{
				GetObject(h_person[stand_pic_num[5]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[5]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[5]++;
				if (stand_pic_num[5] == 30)
				{
					stand_pic_num[5] = 25;
				}
				break;
			}
			case 6:
			{
				GetObject(h_person[stand_pic_num[6]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[6]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[6]++;
				if (stand_pic_num[6] == 35)
				{
					stand_pic_num[6] = 30;
				}
				break;
			}
			case 7:
			{
				GetObject(h_person[stand_pic_num[7]], sizeof(ds), &ds);
				SelectObject(hDCBitmap, h_person[stand_pic_num[7]]);
				int iWidth = bm.biWidth;
				int iHeight = bm.biHeight;
				TransparentBlt(buffer_hdc, human.now_x, human.now_y, iWidth, iHeight, hDCBitmap, 0, 0, RGB(255, 255, 255));
				stand_pic_num[7]++;
				if (stand_pic_num[7] == 40)
				{
					stand_pic_num[7] = 35;
				}
				break;
			}
			}
		}
	}
	BitBlt(*dc, 0, 0, iWidth, iHeight, buffer_hdc, 0, 0, SRCCOPY);
	/*if (t_hDc)
	{
		DeleteDC(t_hDc);
	}
	if (h_temp_bitmap)
	{
		DeleteObject(h_temp_bitmap);
	}*/
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