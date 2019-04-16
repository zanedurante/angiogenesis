#include "Processor.h"
#include "tinytiffreader.h"
#include "tinytiffwriter.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include <queue>
#include <opencv2/imgproc.hpp>

 //For debugging
#include <fstream>

#include <algorithm>
#include <set>

Processor::Processor()
{
	topBottomOff = 110;
	sidesOff = 220;
}

Processor::~Processor()
{

}

//Updates image file and make it binary
bool Processor::updatePicture(std::string fileLink)
{
	//Copy fileLink into fileName
	fileName = new char[fileLink.size() + 1];
	for (unsigned int i = 0; i < fileLink.size(); i++)
		fileName[i] = fileLink[i];
	fileName[fileLink.size()] = '\0';
	cv::Mat image = cv::imread(fileLink);
	if (image.rows == 0 || image.cols == 0)
		return false;

	std::ofstream out("debug.txt");
	int width = image.cols;
	int height = image.rows;

	out << fileLink << '\n';
	out << "Image cols: " << image.cols << "\n";
	out << "Image rows: " << image.rows << std::endl;
	out << std::endl << image.depth() << std::endl;
	out << "starting...\n";


	//Find background pixel value, all values that range far enough from it get set to max (white)
	//Trying median pixel value to serve as background
	std::vector<unsigned char> vals;
	std::vector<unsigned char> med;
	//Divide the image into an h x h grid of subsections
	int h = 45;
	//Find median of nine regions of the image
	for (int k = 0; k < h*h; k++)
	{
		for (int i = (height / h) * (k / h); i < (height / h) * ((k / h) + 1); i++)
		{
			for (int j = (width / h) * (k % h); j < (width / h) * ((k % h) + 1); j++)
			{
				vals.push_back(image.at<cv::Vec3b>(i, j).val[0]);
			}
		}
		//Find median element of vals for that region
		std::nth_element(vals.begin(), vals.begin() + (vals.size()) / 2, vals.end());
		med.push_back(vals.at(vals.size() / 2));
		vals.clear();
	}

	cv::Vec3b black;
	black.val[0] = 0;
	black.val[1] = 0;
	black.val[2] = 0;

	cv::Vec3b white;
	white.val[0] = 255;
	white.val[1] = 255;
	white.val[2] = 255;
	
	//All values less than median make black, make rest white
	for (int k = 0; k < h*h; k++)
	{
		for (int i = (height / h) * (k / h); i < (height / h) * ((k / h) + 1); i++)
		{
			for (int j = (width / h) * (k % h); j < (width / h) * ((k % h) + 1); j++)
			{
				//med[k] is median for this region
				if (image.at<cv::Vec3b>(i, j).val[0] <= med[k] * .95)
					image.at<cv::Vec3b>(i, j) = black;
				else
					image.at<cv::Vec3b>(i, j) = white;
			}
		}
	}
	//Write to file to see
	//Create new file name from image.jpg to image_enhanced.jpg
	std::string oldFile = fileLink;
	std::string newFile = oldFile.insert(oldFile.find("."), "_enhanced");
	enhanced = new char[newFile.size() + 1];
	for (unsigned int i = 0; i < newFile.size(); i++)
		enhanced[i] = newFile[i];
	enhanced[newFile.size()] = '\0'; //Enhanced now stores image_enhanced file path
	cv::imwrite(newFile, image);
	
	out.close();
	//Open previous picture and open along side 
	cv::Mat im2 = cv::imread(fileLink.c_str());

	//Open both images
	cv::namedWindow("pre", cv::WINDOW_NORMAL);
	cv::resizeWindow("pre", 400, 400);       
	imshow("pre", im2);	
	return true;
}

//Boldens lines to fill in gaps
void Processor::enhance()
{
	//Takes enhanced image and adds an h pixel padding to each black pixel
	int h = 8;
	cv::Mat image = cv::imread(std::string(enhanced));
	//Create a copy to use as comparison while changing pixels in image
	cv::Mat copy = cv::imread(std::string(enhanced)); 
	cv::Vec3b black;
	black.val[0] = 0;
	black.val[1] = 0;
	black.val[2] = 0;

	//For each pixel in copy, if it is black, make all h surrounding pixels black in image
	for (int i = h + 2; i < copy.rows - h - 2; i++)
	{
		for (int j = h + 2; j < copy.cols - h - 2; j++) 
		{
			if (copy.at<cv::Vec3b>(i, j) == black)
			{
				//Make surrounding pixels black in image (points are (x, y))
				cv::circle(image, cv::Point(j, i), h, black, -1);
			}
		}
	}

	//Replace image 
	cv::imwrite(std::string(enhanced), image);
}

