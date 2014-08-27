
// Hik_JListVideoProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Hik_JListVideoProcess.h"
#include "Hik_JListVideoProcessDlg.h"
#include "ReceiveVideoData.h"
#include "VideoDataBuffer.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 //bool flagThreadExit = false;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHik_JListVideoProcessDlg 对话框



CHik_JListVideoProcessDlg::CHik_JListVideoProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHik_JListVideoProcessDlg::IDD, pParent)
	, m_bisNetOpen(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHik_JListVideoProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHik_JListVideoProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Login, &CHik_JListVideoProcessDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_Start, &CHik_JListVideoProcessDlg::OnBnClickedButtonStart)
//	ON_BN_CLICKED(IDCANCEL, &CHik_JListVideoProcessDlg::OnBnClickedCancel)
ON_BN_CLICKED(IDC_BUTTON_ShowRGB, &CHik_JListVideoProcessDlg::OnBnClickedButtonShowrgb)
ON_BN_CLICKED(IDC_BUTTON_GaussBK, &CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk)
ON_BN_CLICKED(IDC_BUTTON_Capture, &CHik_JListVideoProcessDlg::OnBnClickedButtonCapture)
ON_BN_CLICKED(IDC_BUTTON_Open, &CHik_JListVideoProcessDlg::OnBnClickedButtonOpen)
ON_BN_CLICKED(IDC_BUTTON_CLIMBdet, &CHik_JListVideoProcessDlg::OnBnClickedButtonClimbdet)
ON_BN_CLICKED(IDC_BUTTON_GaussBK2, &CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk2)
ON_BN_CLICKED(IDC_BUTTON_GaussBK3, &CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk3)
ON_BN_CLICKED(IDC_BUTTON_AreaDet, &CHik_JListVideoProcessDlg::OnBnClickedButtonAreadet)
ON_BN_CLICKED(IDC_BUTTON_Open2s, &CHik_JListVideoProcessDlg::OnBnClickedButtonOpen2s)
 ON_MESSAGE(WM_USER_THREADEND, OnUserThreadend) 
  ON_MESSAGE(WM_USER_THREADEND2, OnUserThreadend2) 
END_MESSAGE_MAP()


// CHik_JListVideoProcessDlg 消息处理程序

BOOL CHik_JListVideoProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_hwndclimb=GetSafeHwnd();
	m_hscreen=GetDlgItem(IDC_STATIC_hscreen)->GetSafeHwnd();
	m_hscreen2=GetDlgItem(IDC_STATIC_hscreen2)->GetSafeHwnd();
	m_hscreen3=GetDlgItem(IDC_STATIC_hscreen3)->GetSafeHwnd();
	m_cWnd=GetDlgItem(IDC_STATIC_PostScreen);
	m_winThread=NULL;
	SetDlgItemText(IDC_EDIT_ClimbDet1,"无警告信息！"); 
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHik_JListVideoProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHik_JListVideoProcessDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHik_JListVideoProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHik_JListVideoProcessDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_ReceiveData.InitHKNET_DVR();//初始化海康网卡
	if (!m_ReceiveData.login())
	{
		//AfxMessageBox("录像文件正在播放，请暂停！");
		//NET_DVR_Cleanup();
		return ;
	}
	m_bisNetOpen=true;
}
LRESULT CHik_JListVideoProcessDlg::OnUserThreadend(WPARAM wParam, LPARAM lParam) //自定义消息响应
{
	SetDlgItemText(IDC_EDIT_ClimbDet1,"请注意，有攀高可能性，警告！！");//UpdateData(false);//TRACE("WM_USER_THREADEND message /n");
	
	return 0;
}
LRESULT CHik_JListVideoProcessDlg::OnUserThreadend2(WPARAM wParam, LPARAM lParam) //自定义消息响应2 表示无警告
{
	SetDlgItemText(IDC_EDIT_ClimbDet1,"无警告信息！！");//UpdateData(false);//TRACE("WM_USER_THREADEND message /n");

	return 0;
}      

