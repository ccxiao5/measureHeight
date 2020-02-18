#pragma once
#ifndef GMM_H
#define GMM_H
#include <core/core.hpp>
#include <vector>
// ����
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
	cv::Vec3d mean;			// һ������RGB����ͨ�������������ֵ��3*3��Э�������һ��Ȩֵ
	cv::Matx33d cov;		// Э���3*3���;���
	cv::Matx33d inverseCov; // Э����������
	double covDeterminant;  // Э���������ʽ
	int totalPiexelCount;
};

// ��ϸ�˹ģ��
class GMM
{
public:
	GMM(int componentsCount = 5);
	void initLearning();//��ʼ��GMMģ��
	void addPixel(int compID, const cv::Vec3d color);//������ص���˹����
	void endLearning();//ѧϰ��ϸ�˹ģ�Ͳ���
	static GMM matToGMM(const cv::Mat &model); //�����Ƴɸ�˹���ģ��
	cv::Mat GMMtoMat() const;//����˹ģ��ת��Ϊ����
	int getComponentsCount() const;// ��÷���������
	double operator()(const cv::Vec3d color) const;//����Gibbs�����ĵ�һ��������
	int mostPossibleComponent(const cv::Vec3d color) const;//����5����˹������������һ��

private:
	std::vector<Component> components;//��������
	std::vector<double> coefs; // Ȩ��
	int totalSampleCount;//addPixel����������

};
#endif 