//Zooms into picture and cuts off top and bottom pixels, and chops off side pixels (by making them black)
void Processor::zoom()
{
	cv::Vec3b black;
	black.val[0] = 0;
	black.val[1] = 0;
	black.val[2] = 0;
	cv::Mat im = cv::imread(std::string(enhanced));
	for (int i = 0; i < im.rows; i++)
	{
		for (int j = 0; j < im.cols; j++)
		{
			//If in the outer edge, make black
			if ((i < topBottomOff || i > im.rows - topBottomOff) || (j < sidesOff || j > im.cols - sidesOff ))
				im.at<cv::Vec3b>(i, j) = black;
		}
	}

	cv::imwrite(std::string(enhanced), im);
}

//Show enhanced picture
void Processor::show(std::string winName)
{
	cv::Mat image = cv::imread(std::string(enhanced));
	cv::namedWindow(winName, cv::WINDOW_NORMAL);
	cv::resizeWindow(winName, 400, 400);
	cv::imshow(winName, image);
}

//Fill in white spaces
void Processor::fillContours()
{
	cv::Vec3b white;
	cv::Vec3b black;
	cv::Vec3b red;
	cv::Vec3b blue;
	for (int i = 0; i < 3; i++)
	{
		white.val[i] = 255;
		black.val[i] = 0;
		if (i == 0)
			blue.val[0] = 255;
		else
			blue.val[i] = 0;
		if (i == 2)
			red.val[2] = 255;
		else
			red.val[i] = 0;
	}
	cv::Mat image = cv::imread(std::string(enhanced));
	//Create new image that is an inverted form of the image
	cv::Mat flood = image.clone();
	//Flood fill pixel (0, 0)
	cv::floodFill(flood, cv::Point(0, 0), cv::Scalar(255, 255, 255));
	cv::Mat inv;
	cv::bitwise_not(flood, inv);
	cv::Mat noiseGone;
	cv::bitwise_or(image, inv, noiseGone);
	cv::bitwise_not(image, inv);

	//Inv is inverse of image, noiseGone is image without any noise
	//bitwise_and gives pixels that are found as noise
	cv::Mat noise;
	cv::bitwise_and(inv, noiseGone, noise);
	//cv::Mat filled = (image | inv);



	//Go through each red pixel and BFS from there
	// If 400 pixels size is reached, fill as black, otherwise keep white********** NOT SURE IF DOING THIS RIGHT
	BFS(noise, white, black, 400);

	//Final image (noiseGone) is bitwise_or of noise and image
	cv::bitwise_or(noise, image, noiseGone);
	cv::imwrite(std::string(enhanced), noiseGone);
}

