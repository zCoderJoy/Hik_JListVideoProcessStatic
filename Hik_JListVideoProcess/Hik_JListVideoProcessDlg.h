
// Hik_JListVideoProcessDlg.h : 头文件
//

#pragma once
#include "receivevideodata.h"
#include "VideoProcess.h"
#include "VideoPostProcess.h"
//class VideoProcess;


// CHik_JListVideoProcessDlg 对话框
class CHik_JListVideoProcessDlg : public CDialogEx
{
// 构造
public:
	CHik_JListVideoProcessDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HIK_JLISTVIDEOPROCESS_DIALOG };

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
	CWinThread *m_winThread,*m_winThread2,*m_winThread3/*三个预处理线程*/,*m_winThreadClimbDet,*m_winThreadAreaDet;
	CWnd *m_cWnd;
	HWND m_hwndclimb;//攀高警告消息响应窗口句柄
	HWND m_hscreen;
	HWND m_hscreen2;//显示通道2
	HWND m_hscreen3;//显示通道3
	bool m_bisNetOpen;//打开模式，是否是网络流
	CString m_spath;//打开文件路径
	CString m_spath2;//打开文件路径2
	VideoDataBuffer m_VideoBuffer,m_VideoBuffer2,m_VideoBuffer3;
	ReceiveVideoData m_ReceiveData;
	VideoProcess m_VideoProcess;
	GaussTargetBuffer m_GaussVideoBuffer,m_GaussVideoBuffer2,m_GaussVideoBuffer3;
	VideoPostProcess m_VideoPostProcess;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonStart();
//	afx_msg void OnBnClickedCancel();	
	afx_msg void OnBnClickedButtonShowrgb();
	afx_msg void OnBnClickedButtonGaussbk();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonCapture();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClimbdet();//攀高检测
	afx_msg void OnBnClickedButtonGaussbk2();//通道二高斯模型
	afx_msg void OnBnClickedButtonGaussbk3();
	afx_msg void OnBnClickedButtonAreadet();//区域检测
	afx_msg void OnBnClickedButtonOpen2s();
	afx_msg LRESULT OnUserThreadend(WPARAM wParam, LPARAM lParam);//自定义消息响应处理攀高检测，显示警告信息
	afx_msg LRESULT OnUserThreadend2(WPARAM wParam, LPARAM lParam);//自定义消息响应处理攀高检测，显示警告信息
};
