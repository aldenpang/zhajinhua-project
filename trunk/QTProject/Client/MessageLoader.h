/********************************************************************
	created:	 2012/08/14
	created:	 14:8:2012   14:47
	file name:	 MessageLoader.h
	author:		 PangShuo
	description:
*********************************************************************/


#ifndef MessageLoader_h__
#define MessageLoader_h__

#include "MyToolkits.h"

class MessageLoader : public Singleton<MessageLoader>
{
public:

	void Load(QString& _file);
	QString& GetStr(quint32 _index);

protected:
private:
	QMap<int, QString> mMessages;
};

#define MSGLOADER MessageLoader::GetSingleton()

#endif // MessageLoader_h__