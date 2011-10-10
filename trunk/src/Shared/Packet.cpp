#include "stdafx.h"
#include "Packet.h"
#include "assert.h"

Packet::Packet()
{
	memset(mBuff, 0, MAX_PACKET_SIZE);
	mPtr = mBuff+POS_DATA;
}

Packet::~Packet()
{}

void Packet::SetData(const char* _data)
{
	assert(_data != NULL);

	memcpy(mBuff, _data, MAX_PACKET_SIZE);
}

bool Packet::IsTokenValid()
{
	long token = 0;
	memcpy(&token, mBuff, sizeof(long));
	if ( token == TOKEN )
		return true;
	else 
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
	long token = TOKEN;
	memcpy(mBuff, &token, sizeof(long));

	return;
}

unsigned int Packet::GetDataLength()
{
	return mPtr - mBuff;
}
