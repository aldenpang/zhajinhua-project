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
#define POS_TOKEN 4
#define POS_MSG_SIZE 0
#define POS_MSG_TYPE 8
#define POS_DATA 12

#define TOKEN 1234

/////////////////////////////// Packet Header Format //////////////////////////////////////////////////
// | byte0 | byte1 | byte2 | byte3 | byte4 | byte5 | byte6 | byte7 | byte8 | byte9 | byte10 | byte11 | 
// |		Message Size		   |			Token			   |			Message Type		 |
/////////////////////////////// Packet Header Format //////////////////////////////////////////////////
#include <QByteArray>
#include <QDataStream>
#include <QSharedPointer>

class Packet
{
public:
	Packet();
	~Packet();

	void SetData(QByteArray& _arr){mBuffer = _arr;}
	const QByteArray& GetData(){return mBuffer;}

	bool IsTokenValid();

	int GetMessage();

	void SetMessage(int _message);

	void End();		// Move mPtr back to begging, call before send, to set token and data size

	Packet& operator << (qint32 _val);
	Packet& operator << (quint32 _val);
	Packet& operator << (qreal _val);
	Packet& operator << (QString& _val);

	Packet& operator >> (qint32& _val);
	Packet& operator >> (quint32& _val);
	Packet& operator >> (qreal& _val);
	Packet& operator >> (QString& _val);


protected:
private:
	int mMessage;
	QByteArray mBuffer;
	QSharedPointer<QDataStream> mPtr;


private:


};
#endif // Packet_h__