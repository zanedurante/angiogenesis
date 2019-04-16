#include <string>
#include <vector>
#include <opencv2/core.hpp>
#pragma once
ref class Processor
{
public:
	Processor();
	~Processor();
	bool updatePicture(std::string fileLink);
	void enhance();
	void zoom();
	void show(std::string);
	int countMeshes();
	void fillContours();
	void skeletonize();
protected:
	int sidesOff;
	int topBottomOff;
	int BFS(cv::Mat& im, cv::Vec3b find, cv::Vec3b repl, int threshold);
	int width;
	int height;
	char* fileName;
	char* enhanced;
};

