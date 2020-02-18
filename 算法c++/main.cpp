#include"iostream"
#include"stdio.h"
#include"BorderMatting.h"
#include "GCApplication.h"
#include <opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc,char *argv[])
{
	GCApplication gcapp;//创建一个对图像操作的整体对象
//	string filename = "c:/test_blue.jpg";
	string filename = "c:/red.jpg";
	//string filename = "c:/blue.jpg";
	gcapp.color = argv[1];//黄、红、蓝
	gcapp.rfHeight = atof(argv[2]);//63、105、33
	Mat rawSrc,src;//dst特征提取后的结果
	rawSrc = imread(filename, 1);
	if (rawSrc.empty())
	{
		printf("can not load the image...\n");
		return -1;
	}
	resize(rawSrc, src, Size(rawSrc.cols*0.2, rawSrc.rows*0.2));
	gcapp.setImageAndWinName(src, "src");
	gcapp.showImage();
	/******************************HOG detector************************************************/
	gcapp.setHogDetector();
	gcapp.showImage(1);

/********************************grabCut**********************************************/
//初始方法自带接口
/*	cv::Mat mask = Mat::zeros(src.size(), CV_8UC1);//分割后的结果
	//两个临时矩阵变量，作为算法的中间变量使用
	cv::Mat bgModel, fgModel;
	cv::Rect rectangle(r.tl(),r.br());//图像的前景对象也就是矩形选中图像
	// GrabCut 分段
	cv::grabCut(src,    //输入图像
		mask,   //分段结果
		rectangle,// 包含前景的矩形 
		bgModel, fgModel, // 前景、背景
		1,        // 迭代次数
		cv::GC_INIT_WITH_RECT); // 用矩形
	
	// 得到可能是前景的像素
	//比较函数保留值为GC_PR_FGD的像素
	cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
	// 产生输出图像
	cv::Mat foreground(src.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//背景值为 GC_BGD=0，作为掩码
	src.copyTo(foreground, mask);
	imshow("foreground", foreground);
	*/
//自己编写
	gcapp.GrabCutting();
	gcapp.showImage(2);

/********************************BorderMatting**********************************************/
	gcapp.BoardMatting();
    gcapp.showImage(3);

/******************************二值化并计算像素距离************************************************/
	gcapp.measurePersonPX();//计算人物像素距离
	gcapp.measureReferencePX();//计算参照物距离
	line(src, gcapp.PA, gcapp.PB, Scalar(0, 0, 255), 3);
	line(src, gcapp.FA, gcapp.FB, Scalar(0, 0, 255), 3);
	imshow("final", src);
	double k;
	k = gcapp.maxPersonPX*1.0 / gcapp.maxRefPX;
	cout << "测量用户像素高度:" << gcapp.maxPersonPX << "参照物像素高度:" << gcapp.maxRefPX <<endl;
	cout << "比例系数k："<<k << endl;
	cout << "实际身高为：" << k*gcapp.rfHeight << endl;
	waitKey();
	return 0;
}
