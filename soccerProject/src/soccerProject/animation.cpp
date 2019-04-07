#include "linkItem.h"

// plot the location of each player in each team

Mat plotEachPlayer(int player1[8], int player2[8], int speed1[8], int speed2[8],
	int distance1[8], int distance2[8], int frame, vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList, vector<vector<Team>> stats) {
	// !!! To do
	//vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList = readData();
	//vector<vector<Team>> stats = computeStats(vvsTrkNodeGlbList);
	int counter = 0;
	for (int i = 0; i < 8; i++) {
		if (player1[i] == 0) {
			counter++;
		}
		if (player2[i] == 0) {
			counter++;
		}
	}
	Mat backgroundImg;
	string filename = ".\\bgImg\\soccerField.png";
	backgroundImg = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
	//write frame number at the top left corner
	char frameCount[32];
	sprintf_s(frameCount, "Frame No.:  %d", frame);
	putText(backgroundImg, frameCount, Point(50, 50),
		FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0, 0), 2, CV_AA, false);
	Point playerCircle;
	int nFtPtCirRad = 30;
	// check for players for team1
	for (int i = 0; i < 8; i++) {
		if (player1[i] == 1) {
			// convert the 3D data to be plotted on the frame image
			playerCircle.x = 155 + (1705 * (vvsTrkNodeGlbList[i + 1][frame].o2dPtFtGlb.x / 46000.0f));
			playerCircle.y = 125 + (1133 * (vvsTrkNodeGlbList[i + 1][frame].o2dPtFtGlb.y / 32000.0f));
			// plot the position of each player (with the player ID)
			circle(backgroundImg, playerCircle, nFtPtCirRad, cv::Scalar(0, 0, 204), 2);
			circle(backgroundImg, playerCircle, nFtPtCirRad, Scalar(255, 255, 255), CV_FILLED, 8, 0);
			char playerID[32];
			sprintf_s(playerID, "%d", (i + 1));
			putText(backgroundImg, playerID, Point((playerCircle.x - nFtPtCirRad + 10), (playerCircle.y + nFtPtCirRad - 10)),
				FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 204), 2, CV_AA, false);
			// plot the instaneous velocity of each player if chosen to show
			int nVdoFps = 25;
			if (frame >= nVdoFps - 1 && speed1[i] == 1) {
				char instantVelocity[32];
				float speed = vvsTrkNodeGlbList[i + 1][frame].fInstVel;
				sprintf_s(instantVelocity, "%.2fm/s", speed);
				if (speed > 3) {
					putText(backgroundImg, instantVelocity, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y - nFtPtCirRad)),
						FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(0, 0, 204), 2, CV_AA, false);
				}
				else {
					putText(backgroundImg, instantVelocity, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y - nFtPtCirRad)),
						FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 204), 2, CV_AA, false);
				}
			}
			// plot the trajactory of the player
			if (counter != 0) {
				for (int k = frame; k > 0; k--) {
					cv::line(backgroundImg, Point((155 + (1705 * (vvsTrkNodeGlbList[i + 1][k].o2dPtFtGlb.x / 46000.0f))), (125 + (1133 * (vvsTrkNodeGlbList[i + 1][k].o2dPtFtGlb.y / 32000.0f)))),
						Point((155 + (1705 * (vvsTrkNodeGlbList[i + 1][k - 1].o2dPtFtGlb.x / 46000.0f))), (125 + (1133 * (vvsTrkNodeGlbList[i + 1][k - 1].o2dPtFtGlb.y / 32000.0f)))),
						Scalar(0, 0, 204), 3);
				}
			}
			// plot the distance of each player
			if (distance1[i] == 1) {
				vector<float> totalDistance = stats[0][frame].getTotalDistancePlayers();
				char distanceSum[32];
				sprintf_s(distanceSum, "%.2fm", totalDistance[i]);
				putText(backgroundImg, distanceSum, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y + nFtPtCirRad + 15)), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 0, 204), 2, CV_AA, false);
			}
		}
	}
	//-----------------------------------------------------------------------------------------------------------------------------
	// check for players for team2
	for (int i = 0; i < 8; i++) {
		if (player2[i] == 1) {
			// convert the 3D data to be plotted on the frame image
			playerCircle.x = 155 + (1705 * (vvsTrkNodeGlbList[i + 9][frame].o2dPtFtGlb.x / 46000.0f));
			playerCircle.y = 125 + (1133 * (vvsTrkNodeGlbList[i + 9][frame].o2dPtFtGlb.y / 32000.0f));
			// plot the position of each player (with the player ID)
			circle(backgroundImg, playerCircle, nFtPtCirRad, cv::Scalar(153, 0, 0), 2);
			circle(backgroundImg, playerCircle, nFtPtCirRad, Scalar(255, 255, 255), CV_FILLED, 8, 0);
			char playerID[32];
			sprintf_s(playerID, "%d", (i + 1));
			putText(backgroundImg, playerID, Point((playerCircle.x - nFtPtCirRad + 10), (playerCircle.y + nFtPtCirRad - 10)),
				FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(153, 0, 0), 2, CV_AA, false);
			// plot the instaneous velocity of each player if chosen to show
			int nVdoFps = 25;
			if (frame >= nVdoFps - 1 && speed2[i] == 1) {
				char instantVelocity[32];
				float speed = vvsTrkNodeGlbList[i + 9][frame].fInstVel;
				sprintf_s(instantVelocity, "%.2fm/s", speed);
				if (speed > 3) {
					putText(backgroundImg, instantVelocity, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y - nFtPtCirRad)),
						FONT_HERSHEY_SIMPLEX, 2.0, cv::Scalar(153, 0, 0), 2, CV_AA, false);
				}
				else {
					putText(backgroundImg, instantVelocity, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y - nFtPtCirRad)),
						FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(153, 0, 0), 2, CV_AA, false);
				}
			}
			// plot the trajactory of the player
			if (counter != 0) {
				for (int k = frame; k > 0; k--) {
					cv::line(backgroundImg, Point((155 + (1705 * (vvsTrkNodeGlbList[i + 9][k].o2dPtFtGlb.x / 46000.0f))), (125 + (1133 * (vvsTrkNodeGlbList[i + 9][k].o2dPtFtGlb.y / 32000.0f)))),
						Point((155 + (1705 * (vvsTrkNodeGlbList[i + 9][k - 1].o2dPtFtGlb.x / 46000.0f))), (125 + (1133 * (vvsTrkNodeGlbList[i + 9][k - 1].o2dPtFtGlb.y / 32000.0f)))),
						Scalar(153, 0, 0), 3);
				}
			}
			// plot the distance of each player
			if (distance2[i] == 1) {
				vector<float> totalDistance = stats[0][frame].getTotalDistancePlayers();
				char distanceSum[32];
				sprintf_s(distanceSum, "%.2fm", totalDistance[i]);
				putText(backgroundImg, distanceSum, Point((playerCircle.x - nFtPtCirRad), (playerCircle.y + nFtPtCirRad + 15)), FONT_HERSHEY_SIMPLEX, 2, Scalar(153, 0, 0), 1, CV_AA, false);
			}
		}
	}
	// check if or not to display the ball
	if (counter == 0) {
		playerCircle.x = 155 + (1705 * (vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.x / 46000.0f));
		playerCircle.y = 125 + (1133 * (vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.y / 32000.0f));
		cv::circle(backgroundImg, playerCircle, nFtPtCirRad / 2, cv::Scalar(0, 0, 0, 0), -1);
	}
	return backgroundImg;
}