void CHik_JListVideoProcessDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!flagThreadExit)//释放线程
	{
		flagThreadExit=true;
		//m_winThread=nullptr;
	}
	else
		flagThreadExit=false;
	if(m_winThread!=NULL) 
	{
		PostThreadMessage(m_winThread->m_nThreadID,WM_QUIT,0,0);
		m_winThread=nullptr;
	}
	if (m_VideoBuffer.GetDeep()<0)
	{
		m_VideoBuffer.Initial(704,576,704*576*3,10);//固定海康
		m_VideoBuffer2.Initial(704,576,704*576*3,10);//固定海康
		m_VideoBuffer3.Initial(704,576,704*576*3,10);//固定海康
		m_GaussVideoBuffer.Initial(704,576,704*576,10);//初始化高斯
		m_GaussVideoBuffer2.Initial(704,576,704*576,10);//初始化高斯
		m_GaussVideoBuffer3.Initial(704,576,704*576,10);//初始化高斯
		m_ReceiveData.InitVideoBuffer(&m_VideoBuffer,&m_VideoBuffer2,&m_VideoBuffer3);//初始化
	}
	//else flagThreadExit=true;
	//else m_winThread->ExitInstance();
	//else AfxEndThread(m_winThread->m_nThreadID);
	else 
		{
			/*if (!flagThreadExit)
			{
				flagThreadExit=true;
			}
			else
				flagThreadExit=false;*/
		
		}
	m_ReceiveData.ShowOriginalVideo(m_hscreen,m_hscreen2,m_hscreen3,m_bisNetOpen,m_spath,m_spath2);
}


//void CHik_JListVideoProcessDlg::OnBnClickedCancel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnCancel();
//}


