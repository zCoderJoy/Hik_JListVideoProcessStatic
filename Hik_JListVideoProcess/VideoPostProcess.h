#pragma once
//提取的当前，背景，前景图像进行处理
#include "GaussTargetBuffer.h"
class VideoPostProcess
{
public:
	VideoPostProcess(void);
	~VideoPostProcess(void);
	bool initVPP(GaussTargetBuffer *GaussBuffer,GaussTargetBuffer *GaussBuffer2);//初始化视频后处理目标值
	bool initVPP(GaussTargetBuffer *GaussBuffer,GaussTargetBuffer *GaussBuffer2,HWND hwndClimb);//初始化视频后处理目标值
	bool initVPP(GaussTargetBuffer *GaussBuffer,GaussTargetBuffer *GaussBuffer2,GaussTargetBuffer *GaussBuffer3);//初始化视频后处理目标值
	//bool ClimbDetect(GaussTargetBuffer *GaussBuffer,GaussTargetBuffer *GaussBuffer2);
	bool ClimbDetect();//攀高检测
	bool AreaDetect();//区域检测
private:
	GaussTargetBuffer *m_pGaussBuffer,*m_pGaussBuffer2,*m_pGaussBuffer3;
	HWND m_hwndClimb;
};

