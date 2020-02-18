#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include <iostream>
#include "GrabCut.h"
#include "BorderMAtting.h"

using namespace std;
using namespace cv;

const Scalar BLUE = Scalar(255, 0, 0); // ����
const Scalar GREEN = Scalar(0, 255, 0);//ǰ��
const Scalar RED = Scalar(0, 0, 255);//color of Rectangle

//��comMaskֵ���Ƹ�binMask
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
	Point PA, PB;//������ߵļ�������
	Point FA, FB;//������ߵļ�������
	int maxPersonPX;//�������������
	int maxRefPX;//����������������
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
	void measurePersonPX();//��������������ؾ���
	void  measureReferencePX();//����������������ؾ���
	Mat getBdImage();

private:
	
	void setRectInMask(Rect r);//������ģ��С��Χ
	const string* winName;
	const Mat* image;
	Mat HOGImage;//������ʾHOGͼ��
	Mat fgImage;//������ʾǰ��
	Mat	bdImage;//borderMatting���ͼ��
	Mat binImage;//������ʾ��ֵͼ
	Mat mask;//��ģ
	Mat bgdModel, fgdModel;//������ǰ��ģ��
	Mat imgThresholded;//��ɫ�����Ĳ�����ͼ��
	Rect rect;//ѡ�о���
	vector<Point> fgdPixelVector, bgdPixelVector, prFgdPixelVector, prBgdPixelVector;//ǰ���������������ܵ�ǰ����������
	GrabCut2D gc;

};


