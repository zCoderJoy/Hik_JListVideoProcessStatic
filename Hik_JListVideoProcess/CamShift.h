#pragma once
#include "cv.h"
#include "highgui.h"
class CamShift
{
public:
	CamShift(void);
	~CamShift(void);
public:
	//选择物体是否
	int m_selectObject,m_trackObject;
	int m_dims;
	IplImage *m_curImage,*m_hsv,*m_hue,*m_mask;
	CvRect m_selectedArea;
	CvRect m_trackWindow;//得出的窗口
	CvHistogram *m_hist;//直方图
	CvConnectedComp m_trackComp;//链接部件
	IplImage *m_backProject;//反向投影图
	//CHistogram *hist;

	float m_hranges_arr[2]; //= {0,180};
	//像素值的范围
	float* m_ranges ;//= hranges_arr;
	CvBox2D m_trackBox;
public:
	bool CamShiftProcess(IplImage *crtImage,CvRect srctw,CvRect newtw);
};

