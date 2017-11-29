#pragma once

struct Mission {

	//const static enum missionType { COOP, GOAL };
	//missionType mType;
	const enum missionStatus { PENDING, AVAILABLE, IN_PROGRESS, ACCOMPLISHED, IMPOSSIBLE };

	int missionId;
	int tileId;

	Mission(int mId, int gId, int pLvl, missionStatus s, int tId = -1, int ppId = -1) :
		missionId{ mId },
		tileId{ tId },
		priorityLvl{pLvl},
		mStatus {s},
		giverId{ gId },
		pressurePlateId{ ppId },
		receiverId{ -1 } {
	}

	int priorityLvl;
	missionStatus mStatus;

	int giverId;
	int receiverId;
	int pressurePlateId;
};



//struct CoopMission : Mission {
//	
//	int giverId;
//	int receiverId;
//	int pressurePlateId;
//
//	CoopMission(int mId, int gId, int pLvl, missionStatus s, int ppId, int tId = -1) : Mission(mId, tId, pLvl, s),
//		giverId{ gId },
//		pressurePlateId{ ppId },
//		receiverId{ -1 } {
//	}
//};

