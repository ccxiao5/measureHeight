#include "GCApplication.h"

//���¸�ֵ������ֵ
void GCApplication::reset()
{
	if (!mask.empty())
		mask.setTo(Scalar::all(GC_BGD));
	bgdPixelVector.clear(); fgdPixelVector.clear();
	prBgdPixelVector.clear();  prFgdPixelVector.clear();
}
Mat GCApplication::getBdImage() {
	return bdImage;
}
//��ֵ��ֵͼ
void GCApplication::setBinImage(Mat img)
{
	binImage = img;
}
//����ͼ��ʹ�������
void GCApplication::setImageAndWinName(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	mask.create(image->size(), CV_8UC1);
	reset();
}
//��ʾ��ʼͼ��
void GCApplication::showImage() {
	imshow("OriginalImage", *image);
}
//��ʾ���ͼ��
void GCApplication::showImage(int Flag)
{
	Mat res;
	Mat binMask;
	getBinMask(mask, binMask);//��mask��ֵ����binMask������
	if (Flag == 1)//���HOG�����ͼ��
	{
		HOGImage = image->clone();
		rectangle(HOGImage, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
		imshow("HOG", HOGImage);
	}
	else if (Flag == 2) //���ǰ��ͼ��
	{
		image->copyTo(fgImage, binMask);
		imshow("foreground", fgImage);
	}
	else if (Flag == 3)//���borderMatting���ͼ��
		imshow("borderMatting", bdImage);
	else if (Flag == 4)//�����ֵͼ
		imshow("��ֵͼ", binImage);
}
//ʹ��rect���趨Mask
void GCApplication::setRectInMask(Rect r)
{
	assert(!mask.empty());
	mask.setTo(GC_BGD);   //GC_BGD == 0 (an obvious background pixels)
	rect = r;
	(mask(rect)).setTo(Scalar(GC_PR_FGD));    //GC_PR_FGD == 3(a possible foreground pixel)
}
//Hog���
void GCApplication::setHogDetector()
{
	vector<Rect> findrects, findrect;
	HOGDescriptor HOG;
	//SVM������
	HOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	//��߶ȼ��
	HOG.detectMultiScale(*image, findrects, 0, Size(4, 4), Size(0, 0), 1.05, 2);
	//��rects��Ƕ��,��ȡ������ľ��δ���rect
	for (int i = 0; i < findrects.size(); i++)
	{
		Rect tRect = findrects[i];
		int j = 0;
		for (; j < findrects.size(); j++)
			if (j != i && (tRect & findrects[j]) == tRect)
				break;
		if (j == findrects.size())
			findrect.push_back(tRect);
	}
	Rect r;//����ѡ������ͼ���еĲ�������
	r.height = -1;
	//��ѡ���������ѡ��ͼƬ��������
	for (int i = 0; i < findrect.size(); i++)
		r = r.height > findrect[i].height ? r : findrect[i];
	// HOG detector���صľ��ο������ͼ���������������Ǽ��پ��ο�Ĵ�С��ʹ�ø��������߿�
	r.x += cvRound(r.width*0.1);
	r.width = cvRound(r.width*0.9);
	r.y += cvRound(r.height*0.07);
	r.height = cvRound(r.height*0.9);
	setRectInMask(r);
}
void GCApplication::GrabCutting()
{
	gc.GrabCut(*image, mask, rect, bgdModel, fgdModel, 1, GC_WITH_RECT);
}
void GCApplication::BoardMatting()
{
	Mat rst = Mat((*image).size(), (*image).type());
	(*image).copyTo(rst);
	bm.Initialize(*image, mask);
	bdImage = bm.Run();
}
void GCApplication::measurePersonPX()
{
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(bdImage, bdImage, MORPH_OPEN, element);//�޳����
	Mat  rstGray;
	cvtColor(bdImage, rstGray, CV_BGR2GRAY);//�ҶȻ�
	PA.x = PA.y = PB.x = PB.y = 0;
	int i, j;
	int flagA, flagB;
	flagA = flagB = 0;
	//��ֵ����ѡȡ�������޵� ����βͬʱ���б���
	binImage = rstGray.clone();
	for (i = 0; i < binImage.rows / 2; i++)
		for (j = 0; j < binImage.cols; j++)
		{
			if (binImage.at<uchar>(i, j) != 0)
			{
				if (!flagA)//��ǵ�һ���������ص����ص�
				{
					PA.x = j;//ע��˳������ԭ��λ�����Ͻ�
					PA.y = i;
					flagA = 1;
				}
				binImage.at<uchar>(i, j) = 255;//���Ǻ�ɫ���ص�ȫ�����ڰ�ɫ
			}
			if (binImage.at<uchar>(binImage.rows - 1 - i, j) != 0)
			{
				if (!flagB)//������һ���������ص����ص�
				{
					PB.x = j;//ע��˳������ԭ��λ�����Ͻ�
					PB.y = binImage.rows - 1 - i;
					flagB = 1;
				}
				binImage.at<uchar>(binImage.rows - 1 - i, j) = 255;//���Ǻ�ɫ���ص�ȫ�����ڰ�ɫ
			}
		}
	showImage(4);
	maxPersonPX = PB.y - PA.y;//��������֮�����ŷ�Ͼ���
}
void GCApplication::measureReferencePX()
{
	Mat imgHSV;
	//�ơ��졢��
	int index;
	if (color == "��ɫ")
		index = 0;
	else if (color == "��ɫ")
		index = 1;
	else
		index = 2;
	int iLowH[3] = { 18,156,110 };
	int iHighH[3] = { 38,180,130 };
	//���ñ��Ͷȵķ�Χֵ
	int iLowS[3] = { 90,43,43 };
	int iHighS = 255;
	//�������ȵķ�Χֵ
	int iLowV[3] = { 90,46 ,46 };
	int iHighV = 255;
	vector<Mat> hsvSplit;
	cvtColor(*image, imgHSV, COLOR_BGR2HSV); //BGR ת���� HSV����Ϊֱ��ͼ���⻯��Ҫ��HSV�ռ���
	imshow("imgHSV1", imgHSV);
	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	inRange(imgHSV, Scalar(iLowH[index], iLowS[index], iLowV[index]), Scalar(iHighH[index], iHighS, iHighV), imgThresholded); //ͼ��ķ�Χ
	imshow("Thresholded", imgThresholded);
	//������ (ȥ��һЩ���)
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	//�ղ��� (����һЩ��ͨ��)
	morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

	//������������ظ߶�
	int i, j;
	int flagA, flagB;
	flagA = flagB = 0;
	//��������б���
	for (i = 0; i < imgThresholded.rows; i++)
	{
		for (j = 0; j < imgThresholded.cols; j++)
			if (imgThresholded.at<uchar>(i, j) == 255 && !flagA) // ��ǵ�һ���������ص����ص�
			{

				FA.x = j;//ע��˳������ԭ��λ�����Ͻ�
				FA.y = i;
				flagA = 1;
				break;
			}
		if (flagA)
			break;
	}
	//��������б���
	for (i = imgThresholded.rows - 1; i > 0; i--)
	{
		for (j = 0; j < imgThresholded.cols; j++)
			if (imgThresholded.at<uchar>(i, j) == 255 && !flagB) // ��ǵ�һ���������ص����ص�
			{

				FB.x = j;//ע��˳������ԭ��λ�����Ͻ�
				FB.y = i;
				flagB = 1;
				break;
			}
		if (flagB)
			break;
	}
	imshow("Thresholded Image", imgThresholded);
	maxRefPX = FB.y - FA.y;
}
