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
		// messages between client and login server
		MSG_CL_LS_LOGIN = 0,
		MSG_LS_CL_LOGIN,

		// messages between client and game server
		MSG_CL_GS_SOMETHING = 100,
	};

	enum LoginResponse
	{
		LOGIN_OK=0,
		LOGIN_USER_NOT_FOUND,
	};
}

#endif // sharedData_h__