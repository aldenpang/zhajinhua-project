/********************************************************************
	created:	2011/09/08
	created:	8:9:2011   16:55
	filename: 	d:\My Projects\ZJHClient&Server\src\Shared\Packet.h
	file path:	d:\My Projects\ZJHClient&Server\src\Shared
	file base:	Packet
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef Packet_h__
#define Packet_h__

#define MAX_PACKET_SIZE 10240
#define MAX_TOKEN_SIZE 16

class Packet
{
public:
	Packet();
	~Packet();

	void SetData(const char* _data);

	bool IsTokenValid();

	int GetMessage();

	void SetMessage(int _message);
	void Put(void* _data, int _size);


protected:
private:
	int mMessage;
	char mBuff[MAX_PACKET_SIZE];
	char mToken[MAX_TOKEN_SIZE+1];	// +1 for '/0'
};
#endif // Packet_h__