//Count number of individual white spaces separated by black
int Processor::countMeshes()
{
	cv::Vec3b black;
	black.val[0] = 0;
	black.val[1] = 0;
	black.val[2] = 0;

	cv::Vec3b white;
	white.val[0] = 255;
	white.val[1] = 255;
	white.val[2] = 255;
	
	//Go through each individual mesh, if white do BFS on it until white patch is fully found
	cv::Mat im = cv::imread(std::string(enhanced));
	//Make grayscale into rgb and create red xs where new it says "Mark in vis"
	cv::Vec3b red;
	red.val[0] = 0;
	red.val[1] = 0;
	red.val[2] = 255;

	cv::Vec3b blue;
	blue.val[0] = 255;
	blue.val[1] = 0;
	blue.val[2] = 0;

	cv::Vec3b green;
	green.val[0] = 0;
	green.val[1] = 255;
	green.val[2] = 0;

	cv::Mat vis = cv::imread(std::string(enhanced));
	int meshes = 0;
	long size;
	//Make a set of visited pairs,
	std::set<std::pair<int, int> > visited;
	for (int i = topBottomOff; i < im.rows -topBottomOff; i++)
	{
		for (int j = sidesOff; j < im.cols - sidesOff; j++)
		{
			if (im.at<cv::Vec3b>(i, j) == white)
			{
				//Mark in vis
				std::pair<int, int> start = std::make_pair(i, j);
				size = 0;
				std::queue<std::pair<int, int>> q;
				q.push(start);
				while (!q.empty())
				{
					if (visited.find(q.front()) != visited.end())
					{
						q.pop();
					}
					else
					{
						visited.insert(q.front());
						//Sides
						if (im.at<cv::Vec3b>(q.front().first - 1, q.front().second) == white)
							q.push(std::make_pair(q.front().first - 1, q.front().second));
						if (im.at<cv::Vec3b>(q.front().first + 1, q.front().second) == white)
							q.push(std::make_pair(q.front().first + 1, q.front().second));
						if (im.at<cv::Vec3b>(q.front().first, q.front().second - 1) == white)
							q.push(std::make_pair(q.front().first, q.front().second - 1));
						if (im.at<cv::Vec3b>(q.front().first, q.front().second + 1) == white)
							q.push(std::make_pair(q.front().first, q.front().second + 1));

						/*
						//Corners
						if (im.at<cv::Vec3b>(q.front().first - 1, q.front().second + 1) == white)
							q.push(std::make_pair(q.front().first - 1, q.front().second  + 1));
						if (im.at<cv::Vec3b>(q.front().first + 1, q.front().second - 1) == white)
							q.push(std::make_pair(q.front().first + 1, q.front().second - 1));
						if (im.at<cv::Vec3b>(q.front().first - 1, q.front().second - 1) == white)
							q.push(std::make_pair(q.front().first - 1, q.front().second - 1));
						if (im.at<cv::Vec3b>(q.front().first + 1, q.front().second + 1) == white)
							q.push(std::make_pair(q.front().first + 1, q.front().second + 1));
							*/
						q.pop();
						size++;
					}
				}
				if (size > 100) //Only include 100+ pixel meshes
				{
					meshes++;
					
					//Red x
					vis.at<cv::Vec3b>(start.first, start.second) = red;
					for (int y = -1; y < 2; y++)
					{
						for (int x = 1; x < 20; x++)
						{
							vis.at<cv::Vec3b>(start.first + y, start.second - x) = red;
							vis.at<cv::Vec3b>(start.first + y, start.second + x) = red;
							vis.at<cv::Vec3b>(start.first + x, start.second + y) = red;
							vis.at<cv::Vec3b>(start.first - x, start.second + y) = red;
						}
					}
				}
					
			}
		}
	}
	//cv::imwrite(std::string(enhanced), vis); ** So as not to interfere with skeletonization
	return meshes;
}

//Skeletonize
void Processor::skeletonize()
{
	cv::Mat im = cv::imread(std::string(enhanced));
	std::set<cv::Point> points;
	do
	{

	} while (!points.empty());
	return;
}


int Processor::BFS(cv::Mat& im, cv::Vec3b find, cv::Vec3b repl, int threshold)
{
	//Go through each red pixel and BFS from there
	// If 500 pixels size is reached, fill as black
	long long size;
	std::set<std::pair<int, int> > visited;
	for (int i = 0; i < im.rows; i++)
	{
		for (int j = 0; j < im.cols; j++)
		{
			if (im.at<cv::Vec3b>(i, j).val[0] != 0)
			{
				if (visited.find(std::make_pair(i, j)) == visited.end())
				{
					std::pair<int, int> start = std::make_pair(i, j);
					size = 0;
					std::queue<std::pair<int, int>> q;
					q.push(start);
					while (!q.empty())
					{
						if (visited.find(q.front()) == visited.end())
						{
							visited.insert(q.front());
							//BFS
							if (im.at<cv::Vec3b>(q.front().first, q.front().second) == find)
							{
								q.push(std::make_pair(q.front().first, q.front().second - 1));
								q.push(std::make_pair(q.front().first, q.front().second + 1));
								q.push(std::make_pair(q.front().first - 1, q.front().second));
								q.push(std::make_pair(q.front().first + 1, q.front().second));

								size++;
							}
						}
						q.pop();
					}
					//If size is above a certain threshold, this pixel group is likely a tubule and not noise
					if (size > threshold)
					{
						//Fill in at start
						cv::floodFill(im, cv::Point(start.second, start.first), repl);
					}
				}
			}
		}
	}

	return size;
}