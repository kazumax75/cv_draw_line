// cv_draw_line.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;

struct vector2D // 2次元ベクトルの構造体
{
	float x;
	float y;
};

vector2D line_pt[5000];
std::vector< cv::Point > approx;

void CallBackFunc(int event, int x, int y, int flags, void* param) {
	Mat image = *((Mat*)param);

	static bool flag_l = false;
	static bool flag_r = false;
	static int idx;

	switch (event) {
		case cv::EVENT_MOUSEMOVE:
			if (flag_l) {
				line_pt[idx].x = x;
				line_pt[idx].y = y;
				approx.emplace_back(cv::Point(x, y));

				cv::circle(image, Point(line_pt[idx].x, line_pt[idx].y), 1, Scalar(0, 0, 255), -1, cv::LINE_AA);
				if(idx - 1 >= 0){
					cv::line(image, Point(line_pt[idx - 1].x, line_pt[idx - 1].y), Point(line_pt[idx].x, line_pt[idx].y),
						Scalar(0, 255, 255), 1, cv::LINE_AA
					);
				}

				cv::imshow("VIZ", image);

				idx++;
			}
			break;
		case cv::EVENT_LBUTTONDOWN:
			flag_l = true;
			idx = 0;

			line_pt[idx].x = x;
			line_pt[idx].y = y;
			approx.emplace_back(cv::Point(x, y));


			break;
		case cv::EVENT_LBUTTONUP:
			if (flag_l) {
				flag_l = false;
				cv::approxPolyDP(
					cv::Mat(approx), 
					approx, 
					0.001 * cv::arcLength(approx, false),
					false
				);
				for (int i = 0; i < approx.size(); ++i) {
					cv::Point p = approx[i];
					cv::circle(image, p, 1, Scalar(0, 255, 0), 4, cv::LINE_AA);
				}
				cv::imshow("VIZ", image);
				//cv::polylines(image, approx, true, cv::Scalar(255, 0, 0), 2);
				
			}
			break;

		case cv::EVENT_MBUTTONDOWN: 

			break;
		case cv::EVENT_RBUTTONDOWN:

			break;
		case cv::EVENT_RBUTTONUP:
			break;
	}
}


int main()
{

	cv::Mat img = cv::Mat::zeros(600, 800, CV_8UC3);
	img = Scalar(255, 255, 255);

	cv::imshow("VIZ", img);


	//コールバックの設定
	cv::setMouseCallback("VIZ", CallBackFunc, &img);

	waitKey(0);
}

