#include "stdafx.h"
//#include "Hik_JListVideoProcessDlg.h"
#include "VideoProcess.h"
#include "CvvImage.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv2/>

//CvVideoWriter *writertest = 0;
//int isColor = 1;
//int fps     = 25;  // or 30
//int frameW  = 704; // 744 for firewire cameras
//int frameH  = 576; // 480 for firewire cameras

VideoProcess::VideoProcess(void)
{
	m_nWidth=0;
	m_nHeight=0;
	m_nSize=0;
	m_nImageNumber=0;
	m_nImageNumber2=0;
	m_nImageNumber3=0;
	m_pCwnd=NULL;//显示的指针
	m_pVideoBuffer=NULL;//数据缓冲区
	m_pFrountImage = NULL;
	m_pCurrentImage = NULL;
	//m_pLastImage = NULL;
	m_pBackGroundImage = NULL;
	//开辟图像空间
	//m_pFrountImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//记得释放内存
	//m_pCurrentImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );
	//m_pLastImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );
	//m_pBackGroundImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );

	//m_pFrountImage->origin = 0;
	//m_pCurrentImage->origin = 0;
	//m_pLastImage->origin = 0;
	//m_pBackGroundImage->origin = 0;
}


VideoProcess::~VideoProcess(void)
{
	if (m_pBackGroundImage)
	{
		cvReleaseImage(&m_pBackGroundImage);
	}
	if (m_pCurrentImage)
	{
		cvReleaseImage(&m_pCurrentImage);
	}
	if (m_pFrountImage)
	{
		cvReleaseImage(&m_pFrountImage);
	}
}
bool VideoProcess::Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer, CWnd  * t_pThis )
{
	//复制传递参数，图像尺寸
	m_nWidth = nWidth;		
	m_nHeight = nHeight;
	m_nSize = nSize;
	m_pVideoBuffer = t_VideoDataBuffer;
	m_pCwnd = t_pThis;
	//开辟图像空间
	//m_pFrountImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );
	//m_pCurrentImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );
	//m_pLastImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );
	//m_pBackGroundImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 3 );

	//m_pFrountImage->origin = 0;
	//m_pCurrentImage->origin = 0;
	//m_pLastImage->origin = 0;
	//m_pBackGroundImage->origin = 0;
	
	return 1;
}
bool VideoProcess::Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer, GaussTargetBuffer *t_GaussBuffer,CWnd  * t_pThis )//初始化处理模块
{
	m_nWidth = nWidth;		
	m_nHeight = nHeight;
	m_nSize = nSize;
	m_pVideoBuffer = t_VideoDataBuffer;
	m_pGaussBuffer=t_GaussBuffer;
	m_pCwnd = t_pThis;
	return true;
}
bool VideoProcess::Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer,VideoDataBuffer * t_VideoDataBuffer2, GaussTargetBuffer *t_GaussBuffer,GaussTargetBuffer *t_GaussBuffer2,CWnd  * t_pThis ,bool openNetType)//重载初始化处理模块2
{
	
		m_nWidth = nWidth;		
		m_nHeight = nHeight;
		m_nSize = nSize;
		m_pVideoBuffer = t_VideoDataBuffer;
		m_pVideoBuffer2 = t_VideoDataBuffer2;
		m_pGaussBuffer= t_GaussBuffer;
		m_pGaussBuffer2 = t_GaussBuffer2;
		m_pCwnd = t_pThis;
		m_openType=openNetType;

	
	return true;
}
bool  VideoProcess::Initiate( int nWidth, int nHeight, int nSize, VideoDataBuffer * t_VideoDataBuffer,VideoDataBuffer * t_VideoDataBuffer2, VideoDataBuffer * t_VideoDataBuffer3,
			  GaussTargetBuffer *t_GaussBuffer,GaussTargetBuffer *t_GaussBuffer2,GaussTargetBuffer *t_GaussBuffer3,CWnd  * t_pThis ,bool openNetType)	//重载初始化处理模块3
{
	m_nWidth = nWidth;		
	m_nHeight = nHeight;
	m_nSize = nSize;
	m_pVideoBuffer = t_VideoDataBuffer;
	m_pVideoBuffer2 = t_VideoDataBuffer2;
	m_pVideoBuffer3 = t_VideoDataBuffer3;
	m_pGaussBuffer= t_GaussBuffer;
	m_pGaussBuffer2 = t_GaussBuffer2;
	m_pGaussBuffer3 = t_GaussBuffer3;
	m_pCwnd = t_pThis;
	m_openType=openNetType;


	return true;
}
bool VideoProcess::ShowVideoRGB()
{
	CRect rect;
	CDC* l_PostScreen=m_pCwnd->GetDC();
	HDC l_Hdc=l_PostScreen->GetSafeHdc();
	m_pCwnd->GetClientRect(&rect);
	unsigned int frameIndex=0;
	int videoDeep=m_pVideoBuffer->GetDeep();
	CvvImage target;
	BYTE *data=NULL;
	data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
	/*writertest=cvCreateVideoWriter("outlist.avi",-1,
		fps,cvSize(frameW,frameH),isColor);*/
	MSG msg;msg.message=0;//传送的消息
	//GetMessage(&msg,0,0,0);
	//if(msg.message == WM_QUIT)
	//	 AfxEndThread(0);//如果收到信息退出线程
	//GetMessageA(&msg,NULL,0,0);
	//cvNamedWindow("zcx1",1);
	//摄像头标定变量
	IplImage *mapx=nullptr;IplImage *mapy=nullptr;CvMat *intrinsic;CvMat *distortion ;
	while (true)
	{
		if(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bWriting==false)
		{   
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=true;
			
			YV12_to_RGB24(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);
			
			IplImage *test=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test,data,m_nWidth*3);

			//CvRect a,b;a.x=0,b.x=0;
			// cvWriteFrame(writertest,test); 
			//camshift处理
			//cvNamedWindow("zcx",1);
			static int kkk=0;
			//标定修正图像
			
			
			if (kkk==0)
			{
				intrinsic = (CvMat*)cvLoad("系数\\Intrinsics.xml");
				distortion = (CvMat*)cvLoad("系数\\Distortion.xml");
				mapx = cvCreateImage(cvGetSize(test),IPL_DEPTH_32F,1);
				mapy = cvCreateImage(cvGetSize(test),IPL_DEPTH_32F,1);
				cvInitUndistortMap(intrinsic,distortion,mapx,mapy);
				cvReleaseMat(&distortion );
				cvReleaseMat(&intrinsic );
				kkk++;
			}
			////IplImage *t = cvCloneImage(test);

			cvRemap(test,test,mapx,mapy);
			//cvReleaseImage(&t);
		/*	CString dd;
			dd.Format("%d",kkk++);*/
			/*cvShowImage("zcx1",test);
			cvWaitKey(1000);*/

			//camshift处理
			//m_camShift.CamShiftProcess(test,a,b);
			//cvDestroyWindow("zcx");
			target.CopyOf(test);
			target.DrawToHDC(l_Hdc,rect);//cvvimage显示
			
			//释放资源
			
			cvReleaseImageHeader(&test);
			
			target.Destroy();
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=false;
			frameIndex=(frameIndex+1)%videoDeep;
			if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
			{
				//cvReleaseImage(&m_pBackGroundImage);
				//cvDestroyWindow("zcx");
				delete data;
				data=nullptr;
				if (mapx)
				{
					cvReleaseImage(&mapx);
					cvReleaseImage(&mapy);
					kkk=0;
				}
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}
			
		}

	}
	return true;
}

