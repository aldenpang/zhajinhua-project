#include "LogModule.h"

//------------------------------------------------------------------------------
LogModule::LogModule()
: mLevel(LL_INFO)
{

}
//------------------------------------------------------------------------------
void LogModule::StInfo(const QString& _text)
{
	QString log = QString("INFO:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(log);
}
//------------------------------------------------------------------------------
void LogModule::StWarn(const QString& _text)
{
	QString log = QString("WARN:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(log);
}

//------------------------------------------------------------------------------
void LogModule::StError(const QString& _text)
{
	QString log = QString("ERROR:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(log);
}

void LogModule::writeToFile( QString _log )
{
	QFile file("Log.txt");
	if(file.open(QIODevice::Append | QIODevice::Text))
	{
		QTextStream out(&file);
		out <<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<_log<<QString("\n");
	}
	file.close();
}
