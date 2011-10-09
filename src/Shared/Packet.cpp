#include "stdafx.h"
#include "Packet.h"
#include "assert.h"

Packet::Packet()
{
	memset(mBuff, 0, MAX_PACKET_SIZE);
	mPtr = mBuff+POS_DATA;

	//SetHeader();

	//SetMessage(100000);

	//int msg = GetMessage();

	//int dd = 222;
	//Put(dd);
	//char dddd[32] = {0};
	//sprintf(dddd, "abddsfasf");
	//Put(dddd);

	return;
}

Packet::~Packet()
{}

void Packet::SetData(const char* _data)
{
	assert(_data != NULL);

	strcpy_s(mBuff, _data);
}

bool Packet::IsTokenValid()
{
	//if(strstr(mBuff, mToken))
	//{
	//	return true;
	//}
	//else
		return false;
}

int Packet::GetMessage()
{
	char* ptr = mBuff+POS_MSG_TYPE;
	
	int msg = 0;
	memcpy(&msg, ptr, sizeof(int));

	return msg;
}

void Packet::SetMessage( int _message )
{
	char* ptr = mBuff+POS_MSG_TYPE;
	memcpy(ptr, &_message, sizeof(int));
}

void Packet::put( void* _data, int _size )
{
	memcpy(mPtr, _data, _size);
	mPtr += _size;
}
void  Packet::Put(int _data)
{
	put(&_data, sizeof(int));
}

void  Packet::Put(float _data)
{
	put(&_data, sizeof(float));
}

void  Packet::Put(char* _data)
{
	int len = strlen(_data);
	put(_data, len);
}

void Packet::get(void* _data, int _size)
{
	assert(_data!=NULL);

	memcpy(_data, mPtr, _size);
}

void Packet::Get(int* _data)
{
	get(_data, sizeof(int));
}

void Packet::Get(float* _data)
{
	get(_data, sizeof(float));
}

void Packet::Get(char* _data, int _length)
{
	get(_data, _length);
}

void Packet::SetHeader()
{
	long token = 1165417621;
	memcpy(mBuff, &token, sizeof(long));

	return;
}

// void Packet::GetData( char* _output )
// {
// 	assert(_output != NULL);
// 
// 	memcpy(_output, mBuff, MAX_PACKET_SIZE);
// 
// }

unsigned int Packet::GetDataLength()
{
	return mPtr - mBuff;
}
