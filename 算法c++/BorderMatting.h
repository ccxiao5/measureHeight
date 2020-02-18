#pragma once
#include"stdio.h"
#include <opencv.hpp>
#include <iostream>
#include <vector>
#include "Information.h"
using namespace cv;
using namespace std;

class BorderMatting
{
private:
	Mat Mask;
	Mat Image;
	Mat Edge;
	int edgeRadius = 6;
	double lamda1 = 50;
	double lamda2 = 1000;

	int MAXDELTA = 30;
	int MAXSIGMA = 10;
	bool haveEdge = false;
public:
	BorderMatting() {};
	~BorderMatting() {};
	double Gaussian(double x, double delta, double sigma);
	double Mmean(double x, double Fmean, double Bmean);
	double Mvar(double x, double Fvar, double Bvar);
	BorderMatting(const Mat& _originImage, const Mat& _mask);
	double Sigmoid(double _r, double _delta, double _sigma);
	uchar valueColor2Gray(Vec3b color);
	void computeMeanVariance(point p, Info &result);
	vector<contourPoint> contourVector;
	vector<contourPoint> Oldcontour;
	void computeNearestPoint();
	double BorderMatting::dataTerm(point _ip, uchar _I, int _delta, int _sigma, Info &result);
	void Initialize(const Mat& _originImage, const Mat& _mask, int threadshold_1 = 1, int threadshold_2 = 4);
	Mat Run();
	int computeEdgeDistance(point p);
	void drawContour();
};

