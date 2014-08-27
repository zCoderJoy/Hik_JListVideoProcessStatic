#include "stdafx.h"
#include "CamShift.h"
#include "ctype.h"

CamShift::CamShift(void)
{
	m_selectObject=0;
	m_trackObject=0;
	m_curImage=0;
	m_dims=48;//分成16个bins
	m_hranges_arr[0]=0,m_hranges_arr[1]=180;//= {1,180};
	m_ranges=m_hranges_arr;
;
}


CamShift::~CamShift(void)
{
	if (m_curImage)
	{
		cvReleaseImage(&m_curImage);
		cvReleaseImage(&m_hsv);
		cvReleaseImage(&m_hue);
		cvReleaseImage(&m_mask);
		cvReleaseImage(&m_backProject);
		cvReleaseHist(&m_hist);
	}
}
CvPoint origin;
void on_mouse( int event, int x, int y, int flags, void* param )
	//鼠标回调函数,该函数用鼠标进行跟踪目标的选择
{
	CamShift *pCamShift=(CamShift*)(param);
	if( !pCamShift->m_curImage )
		return;

	if( pCamShift->m_curImage->origin )
		y = pCamShift->m_curImage->height - y;
	//如果图像原点坐标在左下,则将其改为左上

	if( pCamShift->m_selectObject )
		//select_object为1,表示在用鼠标进行目标选择
			//此时对矩形类selection用当前的鼠标位置进行设置
	{
		pCamShift->m_selectedArea.x = MIN(x,origin.x);
		pCamShift->m_selectedArea.y = MIN(y,origin.y);
		pCamShift->m_selectedArea.width = pCamShift->m_selectedArea.x + CV_IABS(x - origin.x);
		pCamShift->m_selectedArea.height = pCamShift->m_selectedArea.y + CV_IABS(y - origin.y);
		//一下是约束
		pCamShift->m_selectedArea.x = MAX( pCamShift->m_selectedArea.x, 0 );
		pCamShift->m_selectedArea.y = MAX( pCamShift->m_selectedArea.y, 0 );
		pCamShift->m_selectedArea.width = MIN( pCamShift->m_selectedArea.width, pCamShift->m_curImage->width );
		pCamShift->m_selectedArea.height = MIN( pCamShift->m_selectedArea.height, pCamShift->m_curImage->height );
		pCamShift->m_selectedArea.width -= pCamShift->m_selectedArea.x;
		pCamShift->m_selectedArea.height -= pCamShift->m_selectedArea.y;
	}

	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		//鼠标按下,开始点击选择跟踪物体
		origin = cvPoint(x,y);
		pCamShift->m_selectedArea = cvRect(x,y,0,0);
		pCamShift->m_selectObject = 1;
		break;
	case CV_EVENT_LBUTTONUP:
		//鼠标松开,完成选择跟踪物体
		pCamShift->m_selectObject = 0;
		if( pCamShift->m_selectedArea.width > 0 && pCamShift->m_selectedArea.height > 0 )
			//如果选择物体有效，则打开跟踪功能
				pCamShift->m_trackObject = -1;
		break;
	}
}
bool CamShift::CamShiftProcess(IplImage *crtImage,CvRect srctw,CvRect newtw)
{
	if (!crtImage)
	return false;
	//cvSetMouseCallback( "CamShiftDemo", on_mouse, (void*)this);//设置鼠标回调函数
	if (m_trackObject==0)
	{
		m_selectedArea.x=240;
		m_selectedArea.y=150;
		m_selectedArea.width=200;
		m_selectedArea.height=100;
	}
	
	//m_trackObject=1;
	if (!m_curImage)//申请空间
	{
		m_curImage=cvCreateImage(cvGetSize(crtImage),8,3);
		m_curImage->origin=crtImage->origin;
		m_hsv = cvCreateImage( cvGetSize(crtImage), 8, 3 );
		m_hue = cvCreateImage( cvGetSize(crtImage), 8, 1 );
		m_mask = cvCreateImage( cvGetSize(crtImage), 8, 1 );//分配掩膜图像空间

		m_backProject = cvCreateImage( cvGetSize(crtImage), 8, 1 );//分配反向投影图空间,大小一样,单通道
		m_hist = cvCreateHist( 1, &m_dims, CV_HIST_ARRAY, &m_ranges, 1 );//分配直方图空间

	}
	cvCopy(crtImage,m_curImage,0);
	cvCvtColor(m_curImage,m_hsv,CV_BGR2HSV);//把图像从RGB表色系转为HSV表色系

	//if (m_trackObject)
	{
		//制作掩膜板，只处理像素值为H：0~180，S：smin~256，V：vmin~vmax之间的部分
		cvInRangeS( m_hsv, cvScalar(0,30,10,0),cvScalar(180,256,256,0), m_mask );
		cvSplit( m_hsv, m_hue, 0, 0, 0 );//分离H分量
		//m_trackObject=-1;
		if (m_trackObject==0)//如果需要跟踪的物体还没有进行属性提取，则进行选取框类的图像属性提取
		{
			float max_val = 0.f;
			cvSetImageROI( m_hue, m_selectedArea);//设置原选择框为ROI
			cvSetImageROI( m_mask, m_selectedArea );//设置掩膜板选择框为ROI
			cvCalcHist( &m_hue, m_hist, 0, m_mask );//得到选择框内且满足掩膜板内的直方图
			cvGetMinMaxHistValue( m_hist, 0, &max_val, 0, 0 );
			cvConvertScale( m_hist->bins, m_hist->bins, max_val ? 255. / max_val : 0.5, 0 );// 对直方图的数值转为0~255
			cvResetImageROI( m_hue );//去除ROI
			cvResetImageROI( m_mask );//去除ROI

			m_trackWindow=m_selectedArea;
			m_trackObject=1;//置track_object为1,表明属性提取完成

		}
		cvCalcBackProject( &m_hue, m_backProject, m_hist );//计算hue的反向投影图
		cvAnd( m_backProject, m_mask, m_backProject, 0 );//得到掩膜内的反向投影
		if(cvCamShift( m_backProject, m_trackWindow,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1.1 ),
			&m_trackComp, &m_trackBox )<=1)//使用MeanShift算法对backproject中的内容进行搜索,返回跟踪结果
		return false;
			m_trackWindow = m_trackComp.rect;//得到跟踪结果的矩形框
		//if( backproject_mode )
			cvCvtColor( m_backProject, m_curImage, CV_GRAY2BGR );
		if( m_curImage->origin )
				m_trackBox.angle = -m_trackBox.angle;
		cvEllipseBox( m_curImage, m_trackBox, CV_RGB(255,0,0), 3, CV_AA, 0 );

	}
	return true;
}