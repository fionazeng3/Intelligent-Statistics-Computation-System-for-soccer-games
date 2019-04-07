#ifndef SOCCER_H
#define SOCCER_H
#include<fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
using namespace std;
using namespace cv;
using std::cout;
using std::endl;

extern int x;
/*
extern vector<int> y;
extern Team t;
extern vector<Team> list;
*/

struct STrkGap

{
	int nTrkNodeIdx;
	int nTrkNodeNum;
};

struct STrkNode
{
	int nCamId;
	int nFrmCnt;
	int nObjId;
	cv::Rect oRect;
	cv::Point3f o3dPtFt;
	cv::Point2f o2dPtFtGlb;
	float fDist2Cam;
	float fVis;
	float fRat2FrmBrdr;

	float fWgtDepth;
	float fWgtVis;
	float fWgtFrmBrdr;
	float fWgtComb;
};

struct STrkNodeGlb
{
	int nFrmCnt;
	int nObjId;
	cv::Point2f o2dPtFtGlb;
	float fGlbFtPtStd;
	double fInstVel; // unit: m/s
	double fDistSum; // unit: m
};

class Ball
{
public:
	Ball(void) {}
	Ball(int n) {
		setVelocity(0.0);
		setFly(0);
		setState(n);
		setPlayerP(0);
		setPlayerState(0);
	}
	~Ball(void) {}

	inline void setVelocity(double v) { velocity = v; }
	inline void setFly(int f) { fly = f; }
	inline void setState(int s) { state = s; }
	inline void setPositionP(cv::Point2f p_p) { p_position = p_p; }
	inline void setPositionC(cv::Point2f c_p) { c_position = c_p; }
	inline void setPlayerP(int p) { p_player = p; }
	inline void setPlayerState(int s) { playerState = s; }
	inline void setDelta(float x, float y) { delta_x = x; delta_y = y; }
	//inline void setOldPosition(cv::Point2f o_p) { o_position = o_p; }

	inline double getVelocity() { return velocity; }
	inline int getFly() { return fly; }
	inline int getState() { return state; }
	inline cv::Point2f getPositionP() { return p_position; }
	inline cv::Point2f getPositionC() { return c_position; }
	inline int getPlayerP() { return p_player; }
	inline int getPlayerState() { return playerState; }
	inline float getDeltaX() { return delta_x; }
	inline float getDeltaY() { return delta_y; }
	//inline cv::Point2f getOldPosition() { return o_position; }

protected:
	// c_position represents the current position of the ball
	cv::Point2f c_position;
	// velocity represents the current velocity of the ball
	double velocity;
	// fly represents whether the ball is in the fly state, for exp: 0 represents on the ground, 1 represents in the fly state
	int fly;
	// p_player represents the previous player who possessed the ball
	int p_player;
	// p_position represents the position of the previous player who possessed the ball
	cv::Point2f p_position;
	// state represents the ball's state, for exp: 0 represents pass, 1 represents possession, 2 represents outOfField
	int state;
	// used to represent whether the previous player who possess the ball is jumping or not. For exp: 0 represents not jump, 1 represents jump
	// !!! To do: used for detecting shooting by head
	int playerState;
	// delta_x and delta_y are position variance of the ball between the previous frame and the frame before the previous frame
	float delta_x;
	float delta_y;
};

class Team
{
public:
	Team(void) {}
	Team(int n) {
		setTeam(n);
		setShortPass(0);
		setLongPass(0);
		setSuccessShortPass(0);
		setSuccessLongPass(0);
		setPossession(0.0);
		setShots(0);
		setGoals(0);
		setCornerKick(0);
		setFreeKick(0);
		setPenaltyKick(0);
	}
	~Team(void) {}

	inline void setTeam(int n) { num = n; }
	inline void setShortPass(int n) { shortPass = n; }
	inline void setLongPass(int n) { longPass = n; }
	inline void setSuccessShortPass(int n) { successPass_short = n; }
	inline void setSuccessLongPass(int n) { successPass_long = n; }
	inline void setPossession(int p) { possesion = p; }
	inline void setShots(int s) { shots = s; }
	inline void setGoals(int g) { goals = g; }
	inline void setCornerKick(int c) { cornerKick = c; }
	inline void setFreeKick(int f) { freeKick = f; }
	inline void setPenaltyKick(int p) { penaltyKick = p; }
	inline void setMaxSpeedPlayers(std::vector<float> speeds) { maxSpeed_players = speeds; }
	inline void setRushingTimePlayers(std::vector<int> times) { rushingTime_players = times; }
	inline void setTotalDistancePlayers(std::vector<float> distances) { totalDistance_players = distances; }
	inline void setPossessionTimePlayers(std::vector<float> possessions) { possessionTime_players = possessions; }
	inline void setShortPassPlayers(std::vector<int> shortpasses) { shortPass_players = shortpasses; }
	inline void setLongPassesPlayers(std::vector<int> longpasses) { longPass_players = longpasses; }
	inline void setSuccessShortPassPlayers(std::vector<int> successshortpasses) { successShortPass_players = successshortpasses; }
	inline void setSuccessLongPassPlayers(std::vector<int> successlongpasses) { successLongPass_players = successlongpasses; }
	inline void setShotsPlayers(vector<int> shotsplayers) { shots_players = shotsplayers; }
	inline void setGoalsPlayers(vector<int> goalplayers) { goals_players = goalplayers; }
	inline void setPassPlayers(vector<vector<int>> passPlayers) { pass_players = passPlayers; }



