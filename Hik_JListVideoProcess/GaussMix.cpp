#include "StdAfx.h"
#include "GaussMix.h"
#include <math.h>


GaussMix::GaussMix(void)
{
	//-------------------------------------------------------------------------------------------------
	//基本参数设置，调试时修改
	m_dStdThreshold = 15;//7.5	// 区别于OpenCV中的定义，判断符合当前层模型的方差的倍数 /* lambda=2.5 is 99% */
	m_dBackgroundThreshold = 0.75;		//判断为前景的接收比例 /* threshold sum of weights for background test */
	m_dUpdataRateWeight = 0.05;				//更新权重的比例
	m_dUpdataRateWeight2 = 1 - m_dUpdataRateWeight;
	m_dUpdataRateMean = 0.05;//0.03
	m_dUpdataRateSigma = 0.05;//0.03
	m_dUpdataRateSigma2 = 1 - m_dUpdataRateSigma;
	m_nGaussians = 5;					//高斯模型的层数为5
	m_dWeightInit = 0.05;				//初始权重为0.05
	m_nSigmaInit = 2 * m_dStdThreshold;					//初始方差为30
	m_nRefreshPercent = 20;				//每帧更新比例为全部图像的1/30

	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//数据区置空
	m_pfBkMean.clear();
	m_pfBkWeight.clear();
	m_pfBkSigma.clear();
	//-------------------------------------------------------------------------------------------------
}

GaussMix::~GaussMix(void)
{
	int t_nGaussians = m_pfBkMean.size();
	int i;
	for ( i = 0; i < t_nGaussians; i++ )
	{
		double * t_pfData = m_pfBkMean.back();
		delete[] t_pfData;
		m_pfBkMean.pop_back();

		t_pfData = m_pfBkWeight.back();
		delete[] t_pfData;
		m_pfBkWeight.pop_back();

		t_pfData = m_pfBkSigma.back();
		delete[] t_pfData;
		m_pfBkSigma.pop_back();
	}
}

