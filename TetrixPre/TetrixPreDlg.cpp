// TetrixPreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TetrixPre.h"
#include "TetrixPreDlg.h"
#include ".\tetrixpredlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "gdiplus.lib")

CStream    scrStream;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTetrixPreDlg 对话框



CTetrixPreDlg::CTetrixPreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTetrixPreDlg::IDD, pParent)
{
    m_pMemDC = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTetrixPreDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PIC, m_pic);
    DDX_Control(pDX, IDC_DBG_VIEW, m_picDbg);
    DDX_Control(pDX, IDC_EDIT1, m_edit);
}

BEGIN_MESSAGE_MAP(CTetrixPreDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_TEST, OnBnClickedTest)
    ON_BN_CLICKED(IDC_LOAD, OnBnClickedLoad)
    ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
    ON_WM_TIMER()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CTetrixPreDlg 消息处理程序
DWORD WINAPI Playtetris(LPVOID pVoid);
BOOL CTetrixPreDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	scrStream.AddRef();
    //SetTimer(1,500,NULL);
    //DWORD id;
    //HANDLE hThread = ::CreateThread(NULL,0,Playtetris,this,0,&id);
    //if(hThread){
    //    ::CloseHandle(hThread);
    //}
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CTetrixPreDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTetrixPreDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
        if(m_pMemDC){
            CDC* pDC = m_pic.GetDC();
            pDC->BitBlt(0,0,19*16,23*16,m_pMemDC,0,0,SRCCOPY);
            m_pic.ReleaseDC(pDC);
        }
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTetrixPreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTetrixPreDlg::OnBnClickedTest()
{
    // TODO: 在此添加控件通知处理程序代码
    //stream.SeekToBegin();
    //IStream stream;
    //image.Load(_T("E:\\My Programmes\\Tetrix\\JpegTest\\123.jpg"));
    SetTimer(1,50,NULL);
}

void CTetrixPreDlg::OnBnClickedLoad()
{
    // TODO: 在此添加控件通知处理程序代码
    TCHAR szFilters[]=
        _T("Jpeg Files (*.jpg)|*.jpg|Bitmap Files (*.bmp)|*.bmp|PNG Files (*.png)|*.png|All Files (*.*)|*.*||");
    CFileDialog dlg(
        true,_T("*"),_T("*.jpg"),OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|OFN_EXPLORER  ,szFilters,0,0
        );
    if(dlg.DoModal() == IDOK){
        m_image.Load(dlg.GetPathName());
        CDC* pDC = m_pic.GetDC();
        RECT rect;
        m_pic.GetWindowRect(&rect);
        LONG w = rect.right - rect.left;
        LONG h = rect.bottom - rect.top;
        pDC->Rectangle(0,0,w,h);
        m_image.BitBlt(pDC->m_hDC,0,0,w,h,0,0);
        m_pic.ReleaseDC(pDC);
    }
}

void CTetrixPreDlg::OnBnClickedSave()
{
    // TODO: 在此添加控件通知处理程序代码
    TCHAR szFilters[]=
        _T("Header File (*.h)|*.h|All Files (*.*)|*.*||");
    CFileDialog dlg(
        false,_T("*"),_T("image.h"),OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|OFN_EXPLORER  ,szFilters,0,0
        );
    if(dlg.DoModal() == IDOK){
        const LONG w = m_image.GetWidth();
        const LONG h = m_image.GetHeight();
        CString result;
        CString ts;
        result.Format(_T("#define WIDTH  (%d)\n")
                      _T("#define HEIGHT  (%d)\n")
                      _T("/* RGB Array*/\n")
                      _T("#define  RGB_DATA    \\\n"),w,h);
        for(int row = 0; row<h; row++){
            for(int col=0; col<w;col++){
                COLORREF color = m_image.GetPixel(col,row);
                ts.Format(_T("0x%02X,0x%02X,0x%02X,"),
                GetRValue(color),GetGValue(color),GetBValue(color));
                result += ts;
                if( (col&7) == 7){
                    result += _T("\\\n");
                }
            }
            result += _T("\\\n");
        }
        result += _T("\n");
        FILE* fp = _tfopen(dlg.GetPathName(),_T("w+"));
        ::_ftprintf(fp,result);
        fclose(fp);
    }
}

extern  unsigned char TetrisMap[18*24];
extern  unsigned char scrBuf[19*23];
void CTetrixPreDlg::OnTimer(UINT nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    // Every 50ms
    //for(int i=0;i<20;i++){
    //    for(int j=0;j<10;j++){
    //        scrBuf[(i+1)*19+j+1] = i+j+1;//TetrisMap[i*18 + j + 4];
    //    }
    //}
    //for(int i=0;i<4;i++){
    //    for(int j=0;j<4;j++){
    //        scrBuf[(i+4)*19+j+13] = i+j+1;
    //    }
    //}
    //m_keyCode = 4;
    //if(GetKeyState(VK_LEFT) & 0xFF00){
    //    m_keyCode = 1;
    //}else if(GetKeyState(VK_RIGHT) & 0xFF00){
    //    m_keyCode = 2;
    //}else if(GetKeyState(VK_UP) & 0xFF00){
    //    m_keyCode = 3;
    //}else if(GetKeyState(VK_DOWN) & 0xFF00){
    //    m_keyCode = 4;
    //}else{
    //    m_keyCode = 0;
    //}
    //Playtetris(this);
    PostMessage(WM_PLAYACTION,6,0);
    CDialog::OnTimer(nIDEvent);

}

LRESULT CTetrixPreDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(WM_PLAYACTION == message){
        m_keyCode = wParam;
        KillTimer(1);
        SetTimer(1,50,0);
        if(Playtetris(this)){
            CImage m_image;
            GenerateJpeg("Hehe.jpg");
            m_image.Load(&scrStream);
            CDC* pDC = m_pic.GetDC();
            RECT rect;
            m_pic.GetWindowRect(&rect);
            LONG w = 19*16;//rect.right - rect.left;
            LONG h = 23*16;//rect.bottom - rect.top;
            //pDC->Rectangle(0,0,w,h);
            m_image.BitBlt(pDC->m_hDC,0,0,w,h,0,0);

            //if(!m_pMemDC){
            //    CBitmap bmp;
            //    bmp.CreateCompatibleBitmap(pDC,w,h);
            //    m_pMemDC = new CDC();
            //    m_pMemDC->CreateCompatibleDC(pDC);
            //    m_pMemDC->SelectObject(&bmp);
            //}
            //if(m_pMemDC){
            //    //m_pMemDC->BitBlt(0,0,w,h,pDC,0,0,SRCCOPY);
            //    m_image.BitBlt(m_pMemDC->m_hDC,0,0,w,h,0,0);
            //}
            m_pic.ReleaseDC(pDC);
        }
    }
    return CDialog::WindowProc(message, wParam, lParam);
}

void CTetrixPreDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
