struct Mission {

	//const static enum missionType { COOP, GOAL };
	//missionType mType;
	int missionId;
	int tileId;

	Mission(int mId, int tId) :
		missionId{ mId },
		tileId{ tId } {
	}

};



struct CoopMission : Mission {
	
	int giverId;
	int receiverId;
	int pressurePlateId;

	CoopMission(int mId, int gId, int ppId, int tId = -1) : Mission(mId, tId),
		giverId{ gId },
		pressurePlateId{ ppId },
		receiverId{ -1 } {
	}
};

