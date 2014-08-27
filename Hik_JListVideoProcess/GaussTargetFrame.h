#pragma once
class GaussTargetFrame
{
public:
	GaussTargetFrame(void);
	~GaussTargetFrame(void);
public:
	IplImage * m_pForeGroundImage;//前景图像
	IplImage * m_pCurrentImage;//当前图像
	IplImage * m_pBackGroundImage;//背景图像
	bool m_bProcessing;		//是否在处理中
	bool m_bWriting;		//是否在接受新数据中

};

