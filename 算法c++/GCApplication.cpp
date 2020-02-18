#include "GCApplication.h"

//重新赋值类中数值
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
//赋值二值图
void GCApplication::setBinImage(Mat img)
{
	binImage = img;
}
//设置图像和窗口名字
void GCApplication::setImageAndWinName(const Mat& _image, const string& _winName)
{
	if (_image.empty() || _winName.empty())
		return;
	image = &_image;
	winName = &_winName;
	mask.create(image->size(), CV_8UC1);
	reset();
}
//显示初始图像
void GCApplication::showImage() {
	imshow("OriginalImage", *image);
}
//显示结果图像
void GCApplication::showImage(int Flag)
{
	Mat res;
	Mat binMask;
	getBinMask(mask, binMask);//将mask的值赋给binMask来处理
	if (Flag == 1)//输出HOG检测后的图像
	{
		HOGImage = image->clone();
		rectangle(HOGImage, Point(rect.x, rect.y), Point(rect.x + rect.width, rect.y + rect.height), RED, 2);
		imshow("HOG", HOGImage);
	}
	else if (Flag == 2) //输出前景图像
	{
		image->copyTo(fgImage, binMask);
		imshow("foreground", fgImage);
	}
	else if (Flag == 3)//输出borderMatting后的图像
		imshow("borderMatting", bdImage);
	else if (Flag == 4)//输出二值图
		imshow("二值图", binImage);
}
//使用rect来设定Mask
void GCApplication::setRectInMask(Rect r)
{
	assert(!mask.empty());
	mask.setTo(GC_BGD);   //GC_BGD == 0 (an obvious background pixels)
	rect = r;
	(mask(rect)).setTo(Scalar(GC_PR_FGD));    //GC_PR_FGD == 3(a possible foreground pixel)
}
//Hog检测
void GCApplication::setHogDetector()
{
	vector<Rect> findrects, findrect;
	HOGDescriptor HOG;
	//SVM分类器
	HOG.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
	//多尺度检测
	HOG.detectMultiScale(*image, findrects, 0, Size(4, 4), Size(0, 0), 1.05, 2);
	//若rects有嵌套,则取最外面的矩形存入rect
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
	Rect r;//用来选中所测图像中的测量对象。
	r.height = -1;
	//框选出检测结果并选中图片测量对象
	for (int i = 0; i < findrect.size(); i++)
		r = r.height > findrect[i].height ? r : findrect[i];
	// HOG detector返回的矩形框比真正图像轮廓大，所以我们减少矩形框的大小来使得更符合外界边框
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
	morphologyEx(bdImage, bdImage, MORPH_OPEN, element);//剔除噪点
	Mat  rstGray;
	cvtColor(bdImage, rstGray, CV_BGR2GRAY);//灰度化
	PA.x = PA.y = PB.x = PB.y = 0;
	int i, j;
	int flagA, flagB;
	flagA = flagB = 0;
	//二值化并选取两个极限点 ，首尾同时进行遍历
	binImage = rstGray.clone();
	for (i = 0; i < binImage.rows / 2; i++)
		for (j = 0; j < binImage.cols; j++)
		{
			if (binImage.at<uchar>(i, j) != 0)
			{
				if (!flagA)//标记第一个出现像素的像素点
				{
					PA.x = j;//注意顺序，坐标原点位于左上角
					PA.y = i;
					flagA = 1;
				}
				binImage.at<uchar>(i, j) = 255;//将非黑色像素点全部置于白色
			}
			if (binImage.at<uchar>(binImage.rows - 1 - i, j) != 0)
			{
				if (!flagB)//标记最后一个出现像素的像素点
				{
					PB.x = j;//注意顺序，坐标原点位于左上角
					PB.y = binImage.rows - 1 - i;
					flagB = 1;
				}
				binImage.at<uchar>(binImage.rows - 1 - i, j) = 255;//将非黑色像素点全部置于白色
			}
		}
	showImage(4);
	maxPersonPX = PB.y - PA.y;//用纵坐标之差代替欧氏距离
}
void GCApplication::measureReferencePX()
{
	Mat imgHSV;
	//黄、红、蓝
	int index;
	if (color == "黄色")
		index = 0;
	else if (color == "红色")
		index = 1;
	else
		index = 2;
	int iLowH[3] = { 18,156,110 };
	int iHighH[3] = { 38,180,130 };
	//设置饱和度的范围值
	int iLowS[3] = { 90,43,43 };
	int iHighS = 255;
	//设置亮度的范围值
	int iLowV[3] = { 90,46 ,46 };
	int iHighV = 255;
	vector<Mat> hsvSplit;
	cvtColor(*image, imgHSV, COLOR_BGR2HSV); //BGR 转化成 HSV，因为直方图均衡化需要在HSV空间做
	imshow("imgHSV1", imgHSV);
	split(imgHSV, hsvSplit);
	equalizeHist(hsvSplit[2], hsvSplit[2]);
	merge(hsvSplit, imgHSV);

	inRange(imgHSV, Scalar(iLowH[index], iLowS[index], iLowV[index]), Scalar(iHighH[index], iHighS, iHighV), imgThresholded); //图像的范围
	imshow("Thresholded", imgThresholded);
	//开操作 (去除一些噪点)
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	//morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
	//闭操作 (连接一些连通域)
	morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);

	//计算参照物像素高度
	int i, j;
	int flagA, flagB;
	flagA = flagB = 0;
	//从上面进行遍历
	for (i = 0; i < imgThresholded.rows; i++)
	{
		for (j = 0; j < imgThresholded.cols; j++)
			if (imgThresholded.at<uchar>(i, j) == 255 && !flagA) // 标记第一个出现像素的像素点
			{

				FA.x = j;//注意顺序，坐标原点位于左上角
				FA.y = i;
				flagA = 1;
				break;
			}
		if (flagA)
			break;
	}
	//从下面进行遍历
	for (i = imgThresholded.rows - 1; i > 0; i--)
	{
		for (j = 0; j < imgThresholded.cols; j++)
			if (imgThresholded.at<uchar>(i, j) == 255 && !flagB) // 标记第一个出现像素的像素点
			{

				FB.x = j;//注意顺序，坐标原点位于左上角
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
