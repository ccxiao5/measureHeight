#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include <iostream>
#include "GrabCut.h"
#include "BorderMAtting.h"

using namespace std;
using namespace cv;

const Scalar BLUE = Scalar(255, 0, 0); // 背景
const Scalar GREEN = Scalar(0, 255, 0);//前景
const Scalar RED = Scalar(0, 0, 255);//color of Rectangle

//将comMask值复制给binMask
static void getBinMask(const Mat& comMask, Mat& binMask)
{
	if (comMask.empty() || comMask.type() != CV_8UC1)
		CV_Error(CV_StsBadArg, "comMask is empty or has incorrect type (not CV_8UC1)");
	if (binMask.empty() || binMask.rows != comMask.rows || binMask.cols != comMask.cols)
		binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

class GCApplication
{
public:

	BorderMatting bm;
	Point PA, PB;//测量身高的极限两点
	Point FA, FB;//测量身高的极限两点
	int maxPersonPX;//身高像素最大距离
	int maxRefPX;//参照物像素最大距离
	float rfHeight;
	string color;
	void reset();
	void setImageAndWinName(const Mat& _image, const string& _winName);
	void showImage();
	void showImage(int FLAG);
	void setHogDetector();
	void GrabCutting();
	void BoardMatting();
	void setBinImage(Mat image);
	void measurePersonPX();//测量人物最大像素距离
	void  measureReferencePX();//测量参照物最大像素距离
	Mat getBdImage();

private:
	
	void setRectInMask(Rect r);//设置掩模大小范围
	const string* winName;
	const Mat* image;
	Mat HOGImage;//用来显示HOG图像
	Mat fgImage;//用来显示前景
	Mat	bdImage;//borderMatting后的图像
	Mat binImage;//用来显示二值图
	Mat mask;//掩模
	Mat bgdModel, fgdModel;//背景，前景模型
	Mat imgThresholded;//颜色检验后的参照物图像
	Rect rect;//选中矩形
	vector<Point> fgdPixelVector, bgdPixelVector, prFgdPixelVector, prBgdPixelVector;//前、背景向量，可能的前、背景向量
	GrabCut2D gc;

};


