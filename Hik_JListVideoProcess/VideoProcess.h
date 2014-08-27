#pragma once
//#include "VideoDataBuffer.h"
//class VideoDataBuffer;
#include "GaussTargetBuffer.h"
#include "ReceiveVideoData.h"
//class ReceiveVideoData;
#include "GaussMix.h"
#include "CamShift.h"
class VideoProcess  //主要用来展示效果
{
public:
	VideoProcess(void);
	~VideoProcess(void);
	bool YV12_to_RGB24(unsigned char* pYV12, unsigned char* pRGB24, int iWidth, int iHeight);
	bool Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer, CWnd  * t_pThis );	//初始化处理模块
	bool Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer, GaussTargetBuffer *t_GaussBuffer,CWnd  * t_pThis );	//重载初始化处理模块
	bool Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer,VideoDataBuffer * t_VideoDataBuffer2, 
		             GaussTargetBuffer *t_GaussBuffer,GaussTargetBuffer *t_GaussBuffer2,CWnd  * t_pThis ,bool opennNetType);	//重载初始化处理模块2
	bool Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer,VideoDataBuffer * t_VideoDataBuffer2, VideoDataBuffer * t_VideoDataBuffer3,
		GaussTargetBuffer *t_GaussBuffer,GaussTargetBuffer *t_GaussBuffer2,GaussTargetBuffer *t_GaussBuffer3,CWnd  * t_pThis ,bool opennNetType);	//重载初始化处理模块3
	bool ShowVideoRGB();//展示RGB
	bool ShowGaussBG();//显示高斯结果
	bool SaveGaussResults();//保存高斯结果
	bool SaveGaussResults2();//保存高斯结果 通道2
	bool SaveGaussResults3();//保存高斯结果 通道3
	//验证测试函数
	bool IntrusionDetection(IplImage *pForeImage,FILE *pfile);//判断入侵信息
	bool SaveAnalylizeData(IplImage *pCurrentImage,IplImage *pBackImage);//保存源图像、背景图像梯度，亮度变化
	bool FindGradient(IplImage *src,uchar *dst);//计算梯度
	bool AnalyzeLightData(IplImage *pCurrentImage,IplImage *pBackImage);//统计开灯亮度变化明显数据
public:
	int m_nWidth;
	int m_nHeight;
	int m_nSize;
	CWnd *m_pCwnd;//显示的指针
	VideoDataBuffer *m_pVideoBuffer,*m_pVideoBuffer2,*m_pVideoBuffer3;//数据缓冲区
	GaussTargetBuffer *m_pGaussBuffer,*m_pGaussBuffer2,*m_pGaussBuffer3;//目标缓冲区
public://gauss funtion
	GaussMix m_MixGauss,m_MixGauss2,m_MixGauss3;
	CamShift m_camShift;
	int m_nImageNumber,m_nImageNumber2,m_nImageNumber3;	//当前帧号
	IplImage * m_pFrountImage,*m_pFrountImage2,*m_pFrountImage3;//前景图像
	IplImage * m_pCurrentImage;//当前图像
	//IplImage * m_pLastImage;
	IplImage * m_pBackGroundImage,*m_pBackGroundImage2,*m_pBackGroundImage3;//背景图像
private:
	int m_openType; //打开文件类型
};

