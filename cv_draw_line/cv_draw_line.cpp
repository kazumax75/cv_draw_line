// cv_draw_line.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Catmull-RomSpline.h"

#define EPSILON 0.001

using namespace cv;
std::vector< cv::Point > line_pt;

void CallBackFunc(int event, int x, int y, int flags, void* param) {
	Mat image = *((Mat*)param);

	static bool flag_l = false;
	static bool flag_r = false;
	static int idx;

	switch (event) {
		case cv::EVENT_MOUSEMOVE:
			if (flag_l) {
				line_pt.emplace_back(cv::Point(x, y));

				cv::circle(image, Point(line_pt[idx].x, line_pt[idx].y), 1, Scalar(0, 0, 255), -1, cv::LINE_AA);
				if(idx - 1 >= 0){
					cv::line(image, Point(line_pt[idx - 1].x, line_pt[idx - 1].y), Point(line_pt[idx].x, line_pt[idx].y),
						Scalar(0, 0, 255), 2, cv::LINE_AA
					);
				}

				cv::imshow("VIZ", image);

				idx++;
			}
			break;
		case cv::EVENT_LBUTTONDOWN:
			flag_l = true;
			idx = 0;
			
			line_pt.clear();
			line_pt.emplace_back(cv::Point(x, y));


			break;
		case cv::EVENT_LBUTTONUP:
			if (flag_l) {
				flag_l = false;
				std::vector< cv::Point > output_approx;
				
				cv::approxPolyDP(
					cv::Mat(line_pt), 
					output_approx, 
					EPSILON * cv::arcLength(line_pt, false),
					false
				);
				for (int i = 0; i < output_approx.size(); ++i) {
					cv::Point p = output_approx[i];
					// cv::circle(image, p, 1, Scalar(0, 255, 0), 4, cv::LINE_AA);
				}

				vector<cv::Point2f> vData;
				for (int i = 0; i < output_approx.size(); ++i) {
					cv::Point p = output_approx[i];
					vData.emplace_back(cv::Point2f((float)p.x, (float)p.y) );
				}
				
				CatmullRomSpline<cv::Point2f> cmr_spline(vData);
				
				//改めて制御点の表示
				for (int idx = 0; idx < vData.size(); ++idx)
    				cv::circle(image, vData[idx], 1, cv::Scalar(0, 0, 0), -1, cv::LINE_AA);
				
				// 制御点の数だけ繰り返す
				for (int i = 0; i < vData.size() - 1; ++i) {
					//エバリュエータ
					int div = 100;
					for (int j = 0; j < div; ++j) {
						cv::Point2f v = cmr_spline.getValue(i, j / (float)div);
						cv::circle(image, v, 1, cv::Scalar(0, 0, 0), -1, cv::LINE_AA);
					}
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

