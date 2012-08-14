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
		MSG_LS_CL_PLAYERINFO,
		MSG_CL_LS_GAMELIST,
		MSG_LS_CL_GAMELIST,
		MSG_CL_LS_LOGIN_ANONYMOUS,

		//////////////////////////////////////////////
		// messages between client and game server
		//////////////////////////////////////////////
		MSG_CL_GS_LOGIN = 100,
		MSG_GS_CL_LOGIN,

		MSG_CL_GS_BRING_MONEY,
		MSG_GS_CL_BRING_MONEY,

		MSG_CL_GS_TABLE_JOIN,
		MSG_GS_CL_TABLE_JOIN,
		MSG_CL_GS_TABLE_LEAVE,
		MSG_GS_CL_TABLE_LEAVE,

		MSG_GS_CL_TABLE_INFO,

		MSG_GS_CL_START_GAME,
		MSG_GS_CL_BASE_CHIP,
		MSG_GS_CL_DISTRIBUTE,
		MSG_GS_CL_CURRENT_PLAYER,
		MSG_CL_GS_FOLLOW,
		MSG_GS_CL_FOLLOW,
		MSG_CL_GS_GIVEUP,
		MSG_GS_CL_GIVEUP,
		MSG_GS_CL_CURRENT_BID,

		MSG_GS_CL_SYNC_START,		// Sync request
		MSG_CL_GS_SYNC_START,		// Sync response

		MSG_CL_GS_QUERY_MONEY,
		MSG_GS_CL_QUERY_MONEY,

		// gameserver broadcast
		MSG_GS_BC_TABLE_JOIN,
		MSG_GS_BC_TABLE_LEAVE,
		MSG_GS_BC_TABLE_START,
		MSG_GS_BC_TABLE_END,

		//////////////////////////////////////////////
		// messages between lobby/game server and wallet server
		//////////////////////////////////////////////
		MSG_LS_WS_QUERY_USER_WALLET=200,
		MSG_WS_LS_QUERY_USER_WALLET,

		MSG_GS_WS_QUERY_TABLE_WALLET,
		MSG_WS_GS_QUERY_TABLE_WALLET,

		MSG_GS_WS_INSERT_TABLE_WALLET,
		MSG_WS_GS_INSERT_TABLE_WALLET,


	};

	enum ErrorCodes
	{
		// login server
		LOGIN_OK=0,
		ERR_LS_USER_NOT_FOUND,
		ERR_LS_USER_FREEZED,

		// game server
		GS_NO_ERR=100,
		ERR_GS_TABLE_NOT_FOUND,
		ERR_GS_TABLE_FULL,
		ERR_GS_SEAT_OCCUPY,
		ERR_GS_PLAYER_NOT_FOUND,
		ERR_GS_ROOM_NOT_FOUND,
		ERR_GS_MULTI_RESULT,

		// wallet server
		WS_NO_ERR=200,
		ERR_WS_WALLET_NOT_FOUND,
		ERR_WS_MULTI_RESULT,
		ERR_WS_TABLE_WALLET_EXIST,
		ERR_WS_USERWALLET_NOT_ENOUGH,

	};

	enum RoomType
	{
		ZJH=0,
		DIABLO3,
	};

	enum MoneyType
	{
		SILVER_COIN=0,
		GOLD_COIN,
	};

	class RoomInfo 
	{
	public:
		quint32 mRoomID;	//在DB中设定，自动增量
		QString mName;
		RoomType mType;
		QString mIP;
		quint32 mPort;
		quint32 mMinMoney;
		MoneyType mMoneyType; // 0 - silver; 1 - gold
		RoomInfo()
		{

		}
		RoomInfo(RoomInfo* _in)
		{
			mRoomID = _in->mRoomID;
			mName = _in->mName;
			mType = _in->mType;
			mIP = _in->mIP;
			mPort = _in->mPort;
			mMinMoney = _in->mMinMoney;
			mMoneyType = _in->mMoneyType;
		}
		~RoomInfo()
		{

		}
	};

#define MIN_PLAYER 2
#define MAX_PLAYER 4
#define MAX_POKERS_ZJH 52
#define MAX_HAND_POKER 3


}
Q_DECLARE_METATYPE(SharedData::RoomInfo);

#endif // sharedData_h__