int GaussMix::Init( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg )	//初始化背景
{
	m_pBkImg=m_pFrame;
	int i, j;
	//---------------------------------------------------------------------
	//获取图像基本参数
	m_nImageWidth = m_pFrame->width;
	m_nImageHeight = m_pFrame->height;
	m_nDataSize = m_pFrame->imageSize;
	m_nPixNumber = m_nImageWidth * m_nImageHeight;
	m_nChannels = m_pFrame->nChannels;
	m_nLineSize = m_pFrame->widthStep;
	m_nLineExpand = m_nLineSize - m_nImageWidth * m_nChannels;
	//---------------------------------------------------------------------
	

	//---------------------------------------------------------------------
	//清空旧空间
	int t_nGaussians = m_pfBkMean.size();
	for ( i = 0; i < t_nGaussians; i++ )
	{
		double * t_pfData = m_pfBkMean.back();
		delete[] t_pfData;
		m_pfBkMean.pop_back();

		t_pfData = m_pfBkWeight.back();
		delete[] t_pfData;
		m_pfBkWeight.pop_back();

		t_pfData = m_pfBkSigma.back();
		delete[] t_pfData;
		m_pfBkSigma.pop_back();
	}
	//---------------------------------------------------------------------
	
	
	//---------------------------------------------------------------------
	//开辟空间
	for ( i = 0; i < m_nGaussians; i++ )
	{
		double * t_pfBkMean;
		t_pfBkMean = new double [m_nDataSize];
		m_pfBkMean.push_back( t_pfBkMean );

		double * t_pfBkWeight;
		t_pfBkWeight = new double [m_nPixNumber];		//权重，每像素一个
		m_pfBkWeight.push_back( t_pfBkWeight );

		double * t_pfBkSigma;
		t_pfBkSigma = new double [m_nDataSize];
		m_pfBkSigma.push_back( t_pfBkSigma );
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	//初始化数据
	for ( i = 0; i < m_nDataSize; i++ )
	{
		m_pfBkMean[0][i] = (UCHAR)m_pFrame->imageData[i];		//初始化背景0
		m_pfBkSigma[0][i] = m_nSigmaInit;

		for ( j = 1; j < m_nGaussians; j++ )		//依次初始化其他各级背景
		{
			m_pfBkMean[j][i] = -60 * j;
			m_pfBkSigma[j][i] = m_nSigmaInit;
		}
	}

	for ( i = 0; i < m_nPixNumber; i++ )
	{
		m_pfBkWeight[0][i] = 1;

		for ( j = 1; j < m_nGaussians; j++ )		//依次初始化各级背景
		{
			m_pfBkWeight[j][i] = 0;
		}
	}

	//---------------------------------------------------------------------

	return 1;
}

int GaussMix::UpdateRGB( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber )
{
	Update1( m_pFrame, m_pBkImg, m_dCurrentFrameNumber );
	return 1;
}

int GaussMix::UpdateHSV( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber )
{
	Update1( m_pFrame, m_pBkImg, m_dCurrentFrameNumber );
	return 1;
}

int GaussMix::UpdateYUV( IplImage* m_pFrame, IplImage* m_pBkImg, IplImage* m_pFrImg, int m_dCurrentFrameNumber )
{
	int t_nOffset = Update1( m_pFrame, m_pBkImg, m_dCurrentFrameNumber );
	CountFrImage1( m_pFrame, m_pFrImg, t_nOffset );
	return 1;
}

int GaussMix::Update1( IplImage* m_pFrame, IplImage* m_pBkImg, int m_dCurrentFrameNumber )
{
	//---------------------------------------------------------------------
	//计算起始和终止地址
	int t_nNumberOfLine = m_nImageHeight / m_nRefreshPercent;	//当前帧一共多少行需要更新
	if ( m_nImageHeight % m_nRefreshPercent > m_dCurrentFrameNumber % m_nRefreshPercent )	//对行数进行修正（如不能整除，则可能增加一行）
	{
		t_nNumberOfLine++;
	}

	int t_nRange = m_nLineSize * ( m_nRefreshPercent - 1 );
	int t_nRangeWeight = m_nImageWidth * ( m_nRefreshPercent - 1 );

	int i, j, k, l;
	int t_nMatchLevel;		//匹配的模型编号
	double t_dThreshold;		//方差的阈值
	double t_dAddupSigma;		//各颜色通道方差的和
	double t_dDelta;			//当前通道的差

	int t_nPossition;		//当前点的地址号
	int t_nPossitionWeight;	//权重的地址号

	t_nPossition = ( m_dCurrentFrameNumber % m_nRefreshPercent ) * m_nLineSize;	//指向第一行行首
	t_nPossitionWeight = ( m_dCurrentFrameNumber % m_nRefreshPercent ) * m_nImageWidth;	//指向第一行行首

	int m_nStatDelta[512] = {0};	//用于统计灰度差分布的数组
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//开始计算
	for ( i = 0; i < t_nNumberOfLine; i++ )		//一共扫描 t_nNumberOfLine 行
	{
		for ( j = 0; j < m_nImageWidth; j++ )	//每行扫描j个像素
		{
			t_dDelta = (UCHAR)m_pFrame->imageData[t_nPossition] - m_pfBkMean[0][t_nPossition];	//用于统计亮度改变
			m_nStatDelta[ (int)t_dDelta + 255 ] ++;	//统计差值

			//---------------------------------------------------------
			//先找到匹配的模型
			t_nMatchLevel = -1;		//如果没有任何模型能匹配，则置为-1

			for ( k = 0; k < m_nGaussians; k++ )	//扫描各级模型
			{
				t_dAddupSigma = 0;		//方差和清0
				t_dThreshold = 0;		//方差阈值清0

				for ( l = 0; l < m_nChannels; l++ )		//计算各通道阈值和方差和
				{
					t_dDelta = m_pfBkMean[k][t_nPossition + l] - (UCHAR)m_pFrame->imageData[t_nPossition + l];	//计算差

					t_dAddupSigma += t_dDelta * t_dDelta;				//同像素方差累加
					t_dThreshold += m_pfBkSigma[k][t_nPossition + l];	//同像素方差阈值累加
				}

				if ( t_dAddupSigma< t_dThreshold * m_dStdThreshold )	//如果方差小于分布
				{
					t_nMatchLevel = k;
					break;
				}
			}
			//---------------------------------------------------------
			
			//---------------------------------------------------------
			//找到匹配模型后，对该模型进行修改
			if ( t_nMatchLevel != -1 )		//如果某一模型匹配，则更新
			{
				//----------------------------------------------
				//更新参数
				for ( k = 0; k < m_nGaussians; k++ )
				{
					m_pfBkWeight[k][t_nPossitionWeight] = m_pfBkWeight[k][t_nPossitionWeight] * ( m_dUpdataRateWeight2 );	//更新权重
				}
				
				m_pfBkWeight[t_nMatchLevel][t_nPossitionWeight] += m_dUpdataRateWeight;			//更新权重
	
				for ( l = 0; l < m_nChannels; l++ )	//更新均值和方差
				{
					t_dDelta = (UCHAR)m_pFrame->imageData[t_nPossition + l] - m_pfBkMean[t_nMatchLevel][t_nPossition + l];
					m_pfBkMean[t_nMatchLevel][t_nPossition + l] = m_pfBkMean[t_nMatchLevel][t_nPossition + l] + t_dDelta * m_dUpdataRateMean;	//更新均值
					m_pfBkSigma[t_nMatchLevel][t_nPossition + l] = t_dDelta * t_dDelta * m_dUpdataRateSigma + m_pfBkSigma[t_nMatchLevel][t_nPossition + l] * ( m_dUpdataRateSigma2 );

//					m_pBkImg->imageData[t_nPossition + l] = (int)m_pfBkMean[0][t_nPossition + l];	//借用循环，计算背景图像
				}
				//----------------------------------------------

				//----------------------------------------------
				//更新顺序
				for ( k = t_nMatchLevel; k > 0; k-- )	//自下而上对比判断
				{
					if ( m_pfBkWeight[k][t_nPossitionWeight] > m_pfBkWeight[k - 1][t_nPossitionWeight] )	//权重顺序是否发生变化，如果发生变化，更新各层模型
					{
						//交换权值
						double t_dWeight;
						t_dWeight = m_pfBkWeight[k][t_nPossitionWeight];	
						m_pfBkWeight[k][t_nPossitionWeight] = m_pfBkWeight[k -1][t_nPossitionWeight];
						m_pfBkWeight[k - 1][t_nPossitionWeight] = t_dWeight;

						//更新各级背景值
						for ( l = 0; l < m_nChannels; l++ )
						{
							double t_dForChange;
							t_dForChange = m_pfBkMean[k][t_nPossition + l];//交换均值
							m_pfBkMean[k][t_nPossition + l] = m_pfBkMean[k -1][t_nPossition + l];
							m_pfBkMean[k -1][t_nPossition + l] = t_dForChange;

							t_dForChange = m_pfBkSigma[k][t_nPossition + l];//交换方差
							m_pfBkSigma[k][t_nPossition + l] = m_pfBkSigma[k -1][t_nPossition + l];
							m_pfBkSigma[k -1][t_nPossition + l] = t_dForChange;
						}
					}
					else	//否则退出循环
					{
						break;
					}
				}
			}
			else	//如果没有任何模型匹配，则添加新的模型
			{
				m_pfBkWeight[m_nGaussians - 1][t_nPossitionWeight] = m_dUpdataRateWeight;

				double t_dWeightAddup = 0;

				for ( k = 0; k < m_nGaussians; k++ )
				{
					t_dWeightAddup += m_pfBkWeight[k][t_nPossitionWeight];
				}

				for ( k = 0; k < m_nGaussians; k++ )
				{
					m_pfBkWeight[k][t_nPossitionWeight] = m_pfBkWeight[k][t_nPossitionWeight] / t_dWeightAddup;//归一化
				}

				for ( l = 0; l < m_nChannels; l++ )
				{
					m_pfBkMean[m_nGaussians - 1][t_nPossition + l] = (UCHAR)m_pFrame->imageData[t_nPossition + l];
					m_pfBkSigma[m_nGaussians - 1][t_nPossition + l] = m_nSigmaInit;

//					m_pBkImg->imageData[t_nPossition + l] = (UCHAR)m_pfBkMean[0][t_nPossition + l];	//借用循环，计算背景图像
				}

			}
			//---------------------------------------------------------

			//---------------------------------------------------------
			//生成背景图像
			for ( l = 0; l < m_nChannels; l++ )
			{
				m_pBkImg->imageData[t_nPossition + l] = (int)/*(UCHAR)*/m_pfBkMean[0][t_nPossition + l];	//借用循环，计算背景图像
			}
			//---------------------------------------------------------

			//---------------------------------------------------------
			//更新参数
			t_nPossitionWeight ++;
			t_nPossition += m_nChannels;
			//---------------------------------------------------------
		}

		t_nPossitionWeight += t_nRangeWeight;
		t_nPossition += t_nRange;		//每次指向行首第一个字节
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	//统计总体偏差
	int t_nAddUp = m_nStatDelta[100] + m_nStatDelta[101];
	///int t_nAddUp = 0;
	//int sum=0;
	int t_nMaxGrayLevel = 0;	//最大的统计值
	int t_nMaxGrayPos = 0;		//最大统计值所在坐标

	for ( i =102; i < 501; i++ )//求最大值
	{
	t_nAddUp += m_nStatDelta[i];

	if ( t_nAddUp > t_nMaxGrayLevel )
	{
	t_nMaxGrayPos = i;
	t_nMaxGrayLevel = t_nAddUp;
	}

	t_nAddUp -= m_nStatDelta[i-2];
	}
	/*for ( i = 200; i < 401; i++ )
	{
		t_nAddUp += m_nStatDelta[i]*i;
		sum+=m_nStatDelta[i];
		

		
	}
	t_nMaxGrayPos=t_nAddUp/sum;*/

	return ( t_nMaxGrayPos - 255 );
}

int GaussMix::CountFrImage1( IplImage* m_pFrame, IplImage* m_pFrImg, int t_nOffset )
{
	//---------------------------------------------------------------------
	//初始化地址编号
	int t_nPossition = 0;		//当前点的地址号，指向第一行行首
	int t_nPossitionWeight = 0;	//权重的地址号，指向第一行行首
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//定义变量
	int i, j, k, l;
	double t_dAddupSigma;		//方差和
	double t_dAddupWeight;
	double t_dThreshold;		//方差阈值
	double t_dDelta;			//灰度差
	//---------------------------------------------------------------------

	//---------------------------------------------------------------------
	//开始计算前景，计算前景时，按照像素进行计算
	for ( i = 0; i < m_nImageHeight; i++ )
	{
		for ( j = 0; j < m_nImageWidth; j++ )
		{
			t_dAddupWeight = 0;		//权重置0

			for ( k = 0; k < m_nGaussians; k++ )
			{
				t_dAddupSigma = 0;		//方差和清0
				t_dThreshold = 0;		//方差阈值清0

				t_dDelta = (UCHAR)m_pFrame->imageData[t_nPossition] - m_pfBkMean[k][t_nPossition] - t_nOffset;	//计算差
				t_dAddupSigma += t_dDelta * t_dDelta;				//累加方差
				t_dThreshold += m_pfBkSigma[k][t_nPossition];	//累加阈值

				for ( l = 1; l < m_nChannels; l++ )
				{
					t_dDelta = m_pfBkMean[k][t_nPossition + l] - (UCHAR)m_pFrame->imageData[t_nPossition + l];	//计算差

					t_dAddupSigma += t_dDelta * t_dDelta;				//累加方差
					t_dThreshold += m_pfBkSigma[k][t_nPossition + l];	//累加阈值
				}

				if ( t_dThreshold < 10 )
				{
					t_dThreshold = 10;
				}

				if ( t_dAddupSigma < t_dThreshold * m_dStdThreshold )	//如果符合该分布，则视为背景，跳出循环
				{
					m_pFrImg->imageData[t_nPossitionWeight] = 0;
					break;
				}

				t_dAddupWeight += m_pfBkWeight[k][t_nPossitionWeight];

				if ( t_dAddupWeight > m_dBackgroundThreshold )
				{
 					m_pFrImg->imageData[t_nPossitionWeight] = (char)255;
					break;
				}
			}

			t_nPossitionWeight++;
			t_nPossition += m_nChannels;
		}
	}
	//---------------------------------------------------------------------

	return 0;
}
