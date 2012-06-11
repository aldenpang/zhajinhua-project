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

int DBLayer::Connect(const QString& _dbPath, const QString& _dbName)
{
	mDb = QSqlDatabase::addDatabase("QSQLITE", _dbName);
	mDb.setDatabaseName(_dbPath);
	if ( !mDb.open() )
	{
		LOG_INFO(QString("%1:db open error").arg(metaObject()->className()));
	}
	bool isSupportQuerySize = mDb.driver()->hasFeature(QSqlDriver::QuerySize);

	return 0;
}
