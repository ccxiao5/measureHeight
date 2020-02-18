#pragma once
#ifndef GMM_H
#define GMM_H
#include <core/core.hpp>
#include <vector>
// 分量
class Component
{
public:
	Component();
	Component(const cv::Mat &modelComponent);
	cv::Mat exportModel() const;
	void initLearning();
	void addPixel(cv::Vec3d color);
	void endLearning();
	double operator()(const cv::Vec3d &color) const;
	int getCompPixelCount() const;
private:
	cv::Vec3d mean;			// 一个像素RGB三个通道，因此三个均值，3*3个协方差，共用一个权值
	cv::Matx33d cov;		// 协方差，3*3整型矩阵
	cv::Matx33d inverseCov; // 协方差的逆矩阵
	double covDeterminant;  // 协方差的行列式
	int totalPiexelCount;
};

// 混合高斯模型
class GMM
{
public:
	GMM(int componentsCount = 5);
	void initLearning();//初始化GMM模型
	void addPixel(int compID, const cv::Vec3d color);//添加像素到高斯分量
	void endLearning();//学习混合高斯模型参数
	static GMM matToGMM(const cv::Mat &model); //矩阵复制成高斯混合模型
	cv::Mat GMMtoMat() const;//将高斯模型转化为矩阵
	int getComponentsCount() const;// 获得分量的数量
	double operator()(const cv::Vec3d color) const;//计算Gibbs能量的第一个能量项
	int mostPossibleComponent(const cv::Vec3d color) const;//返回5个高斯向量中最大的那一项

private:
	std::vector<Component> components;//分量数组
	std::vector<double> coefs; // 权重
	int totalSampleCount;//addPixel的像素数量

};
#endif 