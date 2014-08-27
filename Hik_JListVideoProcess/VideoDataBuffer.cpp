#include "stdafx.h"
#include "VideoDataBuffer.h"


VideoDataBuffer::VideoDataBuffer(void)
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_nSize = 0;
	m_nChannel=1;
	m_nChannel2=2;
	m_nChannel3=3;
	m_nBufferDeep = -1;
}


VideoDataBuffer::~VideoDataBuffer(void)
{
	int i;
	for ( i = 0; i < m_nBufferDeep; i++ )
	{
		if ( m_VideoBuffer[i].m_pImage != NULL )
		{
			delete m_VideoBuffer[i].m_pImage;
			m_VideoBuffer[i].m_pImage=NULL;
		}
	}

	m_VideoBuffer.clear();
}
bool VideoDataBuffer::Initial( int nWidth, int nHeight, int nSize, int nFrameDeep )
{	
	//图像参数
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nSize = nSize;

	//缓冲区深度
	m_nBufferDeep = nFrameDeep;

	VideoData t_VideoDataPerFrame;		//单帧图像数据

	int i;
	for ( i = 0; i < m_nBufferDeep; i++ )
	{
		//t_VideoDataPerFrame.m_nFrameNumber = i;			//编号

		m_VideoBuffer.push_back( t_VideoDataPerFrame );			//添加

		m_VideoBuffer[i].m_pImage = new UCHAR[ m_nSize ];		//开辟空间

		memset( m_VideoBuffer[i].m_pImage, 0, m_nSize );		//初始化空间
	}

	return true;
}