/////////////////////////////////////////////////////////线程
UINT ThreadProcessRGB(LPVOID pParam)//创建线程，显示RGB
{
	CHik_JListVideoProcessDlg *l_pVPD=(CHik_JListVideoProcessDlg *)pParam;
	l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,l_pVPD->m_cWnd);
	l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	return 0;
}
//通道一
UINT ThreadProcessGauss(LPVOID pParam)//创建线程，保存高斯或显示高斯结果
{
	CHik_JListVideoProcessDlg *l_pVPD=(CHik_JListVideoProcessDlg *)pParam;
	//l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,l_pVPD->m_cWnd);//原处理函数
	l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,&l_pVPD->m_VideoBuffer2,&l_pVPD->m_GaussVideoBuffer,&l_pVPD->m_GaussVideoBuffer2,l_pVPD->m_cWnd,l_pVPD->m_bisNetOpen);//重载的加高斯目标保存函数
	//l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	l_pVPD->m_VideoProcess.SaveGaussResults();//目前保存及展示背景建模
	return 0;
}
//通道2
UINT ThreadProcessGauss2(LPVOID pParam)//创建线程，保存高斯或显示高斯结果
{
	CHik_JListVideoProcessDlg *l_pVPD=(CHik_JListVideoProcessDlg *)pParam;
	//l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,l_pVPD->m_cWnd);//原处理函数
	l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,&l_pVPD->m_VideoBuffer2,&l_pVPD->m_GaussVideoBuffer,&l_pVPD->m_GaussVideoBuffer2,l_pVPD->m_cWnd,l_pVPD->m_bisNetOpen);//重载的加高斯目标保存函数
	//l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	l_pVPD->m_VideoProcess.SaveGaussResults2();//目前保存及展示背景建模
	return 0;
}
//通道3回调函数
UINT ThreadProcessGauss3(LPVOID pParam)//创建线程，保存高斯或显示高斯结果
{
	CHik_JListVideoProcessDlg *l_pVPD=(CHik_JListVideoProcessDlg *)pParam;
	//l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,l_pVPD->m_cWnd);//原处理函数
	l_pVPD->m_VideoProcess.Initiate(704,576,704*576*3,&l_pVPD->m_VideoBuffer,&l_pVPD->m_VideoBuffer2,&l_pVPD->m_VideoBuffer3,&l_pVPD->m_GaussVideoBuffer,&l_pVPD->m_GaussVideoBuffer2,&l_pVPD->m_GaussVideoBuffer3,l_pVPD->m_cWnd,l_pVPD->m_bisNetOpen);//重载的加高斯目标保存函数
	//l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	l_pVPD->m_VideoProcess.SaveGaussResults3();//目前保存及展示背景建模
	return 0;
}
//创建线程，处理攀高
UINT ThreadProcessClimbDet(LPVOID pParam)//创建线程
{
	CHik_JListVideoProcessDlg *l_pVPD2=(CHik_JListVideoProcessDlg *)pParam;
	
	l_pVPD2->m_VideoPostProcess.initVPP(&l_pVPD2->m_GaussVideoBuffer,&l_pVPD2->m_GaussVideoBuffer2,l_pVPD2->m_hwndclimb);//初始化后处理类
	//l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	l_pVPD2->m_VideoPostProcess.ClimbDetect();//攀高检测
	return 0;
}
//创建线程，区域检测
UINT ThreadProcessAreaDet(LPVOID pParam)//创建线程
{
	CHik_JListVideoProcessDlg *l_pVPD2=(CHik_JListVideoProcessDlg *)pParam;

	l_pVPD2->m_VideoPostProcess.initVPP(&l_pVPD2->m_GaussVideoBuffer,&l_pVPD2->m_GaussVideoBuffer2,&l_pVPD2->m_GaussVideoBuffer3);///初始化后处理类
	//l_pVPD->m_VideoProcess.ShowVideoRGB();
	//l_pVPD->m_VideoProcess.ShowGaussBG();
	l_pVPD2->m_VideoPostProcess.AreaDetect();//区域检测
	return 0;
}
void CHik_JListVideoProcessDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*NET_DVR_Logout_V30(userID);

	NET_DVR_Cleanup();	*/
	//g_pVideoBuffer=NULL;
	//m_winThread->ExitInstance();
	//_CrtDumpMemoryLeaks();
	//m_winThread->m_bAutoDelete=false;//手动删除
	//
	//if (m_winThread)
	//{
	//	
	//	delete m_winThread;
	//	m_winThread=NULL;
	//}
	//AfxEndThread(m_winThread->m_nThreadID);
	
	if(m_ReceiveData.CloseVideo())
	{
		
		CDialogEx::OnCancel();
	}
	else
	{
		return;
	}
}


void CHik_JListVideoProcessDlg::OnBnClickedButtonShowrgb()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_winThread!=NULL) 
	{
		PostThreadMessage(m_winThread->m_nThreadID,WM_QUIT,0,0);
		m_winThread=nullptr;
	}
	else

		//m_winThread=new CWinThread;
	/*if (m_winThread)
	{
		if (!flagThreadExit)
		{
			flagThreadExit=true;
		}
		else
		{flagThreadExit=false;AfxBeginThread(ThreadProcessRGB,this);}
	}
	else*/
		m_winThread=AfxBeginThread(ThreadProcessRGB,this);
	
	/*m_VideoProcess.Initiate(704,576,704*576*3,&m_VideoBuffer,m_cWnd);
	m_VideoProcess.ShowVideoRGB();*/
}


void CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_winThread!=NULL) 
	{
		PostThreadMessage(m_winThread->m_nThreadID,WM_QUIT,0,0);
		m_winThread=nullptr;
	}
	else
	/*if (m_winThread)
	{
		if (!flagThreadExit)
		{
			flagThreadExit=true;
		}
		else
		{flagThreadExit=false;AfxBeginThread(ThreadProcessRGB,this);}
	}
	else*/
		m_winThread=AfxBeginThread(ThreadProcessGauss,this);
	
	/*if (m_winThread)
	{
	if (!flagThreadExit)
	{
	flagThreadExit=true;
	m_winThread=nullptr;
	}
	else
	{flagThreadExit=false;AfxBeginThread(ThreadProcessGauss,this);}
	}
	else
	{flagThreadExit=false;m_winThread=AfxBeginThread(ThreadProcessGauss,this);}*/

}


