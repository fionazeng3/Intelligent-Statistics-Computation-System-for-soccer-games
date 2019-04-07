#include"linkItem.h"

// get the max speed of each player in each team
std::vector<float> getEachMaxSpeed(int endFrame, int teamN, vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList) {
	std::vector<float> result;
	// Note: num_player can be replaced by the global variable numPlayers
	int num_player = 8;
	int i = 1;
	for (i = 1; i <= num_player; i++) {
		float max = 0;
		int index = i;
		if (teamN == 2) {
			index += 8;
		}
		for (int j = 0; j < endFrame; j++) {
			float temp = vvsTrkNodeGlbList[index][j].fInstVel;
			if (temp > max && temp <= 12.5) {
				max = temp;
			}
		}
		result.push_back(max);
	}
	return result;
}

// get the rushing time of each player in each team
std::vector<int> getEachRushingTime(int endFrame, int teamN, vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList) {
	std::vector<int> result;
	// Note: num_player can be replaced by the global variable numPlayers
	int num_player = 8;
	int i = 1;
	for (i = 1; i <= num_player; i++) {
		bool isRushing = false;
		int rushTime = 0;
		int index = i;
		if (teamN == 2) {
			index += 8;
		}
		for (int j = 0; j < endFrame; j++) {
			float temp = vvsTrkNodeGlbList[index][j].fInstVel;
			// player starts to rush
			if (temp > 3 && !isRushing) {
				rushTime++;
				isRushing = true;
			}
			// player ends the rushing period
			else if (isRushing && temp < 3) {
				isRushing = false;
			}
		}
		result.push_back(rushTime);
	}
	return result;
}