bool VideoProcess::YV12_to_RGB24(unsigned char* pYV12, unsigned char* pRGB24, int iWidth, int iHeight)
{
	//unsigned char* pRGB24x;
	if(!pYV12 || !pRGB24)
		return false;
	const long nYLen = long(iHeight * iWidth);
	const int nHfWidth = (iWidth>>1);
	if(nYLen<1 || nHfWidth<1) 
		return false;
	unsigned char* yData = pYV12;//Y数据部分
	unsigned char* vData = &yData[nYLen];//V数据部分
	unsigned char* uData = &vData[nYLen>>2];//U数据部分
	if(!uData || !vData)
		return false;
	
	int rgb[3];
	int i, j, m, n, x, y;
	m = -iWidth;
	n = -nHfWidth;
	for(y=0; y < iHeight; y++)//zcx
	{
		for(x=0; x < iWidth; x++)
		{
			i = y*iWidth+x;//每个像素点
			j = (y/2*iWidth/2+x/2);//每四个需要一个,注意格式
			rgb[2] = int(yData[i] + /*1.370705*/1.13983 * (vData[j] - 128)); // r分量值
			rgb[1] = int(yData[i] - /*0.698001*/ 0.39465* (uData[j] - 128)  - /*0.703125 */0.58060* (vData[j] - 128)); // g分量值
			rgb[0] = int(yData[i] + /*1.732446*/ 2.03211* (uData[j] - 128)); // b分量值
			/*j = nYLen - iWidth - m + x;
			i = (j<<1) + j;*/
			i=i*3;//每个像素保存3个字节
			for(j=0; j<3; j++)
			{
				if(rgb[j]>=0 && rgb[j]<=255)
					pRGB24[i + j] = rgb[j];
				else
					pRGB24[i + j] = (rgb[j] < 0) ? 0 : 255;
			}
		}
	}
	return true;
}
bool VideoProcess::ShowGaussBG()
{
	CRect rect;
	CDC* l_PostScreen=m_pCwnd->GetDC();
	HDC l_Hdc=l_PostScreen->GetSafeHdc();
	m_pCwnd->GetClientRect(&rect);
	unsigned int frameIndex=0;//索引

	int videoDeep=m_pVideoBuffer->GetDeep();
	//int GaussBufferDeep=m_pGaussBuffer->GetDeep();//高斯目标的深度

	CvvImage target;
	BYTE *data=NULL;//转换后数据

	MSG msg;msg.message=0;//传送的消息

	m_pFrountImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//申请前景图片空间
	m_pFrountImage->origin=0;//表示顶左结构
	memset(m_pFrountImage->imageData,0,m_nWidth*m_nHeight);
	//第一次处理,初始化混合高斯模型，不对图像进行处理
	while(frameIndex == 0 )
	{
		if ( m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bWriting == false )		//如果该帧当前不在接收数据中,则
		{		
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = true;		//设置标志位,禁止写入
			//采用背景差法
			//InitProcessRGB( frameIndex );
			data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
			YV12_to_RGB24(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);			
			IplImage *test=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test,data,m_nWidth*3);
			//图像处理
			cvSmooth(test,test,CV_GAUSSIAN);//高斯滤波
			m_MixGauss.Init(test,m_pBackGroundImage,m_pFrountImage);//初始化高斯背景
			m_pBackGroundImage=cvCloneImage(test);//->imageData);
			//cvvimage显示		
			target.CopyOf(m_pFrountImage);
			target.DrawToHDC(l_Hdc,rect);

			//释放资源
			delete data;
			cvReleaseImageHeader(&test);
			data=nullptr;
			target.Destroy();
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=false;
			frameIndex=(frameIndex+1)%videoDeep;
			if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
			{
				//cvReleaseVideoWriter(&writertest);
				cvReleaseImage(&m_pBackGroundImage);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}
			//m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = false;		//复位标志位，开放写入
			//frameIndex++;
		}

		Sleep(100);
	}
	while (true)
	{
		if(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bWriting==false)
		{   
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=true;
			data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
			YV12_to_RGB24(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);
			//图像转换
			IplImage *test=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test,data,m_nWidth*3);
			//图像处理
			// cvWriteFrame(writertest,test);   
			int offSet=m_MixGauss.Update1(test,m_pBackGroundImage,m_nImageNumber);//得到背景
			m_MixGauss.CountFrImage1(test,m_pFrountImage,offSet);//得到前景
			//cvvimage显示
			m_nImageNumber++;m_nImageNumber%=10000;
			cvDilate( m_pFrountImage, m_pFrountImage, 0, 1 );//膨胀腐蚀
			cvErode( m_pFrountImage, m_pFrountImage, 0, 3 );
			cvDilate( m_pFrountImage, m_pFrountImage, 0, 2 );
			//target.CopyOf(m_pBackGroundImage);//显示背景
			target.CopyOf(m_pFrountImage);//显示前景
			target.DrawToHDC(l_Hdc,rect);

			//释放资源
			delete data;
			cvReleaseImageHeader(&test);
			data=nullptr;
			target.Destroy();
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=false;
			frameIndex=(frameIndex+1)%videoDeep;
			if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))//是否终止进程
			{
				cvReleaseImage(&m_pBackGroundImage);
				cvReleaseImage(&m_pFrountImage);
				//cvReleaseVideoWriter(&writertest);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}

		}

	}
	return true;
}
//保存高斯目标函数，目前有处理部分 通道一
bool VideoProcess::SaveGaussResults()
{
	CRect rect;//显示模块
	CDC* l_PostScreen=m_pCwnd->GetDC();
	HDC l_Hdc=l_PostScreen->GetSafeHdc();
	m_pCwnd->GetClientRect(&rect);

	FILE *l_file;//保存警报信息
	l_file=fopen("..\\Log\\warning.txt","at+");

	//FILE *datafile;//保存文件

	unsigned int frameIndex=0;//公用的索引值unsigned int writeTargetLocation=0;
	m_pGaussBuffer->writeTargetLocation=0;
	//m_pGaussBuffer2->writeTargetLocation=0;
	int videoDeep=m_pVideoBuffer->GetDeep();//深度目前设置是一样的
	int GaussBufferDeep=m_pGaussBuffer->GetDeep();//高斯目标的深度 暂时也是一样的
	m_pFrountImage = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//申请前景图片空间
	m_pFrountImage->origin=0;//表示顶左结构
	//m_pFrountImage2 = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//申请前景2图片空间
	//m_pFrountImage2->origin=0;//表示顶左结构
	MSG msg;//消息信息
	CvvImage target;
	BYTE *data=NULL;
	data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
	//第一次处理,初始化混合高斯模型，不对图像进行处理
	if(frameIndex == 0 )
	{
		//通道一
		if ( m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bWriting == false )		//如果该帧当前不在接收数据中,则
		{		
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = true;		//设置标志位,禁止写入
			//InitProcessRGB( frameIndex );		
			YV12_to_RGB24(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);			
			IplImage *test=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test,data,m_nWidth*3);

			//cvSmooth(test,test,CV_GAUSSIAN);//高斯滤波
			m_MixGauss.Init(test,m_pBackGroundImage,m_pFrountImage);//初始化高斯背景
			m_pBackGroundImage=cvCloneImage(test);
			//m_pBackGroundImage=cvCloneImage(test);//->imageData);
			//cvvimage显示
			
			/*target.CopyOf(m_pBackGroundImage);
			target.DrawToHDC(l_Hdc,rect);*/

			//释放资源		
			cvReleaseImageHeader(&test);
			
			//target.Destroy();
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=false;
			//frameIndex=(frameIndex+1)%videoDeep;
			if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
			{
				delete data;
				data=nullptr;
				//cvReleaseVideoWriter(&writertest);
				cvReleaseImage(&m_pFrountImage);
				//cvReleaseImage(&m_pFrountImage2);
				cvReleaseImage(&m_pBackGroundImage);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}
			//m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = false;		//复位标志位，开放写入
			//frameIndex++;
		}
		////通道二
		//if ( m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bWriting == false &&m_openType)		//如果该帧当前不在接收数据中,或者是网络流
		//{		
		//	m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing = true;		//设置标志位,禁止写入
		//	//InitProcessRGB( frameIndex );		
		//	YV12_to_RGB24(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);			
		//	IplImage *test2=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
		//	cvSetData(test2,data,m_nWidth*3);
		//	//cvSmooth(test,test,CV_GAUSSIAN);//高斯滤波
		//	m_MixGauss2.Init(test2,m_pBackGroundImage2,m_pFrountImage2);//初始化高斯背景
		//	m_pBackGroundImage2=cvCloneImage(test2);
		//	//m_pBackGroundImage=cvCloneImage(test);//->imageData);
		//	//cvvimage显示
		//
		//	/*target.CopyOf(m_pBackGroundImage2);
		//	target.DrawToHDC(l_Hdc,rect);*/
		//
		//	//释放资源		
		//	cvReleaseImageHeader(&test2);
		//
		//	//target.Destroy();
		//	m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=false;
		//	//frameIndex=(frameIndex+1)%videoDeep;
		//	if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
		//	{
		//		delete data;
		//		data=nullptr;
		//		//cvReleaseVideoWriter(&writertest);
		//		cvReleaseImage(&m_pFrountImage);
		//		cvReleaseImage(&m_pFrountImage2);
		//		cvReleaseImage(&m_pBackGroundImage);
		//		cvReleaseImage(&m_pBackGroundImage2);
		//		AfxEndThread(0);//如果收到信息退出线程
		//		return true;
		//	}
		//	//m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = false;		//复位标志位，开放写入
		//	//frameIndex++;
		//}
		frameIndex=(frameIndex+1)%videoDeep;
		Sleep(10);
	}
	while (true)
	{
		//通道一
		if(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bWriting==false)
		{   
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=true;
			//data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
			YV12_to_RGB24(m_pVideoBuffer->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);

			IplImage *test=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test,data,m_nWidth*3);
			//感兴趣区域
			//CvRect rectp;rectp.x=0,rectp.y=0,rectp.width=352,rectp.height=288;
			//cvSetImageROI(test,rectp);
			int offSet=m_MixGauss.Update1(test,m_pBackGroundImage,m_nImageNumber);//得到背景

			//保存亮度变化明显数据
			//if (offSet>25)
			//{
			//	IplImage *grayBK=cvCreateImage(cvGetSize(m_pBackGroundImage),8,1);
			//	IplImage *grayCRT=cvCreateImage(cvGetSize(test),8,1);
			//	cvCvtColor(m_pBackGroundImage,grayBK,CV_BGR2GRAY);
			//	cvCvtColor(test,grayCRT,CV_BGR2GRAY);
			//	CvMat *l_sMat=cvCreateMat(m_nHeight,m_nWidth,CV_32FC1);
			//	CvMat *l_UcMat=cvCreateMat(m_nHeight,m_nWidth,CV_8UC1);
			//	uchar *graycrtData=(uchar*)(grayCRT->imageData),*graybkData=(uchar*)(grayBK->imageData);
			//	for (int i=0;i<m_nHeight;i++)
			//	{
			//		for (int j=0;j<m_nWidth;j++)
			//		{
			//			l_sMat->data.fl[i*m_nWidth+j]=float(graycrtData[i*m_nWidth+j]-graybkData[i*m_nWidth+j]);
			//			l_UcMat->data.ptr[i*m_nWidth+j]=abs(graycrtData[i*m_nWidth+j]-graybkData[i*m_nWidth+j]);
			//		}
			//	}
 		//		//l_sMat=0;
			//	static int l_number=0;
			//	CString strlightdata,strlightdata2;
			//	strlightdata.Format("..\\LightData\\differdata%d.xml",l_number);
			//	strlightdata2.Format("..\\LightData\\differdata%d.bmp",l_number++);
			//	cv::FileStorage fs(strlightdata.GetString(),cv::FileStorage::WRITE);
			//	fs<<"flightdata"<<l_sMat;
			//	fs.release();
			//	cvSaveImage(strlightdata2,l_UcMat);
			//	cvReleaseMat(&l_sMat);
			//	cvReleaseMat(&l_UcMat);
			//	cvReleaseImage(&grayBK);
			//	cvReleaseImage(&grayCRT);
			//}

			//保存亮度变化明显数据
			/*if (offSet>25)
			{
			AnalyzeLightData(test,m_pBackGroundImage);
			}*/
			m_MixGauss.CountFrImage1(test,m_pFrountImage,offSet);//得到前景
			//m_nImageNumber++;m_nImageNumber%=10000;

			//保存梯度数据
			//SaveAnalylizeData(test,m_pBackGroundImage);
			//保存当前和背景图像
			//static int s_number1=0;
			//CString strbk1,strcrt1;//="save";str=str+num;
			//strbk1.Format("..\\BKImage\\imageBK%d.bmp",s_number1);
			//strcrt1.Format("..\\CrtImage\\imageCRT%d.bmp",s_number1++);
			//cvSaveImage(strbk1,m_pBackGroundImage);
			//cvSaveImage(strcrt1,test);

			//保存高斯结果到目标缓冲池
			if (m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_bProcessing==true)//如果正在处理，写入下一帧
			{
				m_pGaussBuffer->writeTargetLocation=(m_pGaussBuffer->writeTargetLocation+1)%GaussBufferDeep;
			}

			m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_bWriting=true;//设置正在处理，禁止处理
			m_pGaussBuffer->m_gaussTargetBuffer[(m_pGaussBuffer->writeTargetLocation-1+GaussBufferDeep)%GaussBufferDeep].m_bWriting=false;//恢复前一帧
			cvCopy(m_pBackGroundImage,m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_pBackGroundImage);//写入背景图
			cvCopy(test,m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_pCurrentImage);//写入当前图
			cvErode(m_pFrountImage,m_pFrountImage,0,2);//腐蚀
			cvDilate(m_pFrountImage,m_pFrountImage,0,2);//膨胀
			cvCopy(m_pFrountImage,m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_pForeGroundImage);//写入前景图
			m_pGaussBuffer->writeTargetLocation = (m_pGaussBuffer->writeTargetLocation + 1)%GaussBufferDeep;

			
			//cvvimage显示
		/*	target.CopyOf(m_pFrountImage);
			target.DrawToHDC(l_Hdc,rect);*/
			//警报信息
			//IntrusionDetection(m_pFrountImage,l_file);
			//static int num=0;
			//num%=6; //记录六福图像的差值
			//if(!num)
			//{
			//	int s=0;
			//	//cvAbsDiff(m_pFrountImage,m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation-6+m_pGaussBuffer->GetDeep()].m_pForeGroundImage,m_pFrountImage);
			//	for (int i=0;i<m_nHeight/**m_nHeight*/;i++)
			//		for (int j=0;j<m_nWidth/2;j++)
			//		{
			//		
			//				if(m_pFrountImage->imageData[i*m_nWidth/2+j])s++;						
			//		}
			//	
			//
			//	if (s>m_nHeight*m_nWidth/16/2)
			//	{
			//		 
			//		char tmp[64];					
			//		LPNET_DVR_TIME mytime=new NET_DVR_TIME;mytime->dwDay=0;
			//		//NET_DVR_GetDVRConfig(0,NET_DVR_GET_TIMECFG,0xFFFFFFFF,mytime,sizeof(NET_DVR_TIME),LPDWORD(mytime));//接受时间
			//		if (!NET_DVR_GetDVRConfig(0,NET_DVR_GET_TIMECFG,0xFFFFFFFF,mytime,sizeof(NET_DVR_TIME),LPDWORD(mytime)))
			//		{
			//			time_t t = time(0);
			//			strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天：",localtime(&t) ); 
			//		}
			//		else
			//		{
			//			sprintf(tmp,"%d/%d/%d %d:%d:%d: ",mytime->dwYear,mytime->dwMonth,mytime->dwDay,mytime->dwHour,mytime->dwMinute,mytime->dwSecond);
			//		}
			//		//strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天：",localtime(&t) ); 
			//		
			//		fwrite(tmp,strlen(tmp),1,l_file);				
			//		char l_c[]="有人入侵！\n";		
			//		fwrite(l_c,strlen(l_c),1,l_file);
			//		delete mytime;
			//	}
			//}
			//num++;
			
			//释放资源
			//delete data;
			cvReleaseImageHeader(&test);
			//data=nullptr;
			//target.Destroy();
			m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing=false;
			
			//frameIndex=(frameIndex+1)%videoDeep;
			if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
			{
				delete data;data=nullptr;
				fclose(l_file);
				cvReleaseImage(&m_pBackGroundImage);
				//cvReleaseImage(&m_pBackGroundImage2);
				//cvReleaseVideoWriter(&writertest);
				cvReleaseImage(&m_pFrountImage);
				//cvReleaseImage(&m_pFrountImage2);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}

		}
		//通道二
		//if(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bWriting==false &&m_openType)
		//{   
		//	m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=true;
		//	//data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
		//	YV12_to_RGB24(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);
		//	IplImage *test2=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
		//	cvSetData(test2,data,m_nWidth*3);
		//	//感兴趣区域
		//	//CvRect rectp;rectp.x=0,rectp.y=0,rectp.width=352,rectp.height=288;
		//	//cvSetImageROI(test,rectp);
		//	int offSet=m_MixGauss2.Update1(test2,m_pBackGroundImage2,m_nImageNumber);//得到背景
		//	m_MixGauss2.CountFrImage1(test2,m_pFrountImage2,offSet);//得到前景
		//	
		//
		//	//保存高斯结果到目标缓冲池
		//	if (m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation].m_bProcessing==true)//如果正在处理，写入下一帧
		//	{
		//		m_pGaussBuffer2->writeTargetLocation=(m_pGaussBuffer2->writeTargetLocation+1)%GaussBufferDeep;
		//	}
		//	m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_bWriting=true;//设置正在处理，禁止处理
		//	m_pGaussBuffer2->m_gaussTargetBuffer[(m_pGaussBuffer2->writeTargetLocation-1+GaussBufferDeep)%GaussBufferDeep].m_bWriting=false;//恢复前一帧
		//	cvCopy(m_pBackGroundImage2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pBackGroundImage);//写入
		//	cvCopy(test2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pCurrentImage);//写入
		//	cvErode(m_pFrountImage2,m_pFrountImage2,0,2);//腐蚀
		//	cvDilate(m_pFrountImage2,m_pFrountImage2,0,2);//膨胀
		//	cvCopy(m_pFrountImage2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pForeGroundImage);//写入
		//	m_pGaussBuffer2->writeTargetLocation = (m_pGaussBuffer2->writeTargetLocation + 1)%GaussBufferDeep;
		//	
		//	//cvvimage显示
		//	/*target.CopyOf(m_pFrountImage2);
		//	target.DrawToHDC(l_Hdc,rect);*/
		//	
		//	cvReleaseImageHeader(&test2);
		//	//data=nullptr;
		//	//target.Destroy();
		//	m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=false;
		//	
		//	//frameIndex=(frameIndex+1)%videoDeep;
		//	if (flagThreadExit==true||PeekMessage(&msg,0,0,0,0))
		//	{
		//		delete data;
		//		data=nullptr;
		//		fclose(l_file);
		//		cvReleaseImage(&m_pBackGroundImage);
		//		cvReleaseImage(&m_pBackGroundImage2);
		//		//cvReleaseVideoWriter(&writertest);
		//		cvReleaseImage(&m_pFrountImage);
		//		cvReleaseImage(&m_pFrountImage2);
		//		AfxEndThread(0);//如果收到信息退出线程
		//		return true;
		//	}

		//}
		m_nImageNumber++;
		m_nImageNumber%=10000;
		frameIndex=(frameIndex+1)%videoDeep;
	}
	return true;
}
//保存高斯模型通道2
bool VideoProcess::SaveGaussResults2()
{
	//CRect rect;//显示模块
	//CDC* l_PostScreen=m_pCwnd->GetDC();
	//HDC l_Hdc=l_PostScreen->GetSafeHdc();
	//m_pCwnd->GetClientRect(&rect);

	//FILE *l_file;//保存警报信息
	//l_file=fopen("..\\Log\\warning.txt","at+");

	//FILE *datafile;//保存文件

	unsigned int frameIndex=0;//公用的索引值unsigned int writeTargetLocation=0;
	
	m_pGaussBuffer2->writeTargetLocation=0;
	int videoDeep=m_pVideoBuffer->GetDeep();//深度目前设置是一样的
	int GaussBufferDeep=m_pGaussBuffer->GetDeep();//高斯目标的深度 暂时也是一样的
	m_pFrountImage2 = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//申请前景2图片空间
	m_pFrountImage2->origin=0;//表示顶左结构
	MSG msg;//消息信息
	CvvImage target;
	BYTE *data=NULL;
	data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
	//第一次处理,初始化混合高斯模型，不对图像进行处理
	if(frameIndex == 0 )
	{
		
		//通道二
		if ( m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bWriting == false /*&&m_openType*/)		//如果该帧当前不在接收数据中,或者是网络流
		{		
			m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing = true;		//设置标志位,禁止写入
			//InitProcessRGB( frameIndex );		
			YV12_to_RGB24(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);			
			IplImage *test2=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test2,data,m_nWidth*3);

			//cvSmooth(test,test,CV_GAUSSIAN);//高斯滤波
			m_MixGauss2.Init(test2,m_pBackGroundImage2,m_pFrountImage2);//初始化高斯背景
			m_pBackGroundImage2=cvCloneImage(test2);
			//m_pBackGroundImage=cvCloneImage(test);//->imageData);
			//cvvimage显示

			/*target.CopyOf(m_pBackGroundImage2);
			target.DrawToHDC(l_Hdc,rect);*/

			//释放资源		
			cvReleaseImageHeader(&test2);

			//target.Destroy();
			m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=false;
			//frameIndex=(frameIndex+1)%videoDeep;
			if (/*flagThreadExit==true||*/PeekMessage(&msg,0,0,0,0))
			{
				delete data;
				data=nullptr;
				//cvReleaseVideoWriter(&writertest);
				
				cvReleaseImage(&m_pFrountImage2);
				
				cvReleaseImage(&m_pBackGroundImage2);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}
			//m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = false;		//复位标志位，开放写入
			//frameIndex++;
		}
		frameIndex=(frameIndex+1)%videoDeep;
		Sleep(10);
	}
	while (true)
	{
		
		//通道二
		if(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bWriting==false /*&&m_openType*/)
		{   
			m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=true;
			//data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
			YV12_to_RGB24(m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);

			IplImage *test2=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test2,data,m_nWidth*3);
			//感兴趣区域
			//CvRect rectp;rectp.x=0,rectp.y=0,rectp.width=352,rectp.height=288;
			//cvSetImageROI(test,rectp);
			int offSet=m_MixGauss2.Update1(test2,m_pBackGroundImage2,m_nImageNumber2);//得到背景

	
			m_MixGauss2.CountFrImage1(test2,m_pFrountImage2,offSet);//得到前景
			
		
			//保存高斯结果到目标缓冲池
			if (m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_bProcessing==true)//如果正在处理，写入下一帧
			{
				m_pGaussBuffer2->writeTargetLocation=(m_pGaussBuffer2->writeTargetLocation+1)%GaussBufferDeep;
			}

			m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_bWriting=true;//设置正在处理，禁止处理
			m_pGaussBuffer2->m_gaussTargetBuffer[(m_pGaussBuffer2->writeTargetLocation-1+GaussBufferDeep)%GaussBufferDeep].m_bWriting=false;//恢复前一帧
			cvCopy(m_pBackGroundImage2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pBackGroundImage);//写入
			cvCopy(test2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pCurrentImage);//写入
			cvErode(m_pFrountImage2,m_pFrountImage2,0,2);//腐蚀
			cvDilate(m_pFrountImage2,m_pFrountImage2,0,2);//膨胀
			cvCopy(m_pFrountImage2,m_pGaussBuffer2->m_gaussTargetBuffer[m_pGaussBuffer2->writeTargetLocation].m_pForeGroundImage);//写入
			m_pGaussBuffer2->writeTargetLocation = (m_pGaussBuffer2->writeTargetLocation + 1)%GaussBufferDeep;

			
			//cvvimage显示
			/*target.CopyOf(m_pFrountImage2);
			target.DrawToHDC(l_Hdc,rect);*/
			
			cvReleaseImageHeader(&test2);
		
			//target.Destroy();
			m_pVideoBuffer2->m_VideoBuffer[frameIndex].m_bProcessing=false;
			
			//frameIndex=(frameIndex+1)%videoDeep;
			if (/*flagThreadExit==true||*/PeekMessage(&msg,0,0,0,0))
			{
				delete data;
				data=nullptr;
				//fclose(l_file);
				//cvReleaseImage(&m_pBackGroundImage);
				cvReleaseImage(&m_pBackGroundImage2);
				//cvReleaseImage(&m_pFrountImage);
				cvReleaseImage(&m_pFrountImage2);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}

		}
		m_nImageNumber2++;
		m_nImageNumber2%=10000;
		frameIndex=(frameIndex+1)%videoDeep;
	}
	return true;
}
//保存高斯模型通道3
bool VideoProcess::SaveGaussResults3()
{
	CRect rect;//显示模块
	CDC* l_PostScreen=m_pCwnd->GetDC();
	HDC l_Hdc=l_PostScreen->GetSafeHdc();
	m_pCwnd->GetClientRect(&rect);

	//FILE *l_file;//保存警报信息
	//l_file=fopen("..\\Log\\warning.txt","at+");

	//FILE *datafile;//保存文件

	unsigned int frameIndex=0;//公用的索引值unsigned int writeTargetLocation=0;
	
	m_pGaussBuffer3->writeTargetLocation=0;
	int videoDeep=m_pVideoBuffer->GetDeep();//深度目前设置是一样的
	int GaussBufferDeep=m_pGaussBuffer->GetDeep();//高斯目标的深度 暂时也是一样的
	m_pFrountImage3 = cvCreateImage( cvSize(m_nWidth, m_nHeight), IPL_DEPTH_8U, 1 );//申请前景2图片空间
	m_pFrountImage3->origin=0;//表示顶左结构
	MSG msg;//消息信息
	CvvImage target;
	BYTE *data=NULL;
	data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
	//第一次处理,初始化混合高斯模型，不对图像进行处理
	if(frameIndex == 0 )
	{
		
		//通道三
		if ( m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bWriting == false &&m_openType)		//如果该帧当前不在接收数据中,或者是网络流
		{		
			m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bProcessing = true;		//设置标志位,禁止写入
			//InitProcessRGB( frameIndex );		
			YV12_to_RGB24(m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);			
			IplImage *test3=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test3,data,m_nWidth*3);

			//cvSmooth(test,test,CV_GAUSSIAN);//高斯滤波
			m_MixGauss3.Init(test3,m_pBackGroundImage3,m_pFrountImage3);//初始化高斯背景
			m_pBackGroundImage3=cvCloneImage(test3);
			//m_pBackGroundImage=cvCloneImage(test);//->imageData);
			//cvvimage显示

			target.CopyOf(m_pBackGroundImage3);
			target.DrawToHDC(l_Hdc,rect);

			//释放资源		
			cvReleaseImageHeader(&test3);

			target.Destroy();
			m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bProcessing=false;
			//frameIndex=(frameIndex+1)%videoDeep;
			if (/*flagThreadExit==true||*/PeekMessage(&msg,0,0,0,0))
			{
				delete data;
				data=nullptr;
				//cvReleaseVideoWriter(&writertest);
				
				cvReleaseImage(&m_pFrountImage3);
				
				cvReleaseImage(&m_pBackGroundImage3);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}
			//m_pVideoBuffer->m_VideoBuffer[frameIndex].m_bProcessing = false;		//复位标志位，开放写入
			//frameIndex++;
		}
		frameIndex=(frameIndex+1)%videoDeep;
		Sleep(10);
	}
	while (true)
	{
		
		//通道3
		if(m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bWriting==false &&m_openType)
		{   
			m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bProcessing=true;
			//data=new BYTE[m_nWidth*m_nHeight*3];//申请转换暂存空间
			YV12_to_RGB24(m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_pImage,data,m_nWidth,m_nHeight);

			IplImage *test3=cvCreateImageHeader(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U,3);
			cvSetData(test3,data,m_nWidth*3);
			//感兴趣区域
			//CvRect rectp;rectp.x=0,rectp.y=0,rectp.width=352,rectp.height=288;
			//cvSetImageROI(test,rectp);
			int offSet=m_MixGauss3.Update1(test3,m_pBackGroundImage3,m_nImageNumber3);//得到背景

	
			m_MixGauss3.CountFrImage1(test3,m_pFrountImage3,offSet);//得到前景
			
		
			//保存高斯结果到目标缓冲池
			if (m_pGaussBuffer3->m_gaussTargetBuffer[m_pGaussBuffer3->writeTargetLocation].m_bProcessing==true)//如果正在处理，写入下一帧
			{
				m_pGaussBuffer3->writeTargetLocation=(m_pGaussBuffer3->writeTargetLocation+1)%GaussBufferDeep;
			}

			m_pGaussBuffer3->m_gaussTargetBuffer[m_pGaussBuffer3->writeTargetLocation].m_bWriting=true;//设置正在处理，禁止处理
			m_pGaussBuffer3->m_gaussTargetBuffer[(m_pGaussBuffer3->writeTargetLocation-1+GaussBufferDeep)%GaussBufferDeep].m_bWriting=false;//恢复前一帧
			cvCopy(m_pBackGroundImage3,m_pGaussBuffer3->m_gaussTargetBuffer[m_pGaussBuffer3->writeTargetLocation].m_pBackGroundImage);//写入
			cvCopy(test3,m_pGaussBuffer3->m_gaussTargetBuffer[m_pGaussBuffer3->writeTargetLocation].m_pCurrentImage);//写入
			cvErode(m_pFrountImage3,m_pFrountImage3,0,2);//腐蚀
			cvDilate(m_pFrountImage3,m_pFrountImage3,0,2);//膨胀
			cvCopy(m_pFrountImage3,m_pGaussBuffer3->m_gaussTargetBuffer[m_pGaussBuffer3->writeTargetLocation].m_pForeGroundImage);//写入
			m_pGaussBuffer3->writeTargetLocation = (m_pGaussBuffer3->writeTargetLocation + 1)%GaussBufferDeep;

			
			//cvvimage显示
			target.CopyOf(m_pFrountImage3);
			target.DrawToHDC(l_Hdc,rect);
			
			cvReleaseImageHeader(&test3);
		
			target.Destroy();
			m_pVideoBuffer3->m_VideoBuffer[frameIndex].m_bProcessing=false;
			
			//frameIndex=(frameIndex+1)%videoDeep;
			if (/*flagThreadExit==true||*/PeekMessage(&msg,0,0,0,0))
			{
				delete data;
				data=nullptr;
				//fclose(l_file);
				//cvReleaseImage(&m_pBackGroundImage);
				cvReleaseImage(&m_pBackGroundImage3);
				//cvReleaseImage(&m_pFrountImage);
				cvReleaseImage(&m_pFrountImage3);
				AfxEndThread(0);//如果收到信息退出线程
				return true;
			}

		}
		m_nImageNumber3++;
		m_nImageNumber3%=10000;
		frameIndex=(frameIndex+1)%videoDeep;
	}
	return true;
}
//警报信息
bool VideoProcess::IntrusionDetection(IplImage *pForeImage,FILE *pfile)//判断入侵信息
{
	static int num=0,count=0;
	num%=6; //记录六福图像的差值
	if(!num)
	{
		int s=0;
		//cvAbsDiff(m_pFrountImage,m_pGaussBuffer->m_gaussTargetBuffer[m_pGaussBuffer->writeTargetLocation-6+m_pGaussBuffer->GetDeep()].m_pForeGroundImage,m_pFrountImage);
		for (int i=0;i<m_nHeight/**m_nHeight*/;i++)
			for (int j=0;j<m_nWidth/2;j++)
			{

				if(pForeImage->imageData[i*m_nWidth/2+j])s++;						
			}

			if (s>m_nHeight*m_nWidth/16/2)
			{
				count++;
				if(count>1)
				{
					char tmp[64];					
					LPNET_DVR_TIME mytime=new NET_DVR_TIME;mytime->dwDay=0;
					//NET_DVR_GetDVRConfig(0,NET_DVR_GET_TIMECFG,0xFFFFFFFF,mytime,sizeof(NET_DVR_TIME),LPDWORD(mytime));//接受时间
					if (!NET_DVR_GetDVRConfig(0,NET_DVR_GET_TIMECFG,0xFFFFFFFF,mytime,sizeof(NET_DVR_TIME),LPDWORD(mytime)))
					{
						time_t t = time(0);
						strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天：",localtime(&t) ); 
					}
					else
					{
						sprintf(tmp,"%d/%d/%d %d:%d:%d: ",mytime->dwYear,mytime->dwMonth,mytime->dwDay,mytime->dwHour,mytime->dwMinute,mytime->dwSecond);
					}
					//strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天：",localtime(&t) ); 

					fwrite(tmp,strlen(tmp),1,pfile);				
					char l_c[]="有人入侵！\n";		
					fwrite(l_c,strlen(l_c),1,pfile);
					delete mytime;
					count=0;
				}
				
			}
	}
	num++;
	return true;
}

