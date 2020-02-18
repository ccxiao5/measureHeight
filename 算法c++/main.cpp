#include"iostream"
#include"stdio.h"
#include"BorderMatting.h"
#include "GCApplication.h"
#include <opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc,char *argv[])
{
	GCApplication gcapp;//����һ����ͼ��������������
//	string filename = "c:/test_blue.jpg";
	string filename = "c:/red.jpg";
	//string filename = "c:/blue.jpg";
	gcapp.color = argv[1];//�ơ��졢��
	gcapp.rfHeight = atof(argv[2]);//63��105��33
	Mat rawSrc,src;//dst������ȡ��Ľ��
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
//��ʼ�����Դ��ӿ�
/*	cv::Mat mask = Mat::zeros(src.size(), CV_8UC1);//�ָ��Ľ��
	//������ʱ�����������Ϊ�㷨���м����ʹ��
	cv::Mat bgModel, fgModel;
	cv::Rect rectangle(r.tl(),r.br());//ͼ���ǰ������Ҳ���Ǿ���ѡ��ͼ��
	// GrabCut �ֶ�
	cv::grabCut(src,    //����ͼ��
		mask,   //�ֶν��
		rectangle,// ����ǰ���ľ��� 
		bgModel, fgModel, // ǰ��������
		1,        // ��������
		cv::GC_INIT_WITH_RECT); // �þ���
	
	// �õ�������ǰ��������
	//�ȽϺ�������ֵΪGC_PR_FGD������
	cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
	// �������ͼ��
	cv::Mat foreground(src.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	//����ֵΪ GC_BGD=0����Ϊ����
	src.copyTo(foreground, mask);
	imshow("foreground", foreground);
	*/
//�Լ���д
	gcapp.GrabCutting();
	gcapp.showImage(2);

/********************************BorderMatting**********************************************/
	gcapp.BoardMatting();
    gcapp.showImage(3);

/******************************��ֵ�����������ؾ���************************************************/
	gcapp.measurePersonPX();//�����������ؾ���
	gcapp.measureReferencePX();//������������
	line(src, gcapp.PA, gcapp.PB, Scalar(0, 0, 255), 3);
	line(src, gcapp.FA, gcapp.FB, Scalar(0, 0, 255), 3);
	imshow("final", src);
	double k;
	k = gcapp.maxPersonPX*1.0 / gcapp.maxRefPX;
	cout << "�����û����ظ߶�:" << gcapp.maxPersonPX << "���������ظ߶�:" << gcapp.maxRefPX <<endl;
	cout << "����ϵ��k��"<<k << endl;
	cout << "ʵ�����Ϊ��" << k*gcapp.rfHeight << endl;
	waitKey();
	return 0;
}
