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
#define POS_TOKEN 0
#define POS_MSG_SIZE 4
#define POS_MSG_TYPE 8
#define POS_DATA 12

/////////////////////////////// Packet Header Format //////////////////////////////////////////////////
// | byte0 | byte1 | byte2 | byte3 | byte4 | byte5 | byte6 | byte7 | byte8 | byte9 | byte10 | byte11 | 
// |		Token				   |			Message Size	   |			Message Type		 |
/////////////////////////////// Packet Header Format //////////////////////////////////////////////////


class Packet
{
public:
	Packet();
	~Packet();

	void SetData(const char* _data);
	//void GetData(char* _output);
	const char* GetData(){return mBuff;}

	bool IsTokenValid();

	int GetMessage();

	void SetMessage(int _message);

	void Put(int _data);
	void Put(float _data);
	void Put(char* _data);
	
	void Get(int* _data);
	void Get(float* _data);
	void Get(char* _data, int _length);


	void SetHeader();	// call before send

	unsigned int GetDataLength();

protected:
private:
	int mMessage;
	char mBuff[MAX_PACKET_SIZE];
	char* mPtr;

private:
	void put(void* _data, int _size);
	void get(void* _data, int _size);

};
#endif // Packet_h__