void CHik_JListVideoProcessDlg::OnBnClickedButtonCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;//="save";str=str+num;
	static int num=0;
	str.Format("..\\CaptureImage\\save%d.bmp",num++);
	cvSaveImage(str,m_GaussVideoBuffer.m_gaussTargetBuffer[(m_GaussVideoBuffer.writeTargetLocation-1+m_GaussVideoBuffer.GetDeep())%m_GaussVideoBuffer.GetDeep()].m_pForeGroundImage);
}


void CHik_JListVideoProcessDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if(!m_ReceiveData.Open())
	{
		AfxMessageBox("网络流正在播放，请暂停！");
		return;
	}
	if (m_ReceiveData.m_userID>=0)
	{
		NET_DVR_Logout_V30(m_ReceiveData.m_userID);
		m_ReceiveData.m_userID=-1;
		NET_DVR_Cleanup();	
	}

	m_bisNetOpen=false;
	CFileDialog dlg(TRUE,_T("*.mp4"),NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST/*|OFN_HIDEREADONLY*/
		,_T("image files (*.mp4;*.jpg)|*.mp4;*.jpg|ALL Files(*.*)|*.*||"),NULL); // 选项视频的约定
	dlg.m_ofn.lpstrTitle=_T("Open Image"); // 打开文件对话框的标题名
	if (dlg.DoModal()!=IDOK)
	{
		return; // 判断是否获得视频
	}
	m_spath=dlg.GetPathName(); // 获取视频路径
	

}


void CHik_JListVideoProcessDlg::OnBnClickedButtonClimbdet()//攀高检测
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_winThreadClimbDet!=NULL) 
	{
		PostThreadMessage(m_winThreadClimbDet->m_nThreadID,WM_QUIT,0,0);
		m_winThreadClimbDet=nullptr;
	}
	else
	
		m_winThreadClimbDet=AfxBeginThread(ThreadProcessClimbDet,this);
	
}


void CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_winThread2!=NULL) 
	{
		PostThreadMessage(m_winThread2->m_nThreadID,WM_QUIT,0,0);
		m_winThread2=nullptr;
	}
	else

		m_winThread2=AfxBeginThread(ThreadProcessGauss2,this);
	

}


void CHik_JListVideoProcessDlg::OnBnClickedButtonGaussbk3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_winThread3!=NULL) 
	{
		PostThreadMessage(m_winThread3->m_nThreadID,WM_QUIT,0,0);
		m_winThread3=nullptr;
	}
	else

		m_winThread3=AfxBeginThread(ThreadProcessGauss3,this);

}


void CHik_JListVideoProcessDlg::OnBnClickedButtonAreadet()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_winThreadAreaDet!=NULL) 
	{
		PostThreadMessage(m_winThreadAreaDet->m_nThreadID,WM_QUIT,0,0);
		m_winThreadAreaDet=nullptr;
	}
	else

		m_winThreadAreaDet=AfxBeginThread(ThreadProcessAreaDet,this);
}


void CHik_JListVideoProcessDlg::OnBnClickedButtonOpen2s()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_ReceiveData.Open())
	{
		AfxMessageBox("网络流正在播放，请暂停！");
		return;
	}
	if (m_ReceiveData.m_userID>=0)
	{
		NET_DVR_Logout_V30(m_ReceiveData.m_userID);
		m_ReceiveData.m_userID=-1;
		NET_DVR_Cleanup();	
	}

	m_bisNetOpen=false;
	m_spath="E:/FFOutput/ClimbDet/climb1.mp4";
	m_spath2="E:/FFOutput/ClimbDet/climb2.mp4";

}
