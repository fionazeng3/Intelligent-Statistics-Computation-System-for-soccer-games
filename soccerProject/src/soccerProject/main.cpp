#include "linkItem.h"
Mat image;
Mat check;
// button control variables
int run = 0;
int disp = 0;
int dataGeneral = 0;
int dataTeam = 0;
int animation = 0;
int plotTeam = 0;
int passing = 0;
int general = 1;
int frame = 0;
int lineUp = 0;
int interp = -1;
int dir = 1;
int passingA = 0;
// heat map and animation 
// player control
int heatmulti[8] = { { 0 } };
int animation1[8] = { { 0 } };
int animation2[8] = { { 0 } };
int distance1[8] = { { 0 } };
int distance2[8] = { { 0 } };
int speed1[8] = { { 0 } };
int speed2[8] = { { 0 } };
// data team control
int datateam = 1;
int heatteam = 1;
int animationTeam = 1;
int passingATeam = 1;
vector<vector<Team>> stats;
vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList;
vector<vector<vector<float>>> numbers;
// function that is automatically called when mouse clicks
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	bool heatMap = x <= 922 && x >= 699 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool individualData = x <= 699 && x >= 476 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool isAnimation = x <= 1145 && x >= 922 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool data = x <= 699 && x >= 476 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool isLineUp = x <= 253 && x >= 30 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool team1 = x <= 130 && x >= 60 && y <= 190 && y >= 160 && event == EVENT_LBUTTONDOWN;
	bool team2 = x <= 250 && x >= 180 && y <= 190 && y >= 160 && event == EVENT_LBUTTONDOWN;
	bool isDataTeam = x <= 476 && x >= 253 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	bool isPassingA = x <= 1368 && x >= 1145 && y <= 70 && y >= 10 && event == EVENT_LBUTTONDOWN;
	// when heat map is clicked
	if (heatMap && disp == 0)
	{
		for (int i = 0; i < 8; i++) {
			heatmulti[i] = 0;
		}
		heatmapControl(x, y, event);
		disp = 1;
		animation = 0;
		dataGeneral = 0;
		run = 0;
		lineUp = 0;
		dataTeam = 0;
		passingA = 0;
	}
	// choose team1 for display
	if (team1 && disp == 1)
	{
		heatteam = 1;
		for (int i = 0; i < 8; i++) {
			heatmulti[i] = 0;
		}
		heatmapControl(x, y, event);

	}

	// choose team2 for display
	if (team2 && disp == 1)
	{
		heatteam = 2;
		for (int i = 0; i < 8; i++) {
			heatmulti[i] = 0;
		}
		heatmapControl(x, y, event);
	}

	// choose players to display
	if (disp == 1 && event == EVENT_LBUTTONDOWN) {
		heatmapControl(x, y, event);
	}

	// when the passing analysis is clicked 
	if (passingA == 0 && isPassingA) {
		disp = 0;
		dataTeam = 0;
		animation = 0;
		dataGeneral = 0;
		run = 0;
		lineUp = 0;
		passingA = 1;
		passingAControl(x, y, event);
	}
	// choose team1 for passing analysis
	if (passingA == 1 && team1) {
		passingATeam = 1;
		passingAControl(x, y, event);
	}

	// choose team2 for passing analysis
	if (passingA == 1 && team2) {
		passingATeam = 2;
		passingAControl(x, y, event);
	}

	// when the individual data key is clicked
	if (data && dataGeneral == 0)
	{
		disp = 0;
		dataTeam = 0;
		animation = 0;
		dataGeneral = 1;
		run = 0;
		lineUp = 0;
		passingA = 0;
		dataControl(x, y, event);
	}
	// choose team1 for individual data
	if (dataGeneral == 1 && team1) {
		datateam = 1;
		dataControl(x, y, event);
	}
	// choose team2 for individual data
	if (dataGeneral == 1 && team2) {
		datateam = 2;
		dataControl(x, y, event);
	}
	// choose what data to display
	if (dataGeneral == 1 && event == EVENT_LBUTTONDOWN) {
		dataControl(x, y, event);
	}

	// choose to dislay the initial line up 
	if (lineUp == 0 && isLineUp) {
		lineUp = 1;
		dataTeam = 0;
		disp = 0;
		animation = 0;
		dataGeneral = 0;
		run = 0;
		passingA = 0;
		image = imread(".//bgImg//start.jpg");
		rectangle(image, Point(30, 10), Point(253, 70), Scalar(255, 102, 51), CV_FILLED);
		putText(image, "Line-up", Point(50, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
		int temp1[8] = { 1, 1, 1, 1, 1, 1, 1, 1 };
		int temp2[8] = { { 0 } };
		Mat image2 = plotEachPlayer(temp1, temp1, temp2, temp2, temp2, temp2, 0, vvsTrkNodeGlbList, stats);
		Mat image3;
		Size size(940, 500);
		resize(image2, image3, size);
		image = overlay(230, 200, image, image3);
	}

	// choose to display the team data 
	if (dataTeam == 0 && isDataTeam) {
		lineUp = 0;
		dataTeam = 1;
		disp = 0;
		animation = 0;
		dataGeneral = 0;
		run = 0;
		passingA = 0;
		image = imread(".//bgImg//start.jpg");
		rectangle(image, Point(253, 10), Point(476, 70), Scalar(255, 102, 51), CV_FILLED);
		putText(image, "Team Data", Point(273, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
		Mat image2 = imread(".\\graphicResults\\teamDataIm.png");
		Mat image3;
		Size size(1100, 550);
		resize(image2, image3, size);
		image = overlay(150, 200, image, image3);
	}

	// choose to show the animation 
	if (isAnimation && animation == 0) {
		frame = 0;
		for (int i = 0; i < 8; i++) {
			animation1[i] = 0;
			animation2[i] = 0;
			distance1[i] = 0;
			distance2[i] = 0;
			speed1[i] = 0;
			speed2[i] = 0;
		}
		dataTeam = 0;
		animation = 1;
		lineUp = 0;
		disp = 0;
		dataGeneral = 0;
		animationTeam = 1;
		run = 0;
		plotTeam = 0;
		passingA = 0;
		interp = -1;
		dir = 1;
		makeAnimationImage();
	}
	// controls in the animation page
	if (animation == 1 && event == EVENT_LBUTTONDOWN) {
		animationControl(x, y, event);
	}
	imshow("Display window", image);
}


// control the animation display
void animationControl(int x, int y, int event) {
	Size size(940, 500);
	// main control for the animation 
	bool isRun = x <= 630 && x >= 560 && y <= 190 && y >= 160;
	bool isAll = x <= 250 && x >= 180 && y <= 190 && y >= 160;
	bool stop = x <= 130 && x >= 60 && y <= 190 && y >= 160;
	bool select = x <= 860 && x >= 760 && y <= 190 && y >= 160;
	bool forw = x <= 690 && x >= 640 && y <= 190 && y >= 160;
	bool back = x <= 550 && x >= 500 && y <= 190 && y >= 160;
	// if play all is clicked 
	if (isAll) {
		plotTeam = (plotTeam + 1) % 2;
		if (plotTeam == 1) {
			for (int i = 0; i < 8; i++) {
				animation1[i] = 1;
				animation2[i] = 1;
				rectangle(image, Point(40, 260 + 50 * i), Point(90, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
				rectangle(image, Point(1180, 260 + 50 * i), Point(1230, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
			}
			Mat result;
			result = plotEachPlayer(animation1, animation2, speed1, speed2, distance1, distance2, frame, vvsTrkNodeGlbList, stats);
			Mat image3;
			resize(result, image3, size);
			image = overlay(230, 200, image, image3);
			rectangle(image, Point(180, 160), Point(250, 190), Scalar(0, 0, 0), CV_FILLED);
			putText(image, "Play All", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
		}
		else {
			for (int i = 0; i < 8; i++) {
				rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
				putText(image, "Play All", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
				animation1[i] = 0;
				animation2[i] = 0;
				speed1[i] = 0;
				distance1[i] = 0;
				speed2[i] = 0;
				speed2[i] = 0;
				rectangle(image, Point(120, 260 + 50 * i), Point(140, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(170, 260 + 50 * i), Point(190, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(1260, 260 + 50 * i), Point(1280, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(1310, 260 + 50 * i), Point(1330, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(40, 260 + 50 * i), Point(90, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(1180, 260 + 50 * i), Point(1230, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
			}
		}
	}
	// change the status of play
	if (isRun) {
		run = (run + 1) % 2;
		if (run == 0) {
			rectangle(image, Point(560, 160), Point(630, 190), Scalar(64, 64, 64), CV_FILLED);
			line(image, Point(580, 170), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
			line(image, Point(580, 170), Point(610, 175), Scalar(0, 128, 0), 2, 8, 0);
			line(image, Point(610, 175), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);

		}
		else {
			rectangle(image, Point(560, 160), Point(630, 190), Scalar(64, 64, 64), CV_FILLED);
			rectangle(image, Point(585, 170), Point(590, 180), Scalar(106, 236, 247), CV_FILLED);
			rectangle(image, Point(600, 170), Point(605, 180), Scalar(106, 236, 247), CV_FILLED);
		}
	}
	if (stop) {
		Mat image2 = imread(".//bgImg//soccerField.png");
		Mat image3;
		resize(image2, image3, size);
		image = overlay(230, 200, image, image3);
		rectangle(image, Point(560, 160), Point(630, 190), Scalar(64, 64, 64), CV_FILLED);
		line(image, Point(580, 170), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
		line(image, Point(580, 170), Point(610, 175), Scalar(0, 128, 0), 2, 8, 0);
		line(image, Point(610, 175), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
		frame = 0;
		run = 0;
	}
	if (select) {
		if (interp == -1) {
			interp = frame;
		}
		else {
			float stepx = 0;
			float stepy = 0;
			int length = 0;
			if (frame > interp) {
				length = frame - interp + 2;
				stepx = vvsTrkNodeGlbList[0][frame + 1].o2dPtFtGlb.x - vvsTrkNodeGlbList[0][interp - 1].o2dPtFtGlb.x;
				stepy = vvsTrkNodeGlbList[0][frame + 1].o2dPtFtGlb.y - vvsTrkNodeGlbList[0][interp - 1].o2dPtFtGlb.y;
				for (int i = interp; i <= frame; i++) {
					vvsTrkNodeGlbList[0][i].o2dPtFtGlb.x = vvsTrkNodeGlbList[0][i - 1].o2dPtFtGlb.x + stepx / length;
					vvsTrkNodeGlbList[0][i].o2dPtFtGlb.y = vvsTrkNodeGlbList[0][i - 1].o2dPtFtGlb.y + stepy / length;
				}
			}
			else {
				length = interp - frame + 2;
				stepx = vvsTrkNodeGlbList[0][interp + 1].o2dPtFtGlb.x - vvsTrkNodeGlbList[0][frame - 1].o2dPtFtGlb.x;
				stepy = vvsTrkNodeGlbList[0][interp + 1].o2dPtFtGlb.y - vvsTrkNodeGlbList[0][frame - 1].o2dPtFtGlb.y;
				for (int i = frame; i <= interp; i++) {
					vvsTrkNodeGlbList[0][i].o2dPtFtGlb.x = vvsTrkNodeGlbList[0][i - 1].o2dPtFtGlb.x + stepx / length;
					vvsTrkNodeGlbList[0][i].o2dPtFtGlb.y = vvsTrkNodeGlbList[0][i - 1].o2dPtFtGlb.y + stepy / length;
				}
			}
			rectangle(image, Point(760, 160), Point(860, 190), Scalar(64, 64, 64), CV_FILLED);
			putText(image, "Select", Point(790, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
			frame = 0;
			interp = -1;
			run = 0;
		}
	}
	// choose the players
	for (int i = 0; i < 8; i++) {
		// team1
		// if a player is chosen
		if (x >= 40 && x <= 90 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			animation1[i] = (animation1[i] + 1) % 2;
			if (animation1[i] == 0) {
				speed1[i] = 0;
				distance1[i] = 0;
				rectangle(image, Point(120, 260 + 50 * i), Point(140, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(170, 260 + 50 * i), Point(190, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(40, 260 + 50 * i), Point(90, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				plotTeam = 0;
				rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
				putText(image, "Play All", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
			}
			else {
				rectangle(image, Point(40, 260 + 50 * i), Point(90, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
				Mat result;
				result = plotEachPlayer(animation1, animation2, speed1, speed2, distance1, distance2, frame, vvsTrkNodeGlbList, stats);
				Mat image3;
				resize(result, image3, size);
				image = overlay(230, 200, image, image3);
			}
		}
		if (x >= 120 && x <= 140 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			speed1[i] = (speed1[i] + 1) % 2;
			if (speed1[i] == 0) {
				rectangle(image, Point(120, 260 + 50 * i), Point(140, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
			}
			else {
				rectangle(image, Point(120, 260 + 50 * i), Point(140, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
			}
		}
		if (x >= 170 && x <= 190 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			distance1[i] = (distance1[i] + 1) % 2;
			if (distance1[i] == 0) {
				rectangle(image, Point(170, 260 + 50 * i), Point(190, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
			}
			else {
				rectangle(image, Point(170, 260 + 50 * i), Point(190, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
			}
		}
		// team2
		if (x >= 1180 && x <= 1230 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			animation2[i] = (animation2[i] + 1) % 2;
			if (animation2[i] == 0) {
				speed2[i] = 0;
				distance2[i] = 0;
				rectangle(image, Point(1260, 260 + 50 * i), Point(1280, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(1310, 260 + 50 * i), Point(1330, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				rectangle(image, Point(1180, 260 + 50 * i), Point(1230, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
				plotTeam = 0;
				rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
				putText(image, "Play All", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
			}
			else {
				rectangle(image, Point(1180, 260 + 50 * i), Point(1230, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
				Mat result;
				result = plotEachPlayer(animation1, animation2, speed1, speed2, distance1, distance2, frame, vvsTrkNodeGlbList, stats);
				Mat image3;
				resize(result, image3, size);
				image = overlay(230, 200, image, image3);
			}
		}
		if (x >= 1260 && x <= 1280 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			speed2[i] = (speed2[i] + 1) % 2;
			if (speed2[i] == 0) {
				rectangle(image, Point(1260, 260 + 50 * i), Point(1280, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
			}
			else {
				rectangle(image, Point(1260, 260 + 50 * i), Point(1280, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
			}
		}
		if (x >= 1310 && x <= 1330 && y >= 260 + 50 * i && y <= 280 + 50 * i) {
			distance2[i] = (distance2[i] + 1) % 2;
			if (distance2[i] == 0) {
				rectangle(image, Point(1310, 260 + 50 * i), Point(1330, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
			}
			else {
				rectangle(image, Point(1310, 260 + 50 * i), Point(1330, 280 + 50 * i), Scalar(64, 64, 64), CV_FILLED);
			}
		}

	}
	if (forw) {
		rectangle(image, Point(500, 160), Point(550, 190), Scalar(64, 64, 64), CV_FILLED);
		line(image, Point(515, 175), Point(535, 185), Scalar(255, 255, 255), 2, 8, 0);
		line(image, Point(515, 175), Point(535, 165), Scalar(255, 255, 255), 2, 8, 0);

		rectangle(image, Point(640, 160), Point(690, 190), Scalar(0, 0, 0), CV_FILLED);
		line(image, Point(675, 175), Point(655, 185), Scalar(255, 255, 255), 2, 8, 0);
		line(image, Point(675, 175), Point(655, 165), Scalar(255, 255, 255), 2, 8, 0);
		dir = 1;
	}
	if (back) {
		dir = -1;
		rectangle(image, Point(500, 160), Point(550, 190), Scalar(0, 0, 0), CV_FILLED);
		line(image, Point(515, 175), Point(535, 185), Scalar(255, 255, 255), 2, 8, 0);
		line(image, Point(515, 175), Point(535, 165), Scalar(255, 255, 255), 2, 8, 0);

		rectangle(image, Point(640, 160), Point(690, 190), Scalar(64, 64, 64), CV_FILLED);
		line(image, Point(675, 175), Point(655, 185), Scalar(255, 255, 255), 2, 8, 0);
		line(image, Point(675, 175), Point(655, 165), Scalar(255, 255, 255), 2, 8, 0);
	}
	if (interp != -1) {
		rectangle(image, Point(760, 160), Point(860, 190), Scalar(255, 255, 255), CV_FILLED);
		putText(image, to_string(interp) + "-" + to_string(frame), Point(765, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 0, 0), 1, CV_AA);
	}
	Mat result;
	while (run && (frame < 3000) && (frame >= 0)) {
		result = plotEachPlayer(animation1, animation2, speed1, speed2, distance1, distance2, frame, vvsTrkNodeGlbList, stats);
		Mat image3;
		resize(result, image3, size);
		image = overlay(230, 200, image, image3);
		imshow("Display window", image);
		waitKey(1);
		if (interp != -1) {
			rectangle(image, Point(760, 160), Point(860, 190), Scalar(255, 255, 255), CV_FILLED);
			putText(image, to_string(interp) + "-" + to_string(frame), Point(765, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 0, 0), 1, CV_AA);
		}
		frame = frame + dir;
	}
	if (frame < 0 || frame>3000) {
		frame = 0;
		run = 0;
		rectangle(image, Point(560, 160), Point(630, 190), Scalar(64, 64, 64), CV_FILLED);
		line(image, Point(580, 170), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
		line(image, Point(580, 170), Point(610, 175), Scalar(0, 128, 0), 2, 8, 0);
		line(image, Point(610, 175), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
	}
}

// make the initial image for the animation page
void makeAnimationImage() {
	Size size(940, 500);
	image = imread(".\\bgImg\\start.jpg");
	// highlight the tab
	rectangle(image, Point(922, 10), Point(1145, 70), Scalar(255, 102, 51), CV_FILLED);
	putText(image, "Animation", Point(942, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	// button for stop
	rectangle(image, Point(60, 160), Point(130, 190), Scalar(64, 64, 64), CV_FILLED);
	rectangle(image, Point(90, 170), Point(100, 180), Scalar(0, 0, 128), CV_FILLED);
	// button for run/ pause
	rectangle(image, Point(560, 160), Point(630, 190), Scalar(64, 64, 64), CV_FILLED);
	// button for display all 
	rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
	putText(image, "Play All", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
	// button for backward
	rectangle(image, Point(500, 160), Point(550, 190), Scalar(64, 64, 64), CV_FILLED);
	line(image, Point(515, 175), Point(535, 185), Scalar(255, 255, 255), 2, 8, 0);
	line(image, Point(515, 175), Point(535, 165), Scalar(255, 255, 255), 2, 8, 0);
	// button for forward
	rectangle(image, Point(640, 160), Point(690, 190), Scalar(0, 0, 0), CV_FILLED);
	line(image, Point(675, 175), Point(655, 185), Scalar(255, 255, 255), 2, 8, 0);
	line(image, Point(675, 175), Point(655, 165), Scalar(255, 255, 255), 2, 8, 0);
	// button for error selection
	rectangle(image, Point(760, 160), Point(860, 190), Scalar(64, 64, 64), CV_FILLED);
	putText(image, "Select", Point(790, 180), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0, 128, 128), 1, CV_AA);
	// draw the play logo on the button
	line(image, Point(580, 170), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
	line(image, Point(580, 170), Point(610, 175), Scalar(0, 128, 0), 2, 8, 0);
	line(image, Point(610, 175), Point(580, 180), Scalar(0, 128, 0), 2, 8, 0);
	// overlay the soccer field
	Mat image2 = imread(".//bgImg//soccerField.png");
	Mat image3;
	resize(image2, image3, size);
	image = overlay(230, 200, image, image3);
	// write team1 and team2 on the page
	putText(image, "Team1", Point(90, 230), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Team2", Point(1230, 230), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	// draw the rectangle boxes for each player 
	for (int i = 0; i < 8; i++) {
		// draw boxes for team 1 
		rectangle(image, Point(40, 260 + 50 * i), Point(90, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		rectangle(image, Point(120, 260 + 50 * i), Point(140, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		rectangle(image, Point(170, 260 + 50 * i), Point(190, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		putText(image, "Player" + to_string(i + 1), Point(40, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);
		putText(image, "Speed", Point(115, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);
		putText(image, "Distance", Point(165, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);

		// draw boxes for team 2
		rectangle(image, Point(1180, 260 + 50 * i), Point(1230, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		rectangle(image, Point(1260, 260 + 50 * i), Point(1280, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		rectangle(image, Point(1310, 260 + 50 * i), Point(1330, 280 + 50 * i), Scalar(255, 255, 255), CV_FILLED);
		putText(image, "Player" + to_string(i + 1), Point(1180, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);
		putText(image, "Speed", Point(1255, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);
		putText(image, "Distance", Point(1305, 290 + 50 * i), FONT_HERSHEY_COMPLEX, 0.3, Scalar(255, 255, 255), 1, CV_AA);
	}
}

// control the display for the heat map
void heatmapControl(int x, int y, int event) {
	Size size(940, 500);
	for (int i = 0; i < 8; i++) {
		if (x < 270 && x>250 && y< 230 + 54 * i && y > 210 + 54 * i) {
			// reverse of what is chosen 
			heatmulti[i] = (heatmulti[i] + 1) % 2;
		}
	}
	// overlay the heat mat picture on top of the background image
	Mat image2 = heatmapGen(heatmulti, heatteam, vvsTrkNodeGlbList);
	cout << image2.rows << endl;
	Mat image3;

	resize(image2, image3, size);
	image = imread(".//bgImg//start.jpg");
	rectangle(image, Point(699, 10), Point(922, 70), Scalar(255, 102, 51), CV_FILLED);
	putText(image, "Heat Map", Point(719, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	image = overlay(230, 200, image, image3);
	// label team1 and team2
	if (heatteam == 1) {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(64, 64, 64), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(128, 0, 0), CV_FILLED);
	}
	else {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(128, 0, 0), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
	}
	putText(image, "Team1", Point(70, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Team2", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
}

// control the display for passing analysis
void passingAControl(int x, int y, int event) {
	image = imread(".\\bgImg\\start.jpg");
	rectangle(image, Point(1368, 10), Point(1147, 70), Scalar(255, 102, 51), CV_FILLED);
	putText(image, "Individual Data", Point(496, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Passing Analysis", Point(1165, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	// high light team1 and team2
	if (passingATeam == 1) {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(64, 64, 64), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(128, 0, 0), CV_FILLED);
	}
	else if (passingATeam == 2) {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(128, 0, 0), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
	}
	putText(image, "Team1", Point(70, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Team2", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	Mat image2 = imread(".\\graphicResults\\passingDataIm " + to_string(passingATeam) + ".png");
	Mat image3;
	Size size(1300, 550);
	resize(image2, image3, size);
	image = overlay(50, 200, image, image3);
}

// control the display for individual data
void dataControl(int x, int y, int event) {
	image = imread(".\\bgImg\\start.jpg");
	rectangle(image, Point(476, 10), Point(699, 70), Scalar(255, 102, 51), CV_FILLED);
	putText(image, "Individual Data", Point(496, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);

	// label team1 and team2
	if (datateam == 1) {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(64, 64, 64), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(128, 0, 0), CV_FILLED);
	}
	else if (datateam == 2) {
		rectangle(image, Point(60, 160), Point(130, 190), Scalar(128, 0, 0), CV_FILLED);
		rectangle(image, Point(180, 160), Point(250, 190), Scalar(64, 64, 64), CV_FILLED);
	}
	putText(image, "Team1", Point(70, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Team2", Point(190, 180), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	// draw the the three boxes for the three types of data
	rectangle(image, Point(30, 100), Point(170, 150), Scalar(64, 64, 64), CV_FILLED);
	rectangle(image, Point(180, 100), Point(320, 150), Scalar(64, 64, 64), CV_FILLED);
	// mark the three boxes
	putText(image, "General Data", Point(40, 140), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(image, "Passing Data", Point(190, 140), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	bool generalData = x <= 170 && x >= 30 && y <= 150 && y >= 100 && event == EVENT_LBUTTONDOWN;
	bool passingData = x <= 320 && x >= 180 && y <= 150 && y >= 100 && event == EVENT_LBUTTONDOWN;
	Mat image3;
	Size size(1100, 550);
	// update the status
	if (generalData) {
		general = 1;
		passing = 0;
	}
	if (passingData) {
		passing = 1;
		general = 0;
	}

	// choose to display general data
	if (general == 1) {
		rectangle(image, Point(30, 100), Point(170, 150), Scalar(255, 102, 51), CV_FILLED);
		putText(image, "General Data", Point(40, 140), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
		Mat image2 = imread(".\\graphicResults\\general" + to_string(datateam) + ".png");
		resize(image2, image3, size);
		image = overlay(150, 200, image, image3);
	}
	// choose to display passing data
	if (passing == 1) {
		rectangle(image, Point(180, 100), Point(320, 150), Scalar(255, 102, 51), CV_FILLED);
		putText(image, "Passing Data", Point(190, 140), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
		Mat image2 = imread(".\\graphicResults\\passing" + to_string(datateam) + ".png");
		resize(image2, image3, size);
		image = overlay(150, 200, image, image3);
	}
}

// overlay a certain image on top of the other
Mat overlay(int x, int y, Mat background, Mat over) {
	Mat im;
	background.copyTo(im);
	for (int i = 0; i < over.rows; i++) {
		for (int j = 0; j < over.cols; j++) {
			im.at<Vec3b>(i + y, j + x) = over.at<Vec3b>(i, j);
		}
	}
	return im;
}
String arraytostring(int* num) {
	string str = "";
	for (int i = 0; i < 8; i++) {
		str = str + to_string(num[7 - i]);
	}
	return str;
}

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
}

// create all the background pictures here
void makemenu() {
	// read in the background image
	Size size(1400, 750);
	Size sizeInitial(1340, 600);
	Mat src = imread(".\\bgImg\\background.jpg");
	Mat destination;
	resize(src, destination, size);
	imwrite(".\\bgImg\\official.jpg", destination);
	Mat initial(750, 1400, CV_8UC3, Scalar(234, 138, 28));
	Mat image3;
	resize(src, image3, sizeInitial);
	initial = overlay(30, 100, initial, image3);
	rectangle(initial, Point(30, 10), Point(1370, 70), Scalar(102, 0, 0), CV_FILLED);
	putText(initial, "Line-up", Point(50, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Team Data", Point(273, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Individual Data", Point(496, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Heat Map", Point(719, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Animation", Point(942, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Passing Analysis", Point(1165, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(initial, "Soccer Data Analysis", Point(240, 480), FONT_HERSHEY_COMPLEX, 2.3, Scalar(255, 255, 255), 2, CV_AA);
	putText(initial, "UW Information Processing Lab", Point(980, 600), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255, 255, 255), 1, CV_AA);
	imwrite(".\\bgImg\\initial.jpg", initial);
	Mat start = imread(".\\bgImg\\official.jpg");
	rectangle(start, Point(30, 10), Point(1370, 70), Scalar(102, 0, 0), CV_FILLED);
	putText(start, "Line-up", Point(50, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(start, "Team Data", Point(273, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(start, "Individual Data", Point(496, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(start, "Heat Map", Point(719, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(start, "Animation", Point(942, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	putText(start, "Passing Analysis", Point(1165, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 255, 255), 1, CV_AA);
	// background strip
	rectangle(start, Point(30, 150), Point(1370, 750), Scalar(255, 102, 51), CV_FILLED);
	rectangle(start, Point(30, 200), Point(1370, 750), Scalar(102, 0, 0), CV_FILLED);

	imwrite(".\\bgImg\\start.jpg", start);
}
int main() {

	//vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList, vector<vector<Team>> stats
	//vector<vector<Team>> stats = computeStats(vvsTrkNodeGlbList);
	int x = 1;
	vvsTrkNodeGlbList = readData();
	stats = computeStats(vvsTrkNodeGlbList);

	makeGeneralImage(1, vvsTrkNodeGlbList, stats);
	makeGeneralImage(2, vvsTrkNodeGlbList, stats);
	makePassingImage(1, stats);
	makePassingImage(2, stats);
	makeTeamDataImage(stats);
	makePassingAImage(stats, 1);
	makePassingAImage(stats, 2);
	makemenu();
	image = imread(".\\bgImg\\initial.jpg");
	namedWindow("Display window", WINDOW_NORMAL);
	resizeWindow("Display window", 1200, 750);
	setMouseCallback("Display window", CallBackFunc, NULL);
	waitKey(0);

	system("pause");
	return 0;
}