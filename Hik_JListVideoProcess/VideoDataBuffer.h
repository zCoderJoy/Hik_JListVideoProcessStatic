#pragma once
#include "VideoData.h"

#include <vector>
using namespace std;
//class VideoData;
class VideoDataBuffer
{
public:	
	//图像参数
	int m_nWidth;
	int m_nHeight;
	int m_nSize;//图像大小	
	int m_nChannel;
	int m_nChannel2;//通道2
	int m_nChannel3;//通道3
	int m_nBufferDeep;		//缓冲区深度
public:
	VideoDataBuffer(void);
	~VideoDataBuffer(void);
	vector<VideoData> m_VideoBuffer;//图像缓存区
public:
	bool Initial( int nWidth, int nHeight, int nSize, int nFrameDeep );		//初始化函数
	int GetDeep(){return m_nBufferDeep;}
};

