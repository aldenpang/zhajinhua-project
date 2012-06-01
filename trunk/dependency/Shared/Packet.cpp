#include "Packet.h"
#include "assert.h"
#include <string>

Packet::Packet()
: mMessage(0)
{
	mPtr = QSharedPointer<QDataStream>(new QDataStream(&mBuffer, QIODevice::ReadWrite));
	mPtr->setVersion(QDataStream::Qt_4_8);
	mPtr->setByteOrder(QDataStream::LittleEndian);

	*(mPtr.data())<<quint32(0)<<quint32(0)<<quint32(0);
}

Packet::~Packet()
{}

bool Packet::IsTokenValid()
{
	quint32 token = 0;
	mPtr->device()->seek(POS_TOKEN);
	*(mPtr.data())>>token;
	mPtr->device()->seek(POS_DATA);
	if ( token == TOKEN )
		return true;
	else 
		return false;
}

int Packet::GetMessage()
{
	int msg = 0;
	mPtr->device()->seek(POS_MSG_TYPE);
	*(mPtr.data())>>msg;
	mPtr->device()->seek(POS_DATA);

	return msg;
}

void Packet::SetMessage( int _message )
{
	mMessage = _message;
}

void Packet::End()
{
	mPtr->device()->seek(0);
	*(mPtr.data())<<mBuffer.length();
	*(mPtr.data())<<TOKEN;
	*(mPtr.data())<<mMessage;
}

Packet& Packet::operator << (qint32 _val)
{
	*(mPtr.data())<<_val;
	return *this;
}

Packet& Packet::operator << (quint32 _val)
{
	*(mPtr.data())<<_val;
	return *this;
}

Packet& Packet::operator << (qreal _val)
{
	*(mPtr.data())<<_val;
	return *this;
}

Packet& Packet::operator << (QString& _val)
{
	QByteArray arr = _val.toUtf8();
	quint32 s = arr.size();
	*(mPtr.data())<<s;
	for ( int i = 0; i<s; ++i )
	{
		*(mPtr.data())<<quint8(arr.at(i));
	}
	return *this;
}

Packet& Packet::operator >> (qint32& _val)
{
	if ( mPtr->atEnd() )
		_val = 0;
	else
	{
		*(mPtr.data())>>_val;
	}
	return *this;
}

Packet& Packet::operator >> (quint32& _val)
{
	if ( mPtr->atEnd() )
		_val = 0;
	else
	{
		*(mPtr.data())>>_val;
	}
	return *this;
}

Packet& Packet::operator >> (qreal& _val)
{
	if ( mPtr->atEnd() )
		_val = 0.0f;
	else
	{
		*(mPtr.data())>>_val;
	}
	return *this;
}

Packet& Packet::operator >> (QString& _val)
{
	if ( mPtr->atEnd() )
		_val = QString();
	else
	{
		QByteArray arr;
		quint32 s = 0;
		*(mPtr.data())>>s;
		for ( int i = 0; i<s; ++i )
		{
			quint8 chr;
			*(mPtr.data())>>chr;
			arr.append(chr);
		}
		_val = QString::fromUtf8(arr);
	}

	return *this;
}