// helper function to compute short/long pass, shots, possession rate and pass rate for each team and each player
void computeStats_helper(int f, int objectId, int p_player, Team& red, Team& blue, cv::Point2f p_position, vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList) {
	// add possession to corronsponding team and player
	if (objectId / 10 == 1) {
		// add possession to the red team, unit is frame
		int p_redPossession = red.getPossession();
		red.setPossession(p_redPossession + 1);
		// add possession to the player in the red team, unit is second
		std::vector<float> p_possession = red.getPossessionTimePlayers();
		p_possession[objectId % 10 - 1] = p_possession[objectId % 10 - 1] + 1.0 / 25;
		red.setPossessionTimePlayers(p_possession);
		// might have bugs 
	}
	else {
		// add possession to the blue team, unit is frame
		int p_bluePossession = blue.getPossession();
		blue.setPossession(p_bluePossession + 1);
		// add possession to the player in the blue team, unit is second
		std::vector<float> p_possession = blue.getPossessionTimePlayers();
		p_possession[objectId % 10 - 1] = p_possession[objectId % 10 - 1] + 1.0 / 25;
		blue.setPossessionTimePlayers(p_possession);
	}



	// compute long/short pass and shots if the current and previous player who possess the ball are different
	if (objectId != p_player) {
		//printf("previous player is: %d\n", p_player);
		//printf("current player is: %d\n", objectId);
		// if the player in red team pass the ball
		if (p_player / 10 == 1) {
			// add one shot if the goal keeper of blue team possess the ball
			// Note: Shooting has many cases, exp: attacker shoot -> outOFfield, attacker shoot -> defender save to outside, or attacker shoot -> goalkeeper clean the ball to outside or inside 
			//       For now, we only can detect cases that attacker shoot -> outOfField.
			// !!! To do: add previous player's position to give more accurate result of shots since shooting always happen around goal area
			// !!! To do: add player jump state to detect shooting with head
			// !!! To do: add cases when the goal keeper clean the shots outside the field or inside the field
			if (objectId % 10 == 1 && objectId / 10 == 2) {
				int o_shots = red.getShots();
				red.setShots(o_shots + 1);
				//std::printf("red team shot: %d!\n", red.getShots());
			}
			else {
				// add data to pass analysis
				vector<vector<int>> pass_red = red.getPassPlayers();
				pass_red[p_player % 10 - 1][objectId % 10 - 1] = pass_red[p_player % 10 - 1][objectId % 10 - 1] + 1;
				red.setPassPlayers(pass_red);

				// compute the distance between the previous player who pass the ball and the receiver player
				double d = cv::norm(p_position - vvsTrkNodeGlbList[objectId % 10][f].o2dPtFtGlb) / 1000.0;
				if (d < 0.0) {
					d = -d;
				}
				//printf("pass distance is: %0.2f\n", d);
				// Note(long-short pass distance threshold): 15 is manual distance threshold to determine whether a pass is a long pass or short pass
				//       if set to be smaller, there might be more long pass. If set to be bigger, there might be more short pass
				if (d < 15.0) {
					// add one short pass to the red team
					int o_shortPass = red.getShortPass();
					red.setShortPass(o_shortPass + 1);
					// add one short pass to the player in the red team
					std::vector<int> o_shortPassList = red.getShortPassPlayers();
					//printf("index is: %d\n", p_player % 10 - 1);
					o_shortPassList[p_player % 10 - 1] = o_shortPassList[p_player % 10 - 1] + 1;
					red.setShortPassPlayers(o_shortPassList);
					//printf("red team's shortPass: %d\n", red.getShortPass());
				}
				else {
					// add one long pass to the red team
					int o_longPass = red.getLongPass();
					red.setLongPass(o_longPass + 1);
					// add one long pass to the player in the red team
					std::vector<int> o_longPassList = red.getLongPassPlayers();
					o_longPassList[p_player % 10 - 1] = o_longPassList[p_player % 10 - 1] + 1;
					red.setLongPassesPlayers(o_longPassList);
					//printf("red team's longPass: %d\n", red.getLongPass());
				}

				// if pass to the right person, add successful pass
				if (objectId / 10 == 1) {
					if (d < 15.0) {
						// add one successful short pass to the red team
						int o_sucessShortPass = red.getSuccessShortPass();
						red.setSuccessShortPass(o_sucessShortPass + 1);
						// add one successful short pass to the player in the red team
						std::vector<int> o_successShortPassList = red.getSuccessShortPassPlayers();
						o_successShortPassList[p_player % 10 - 1] = o_successShortPassList[p_player % 10 - 1] + 1;
						red.setSuccessShortPassPlayers(o_successShortPassList);
					}
					else {
						// add one successful long pass to the red team
						int o_successLongPass = red.getSuccessLongPass();
						red.setSuccessLongPass(o_successLongPass + 1);
						// add one successful long pass to the player in the red team
						std::vector<int> o_successLongPassList = red.getSuccessLongPassPlayers();
						o_successLongPassList[p_player % 10 - 1] = o_successLongPassList[p_player % 10 - 1] + 1;
						red.setSuccessLongPassPlayers(o_successLongPassList);
					}
					//printf("red team's successShortPass: %d\n", red.getSuccessShortPass());
					//printf("red team's successLongPass: %d\n", red.getSuccessLongPass());
				}
			}
		}
		// if the player in blue team pass the ball
		else if (p_player / 10 == 2) {
			// add shots if the goal keeper if red team possess the ball
			if (objectId % 10 == 1 && objectId / 10 == 1) {
				int o_shots = blue.getShots();
				blue.setShots(o_shots + 1);
				//std::printf("blue team shot: %d!\n", blue.getShots());
			}
			else {
				// add data to pass analysis
				vector<vector<int>> pass_blue = blue.getPassPlayers();
				pass_blue[p_player % 10 - 1][objectId % 10 - 1] = pass_blue[p_player % 10 - 1][objectId % 10 - 1] + 1;
				blue.setPassPlayers(pass_blue);

				double d = cv::norm(p_position - vvsTrkNodeGlbList[objectId % 10 + 8][f].o2dPtFtGlb) / 1000.0;
				//printf("pass distance is: %0.2f\n", d);
				if (d < 0.0) {
					d = -d;
				}
				if (d < 15.0) {
					// add one short pass to the blue team
					int o_shortPass = blue.getShortPass();
					blue.setShortPass(o_shortPass + 1);
					// add one short pass to the player in the blue team
					std::vector<int> o_shortPassList = blue.getShortPassPlayers();
					o_shortPassList[p_player % 10 - 1] = o_shortPassList[p_player % 10 - 1] + 1;
					blue.setShortPassPlayers(o_shortPassList);
					//printf("blue team's shortPass: %d\n", blue.getShortPass());
				}
				else {
					// add one long pass to the blue team
					int o_longPass = blue.getLongPass();
					blue.setLongPass(o_longPass + 1);
					// add one long pass to the player in the blue team
					std::vector<int> o_longPassList = blue.getLongPassPlayers();
					o_longPassList[p_player % 10 - 1] = o_longPassList[p_player % 10 - 1] + 1;
					blue.setLongPassesPlayers(o_longPassList);
					//printf("blue team's longPass: %d\n", blue.getLongPass());
				}
				// if pass to the right person
				if (objectId / 10 == 2) {
					if (d < 15.0) {
						// add one successful short pass to the blue team
						int o_sucessShortPass = blue.getSuccessShortPass();
						blue.setSuccessShortPass(o_sucessShortPass + 1);
						// add one successful short pass to the player in the blue team
						std::vector<int> o_successShortPassList = blue.getSuccessShortPassPlayers();
						o_successShortPassList[p_player % 10 - 1] = o_successShortPassList[p_player % 10 - 1] + 1;
						blue.setSuccessShortPassPlayers(o_successShortPassList);
					}
					else {
						// add one successful long pass to the blue team
						int o_successLongPass = blue.getSuccessLongPass();
						blue.setSuccessLongPass(o_successLongPass + 1);
						// add one successful long pass to the player in the red team
						std::vector<int> o_successLongPassList = blue.getSuccessLongPassPlayers();
						o_successLongPassList[p_player % 10 - 1] = o_successLongPassList[p_player % 10 - 1] + 1;
						blue.setSuccessLongPassPlayers(o_successLongPassList);
					}
					//printf("blue team's successShortPass: %d\n", blue.getSuccessShortPass());
					//printf("blue team's successLongPass: %d\n", blue.getSuccessLongPass());
				}
			}
		}
	}
}

