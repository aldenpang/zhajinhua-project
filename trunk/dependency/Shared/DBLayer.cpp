#include "DBLayer.h"
#include <QSqlDriver>

DBLayer::DBLayer()
{

}

DBLayer::~DBLayer()
{
	if ( mDb.isOpen() )
	{
		mDb.close();
	}
}

int DBLayer::Connect(QString _dbName)
{
	mDb = QSqlDatabase::addDatabase("QSQLITE");
	mDb.setDatabaseName(_dbName);
	if ( !mDb.open() )
	{
		emit SiInfo(QString("%1:db open error").arg(metaObject()->className()));
	}
	bool isSupportQuerySize = mDb.driver()->hasFeature(QSqlDriver::QuerySize);

	return 0;
}
