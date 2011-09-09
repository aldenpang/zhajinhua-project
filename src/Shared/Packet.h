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

class Packet
{
public:
	Packet();
	~Packet();

	void SetMessage(int _message);
	void WriteData(void* _data, int _size);


protected:
private:
	int mMessage;
	char mBuff[MAX_PACKET_SIZE];
};
#endif // Packet_h__