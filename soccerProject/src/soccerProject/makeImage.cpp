#include "linkItem.h"

// make the image showing basic informations of a certain team
void makeGeneralImage(int teamN, vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList, vector<vector<Team>> stats) {
	vector<float> maxSpeed = getEachMaxSpeed(3000, teamN, vvsTrkNodeGlbList);
	vector<float> totalDistance;
	for (int i = 1; i <= 8; i++) {
		if (teamN == 1) {
			totalDistance.push_back(vvsTrkNodeGlbList[i][2999].fDistSum);
		}
		else {
			totalDistance.push_back(vvsTrkNodeGlbList[i + 8][2999].fDistSum);
		}
	}

	vector<int> rushingTime = getEachRushingTime(3000, teamN, vvsTrkNodeGlbList);
	vector<float> pocessionTime = stats[teamN - 1][2999].getPossessionTimePlayers();

	// create a new image
	Mat playerInfo1(550, 1100, CV_8UC3, Scalar(100, 10, 10));
	rectangle(playerInfo1, Point(50, 50), Point(1050, 550), Scalar(255, 102, 51), CV_FILLED);
	double location1 = 105;
	for (int i = 0; i < 4; i++) {
		rectangle(playerInfo1, Point(50, location1), Point(1050, location1 + 55), Scalar(255, 204, 153), CV_FILLED);
		location1 += 110;
	}
	putText(playerInfo1, "Team" + to_string(teamN) + " Individual General Information", Point(50, 30), 5, 1, Scalar(255, 255, 255), 1, 8, false);
	putText(playerInfo1, "Player", Point(50, 100), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
	putText(playerInfo1, "Total Running Distance(m)", Point(150, 100), 4, 0.5, Scalar(0, 0, 0), 1.7, CV_AA, false);
	putText(playerInfo1, "Max Speed(m/s)", Point(440, 100), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
	putText(playerInfo1, "Rushing Times", Point(630, 100), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
	putText(playerInfo1, "Possession Time(s)", Point(810, 100), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
	double textLocation1 = 146;
	for (int i = 1; i <= 8; i++) {
		putText(playerInfo1, to_string(i), Point(50, textLocation1), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
		putText(playerInfo1, to_string(totalDistance[i - 1]), Point(150, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		putText(playerInfo1, to_string(maxSpeed[i - 1]), Point(440, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		putText(playerInfo1, to_string(rushingTime[i - 1]), Point(630, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		putText(playerInfo1, to_string(pocessionTime[i - 1]), Point(810, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		textLocation1 += 55;
	}
	string generalN = ".\\graphicResults\\general" + to_string(teamN) + ".png";
	imwrite(generalN, playerInfo1);
}

// maek the image showing passing datas for a certain team
void makePassingImage(int teamN, vector<vector<Team>> stats) {
	// create a new image
	Mat passingN(550, 1300, CV_8UC3, Scalar(100, 10, 10));
	rectangle(passingN, Point(50, 50), Point(1250, 550), Scalar(255, 102, 51), CV_FILLED);
	double location1 = 105;
	for (int i = 0; i < 4; i++) {
		rectangle(passingN, Point(50, location1), Point(1250, location1 + 55), Scalar(255, 204, 153), CV_FILLED);
		location1 += 110;
	}
	putText(passingN, "Team" + to_string(teamN) + " Individual Basic Passing Information", Point(50, 30), 5, 1, Scalar(255, 255, 255), 1, CV_AA, false);
	putText(passingN, "Player", Point(50, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Total Pass Times", Point(130, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Total Pass Success%", Point(300, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Short Pass Times", Point(500, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Short Pass Success%", Point(680, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Long Pass Times", Point(880, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingN, "Long Pass Success%", Point(1060, 100), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	double textLocation1 = 146;
	vector<int> longTotal = stats[teamN - 1][2999].getLongPassPlayers();
	vector<int> longSuccess = stats[teamN - 1][2999].getSuccessLongPassPlayers();
	vector<int> shortTotal = stats[teamN - 1][2999].getShortPassPlayers();
	vector<int> shortSuccess = stats[teamN - 1][2999].getSuccessShortPassPlayers();
	vector<int> totalPass;
	for (int i = 1; i <= 8; i++) {
		totalPass.push_back(shortTotal[i - 1] + longTotal[i - 1]);
		//player
		putText(passingN, to_string(i), Point(50, textLocation1), 4, 0.5, Scalar(0, 0, 0), 1.5, CV_AA, false);
		//total pass Times
		putText(passingN, to_string(totalPass[i - 1]), Point(130, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		// total pass success rate
		double rate_total_success;
		string rateTotalSuccess;
		if (totalPass[i - 1] == 0) {
			rateTotalSuccess = "-";
			putText(passingN, rateTotalSuccess, Point(300, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		}
		else {
			char rate_total[32];
			rate_total_success = 100.0* (shortSuccess[i - 1] + longSuccess[i - 1]) / (totalPass[i - 1]);
			sprintf_s(rate_total, "%.2f%%", rate_total_success);
			putText(passingN, rate_total, Point(300, textLocation1), 5, 0.7, Scalar(0, 0, 0), 1, CV_AA, false);
		}

		// total short pass times
		putText(passingN, to_string(shortTotal[i - 1]), Point(500, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		// short pass success rate
		double rate1;
		string rateShort;
		if (shortTotal[i - 1] == 0) {
			rateShort = "-";
			putText(passingN, rateShort, Point(680, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		}
		else {
			rate1 = 100.0*shortSuccess[i - 1] / shortTotal[i - 1];
			char rate_short[32];
			sprintf_s(rate_short, "%.2f%%", rate1);
			putText(passingN, rate_short, Point(680, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		}

		// total long pass times
		putText(passingN, to_string(longTotal[i - 1]), Point(880, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		// long pass success rate
		double rate;
		string rateLong;
		if (longTotal[i - 1] == 0) {
			rateLong = "-";
			putText(passingN, rateLong, Point(1060, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		}
		else {
			char rate_long[32];
			rate = 100.0*longSuccess[i - 1] / longTotal[i - 1];
			sprintf_s(rate_long, "%.2f%%", rate);
			putText(passingN, rate_long, Point(1060, textLocation1), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		}


		textLocation1 += 55;
	}

	string passing = ".\\graphicResults\\passing" + to_string(teamN) + ".png";
	imwrite(passing, passingN);
}

void makeTeamDataImage(vector<vector<Team>> stats) {
	Mat teamDN(550, 1300, CV_8UC3, Scalar(100, 10, 10));
	double location = 55;
	for (int i = 0; i <= 7; i++) {
		rectangle(teamDN, Point(450, location), Point(550, location + 50), Scalar(255, 204, 153), CV_FILLED);
		rectangle(teamDN, Point(550, location), Point(750, location + 50), Scalar(255, 102, 51), CV_FILLED);
		rectangle(teamDN, Point(750, location), Point(850, location + 50), Scalar(255, 204, 153), CV_FILLED);
		location += 55;
	}
	// team 1 goal
	putText(teamDN, to_string(stats[0][2999].getGoals()), Point(460, 100), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// team 2 goal
	putText(teamDN, to_string(stats[1][2999].getGoals()), Point(760, 100), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw the goal strip
	int goalTotal = stats[0][2999].getGoals() + stats[1][2999].getGoals();
	if (goalTotal == 0) {
		rectangle(teamDN, Point(445, 64), Point(450, 106), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 64), Point(855, 106), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double goalRate1 = (double)stats[0][2999].getGoals() / goalTotal;
		double goalRate2 = (double)stats[2][2999].getGoals() / goalTotal;
		rectangle(teamDN, Point(450 - 450 * goalRate1, 64), Point(450, 106), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 64), Point(850 + 450 * goalRate2, 106), Scalar(204, 255, 153), CV_FILLED);
	}
	// team1 penalty kik
	putText(teamDN, to_string(stats[0][2999].getPenaltyKick()), Point(460, 155), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// team2 penalty kik
	putText(teamDN, to_string(stats[1][2999].getPenaltyKick()), Point(760, 155), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw penalty kik strip 
	int kikTotal = stats[0][2999].getPenaltyKick() + stats[1][2999].getPenaltyKick();
	if (kikTotal == 0) {
		rectangle(teamDN, Point(445, 119), Point(450, 161), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850,119), Point(855, 161), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double kikRate1 = (double)stats[0][2999].getPenaltyKick() / kikTotal;
		double kikRate2 = (double)stats[1][2999].getPenaltyKick() / kikTotal;
		rectangle(teamDN, Point(450 - 450 * kikRate1, 119), Point(450, 161), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 119), Point(850 + 450 * kikRate2, 161), Scalar(204, 255, 153), CV_FILLED);
	}
	// team1 shoot
	putText(teamDN, to_string(stats[0][2999].getShots()), Point(460, 210), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// team2 shoot
	putText(teamDN, to_string(stats[1][2999].getShots()), Point(760, 210), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw shhot strip
	int shootTotal = stats[0][2999].getShots() + stats[1][2999].getShots();
	if (shootTotal == 0) {
		rectangle(teamDN, Point(445, 174), Point(450, 216), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 174), Point(855, 216), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double shootRate1 = (double)stats[0][2999].getShots() / shootTotal;
		double shootRate2 = (double)stats[1][2999].getShots() / shootTotal;
		rectangle(teamDN, Point(450 - 450 * shootRate1, 174), Point(450, 216), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 174), Point(850 + 450 * shootRate2, 216), Scalar(204, 255, 153), CV_FILLED);
	}
	// control rate  team 1
	int totalPossession = stats[0][2999].getPossession() + stats[1][2999].getPossession();
	double control1 = 0.0;
	double control2 = 0.0;
	if (totalPossession != 0) {
		control1 = stats[0][2999].getPossession() * 100.0 / totalPossession;
		control2 = stats[1][2999].getPossession() * 100.0 / totalPossession;
	}
	char control_one[32];
	sprintf_s(control_one, "%.1f%%", control1);
	char control_two[32];
	sprintf_s(control_two, "%.1f%%", control2);
	putText(teamDN, control_one, Point(460, 265), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// conteol rate  team 2
	putText(teamDN, control_two, Point(760, 265), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw control rate strip
	rectangle(teamDN, Point(450 - 450 * (control1/100), 229), Point(450, 271), Scalar(204, 229, 255), CV_FILLED);
	rectangle(teamDN, Point(850, 229), Point(850 + 450 * (control2/100), 271), Scalar(204, 255, 153), CV_FILLED);
	// passing team 1 
	int totalPassing_one = stats[0][2999].getLongPass() + stats[0][2999].getShortPass();
	int totalPassing_two = stats[1][2999].getLongPass() + stats[1][2999].getShortPass();
	putText(teamDN, to_string(totalPassing_one), Point(460, 320), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// passing team 2 
	putText(teamDN, to_string(totalPassing_two), Point(760, 320), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);

	double passRate1 = 1.0;
	double passRate2 = 1.0;
	if (totalPassing_one != 0) {
		passRate1 = (stats[0][2999].getSuccessShortPass() + stats[0][2999].getSuccessLongPass()) * 100.0 / totalPassing_one;
	}
	if (totalPassing_two != 0) {
		passRate2 = (stats[1][2999].getSuccessShortPass() + stats[1][2999].getSuccessLongPass()) * 100.0 / totalPassing_two;
	}
	char passRate_one[32];
	sprintf_s(passRate_one, "%.1f%%", passRate1);
	char passRate_two[32];
	sprintf_s(passRate_two, "%.1f%%", passRate2);
	// draw passing strip 
	int passingTotal = totalPassing_one + totalPassing_two;
	if (passingTotal == 0) {
		rectangle(teamDN, Point(445, 284), Point(450, 326), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 284), Point(855, 326), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double passingRate1 = (double)totalPassing_one / passingTotal;
		double passingRate2 = (double)totalPassing_two / passingTotal;
		rectangle(teamDN, Point(450 - 450 * passingRate1, 284), Point(450, 326), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 284), Point(850 + 450 * passingRate2, 326), Scalar(204, 255, 153), CV_FILLED);
	}
	// passing rate 1 
	putText(teamDN, passRate_one, Point(460, 375), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// pssing rate 2 
	putText(teamDN, passRate_two, Point(760, 375), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw passing success rate strip
	int successTotal = passRate1 + passRate2;
	if (passingTotal == 0) {
		rectangle(teamDN, Point(445, 339), Point(450, 381), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 339), Point(855, 381), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double successRate1 = (double)passRate1 / successTotal;
		double successRate2 = (double)passRate2 / successTotal;
		rectangle(teamDN, Point(450 - 450 * successRate1, 339), Point(450, 381), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 339), Point(850 + 450 * successRate2, 381), Scalar(204, 255, 153), CV_FILLED);
	}
	// free kik 1
	putText(teamDN, to_string(stats[0][2999].getFreeKick()), Point(460, 430), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// free kik 2 
	putText(teamDN, to_string(stats[1][2999].getFreeKick()), Point(760, 430), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw free kik strip
	int freeTotal = stats[0][2999].getFreeKick() + stats[1][2999].getFreeKick();
	if (freeTotal == 0) {
		rectangle(teamDN, Point(445, 394), Point(450, 436), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 394), Point(855, 436), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double freeRate1 = (double)stats[0][2999].getFreeKick() / freeTotal;
		double freeRate2 = (double)stats[1][2999].getFreeKick() / freeTotal;
		rectangle(teamDN, Point(450 - 450 * freeRate1, 394), Point(450, 436), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 394), Point(850 + 450 * freeRate2, 436), Scalar(204, 255, 153), CV_FILLED);
	}
	// corner kik 1
	putText(teamDN, to_string(stats[0][2999].getCornerKick()), Point(460, 485), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// corner kik 2 
	putText(teamDN, to_string(stats[1][2999].getCornerKick()), Point(760, 485), 5, 0.7, Scalar(0, 0, 0), 1.5, CV_AA, false);
	// draw corner kik strip
	int cornerTotal = stats[0][2999].getCornerKick() + stats[1][2999].getCornerKick();
	if (cornerTotal == 0) {
		rectangle(teamDN, Point(445, 449), Point(450, 491), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 449), Point(855, 491), Scalar(204, 255, 153), CV_FILLED);
	}
	else {
		double cornerRate1 = (double)stats[0][2999].getCornerKick() / cornerTotal;
		double cornerRate2 = (double)stats[1][2999].getCornerKick() / cornerTotal;
		rectangle(teamDN, Point(450 - 450 * cornerRate1, 449), Point(450, 491), Scalar(204, 229, 255), CV_FILLED);
		rectangle(teamDN, Point(850, 449), Point(850 + 450 * cornerRate2, 491), Scalar(204, 255, 153), CV_FILLED);
	}
	putText(teamDN, "TEAM 1", Point(230, 20), 5, 1, Scalar(255, 255, 255), 1, 8, false);
	putText(teamDN, "TEAM 2", Point(1030, 20), 5, 1, Scalar(255, 255, 255), 1, 8, false);
	putText(teamDN, "GOAL", Point(630, 85), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "PENALTY KICK", Point(590, 140), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "SHOOT", Point(625, 195), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "CONTROL RATE", Point(590, 250), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "PASSING", Point(600, 305), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "PASSING RATE", Point(590, 360), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "FREE KICK", Point(600, 415), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(teamDN, "CORNER KICK", Point(595, 470), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	//putText(teamDN, "PENALTY", Point(625, 525), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	string teamDataIm = ".\\graphicResults\\teamDataIm.png";
	imwrite(teamDataIm, teamDN);
}

void makePassingAImage(vector<vector<Team>> stats, int teamN) {
	int size = 8;
	Mat passingDN(550, 1100, CV_8UC3, Scalar(255, 255, 255));
	double width = 660 / size;
	double height = 440 / size;
	for (int i = 0; i <= size; i++) {
		line(passingDN, Point(220 + width * i, 0), Point(220 + width * i, 550), Scalar(160, 160, 160), 1, 8, 0);
	}
	for (int i = 0; i <= size; i++) {
		line(passingDN, Point(0, 110 + height * i), Point(1100, 110 + height * i), Scalar(160, 160, 160), 1, 8, 0);
	}
	for (int i = 0; i <= size; i++) {
		line(passingDN, Point(220 + width * i, 110 + height * i), Point(220 + width * (i + 1), 110 + height * (i + 1)), Scalar(160, 160, 160), 1, 8, 0);
	}

	putText(passingDN, "From/To", Point(70, 50), 5, 1, Scalar(0, 0, 0), 0.5, CV_AA, false);
	putText(passingDN, "Total Passing", Point(900, 50), 5, 1, Scalar(0, 0, 0), 0.5, CV_AA, false);

	// write in the data 
	for (int i = 0; i < size; i++) {
		// fill in the vertical player number
		putText(passingDN, to_string(i + 1), Point(90, 135 + height* i), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
		for (int j = 0; j < size; j++) {
			// fill in the horizontal player number
			putText(passingDN, to_string(i + 1), Point(245 + width * i, 60), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
			//teamN 
			// i stands for passing from 
			// j stands for passing to
			if (i != j) {
				vector<vector<int>> passPlayers = stats[teamN - 1][2999].getPassPlayers();
				putText(passingDN, to_string(passPlayers[i][j]), Point(245 + width*j, 135 + height* i), 4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
			}
		}
	}
	string passingDataIm = ".\\graphicResults\\passingDataIm " + to_string(teamN) + ".png";
	vector<int> totalLongPass = stats[teamN - 1][2999].getSuccessLongPassPlayers();
	vector<int> totalShortPass = stats[teamN - 1][2999].getSuccessShortPassPlayers();
	for (int i = 0; i < 8; i++) {
		putText(passingDN, to_string(totalLongPass[i] + totalShortPass[i]), Point(900, 135 + height* i),4, 0.5, Scalar(0, 0, 0), 0.5, CV_AA, false);
	}
	imwrite(passingDataIm, passingDN);
}
