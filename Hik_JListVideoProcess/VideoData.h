#pragma once
class VideoData
{
public:
	VideoData(void);
	~VideoData(void);
public:
	UCHAR *m_pImage;
	bool m_bProcessing;		//是否在处理中
	bool m_bWriting;		//是否在接受新数据中
};

