/********************************************************************
	created:	2011/09/05
	created:	5:9:2011   16:53
	filename: 	d:\My Projects\ZJHClient&Server\src\Shared\sharedData.h
	file path:	d:\My Projects\ZJHClient&Server\src\Shared
	file base:	sharedData
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef sharedData_h__
#define sharedData_h__

namespace SharedData
{
	enum
	{
		//////////////////////////////////////////////
		// messages between client and login server
		//////////////////////////////////////////////
		MSG_CL_LS_LOGIN = 0,
		MSG_LS_CL_LOGIN,
		MSG_CL_LS_GAMELIST,
		MSG_LS_CL_GAMELIST,

		//////////////////////////////////////////////
		// messages between client and game server
		//////////////////////////////////////////////
		MSG_CL_GS_LOGIN = 100,
		MSG_GS_CL_LOGIN,

		MSG_CL_GS_TABLE_JOIN,
		MSG_GS_CL_TABLE_JOIN,
		MSG_CL_GS_TABLE_LEAVE,
		MSG_GS_CL_TABLE_LEAVE,

		MSG_GS_START_GAME,

		// gameserver broadcast
		MSG_GS_BC_TABLE_JOIN,
		MSG_GS_BC_TABLE_LEAVE,
		MSG_GS_BC_TABLE_START,
		MSG_GS_BC_TABLE_END,

	};

	enum ErrorCodes
	{
		// login server
		LOGIN_OK=0,
		LOGIN_USER_NOT_FOUND,

		// game server
		GS_NO_ERR=100,
		ERR_GS_TABLE_NOT_FOUND,
		ERR_GS_TABLE_FULL,
		ERR_GS_PLAYER_NOT_FOUND,
		ERR_GS_ROOM_NOT_FOUND,
		ERR_GS_MULTI_RESULT,



	};

	struct RoomInfo 
	{
		QString mName;
		quint32 mType;
		QString mIP;
		quint32 mPort;
		quint32 mScore;
		quint32 mUnit; // 0 - silver; 1 - gold
	};
}

#endif // sharedData_h__