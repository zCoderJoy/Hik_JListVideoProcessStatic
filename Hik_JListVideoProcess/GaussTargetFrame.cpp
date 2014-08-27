#include "stdafx.h"
#include "GaussTargetFrame.h"


GaussTargetFrame::GaussTargetFrame(void)
{
	m_pForeGroundImage=nullptr;
	m_pCurrentImage=nullptr;
	m_pBackGroundImage=nullptr;
	m_bProcessing=false;
	m_bWriting=true;//为了防止误读尚未写入的图像，所以开始所有置写入中状态。
}


GaussTargetFrame::~GaussTargetFrame(void)
{

}