// main function to compute statistics such as short/long pass, shots, possession rate, passing rate, free kick, corner kick, penalty kick etc.
// All statistics are based on vvsTrkNodeGlbList, which is the tracking result
// vvsTrkNodeGlbList[i][j] denotes all tracking results for object i at frame j 
vector<vector<Team>> computeStats(vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList) {
	float fGlbFtPtStdThres = 5000.0f;
	// variables to calculate the ball's trajecotry 
	// slope of ball's trajactory for the current frame
	float c_slope = FLT_MAX;
	// slope of ball's trajactory for the previous frame
	float p_slope = FLT_MAX;
	// ball position change in x-axis for the current frame
	float c_deltaX = 0.0;
	// ball position change in y-axis for the current frame
	float c_deltaY = 0.0;
	// ball position change in x-axis for the previous frame
	float p_deltaX = c_deltaX;
	// ball position change in y-axis for the previous frame
	float p_deltaY = c_deltaY;

	// initialize team1
	Team red = Team(0);
	// initialize team2
	Team blue = Team(1);
	// initialize the ball state
	Ball ball = Ball(1);
	// redStat stores red team's statistics for each frame
	std::vector<Team> redStat;
	// blueStat stores blue team's statistics for each frame
	std::vector<Team> blueStat;
	// ball stores ball's states for each frame
	// std::vector<Ball> ballStat;

	// compute maximum speed, total rushing time and total running distance for each player offline
	std::vector<float> maxSpeed_red;
	std::vector<float> maxSpeed_blue;
	std::vector<int> rushingTime_red;
	std::vector<int> rushingTime_blue;
	std::vector<float> totalDistance_red;
	std::vector<float> totalDistance_blue;
	// Note: 3000 can be replaced by a global variable nFrames 
	maxSpeed_red = getEachMaxSpeed(3000, 1, vvsTrkNodeGlbList);
	red.setMaxSpeedPlayers(maxSpeed_red);
	maxSpeed_blue = getEachMaxSpeed(3000, 2, vvsTrkNodeGlbList);
	blue.setMaxSpeedPlayers(maxSpeed_blue);
	rushingTime_red = getEachRushingTime(3000, 1, vvsTrkNodeGlbList);
	red.setRushingTimePlayers(rushingTime_red);
	rushingTime_blue = getEachRushingTime(3000, 2, vvsTrkNodeGlbList);
	blue.setRushingTimePlayers(rushingTime_blue);


	// compute free kick, penalty kick and corner kick for each team 
	int samePosition = 0;
	// Note: 2999 can be replaced by a global variable numFrames - 1
	for (int frame = 1; frame < 2999; frame++) {
		double positionChange = cv::norm(vvsTrkNodeGlbList[0][frame - 1].o2dPtFtGlb - vvsTrkNodeGlbList[0][frame].o2dPtFtGlb);
		//printf("frame: %d, positionChange: %0.2f\n", frame, positionChange);
		if (positionChange < 0) {
			positionChange = -positionChange;
		}
		// Note(free kick/penalty kick/corner kick position change threshold): 30 is a manual distance threshold to detect whether the ball change its position from previous frame
		//       if it set to be smaller, there might be more missing cases. If it set to be bigger, there might be more wrong cases.
		if (positionChange <= 30.0 && vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.x >= 0 && vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.x <= 46000 && vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.y >= 0 && vvsTrkNodeGlbList[0][frame].o2dPtFtGlb.y <= 32000) {
			samePosition = samePosition + 1;
		}
		else {
			samePosition = 0;
		}
		//printf("samePosition: %d\n", samePosition);

		// Assume any free kick, penalty kick and corner kick will put the ball in the same position for at least 2 seconds
		// Note1: 25 is nFps, replace it with nFps when nFps is a global variable 
		// Note2(free kick/penalty kick/corner kick time threshold): 2 seconds is a manual time threshold, can be changed if trigger wrong statistics, 
		//            if it set to be smaller, then there might be more wrong cases. If it set to be bigger, then there might be more missing cases.
		if (samePosition >= 2 * 25 && cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - vvsTrkNodeGlbList[0][frame + 1].o2dPtFtGlb) > 30.0) {
			// define 4 corner points
			// Note: replace all specific positions with field width and length and proportion
			cv::Point2f corner1(0.0f, 0.0f);
			cv::Point2f corner2(0.0f, 32000.0f);
			cv::Point2f corner3(46000.0f, 0.0f);
			cv::Point2f corner4(46000.0f, 32000.0f);
			cv::Point2f penaltyRed(5300.0f, 16000.0f);
			cv::Point2f penaltyBlue(40700.0f, 16000.0f);
			cv::Point2f central(23000.0f, 16000.0f);

			// Note1(free kick/penalty kick/corner kick position distance threshold): 20 is a manual distance threshold to determine whether the ball is located on some certain point
			//            if it set to be smaller, then there might be more missing cases. If it set to be bigger, then there might be more wrong cases. 
			if (cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - corner3) <= 20.0 || cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - corner4) <= 20.0) {
				int cornerRed = red.getCornerKick();
				red.setCornerKick(cornerRed + 1);
				//printf("red team corner kick: %d", cornerRed + 1);
			}
			else if (cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - corner1) <= 20.0 || cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - corner2) <= 20.0) {
				int cornerBlue = blue.getCornerKick();
				blue.setCornerKick(cornerBlue + 1);
				//printf("blue team corner kick: %d", cornerBlue + 1);
			}
			else if (cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - penaltyRed) <= 20.0) {
				int penaltyBlue = blue.getPenaltyKick();
				blue.setPenaltyKick(penaltyBlue + 1);
				//printf("blue team penalty kick: %d", penaltyBlue + 1);
			}
			else if (cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - penaltyBlue) <= 20.0) {
				int penaltyRed = red.getPenaltyKick();
				red.setPenaltyKick(penaltyRed + 1);
				//printf("red team penalty kick: %d", penaltyRed + 1);
			}
			else {
				double min_distance = 1000000.0;
				int objectId = 0;
				// cumpute the minimum distance between the ball and player
				for (int o = 1; o <= 16; o++) {
					double distance = cv::norm(vvsTrkNodeGlbList[o][frame].o2dPtFtGlb - vvsTrkNodeGlbList[0][frame].o2dPtFtGlb) / 1000.0;
					if (distance < 0.0) {
						distance = -distance;
					}
					if (distance < min_distance) {
						min_distance = distance;
						objectId = vvsTrkNodeGlbList[o][frame].nObjId;
					}
				}
				// if the ball is put in the central point, add goals
				// Note1: We might need to change the way to compute goals 
				//            since in this way we cannot tell which player kick this goal
				// Note2: 20 is a manual distance threshold to determine whether the ball is located on some certain point
				//        can be changed if it trigger wrong statistics
				if (cv::norm(vvsTrkNodeGlbList[0][frame].o2dPtFtGlb - central) <= 20.0) {
					if (objectId / 10 == 1) {
						int goalBlue = blue.getGoals();
						blue.setGoals(goalBlue + 1);
						//printf("blue team goals: %d", goalBlue + 1);
					}
					else {
						int goalRed = red.getGoals();
						red.setGoals(goalRed + 1);
						//printf("red team goals: %d", goalRed + 1);
					}
				}
				// if the ball is put in other locations, add free kicks
				else {
					// if it is goalkeeper who kick the ball out, it is not a free kick
					if (objectId % 10 != 1) {
						if (objectId / 10 == 1) {
							int freeRed = red.getFreeKick();
							red.setFreeKick(freeRed + 1);
							//printf("red team free kick: %d", freeRed + 1);
						}
						else {
							int freeBlue = blue.getGoals();
							blue.setFreeKick(freeBlue + 1);
							//printf("blue team free kick: %d", freeBlue + 1);
						}
					}
				}
			}
		}
	}


	// compute sum of distance for each player in red team 
	// Note: 1 and 8 can be changed by global variable teamIdStart and teamIdEnd
	for (int index = 1; index <= 8; index++) {
		totalDistance_red.push_back(vvsTrkNodeGlbList[index][2999].fDistSum);
	}
	red.setTotalDistancePlayers(totalDistance_red);

	// compute sum of distance for each player in blue team offline
	// Note: 9 and 16 can be changed by global variable (teamIdStart + num_players) and (team2IdEnd + num_players)
	for (int index = 9; index <= 16; index++) {
		totalDistance_blue.push_back(vvsTrkNodeGlbList[index][2999].fDistSum);
	}
	blue.setTotalDistancePlayers(totalDistance_blue);

	// the possession time(s) for each player in red team
	std::vector<float> possession_red;
	// possession time(s) for each player in blue team
	std::vector<float> possession_blue;

	// the number of short pass for each player in red team
	std::vector<int> shortPass_red;
	// the number of successful short pass for each player in red team
	std::vector<int> shortPass_red_success;
	// the number of long pass for each player in red team
	std::vector<int> longPass_red;
	// the number of successful long pass for each player in red team
	std::vector<int> longPass_red_success;

	// the number of short pass for each player in blue team
	std::vector<int> shortPass_blue;
	// the number of successful short pass for each player in blue team
	std::vector<int> shortPass_blue_success;
	// the number of long pass for each player in blue team
	std::vector<int> longPass_blue;
	// the number of successful long pass for each player in blue team
	std::vector<int> longPass_blue_success;
	// for pass analysis
	vector<int> passes_red;
	vector<int> passes_blue;
	std::vector<vector<int>> pass_players_red;
	std::vector<vector<int>> pass_players_blue;

	// initialize all vectors to compute other statistics in red and blue team
	// Note: 0 ~ 8 can be replaced by 0 ~ numPlayers 
	for (int i = 0; i < 8; i++) {
		possession_red.push_back(0.0);
		shortPass_red.push_back(0);
		shortPass_red_success.push_back(0);
		longPass_red.push_back(0);
		longPass_red_success.push_back(0);

		possession_blue.push_back(0.0);
		shortPass_blue.push_back(0);
		shortPass_blue_success.push_back(0);
		longPass_blue.push_back(0);
		longPass_blue_success.push_back(0);

		passes_red.push_back(0);
		passes_blue.push_back(0);
	}

	// Note: 0 ~ 8 can be replaced by 0 ~ numPlayers 
	for (int i = 0; i < 8; i++) {
		pass_players_red.push_back(passes_red);
		pass_players_blue.push_back(passes_blue);
	}

	red.setPossessionTimePlayers(possession_red);
	red.setShortPassPlayers(shortPass_red);
	red.setSuccessShortPassPlayers(shortPass_red_success);
	red.setLongPassesPlayers(longPass_red);
	red.setSuccessLongPassPlayers(longPass_red_success);
	red.setPassPlayers(pass_players_red);

	blue.setPossessionTimePlayers(possession_blue);
	blue.setShortPassPlayers(shortPass_blue);
	blue.setSuccessShortPassPlayers(shortPass_blue_success);
	blue.setLongPassesPlayers(longPass_blue);
	blue.setSuccessLongPassPlayers(longPass_blue_success);
	blue.setPassPlayers(pass_players_blue);



	// compute statistics based on ball's state transition frame by frame
	// ball state: Pass 0; Possesion 1; OutOfField 2
	std::printf("begin stats:\n");
	// for each frame
	// Note: 3000 can be replaced by the global variable numFrames
	for (int f = 0; f < 3000; f++) {
		double min_distance = 1000000.0;
		int objectId = 0;
		// cumpute the minimum distance the ball and all players
		// Note: 1 ~ 16 can replaced by 1 ~ numPlayers * 2
		for (int o = 1; o <= 16; o++) {
			double distance = cv::norm(vvsTrkNodeGlbList[o][f].o2dPtFtGlb - vvsTrkNodeGlbList[0][f].o2dPtFtGlb) / 1000.0;
			if (distance < 0.0) {
				distance = -distance;
			}
			if (distance < min_distance) {
				min_distance = distance;
				objectId = vvsTrkNodeGlbList[o][f].nObjId;
			}
		}
		//printf("frame is: %d and min_distance is: %0.5f and cloest object is %d\n", f, min_distance, objectId);

		// get the previous state of the ball
		int p_ballState = ball.getState();
		// get the slope of ball's trajectory
		if (f > 1) {
			p_deltaX = vvsTrkNodeGlbList[0][f - 1].o2dPtFtGlb.x - vvsTrkNodeGlbList[0][f - 2].o2dPtFtGlb.x;
			p_deltaY = vvsTrkNodeGlbList[0][f - 1].o2dPtFtGlb.y - vvsTrkNodeGlbList[0][f - 2].o2dPtFtGlb.y;
			c_deltaX = vvsTrkNodeGlbList[0][f].o2dPtFtGlb.x - vvsTrkNodeGlbList[0][f - 1].o2dPtFtGlb.x;
			c_deltaY = vvsTrkNodeGlbList[0][f].o2dPtFtGlb.y - vvsTrkNodeGlbList[0][f - 1].o2dPtFtGlb.y;
			if (p_deltaX != 0) {
				p_slope = p_deltaY / p_deltaX;
			}
			else {
				c_slope = FLT_MAX;
			}
			if (c_deltaX != 0) {
				c_slope = c_deltaY / c_deltaX;
			}
			else {
				c_slope = FLT_MAX;
			}
		}
		//printf("ball's p_lope is: %0.2f; c_slope is: %0.2f\n", p_slope, c_slope);

		// objectIndex is the index in vvsTrkNodeGlbList for that object
		// Note: For game of 8 players, ObjectId -> ObjectIndex map: 11 -> 1, 12 -> 2 ... 21 -> 9, 22 -> 10
		//       The mapping will be changed if it is a game of 11 players
		int objectIndex = objectId % 10;
		if (objectId / 10 == 2) {
			objectIndex += 8;
		}

		// compute ball speed change
		float delta_vBall = 0.0;
		if (f > 0) {
			delta_vBall = vvsTrkNodeGlbList[0][f].fInstVel - vvsTrkNodeGlbList[0][f - 1].fInstVel;
		}
		// printf("player speed: %0.2f; ball speed: %0.2f\n", vvsTrkNodeGlbList[objectIndex][f].fInstVel, vvsTrkNodeGlbList[0][f].fInstVel);

		// ball is in Possession state if:
		//          case1. Ball is not flying and the ball-player distance(min_distance) is very small(In most cases, when the ball-player distance is very small, the player is possessing the ball)
		//          case2. Ball is not flying and the ball-player distance(min_distance) is small and ball trajectory change(The player pass the ball without stopping it)
		//          case3. The ball-player distance(min_distance) is relatively small and Ball's velocity is small (The player stop the ball and pass the ball to others)
		// we can compute pass/shots statistics whenever we detect a new Possession state for the ball
		// !!! To do: detect player shoot by head, when the player jump and the ball is flying and their distance is small enough, it is also a possesion state

		// Note1(ball_player distance threshold1): 0.5 is a manual ball-player distance threshold, if it set to be bigger, there might be more wrong passes, it it set to be smaller, there might be more missing passes
		// Note2(ball-player distance threshold2): 0.7 is a manual ball-player distance threshold, if it set to be bigger, there might be more wrong passes, it it set to be smaller, there might be more missing passes
		// Note3: p_deltaX * c_deltaX < 0 || p_deltaY *c_deltaY < 0 is the condition I used to detect whether ball's trajactory change, it works in the 2-minutes video, we might need to change or optimize the condition to detect ball's trajectory change
		// Note4(ball_player distance threshold3): 1.4 is a manual ball-player distance threshold, if it set to be bigger, there might be more wrong passes, it it set to be smaller, there might be more missing passes
		// Note5(ball velocity threshold): 2.9 is a manual ball velocity threshold, if it set to be bigger, there might be more wrong passes, it it set to be smaller, there might be more missing passes
		if ((min_distance <= 0.5 && (vvsTrkNodeGlbList[0][f].fGlbFtPtStd <= fGlbFtPtStdThres)) || (min_distance <= 0.7 && (p_deltaX * c_deltaX < 0 || p_deltaY *c_deltaY < 0) && vvsTrkNodeGlbList[0][f].fGlbFtPtStd <= fGlbFtPtStdThres) || (min_distance <= 1.4 && vvsTrkNodeGlbList[0][f].fInstVel < 2.9)) {
			//printf("frame: %d  ball is in possession state:\n", f);
			if (min_distance <= 0.7) {
				//printf("ball and player %d has very close distance\n", objectId);
			}
			else {
				//printf("player %d is close to the ball and ball is slow", objectId);
				//printf("player speed : %0.2f; ball speed : %0.2f\n", vvsTrkNodeGlbList[objectIndex][f].fInstVel, vvsTrkNodeGlbList[0][f].fInstVel);
			}

			// get the player ID who previously possess the ball
			int p_player = ball.getPlayerP();
			// get the player's position who previouly possess the ball
			cv::Point2f p_position = ball.getPositionP();

			computeStats_helper(f, objectId, p_player, red, blue, p_position, vvsTrkNodeGlbList);

			// update the ball stats
			ball.setState(1);
			ball.setPlayerP(objectId);
			if (objectId / 10 == 2) {
				ball.setPositionP(vvsTrkNodeGlbList[objectId % 10 + 8][f].o2dPtFtGlb);
			}
			else {
				ball.setPositionP(vvsTrkNodeGlbList[objectId % 10][f].o2dPtFtGlb);
			}
		}
		// ball is in outOfField state if the ball is out of border
		// Note: 46000, 32000 can be replaced by global variable fieldLength and fieldWidth
		else if (vvsTrkNodeGlbList[0][f].o2dPtFtGlb.x >= 46000.0 || vvsTrkNodeGlbList[0][f].o2dPtFtGlb.x <= 0.0 || vvsTrkNodeGlbList[0][f].o2dPtFtGlb.y <= 0.0 || vvsTrkNodeGlbList[0][f].o2dPtFtGlb.y >= 32000.0) {
			ball.setState(2);
			//printf("ball is in outOfField state\n", f);
		}
		// ball is in Pass state in other cases
		// Note: There are some cases that ball-player distance is large when the player pass the ball without stopping it
		//          we should add this case to possession state, otherwise we will missing some passes
		//        But it will generate some wrong pass because of the irregular ball trajectory 
		//       we can remove this case if the tracking result is precise in this case
		else {
			// if meet the conditions:
			//          1. ball's previous state is passing
			//          2. ball's trajactory change, exp: absolute value of slope change in 20(can be changed or deleted) and c_deltaX * p_deltaX < 0 || c_deltaY * p_deltaY < 0
			//          3. player-ball distance within [1.3, 2.1]
			//          4. velocity of the ball changes(delta_vBall >= 0.03)
			// then it is a case where the player pass the ball without stopping it
			if (p_ballState == 0 && fabs(fabs(c_slope) - fabs(p_slope)) < 20.0 && min_distance <= 2.1 && min_distance >= 1.3 && vvsTrkNodeGlbList[0][f].fInstVel >= 4.2) {
				if ((c_deltaX * p_deltaX < 0 || c_deltaY * p_deltaY < 0) && fabs(delta_vBall) >= 0.03) {
					//printf("ball is in possession state\n");
					//printf("ball trajactory change, player %d pass the ball\n", objectId);
					// get the player ID who previously possess the ball
					int p_player = ball.getPlayerP();
					// get the player's position who previouly possess the ball
					cv::Point2f p_position = ball.getPositionP();

					computeStats_helper(f, objectId, p_player, red, blue, p_position, vvsTrkNodeGlbList);

					// set possession state to the player who pass the ball
					ball.setState(1);
					ball.setPlayerP(objectId);
					if (objectId / 10 == 2) {
						ball.setPositionP(vvsTrkNodeGlbList[objectId % 10 + 8][f].o2dPtFtGlb);
					}
					else {
						ball.setPositionP(vvsTrkNodeGlbList[objectId % 10][f].o2dPtFtGlb);
					}
				}
			}
			// if ball is in passing state and ball-player has similar speed, the player is dribbling
			else if (objectId == ball.getPlayerP() && fabs(vvsTrkNodeGlbList[objectIndex][f].fInstVel - vvsTrkNodeGlbList[0][f].fInstVel) <= 0.2) {
				//printf("ball is in possesion state\n");
				//printf("player %d is dribbling!\n", objectId);

				// add possession to corronsponding team
				if (objectId / 10 == 1) {
					int p_redPossession = red.getPossession();
					red.setPossession(p_redPossession + 1);
					std::vector<float> p_possession = red.getPossessionTimePlayers();
					p_possession[objectId % 10 - 1] = p_possession[objectId % 10 - 1] + 1.0 / 25;
					red.setPossessionTimePlayers(p_possession);
				}
				else {
					int p_bluePossession = blue.getPossession();
					blue.setPossession(p_bluePossession + 1);
					std::vector<float> p_possession = blue.getPossessionTimePlayers();
					p_possession[objectId % 10 - 1] = p_possession[objectId % 10 - 1] + 1.0 / 25;
					blue.setPossessionTimePlayers(p_possession);
				}

				// set possession state to the player who pass the ball
				ball.setState(1);
				ball.setPlayerP(objectId);
				if (objectId / 10 == 2) {
					ball.setPositionP(vvsTrkNodeGlbList[objectId % 10 + 8][f].o2dPtFtGlb);
				}
				else {
					ball.setPositionP(vvsTrkNodeGlbList[objectId % 10][f].o2dPtFtGlb);
				}
			}
			else {
				// printf("ball is in passing state\n", f);
				ball.setState(0);
			}
		}


		redStat.push_back(red);
		blueStat.push_back(blue);
	}

	// store the result
	// result[0] denotes red team's statistics for each frame, result[1] denotes blue team's statistics for each frame
	vector<vector<Team>> result;
	result.push_back(redStat);
	result.push_back(blueStat);



	// for test
	std::vector<int> shortpasses_red = redStat[2999].getShortPassPlayers();
	std::vector<int> longpasses_red = redStat[2999].getLongPassPlayers();
	std::vector<int> shortpasses_blue = blueStat[2999].getShortPassPlayers();
	std::vector<int> longpasses_blue = blueStat[2999].getLongPassPlayers();
	std::vector<int> success_short_red = redStat[2999].getSuccessShortPassPlayers();
	std::vector<int> success_short_blue = blueStat[2999].getSuccessShortPassPlayers();
	std::vector<int> success_long_red = redStat[2999].getSuccessLongPassPlayers();
	std::vector<int> success_long_blue = blueStat[2999].getSuccessLongPassPlayers();
	std::vector<int> rushingtime_red = redStat[2999].getRushingTimePlayers();
	std::vector<int> rushingtime_blue = blueStat[2999].getRushingTimePlayers();
	std::vector<float> possession_reds = redStat[2999].getPossessionTimePlayers();
	std::vector<float> possession_blues = blueStat[2999].getPossessionTimePlayers();
	int redTotalPass = redStat[2999].getSuccessLongPass() + redStat[2999].getSuccessShortPass();
	int blueTotalPass = blueStat[2999].getSuccessLongPass() + blueStat[2999].getSuccessShortPass();
	// test statistics for each team
	std::printf("red team total short pass: %d\n", redStat[2999].getShortPass());
	std::printf("red team total long pass: %d\n", redStat[2999].getLongPass());
	std::printf("red team total success pass: %d\n", redTotalPass);
	std::printf("blue team total short pass: %d\n", blueStat[2999].getShortPass());
	std::printf("blue team total long pass: %d\n", blueStat[2999].getLongPass());
	std::printf("blue team total success pass: %d\n", blueTotalPass);
	std::printf("red team total long pass: %d\n", blueStat[2999].getLongPass());
	// test statistics for each player
	for (int i = 0; i < 8; i++) {
		std::printf("red player %d SP: %d, SSP: %d, LP: %d, SLP: %d, rushing: %d, possession: %0.2f\n", i, shortpasses_red[i], success_short_red[i], longpasses_red[i], success_long_red[i], rushingtime_red[i], possession_reds[i]);
		std::printf("blue player %d SP: %d, SSP: %d, LP: %d, SLP: %d, rushing: %d, possession: %0.2f\n", i, shortpasses_blue[i], success_short_blue[i], longpasses_blue[i], success_long_blue[i], rushingtime_blue[i], possession_blues[i]);
	}
	vector<vector<int>> red_pass = redStat[2999].getPassPlayers();
	vector<vector<int>> blue_pass = blueStat[2999].getPassPlayers();
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("red team player %d pass to player %d %d times\n", i + 1, j + 1, red_pass[i][j]);
			printf("blue team player %d pass to player %d %d time\n", i + 1, j + 1, blue_pass[i][j]);
		}
	}

	return result;

}

/*
Configuration parameters:
1. ball_player distance threshold1(0.5), 
2. ball-player distance threshold2(0.7) 
3. ball_player distance threshold3(1.4) 
4. ball velocity threshold(2.9)
5. free kick/penalty kick/corner kick time threshold(2 seconds)
6. free kick/penalty kick/corner kick position distance threshold(20)
7. free kick/penalty kick/corner kick position change threshold(30)
8. long-short pass distance threshold(15)

*/