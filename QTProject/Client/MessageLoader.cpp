#include "stdafx.h"
#include "MessageLoader.h"


void MessageLoader::Load( QString& _file )
{
	QFile file(_file);
	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
		return;

	QTextStream ts(&file);
	while ( !ts.atEnd() )
	{
		QString& line = ts.readLine();
		QRegExp rx("(\\d*),(.*)");
		int pos = line.indexOf(rx);
		if ( pos != -1 )
		{
			mMessages[rx.cap(1).toUInt()] = rx.cap(2);
		}
	}
}

QString& MessageLoader::GetStr( quint32 _index )
{
	return mMessages[_index];
}
