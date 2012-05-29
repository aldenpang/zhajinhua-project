#include "LogModule.h"
#include <QDir>
#include <QDate>

//------------------------------------------------------------------------------
LogModule::LogModule()
: mLevel(LL_INFO)
, mLogDir(QDir::currentPath()+QString("/Log/"))
, mLogFileName("Unknown_Log")
{
	QDir dir;
	dir.mkdir(mLogDir);

	QDate date = QDate::currentDate();
	mTodayStr = QString("_%1_%2_%3").arg(date.year()).arg(date.month()).arg(date.day());
}

//------------------------------------------------------------------------------
void LogModule::StInfo(const QString& _text)
{
	QString log = QString("INFO:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(LL_INFO, log);
}
//------------------------------------------------------------------------------
void LogModule::StWarn(const QString& _text)
{
	QString log = QString("WARN:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(LL_WARN, log);
}

//------------------------------------------------------------------------------
void LogModule::StError(const QString& _text)
{
	QString log = QString("ERROR:%1").arg(_text);
	qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<log;
	writeToFile(LL_ERROR, log);
}

void LogModule::writeToFile(LogLevel _level,  QString _log )
{
	QString color;
	if ( _level == LL_INFO )
		color = "#006000";
	else if ( _level == LL_WARN )
		color = "#FF8000";
	else if ( _level == LL_ERROR )
		color = "#AE0000";
	else 
		color = "#000000";

	QFile file(mLogDir+mLogFileName+mTodayStr+QString(".html"));
	if(file.open(QIODevice::Append | QIODevice::Text))
	{
		QTextStream out(&file);
		out <<QString("<font color=\"")+color+QString("\">")<<QTime::currentTime().toString("hh:mm:ss.zzz")<<"-"<<_log<<QString("</font><br>");
	}
	file.close();
}
