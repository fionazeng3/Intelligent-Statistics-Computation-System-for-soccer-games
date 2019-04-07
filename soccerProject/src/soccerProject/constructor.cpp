#include"linkItem.h"
vector<vector<STrkNodeGlb>> readData() {
	int nFrmCnt = 0, nFtPtCirRad = 20, nSmhWinSz = 9, nVdoFps = 25;
	float fWgtDepthSum, fWgtVisSum, fWgtFrmBrdrSum, fWgtCombTotal, fSmhWinSumX = 0.0f, fSmhWinSumY = 0.0f, fInstVelSum = 0.0f, fInstVelRunThres = 3.0f,
		fRat2FrmBrdrThres = 0.007f, fGlbFtPtStdThres = 5000.0f;
	bool bFlagEnd = false, bFlagNewFrm = false, bFlagNewGap = false;
	char acOutputTrkGlb[256], acOutputTrkGlbId[32], acTrkNodeId[32], acFrmCnt[32], acInstVel[32], acDistSum[32];
	cv::Mat oImgPlot;
	cv::Point oFtPtCirCtr;
	STrkNode sTrkNode;
	STrkNodeGlb sTrkNodeGlb;
	std::ifstream ifsTrkRst;
	std::vector<int> vnTrkNodeCurrMaxIdxList;
	std::vector<STrkGap> vsTrkGapList;
	std::vector<STrkNode> vsTrkNodeCurrFrmList;
	vector<vector<STrkNodeGlb>> raw_data;


	/*
	// variables to calculate the ball's trajecotry
	float c_slope = FLT_MAX;
	float p_slope = FLT_MAX;
	float c_deltaX = 0.0;
	float c_deltaY = 0.0;
	float p_deltaX = c_deltaX;
	float p_deltaY = c_deltaY;
	*/

	std::vector<int> m_vnObjIdList;
	m_vnObjIdList.push_back(0);
	m_vnObjIdList.push_back(11);
	m_vnObjIdList.push_back(12);
	m_vnObjIdList.push_back(13);
	m_vnObjIdList.push_back(14);
	m_vnObjIdList.push_back(15);
	m_vnObjIdList.push_back(16);
	m_vnObjIdList.push_back(17);
	m_vnObjIdList.push_back(18);
	m_vnObjIdList.push_back(21);
	m_vnObjIdList.push_back(22);
	m_vnObjIdList.push_back(23);
	m_vnObjIdList.push_back(24);
	m_vnObjIdList.push_back(25);
	m_vnObjIdList.push_back(26);
	m_vnObjIdList.push_back(27);
	m_vnObjIdList.push_back(28);


	printf("game begin:\n");
	std::vector<CCamData> m_voCamDataList;
	m_voCamDataList.push_back(CCamData(std::string(".\\data\\3dtrk_cam1.txt"), 1));
	m_voCamDataList.push_back(CCamData(std::string(".\\data\\3dtrk_cam2.txt"), 2));
	m_voCamDataList.push_back(CCamData(std::string(".\\data\\3dtrk_cam3.txt"), 3));
	m_voCamDataList.push_back(CCamData(std::string(".\\data\\3dtrk_cam7.txt"), 7));

	for (std::vector<CCamData>::iterator it = m_voCamDataList.begin(); it != m_voCamDataList.end(); ++it)
		vnTrkNodeCurrMaxIdxList.push_back(-1);
	// read tracking results from 4 cameras
	for (std::vector<CCamData>::iterator it = m_voCamDataList.begin(); it != m_voCamDataList.end(); ++it)
	{
		ifsTrkRst.open(it->getTrkRstPath());

		while (!ifsTrkRst.eof())
		{
			char acBuf[256] = { 0 };
			ifsTrkRst.getline(acBuf, 256);
			sscanf(acBuf, "%d %d %d %d %d %d %d %f %f %f %f %f %f %f %f", &sTrkNode.nCamId, &sTrkNode.nFrmCnt, &sTrkNode.nObjId,
				&sTrkNode.oRect.x, &sTrkNode.oRect.y, &sTrkNode.oRect.width, &sTrkNode.oRect.height,
				&sTrkNode.o3dPtFt.x, &sTrkNode.o3dPtFt.y, &sTrkNode.o3dPtFt.z,
				&sTrkNode.o2dPtFtGlb.x, &sTrkNode.o2dPtFtGlb.y,
				&sTrkNode.fDist2Cam, &sTrkNode.fVis, &sTrkNode.fRat2FrmBrdr);
			it->addTrkNode(sTrkNode);
		}

		ifsTrkRst.close();

		printf("finish reading tracking results of camera ID %d\n", it->getCamId());
	}

	// perform data association across multiple cameras
	// for each frame
	while (true)
	{
		for (std::vector<CCamData>::iterator it = m_voCamDataList.begin(); it != m_voCamDataList.end(); ++it)
			vnTrkNodeCurrMaxIdxList[it - m_voCamDataList.begin()] = it->getTrkNodeCurrMaxIdx();

		// for each object, find that object in each camera and add that to the current frame list
		for (int i = 0; i < m_vnObjIdList.size(); i++)
		{
			//// for debug
			//if (0 < nFrmCnt)
			//	cv::waitKey(0);

			// vsTrkNodeCurrFrmList stores the same object from each camera in current frame
			std::vector<STrkNode>().swap(vsTrkNodeCurrFrmList);

			// for each camera
			for (std::vector<CCamData>::iterator it = m_voCamDataList.begin(); it != m_voCamDataList.end(); ++it)
			{
				if (it->getTrkNodeCurrMaxIdx() < it->getTrkNodeListSz())
				{
					// searching from the max index from previous frame to the end of the node list
					// ?? what if one camera missed one object in certain frame
					for (int j = it->getTrkNodeCurrMaxIdx() + 1; j < it->getTrkNodeListSz(); j++)
					{
						sTrkNode = it->getTrkNode(j);

						if (m_vnObjIdList[i] == sTrkNode.nObjId)
						{
							vsTrkNodeCurrFrmList.push_back(sTrkNode);
							// update the max index for next frame searching
							if (j > vnTrkNodeCurrMaxIdxList[it - m_voCamDataList.begin()])
								vnTrkNodeCurrMaxIdxList[it - m_voCamDataList.begin()] = j;

							break;
						}
					}
				}
			}

			if (vsTrkNodeCurrFrmList.empty())
			{
				bFlagEnd = true;
				break;
			}

			// weight for depth
			fWgtDepthSum = 0.0f;
			// weight for visibility
			fWgtVisSum = 0.0f;
			// weight for distance to frame border
			fWgtFrmBrdrSum = 0.0f;

			for (std::vector<STrkNode>::iterator it = vsTrkNodeCurrFrmList.begin(); it != vsTrkNodeCurrFrmList.end(); ++it)
			{
				it->fWgtDepth = (1000.0f / it->fDist2Cam) * (1000.0f / it->fDist2Cam);
				fWgtDepthSum += it->fWgtDepth;
				it->fWgtVis = it->fVis * it->fVis;
				fWgtVisSum += it->fWgtVis;
				it->fWgtFrmBrdr = (it->fRat2FrmBrdr >= fRat2FrmBrdrThres) ? 1.0f : ((it->fRat2FrmBrdr / fRat2FrmBrdrThres) * (it->fRat2FrmBrdr / fRat2FrmBrdrThres));
				fWgtFrmBrdrSum += it->fWgtFrmBrdr;
			}

			sTrkNodeGlb.o2dPtFtGlb = cv::Point2f(0.0f, 0.0f);
			fWgtCombTotal = 0.0f;

			for (std::vector<STrkNode>::iterator it = vsTrkNodeCurrFrmList.begin(); it != vsTrkNodeCurrFrmList.end(); ++it)
			{
				if (fWgtDepthSum > 0.0f)
					it->fWgtDepth /= fWgtDepthSum;
				else
					it->fWgtDepth = 0.0f;

				if (fWgtVisSum > 0.0f)
					it->fWgtVis /= fWgtVisSum;
				else
					it->fWgtVis = 0.0f;

				if (fWgtFrmBrdrSum > 0.0f)
					it->fWgtFrmBrdr /= fWgtFrmBrdrSum;
				else
					it->fWgtFrmBrdr = 0.0f;

				it->fWgtComb = it->fWgtDepth * it->fWgtVis * it->fWgtFrmBrdr;
				fWgtCombTotal += it->fWgtComb;
				sTrkNodeGlb.o2dPtFtGlb.x += it->o2dPtFtGlb.x * it->fWgtComb;
				sTrkNodeGlb.o2dPtFtGlb.y += it->o2dPtFtGlb.y * it->fWgtComb;
			}

			sTrkNodeGlb.nFrmCnt = nFrmCnt;
			sTrkNodeGlb.nObjId = m_vnObjIdList[i];

			if (fWgtCombTotal > 0.0f)
			{
				sTrkNodeGlb.o2dPtFtGlb.x /= fWgtCombTotal;
				sTrkNodeGlb.o2dPtFtGlb.y /= fWgtCombTotal;
			}
			else
			{
				sTrkNodeGlb.o2dPtFtGlb.x = FLT_MAX;
				sTrkNodeGlb.o2dPtFtGlb.y = FLT_MAX;
			}

			// If the standard deviation of global foot point is large (over a threshold), the ball is likely to be in the air.
			// Look for the points that the standard deviation is small and connect them as a straight line.
			sTrkNodeGlb.fGlbFtPtStd = 0.0f;

			for (std::vector<STrkNode>::iterator it = vsTrkNodeCurrFrmList.begin(); it != vsTrkNodeCurrFrmList.end(); ++it)
				sTrkNodeGlb.fGlbFtPtStd += std::pow(cv::norm(sTrkNodeGlb.o2dPtFtGlb - it->o2dPtFtGlb), 2) * it->fWgtComb;

			if (fWgtCombTotal > 0.0f)
				sTrkNodeGlb.fGlbFtPtStd = std::sqrt(sTrkNodeGlb.fGlbFtPtStd / fWgtCombTotal);
			else
				sTrkNodeGlb.fGlbFtPtStd = FLT_MAX;

			if (sTrkNodeGlb.fGlbFtPtStd > fGlbFtPtStdThres)
			{
				sTrkNodeGlb.o2dPtFtGlb.x = FLT_MAX;
				sTrkNodeGlb.o2dPtFtGlb.y = FLT_MAX;
			}

			while (raw_data.size() <= i)
			{
				std::vector<STrkNodeGlb> vsTrkNodeGlbList;
				raw_data.push_back(vsTrkNodeGlbList);
			}
			// raw_data[i] represents object i's global 2D position in a vector based on frames
			raw_data[i].push_back(sTrkNodeGlb);
		}



		if (bFlagEnd)
			break;

		for (std::vector<CCamData>::iterator it = m_voCamDataList.begin(); it != m_voCamDataList.end(); ++it)
			it->setTrkNodeCurrMaxIdx(vnTrkNodeCurrMaxIdxList[it - m_voCamDataList.begin()]);

		nFrmCnt++;
	}

	// fill the tracking gaps
	for (std::vector <std::vector<STrkNodeGlb>>::iterator it = raw_data.begin(); it != raw_data.end(); ++it)
	{
		std::vector<STrkGap>().swap(vsTrkGapList);

		for (int i = 0; i < it->size(); i++)
		{
			bFlagNewGap = false;

			if ((FLT_MAX == it->at(i).o2dPtFtGlb.x) || (FLT_MAX == it->at(i).o2dPtFtGlb.y))
			{
				if (vsTrkGapList.size() == 0)
					bFlagNewGap = true;
				else
				{
					STrkGap sTrkGapLast = vsTrkGapList.back();

					if (i == (sTrkGapLast.nTrkNodeIdx + sTrkGapLast.nTrkNodeNum))
						vsTrkGapList[vsTrkGapList.size() - 1] = { sTrkGapLast.nTrkNodeIdx, (sTrkGapLast.nTrkNodeNum + 1) };
					else
						bFlagNewGap = true;
				}

			}

			if (bFlagNewGap)
				vsTrkGapList.push_back({ i, 1 });
		}

		for (int i = 0; i < vsTrkGapList.size(); i++)
		{
			if ((vsTrkGapList[i].nTrkNodeIdx > 0) && ((vsTrkGapList[i].nTrkNodeIdx + vsTrkGapList[i].nTrkNodeNum) < it->size()))
			{
				STrkNodeGlb sTrkNodeGlbPrev = it->at(vsTrkGapList[i].nTrkNodeIdx - 1);
				STrkNodeGlb sTrkNodeGlbAftr = it->at(vsTrkGapList[i].nTrkNodeIdx + vsTrkGapList[i].nTrkNodeNum);
				float inc2dPtFtGlbX = (sTrkNodeGlbAftr.o2dPtFtGlb.x - sTrkNodeGlbPrev.o2dPtFtGlb.x) / (vsTrkGapList[i].nTrkNodeNum + 1);
				float inc2dPtFtGlbY = (sTrkNodeGlbAftr.o2dPtFtGlb.y - sTrkNodeGlbPrev.o2dPtFtGlb.y) / (vsTrkGapList[i].nTrkNodeNum + 1);

				for (int j = vsTrkGapList[i].nTrkNodeIdx; j < (vsTrkGapList[i].nTrkNodeIdx + vsTrkGapList[i].nTrkNodeNum); j++)
				{

					it->at(j).o2dPtFtGlb.x = sTrkNodeGlbPrev.o2dPtFtGlb.x + (inc2dPtFtGlbX * (j - vsTrkGapList[i].nTrkNodeIdx + 1));
					it->at(j).o2dPtFtGlb.y = sTrkNodeGlbPrev.o2dPtFtGlb.y + (inc2dPtFtGlbY * (j - vsTrkGapList[i].nTrkNodeIdx + 1));
				}
			}
		}
	}

	// for each object of all frames
	// raw_data[i] is a vector of STrkNodeGlb for object i, each object in this vector it->at(j) represents object i's global 2D position, speed, sumDistance in frame j
	for (std::vector <std::vector<STrkNodeGlb>>::iterator it = raw_data.begin(); it != raw_data.end(); ++it)
	{
		// smooth the trajectory
		// for each frame
		for (int i = 0; i < it->size(); i++)
		{
			if ((i >= ((nSmhWinSz - 1) / 2)) && (i < (it->size() - ((nSmhWinSz - 1) / 2))))
			{
				if ((it->at(i - ((nSmhWinSz - 1) / 2)).o2dPtFtGlb.x != FLT_MAX) && (it->at(i - ((nSmhWinSz - 1) / 2)).o2dPtFtGlb.y != FLT_MAX) &&
					(it->at(i + ((nSmhWinSz - 1) / 2)).o2dPtFtGlb.x != FLT_MAX) && (it->at(i + ((nSmhWinSz - 1) / 2)).o2dPtFtGlb.y != FLT_MAX))
				{
					fSmhWinSumX = 0.0f, fSmhWinSumY = 0.0f;

					for (int j = (i - ((nSmhWinSz - 1) / 2)); j <= (i + ((nSmhWinSz - 1) / 2)); j++)
					{
						fSmhWinSumX += it->at(j).o2dPtFtGlb.x;
						fSmhWinSumY += it->at(j).o2dPtFtGlb.y;

					}

					it->at(i).o2dPtFtGlb.x = fSmhWinSumX / nSmhWinSz;
					it->at(i).o2dPtFtGlb.y = fSmhWinSumY / nSmhWinSz;
				}
			}

		}

		// compute instantaneous velocity and sum of distance
		// for each frame
		for (int i = 0; i < it->size(); i++)
		{
			it->at(i).fInstVel = 0.0f;

			if (i >= nVdoFps - 1)
			{
				//printf("begin calculating velocity\n");
				// sum up the distance for previous 25 frames = 1 second
				for (int j = (i - (nVdoFps - 1)); j < i; j++) {
					it->at(i).fInstVel += cv::norm(it->at(j).o2dPtFtGlb - it->at(j + 1).o2dPtFtGlb) / 1000.0f;
					//printf("velocity is: %0.2f\n", it->at(i).fInstVel);
				}
			}

			/*
			// previous 12 frames + following 12 frames
			if (i >= nVdoFps / 2 && i <= 3000 - nVdoFps / 2) {
			for (int j = (i - (nVdoFps / 2)); j < i + nVdoFps / 2 - 1; j++) {
			it->at(i).fInstVel += cv::norm(it->at(j).o2dPtFtGlb - it->at(j + 1).o2dPtFtGlb) / 1000.0f;
			}
			}
			*/
			//printf("frame is: %d\n", i);
			//printf("Finally, velocity for object %d in frame %d is: %0.2f\n", it - raw_data.begin(), i, it->at(i).fInstVel);

			if (i > 0) {
				double newDistance = cv::norm(it->at(i).o2dPtFtGlb - it->at(i - 1).o2dPtFtGlb) / 1000.0;
				if (newDistance <= 0.417) {
					it->at(i).fDistSum = it->at(i - 1).fDistSum + newDistance;
				}
				else {
					it->at(i).fDistSum = it->at(i - 1).fDistSum;
				}
				//printf("object's x is: %d  y is: %d\n", it->at(i).o2dPtFtGlb.x, it->at(i).o2dPtFtGlb.y);
			}
		}
	}
	return raw_data;
}