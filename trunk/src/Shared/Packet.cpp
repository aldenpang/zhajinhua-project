#include "stdafx.h"
#include "Packet.h"

Packet::Packet()
{
	memset(mBuff, 0, MAX_PACKET_SIZE);
}

Packet::~Packet()
{}


void Packet::SetMessage( int _message )
{

}

void Packet::WriteData( void* _data, int _size )
{

}
