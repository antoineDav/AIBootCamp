#pragma once

struct Mission {

	const enum missionStatus { PENDING, AVAILABLE, IN_PROGRESS, ACCOMPLISHED, IMPOSSIBLE };

	int missionId;
	int tileId;

	Mission(int mId, int gId, int pLvl, missionStatus s, int cId = -1, int tId = -1, int ppId = -1) :
		missionId{ mId },
		tileId{ tId },
		priorityLvl{pLvl},
		mStatus {s},
		giverId{ gId },
		pressurePlateId{ ppId },
		receiverId{ -1 },
		conditionTileId{ cId } {
	}

	int priorityLvl;
	missionStatus mStatus;

	int giverId;
	int receiverId;
	int pressurePlateId;
	int conditionTileId; //Tile to attain by giver in order for mission to be accomplished
};
