struct Mission {

	//const static enum missionType { COOP, GOAL };
	//missionType mType;
	int missionId;

	int giverId;
	int receiverId;

	int tileId;

	Mission(int mId, int tId) :
		missionId{ mId },
		giverId{ -1 },
		receiverId{ -1 },
		tileId{ tId } {
	}
};