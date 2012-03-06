/********************************************************************
	created:	2012/03/06
	created:	6:3:2012   10:22
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\DBLayer.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	DBLayer
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef DBLayer_h__
#define DBLayer_h__

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

#include "MyToolkits.h"

class DBLayer : public QObject
{
Q_OBJECT
signals:
	void SiInfo(QString _info);
	void SiError(QString _error);
public:
	DBLayer();
	~DBLayer();

	int Connect(QString _dbName);

protected:
	QSqlDatabase mDb;
private:
};

#endif // DBLayer_h__