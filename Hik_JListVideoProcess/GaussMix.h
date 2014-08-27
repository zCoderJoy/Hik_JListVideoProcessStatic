
#pragma once

#include "cv.h"
#include <vector>
using namespace std;


class GaussMix
{
public:
	GaussMix(void);
	~GaussMix(void);


//公有函数
public:
	int Init( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg );		//初始化背景
	int UpdateRGB( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber );		//更新背景
	int UpdateYUV( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber );
	int UpdateHSV( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber );
	int UpdateGray( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber );

//高斯模型基本参数设定
private:
	double m_dStdThreshold;				//判断符合当前层模型的方差的倍数 /* lambda=2.5 is 99% */
	double m_dBackgroundThreshold;		//判断为前景的接收比例 /* threshold sum of weights for background test */
	double m_dUpdataRateWeight;				//更新权重的比例
	double m_dUpdataRateWeight2;	//为了提高速度，避免重复计算引入变量
	double m_dUpdataRateMean;
	double m_dUpdataRateSigma;
	double m_dUpdataRateSigma2;
	double m_dWeightInit;				//初始权重
	int m_nGaussians;					//高斯模型的层数
	double m_nSigmaInit;					//初始方差
	int m_nRefreshPercent;				//每帧更新的比例

//私有函数
	public:
	int Update1( IplImage* m_pFrame, IplImage* m_pBkImg, int m_dCurrentFrameNumber );	//生成背景方法１，部分更新法，块状更新

	int CountFrImage1( IplImage* m_pFrame, IplImage* m_pFrImg, int t_nOffset );	//计算前景图像

//私有数据区
private:
	int m_nImageWidth;			//图像宽度
	int m_nImageHeight;			//图像高度
	int m_nPixNumber;			//图像像素数目
	int m_nDataSize;			//图像数据大小
	int m_nLineExpand;			//图像每行后的数据
	int m_nLineSize;			//图像每行的字节数
	int m_nChannels;			//通道数

	vector <double *> m_pfBkMean;		//背景图像均值
	
	vector <double *> m_pfBkWeight;		//背景图像权值

	vector <double *> m_pfBkSigma;		//背景图像方差
};
