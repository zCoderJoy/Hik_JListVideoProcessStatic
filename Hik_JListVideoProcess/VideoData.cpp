#include "stdafx.h"
#include "VideoData.h"


VideoData::VideoData(void)
{
	m_pImage=NULL;
	m_bProcessing = false;
	m_bWriting = true;			//为了防止误读尚未写入的图像，所以开始所有置写入中状态。
}


VideoData::~VideoData(void)
{
}
