#pragma once
#include "GaussTargetFrame.h"
#include <vector>
using namespace std;
class GaussTargetBuffer
{
public:
	GaussTargetBuffer(void);
	~GaussTargetBuffer(void);
public://参数
	int m_nWidth;
	int m_nHeight;
	int m_nSize;//图像大小	
	int m_nChannel;
	int m_ndeepDegree;//深度
	vector <GaussTargetFrame> m_gaussTargetBuffer;
	unsigned int writeTargetLocation;
public:
	bool Initial( int nWidth, int nHeight, int nSize, int nFrameDeep );		//初始化函数
	int GetDeep(){return m_ndeepDegree;}
};