bool VideoProcess::SaveAnalylizeData(IplImage *pCurrentImage,IplImage *pBackImage)//保存源图像、背景图像梯度，亮度变化
{
	IplImage *pCrtImgGrad,*pBGImgGrads,*grayCrt,*grayBK;
	static int s_number=0;
	uchar *bkData=new uchar[pBackImage->width*pBackImage->height],*crtData=new uchar[pCurrentImage->width*pCurrentImage->height];
	grayCrt=cvCreateImage(cvGetSize(pCurrentImage),IPL_DEPTH_8U,1);//申请空间
	grayBK=cvCreateImage(cvGetSize(pBackImage),IPL_DEPTH_8U,1);
	/*sobelCrt=cvCreateImage(cvGetSize(pCurrentImage),IPL_DEPTH_16S,1);
	sobelBK=cvCreateImage(cvGetSize(pBackImage),IPL_DEPTH_16S,1);
	sobelBKy=cvCreateImage(cvGetSize(pBackImage),IPL_DEPTH_16S,1);
	sobelCrty=cvCreateImage(cvGetSize(pBackImage),IPL_DEPTH_16S,1);*/
	pCrtImgGrad=cvCreateImageHeader(cvGetSize(pCurrentImage),IPL_DEPTH_8U,1);
	pBGImgGrads=cvCreateImageHeader(cvGetSize(pBackImage),IPL_DEPTH_8U,1);
	
	cvCvtColor(pCurrentImage,grayCrt,CV_BGR2GRAY);//转换为灰度图像
	cvCvtColor(pBackImage,grayBK,CV_BGR2GRAY);
	//cvNamedWindow("frame");
	//cvShowImage("frame",grayCrt);
	//cvWaitKey(1);//等待

	//cvDestroyWindow("frame");
	//cvSobel(grayBK,sobelBK,1,0,3);//梯度运算
	//cvSobel(grayCrt,sobelCrt,1,0,3);
	//cvSobel(grayBK,sobelBKy,0,1,3);
	//cvSobel(grayCrt,sobelCrty,0,1,3);


	FindGradient(grayCrt,crtData);
	FindGradient(grayBK,bkData);
	cvSetData(pCrtImgGrad,crtData,pCrtImgGrad->widthStep);
	cvSetData(pBGImgGrads,bkData,pBGImgGrads->widthStep);
	/*for (int i=0;i<pCurrentImage->height;i++)
	{
		for (int j=0;j<pCurrentImage->width;j++)
		{
			pBGImgGrads->imageData[i*pCurrentImage->width+j]=sqrt(sobelBK->imageData[i*pCurrentImage->width+j]*sobelBK->imageData[i*pCurrentImage->width+j]+
				sobelBKy->imageData[i*pCurrentImage->width+j]*sobelBKy->imageData[i*pCurrentImage->width+j]);
		}
	}

	for (int i=0;i<pCurrentImage->height;i++)
	{
		for (int j=0;j<pCurrentImage->width;j++)
		{
			pCrtImgGrad->imageData[i*pCurrentImage->width+j]=sqrt(sobelBKy->imageData[i*pCurrentImage->width+j]*sobelBKy->imageData[i*pCurrentImage->width+j]+
				sobelCrty->imageData[i*pCurrentImage->width+j]*sobelCrty->imageData[i*pCurrentImage->width+j]);
		}
	}*/
	/*IplImage *sobelBK8u=cvCreateImage(cvGetSize(sobelBK),IPL_DEPTH_8U,1);
	IplImage *sobelCRT8u=cvCreateImage(cvGetSize(sobelCrt),IPL_DEPTH_8U,1);
	cvConvertScaleAbs(sobelBK,sobelBK8u,1,0);
	cvConvertScaleAbs(sobelCrt,sobelCRT8u,1,0);*/

	CString strbk,strcrt,strcrtgray;//="save";str=str+num;
	strbk.Format("..\\GradImage\\imageBK%d.bmp",s_number);
	strcrt.Format("..\\GradImage\\imageCRT%d.bmp",s_number++);
	strcrtgray.Format("..\\GradImage\\imagecrtgray%d.bmp",s_number);
	if (s_number>650)
	{
		cvSaveImage(strcrtgray,grayCrt);
	}
	cvSaveImage(strcrt,pCrtImgGrad);
	cvSaveImage(strbk,pBGImgGrads);
	delete []bkData;delete []crtData;
	/*cvReleaseImage(&sobelBK8u);
	cvReleaseImage(&sobelCRT8u);*/
	cvReleaseImageHeader(&pCrtImgGrad);
	cvReleaseImageHeader(&pBGImgGrads);
	/*cvReleaseImage(&sobelBKy);
	cvReleaseImage(&sobelCrty);
	cvReleaseImage(&sobelBK);
	cvReleaseImage(&sobelCrt);*/
	cvReleaseImage(&grayBK);
	cvReleaseImage(&grayCrt);

	return true;
}
bool VideoProcess::FindGradient(IplImage *src,uchar *dst)
{
	uchar *pData=reinterpret_cast<uchar *>(src->imageData);
	//CvMat *mymat;mymat=cvCreateMat(src->height,src->width,CV_32FC1);
	float max=0,m;
	for (int i=1;i<src->height-1;i++)
	{
		for (int j=1;j<src->width-1;j++)
		{
			/*m=sqrt((src->imageData[(i)*src->widthStep+j+1]-src->imageData[(i)*src->widthStep+j])*(src->imageData[(i)*src->widthStep+j+1]-src->imageData[(i)*src->widthStep+j])+
			(src->imageData[(i+1)*src->widthStep+j]-src->imageData[(i)*src->widthStep+j])*(src->imageData[(i+1)*src->widthStep+j]-src->imageData[(i)*src->widthStep+j]));*/

			//dst->imageData[i*src->width+j]=sqrt((src->imageData[(i-1)*src->width+j-1]+src->imageData[(i-1)*src->width+j]+src->imageData[(i-1)*src->width+j+1]-
			//	src->imageData[(i+1)*src->width+j-1]-src->imageData[(i+1)*src->width+j]-src->imageData[(i+1)*src->width+j+1])
			//	*(src->imageData[(i-1)*src->width+j-1]+src->imageData[(i-1)*src->width+j]+src->imageData[(i-1)*src->width+j+1]-
			//	src->imageData[(i+1)*src->width+j-1]-src->imageData[(i+1)*src->width+j]-src->imageData[(i+1)*src->width+j+1])+//加另外一个偏导x

			//	((src->imageData[(i-1)*src->width+j-1]+src->imageData[(i)*src->width+j-1]+src->imageData[(i+1)*src->width+j-1]-
			//	src->imageData[(i-1)*src->width+j+1]-src->imageData[(i)*src->width+j+1]-src->imageData[(i+1)*src->width+j+1]))
			//	*((src->imageData[(i-1)*src->width+j-1]+src->imageData[(i)*src->width+j-1]+src->imageData[(i+1)*src->width+j-1]-
			//	src->imageData[(i-1)*src->width+j+1]-src->imageData[(i)*src->width+j+1]-src->imageData[(i+1)*src->width+j+1])));

			m=sqrt((pData[(i)*src->widthStep+j+1]-pData[(i)*src->widthStep+j])*(pData[(i)*src->widthStep+j+1]-pData[(i)*src->widthStep+j])+
			(pData[(i+1)*src->widthStep+j]-pData[(i)*src->widthStep+j])*(pData[(i+1)*src->widthStep+j]-pData[(i)*src->widthStep+j]));
			if (m>max)
			{
				max=m;
			}
			if(m>255)
			{
				dst[i*src->widthStep+j]=255;
				//dst->imageData[i*src->widthStep+j]=255;
			}
			else dst[i*src->widthStep+j]=m;
			//mymat->data.fl[i*src->width+j]=m;
		}
	}
	/*for (int i=1;i<src->height-1;i++)
	{
		for (int j=1;j<src->width-1;j++)
		{
			dst->imageData[i*src->width+j]=int(mymat->data.fl[i*src->width+j]/max*255);
		}
	}*/
	//cvReleaseMat(&mymat);
	return true; 
}
bool VideoProcess::AnalyzeLightData(IplImage *pCurrentImage,IplImage *pBackImage)//统计开灯亮度变化明显数据
{
		IplImage *grayBK=cvCreateImage(cvGetSize(pBackImage),8,1);
		IplImage *grayCRT=cvCreateImage(cvGetSize(pCurrentImage),8,1);
		cvCvtColor(pBackImage,grayBK,CV_BGR2GRAY);
		cvCvtColor(pCurrentImage,grayCRT,CV_BGR2GRAY);
		CvMat *l_sMat=cvCreateMat(m_nHeight,m_nWidth,CV_32FC1);
		CvMat *l_UcMat=cvCreateMat(m_nHeight,m_nWidth,CV_8UC1);
		uchar *graycrtData=(uchar*)(grayCRT->imageData),*graybkData=(uchar*)(grayBK->imageData);
		for (int i=0;i<m_nHeight;i++)
		{
			for (int j=0;j<m_nWidth;j++)
			{
				l_sMat->data.fl[i*m_nWidth+j]=float(graycrtData[i*m_nWidth+j]-graybkData[i*m_nWidth+j]);
				l_UcMat->data.ptr[i*m_nWidth+j]=abs(graycrtData[i*m_nWidth+j]-graybkData[i*m_nWidth+j]);
			}
		}
			//l_sMat=0;
		static int l_number=0;
		CString strlightdata,strlightdata2;
		strlightdata.Format("..\\LightData\\differdata%d.xml",l_number);
		strlightdata2.Format("..\\LightData\\differdata%d.bmp",l_number++);
		cv::FileStorage fs(strlightdata.GetString(),cv::FileStorage::WRITE);
		fs<<"flightdata"<<l_sMat;
		fs.release();
		cvSaveImage(strlightdata2,l_UcMat);
		cvReleaseMat(&l_sMat);
		cvReleaseMat(&l_UcMat);
		cvReleaseImage(&grayBK);
		cvReleaseImage(&grayCRT);
		return true;
}