	inline int getTeam() { return num; }
	inline int getShortPass() { return shortPass; }
	inline int getLongPass() { return longPass; }
	inline int getSuccessShortPass() { return successPass_short; }
	inline int getSuccessLongPass() { return successPass_long; }
	inline int getPossession() { return possesion; }
	inline int getShots() { return shots; }
	inline int getGoals() { return goals; }
	inline int getCornerKick() { return cornerKick; }
	inline int getFreeKick() { return freeKick; }
	inline int getPenaltyKick() { return penaltyKick; }
	inline std::vector<float> getMaxSpeedPlayers() { return maxSpeed_players; }
	inline std::vector<int> getRushingTimePlayers() { return rushingTime_players; }
	inline std::vector<float> getTotalDistancePlayers() { return totalDistance_players; }
	inline std::vector<float> getPossessionTimePlayers() { return possessionTime_players; }
	inline std::vector<int> getShortPassPlayers() { return shortPass_players; }
	inline std::vector<int> getLongPassPlayers() { return longPass_players; }
	inline std::vector<int> getSuccessShortPassPlayers() { return successShortPass_players; }
	inline std::vector<int> getSuccessLongPassPlayers() { return successLongPass_players; }
	inline vector<int> getShotsPlayers() { return shots_players; }
	inline vector<int> getGoalsPlayers() { return goals_players; }
	inline vector<vector<int>> getPassPlayers() { return pass_players; }


protected:
	// num represents which team, for exp:1 means red, 2 means blue
	int num;
	// number of short pass for the team
	int shortPass;
	// number of long pass for the team
	int longPass;
	// number of successful short pass for the team
	int successPass_short;
	// number of successful long pass for the team
	int successPass_long;
	// number of frames that the team possess the ball
	int possesion;
	// number of shots for the team
	int shots;
	// number of goal for the team
	int goals;
	// number of corner kick for the team
	int cornerKick;
	// number of penalty kick for the team
	int penaltyKick;
	// numeber of free kick for the team
	int freeKick;

	// max speed for each player
	std::vector<float> maxSpeed_players;
	// rushing time for each player
	std::vector<int> rushingTime_players;
	// total running distance for each player
	std::vector<float> totalDistance_players;
	// possession time for each player
	std::vector<float> possessionTime_players;
	// all short pass for each player
	std::vector<int> shortPass_players;
	// all long pass for each player
	std::vector<int> longPass_players;
	// successful short pass for each player
	std::vector<int> successShortPass_players;
	// successful long pass for each player
	std::vector<int> successLongPass_players;
	// shots attempted for each player
	std::vector<int> shots_players;
	// goals for each player
	std::vector<int> goals_players;
	// pass anaylis for each player, stores the number of passes between two players in the team
	// exp: pass_players[i][j] stores the number of passes between player i and player j
	std::vector<vector<int>> pass_players;
};




class CCamData
{
public:
	CCamData(void) {}
	CCamData(std::string sgTrkRstPath, int nCamId)
	{
		setTrkRstPath(sgTrkRstPath);
		setCamId(nCamId);
		setTrkNodeCurrMaxIdx(-1);
		std::vector<STrkNode>().swap(m_vsTrkNodeList);
	}
	~CCamData(void) {}

	inline void setTrkRstPath(std::string sgTrkRstPath) { m_sgTrkRstPath = sgTrkRstPath; }
	inline void setCamId(int nCamId) { m_nCamId = nCamId; }
	inline void setTrkNodeCurrMaxIdx(int nTrkNodeCurrMaxIdx) { m_nTrkNodeCurrMaxIdx = nTrkNodeCurrMaxIdx; }
	inline void addTrkNode(STrkNode sTrkNode) { m_vsTrkNodeList.push_back(sTrkNode); }

	inline std::string getTrkRstPath(void) { return m_sgTrkRstPath; }
	inline int getCamId(void) { return m_nCamId; }
	inline int getTrkNodeCurrMaxIdx(void) { return m_nTrkNodeCurrMaxIdx; }
	inline STrkNode getTrkNode(int nTrkNodeIdx) { return m_vsTrkNodeList[nTrkNodeIdx]; }
	inline int getTrkNodeListSz(void) { return m_vsTrkNodeList.size(); }

protected:
	//! path of tracking results
	std::string m_sgTrkRstPath;
	//! camera ID
	int m_nCamId;
	//! previous index for the list of tracking nodes
	int m_nTrkNodeCurrMaxIdx;
	//! list of tracking nodes
	std::vector<STrkNode> m_vsTrkNodeList;
};


Mat plotEachPlayer(int player1[8], int player2[8], int speed1[8], int speed2[8],
	int distance1[8], int distance2[8], int frame, vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList, vector<vector<Team>> stats);
void makeGeneralImage(int teamN, vector<vector<STrkNodeGlb>> vvsTrkNodeGlbList, vector<vector<Team>> stats);
void makePassingImage(int teamN, vector<vector<Team>> stats);
void animationControl(int x, int y, int event);
void heatmapControl(int x, int y, int event);
void dataControl(int x, int y, int event);
void passingAControl(int x, int y, int event);
void makeAnimationImage();
Mat color(Mat background, Mat over);
String arraytostring(int* num);
Mat overlay(int x, int y, Mat background, Mat over);
vector<float> getEachMaxSpeed(int endFrame, int teamN, vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList);
vector<int> getEachRushingTime(int endFrame, int teamN, vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList);
vector<vector<STrkNodeGlb>> readData();
vector<vector<Team>> computeStats(vector <vector<STrkNodeGlb>> vvsTrkNodeGlbList);
Mat heatmapGen(int* playerSelect, int team, vector<vector<STrkNodeGlb>> numbers);
void makeTeamDataImage(vector<vector<Team>> stats);
void makePassingAImage(vector<vector<Team>> stats, int teamN);
#endif //SOCCER_H #pragma once

