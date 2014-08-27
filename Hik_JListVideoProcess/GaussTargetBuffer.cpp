#include "stdafx.h"
#include "GaussTargetBuffer.h"


GaussTargetBuffer::GaussTargetBuffer(void):m_nWidth(704),m_nHeight(576),m_nSize(0),m_ndeepDegree(0),m_nChannel(3),writeTargetLocation(0)
{
	
}


GaussTargetBuffer::~GaussTargetBuffer(void)
{
	
	for (int i=0;i<m_ndeepDegree;i++)
	{
		if (m_gaussTargetBuffer[i].m_pCurrentImage!=nullptr)
		{
			cvReleaseImage(&m_gaussTargetBuffer[i].m_pCurrentImage);
			cvReleaseImage(&m_gaussTargetBuffer[i].m_pForeGroundImage);
			cvReleaseImage(&m_gaussTargetBuffer[i].m_pBackGroundImage);
		}
	}
}
bool GaussTargetBuffer::Initial( int nWidth, int nHeight, int nSize, int nFrameDeep )	//初始化函数
{
	m_nWidth=nWidth;
	m_nHeight=nHeight;
	m_nSize=nSize;
	m_ndeepDegree=nFrameDeep;//深度

	GaussTargetFrame temp;
	for (int i=0;i<m_ndeepDegree;i++)//申请帧缓存区
	{	
		//开辟图像空间
		temp.m_pForeGroundImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );
		temp.m_pCurrentImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );
		temp.m_pBackGroundImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );

		temp.m_pForeGroundImage->origin = 0;
		temp.m_pCurrentImage->origin = 0;
		temp.m_pBackGroundImage->origin = 0;
		m_gaussTargetBuffer.push_back(temp);

	}
	return true;
}