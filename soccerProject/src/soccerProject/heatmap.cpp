#include "linkItem.h"

/*
Mat overlay(int x, int y, Mat background, Mat over) {
Mat im;
background.copyTo(im);
for (int i = 0; i < over.rows; i++) {
for (int j = 0; j < over.cols; j++) {
im.at<Vec3b>(i + y, j + x) = over.at<Vec3b>(i, j);
}
}
return im;
}*/
/*
Mat color(Mat background, Mat over) {
Mat im;
background.copyTo(im);
for (int i = 0; i < over.rows; i++) {
for (int j = 0; j < over.cols; j++) {
if (over.at<uchar>(i, j) <80) {
im.at<Vec3b>(i, j) = Vec3b(40, 51, 255);
}
else if (over.at<uchar>(i, j) <140) {
im.at<Vec3b>(i, j) = Vec3b(43, 121, 255);
}
else if (over.at<uchar>(i, j) <180) {
im.at<Vec3b>(i, j) = Vec3b(40, 255, 255);
}
else if (over.at<uchar>(i, j) <210) {
im.at<Vec3b>(i, j) = Vec3b(54, 255, 12);
}
else if (over.at<uchar>(i, j) <230) {
im.at<Vec3b>(i, j) = Vec3b(255, 220, 12);
}
}
}
return im;
}*/

Mat heatmapGen(int* playerSelect, int team, vector<vector<STrkNodeGlb>> numbers) {
	int xpos = 0;
	int ypos = 0;
	int map[29][20] = { { 0 } };
	int max = 0;
	Mat field = imread("soccerfield.png");
	Mat image = imread("fieldcheck.jpg");
	Mat check = imread("check.jpg");
	resize(check, check, Size(20, 20), 1);
	Mat image4(field.rows, field.cols, CV_8UC1, Scalar(255));
	for (int i = 0; i < 8; i++) {
		if (playerSelect[i] == 0) {
			rectangle(image, Point(20, (i * 2 + 1) * 40 - 20), Point(40, (i * 2 + 1) * 40), Scalar(255, 255, 255), CV_FILLED);
		}
		else {
			image = overlay(20, (i * 2 + 1) * 40 - 20, image, check);
		}
	}

	for (int j = 0; j < 8; j++) {
		if (playerSelect[j] == 1) {
			for (int i = 0; i < numbers[j + 8 * (team - 1) + 1].size() - 1; i++) {
				xpos = (int)((numbers[j + 8 * (team - 1) + 1][i].o2dPtFtGlb.x * 1235 / 46000 + 110) / 50 + 0.5);
				ypos = (int)((numbers[j + 8 * (team - 1) + 1][i].o2dPtFtGlb.y * 820 / 32000 + 90) / 50 + 0.5);
				map[xpos][ypos] += 1;
				if (map[xpos][ypos] > max) {
					max = map[xpos][ypos];
				}
			}
		}
	}

	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 20; j++) {
			if (map[i][j] >0.6*max) {
				rectangle(image4, Point(i * 50, j * 50), Point((i + 1) * 50, (j + 1) * 50), Scalar(0), CV_FILLED);
			}
			else if (map[i][j] > 0.45*max) {
				rectangle(image4, Point(i * 50, j * 50), Point((i + 1) * 50, (j + 1) * 50), Scalar(50), CV_FILLED);
			}
			else if (map[i][j] >0.3*max) {
				rectangle(image4, Point(i * 50, j * 50), Point((i + 1) * 50, (j + 1) * 50), Scalar(100), CV_FILLED);
			}
			else if (map[i][j] >0.1*max) {
				rectangle(image4, Point(i * 50, j * 50), Point((i + 1) * 50, (j + 1) * 50), Scalar(150), CV_FILLED);
			}
			else if (map[i][j] >0) {
				rectangle(image4, Point(i * 50, j * 50), Point((i + 1) * 50, (j + 1) * 50), Scalar(200), CV_FILLED);
			}
		}
	}
	blur(image4, image4, Size(50, 50), Point(-1, -1), BORDER_DEFAULT);
	blur(image4, image4, Size(50, 50), Point(-1, -1), BORDER_DEFAULT);
	image4 = color(field, image4);
	addWeighted(image4, 0.7, field, 0.3, 0, image4);
	resize(image4, image4, Size(900 * field.cols / 1448, 900 * field.rows / 1448), 1);
	image = overlay(60, 20, image, image4);
	return image;
}