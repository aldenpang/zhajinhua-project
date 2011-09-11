#include "stdafx.h"
#include "Packet.h"

Packet::Packet()
{
	memset(mBuff, 0, MAX_PACKET_SIZE);
	memset(mToken, 0, MAX_TOKEN_SIZE+1);	// +1 for '/0'
	strcpy_s(mToken, "pangshuopangshuo");
}

Packet::~Packet()
{}

void Packet::SetData(const char* _data)
{
	strcpy_s(mBuff, _data);
}

bool Packet::IsTokenValid()
{
	if(strstr(mBuff, mToken))
	{
		return true;
	}
	else
		return false;
}

int Packet::GetMessage()
{
	char msg = mBuff[MAX_TOKEN_SIZE];
	
	return atoi(&msg);
}

void Packet::SetMessage( int _message )
{

}

void Packet::Put( void* _data, int _size )
{

}
