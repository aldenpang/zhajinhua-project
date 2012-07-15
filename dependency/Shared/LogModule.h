/********************************************************************
	Copyright (C) 2005 by Pang Shuo
	@date:	    2012-1-20   16:03
	@file: 		LogModule.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _LOGMODULE_H_
#define _LOGMODULE_H_
#include <QString>
#include <QList>
#include <QQueue>
#include <QFile>
#include <QThread>
#include <QtNetwork>
#include <QSharedPointer>

/** */
enum LogLevel
{
	LL_INFO=0,
	LL_WARN,
	LL_ERROR,

	LL_TOTAL
};

class LogModule : public QObject
{
	Q_OBJECT
public:
	~LogModule(){};

	static LogModule& GetSingleton()
	{
		static LogModule singleton;
		return singleton;
	}
public slots:
	void SetOutputLevel(LogLevel _level){mLevel = _level;}

	void StInfo(const QString& _text);
	void StWarn(const QString& _text);
	void StError(const QString& _text);
		
public:
	void SetModuleName(const QString _fileName){ mLogFileName = _fileName; }
protected:
private:
	LogModule();	

	LogLevel mLevel;
	QString mLogDir;
	QString mLogFileName;
	QString mTodayStr;

	void writeToFile(LogLevel _level, QString _log);
};
#define LOG LogModule::GetSingleton()

//#define DEF_LOG signals: void SiInfo(const QString& _text);void SiWarn(const QString& _text);void SiError(const QString& _text);

#define LOG_INFO(x) emit SiInfo(QString("[%1]-%2").arg(__FUNCTION__).arg(x))
#define LOG_WARN(x) emit SiWarn(QString("[%1]-%2").arg(__FUNCTION__).arg(x))
#define LOG_ERR(x) emit SiError(QString("[%1]-%2").arg(__FUNCTION__).arg(x))

#define LOG_D_INFO(x) LOG.StInfo(QString("[%1]-%2").arg(__FUNCTION__).arg(x))
#define LOG_D_WARN(x) LOG.StWarn(QString("[%1]-%2").arg(__FUNCTION__).arg(x))
#define LOG_D_ERR(x) LOG.StError(QString("[%1]-%2").arg(__FUNCTION__).arg(x))

#endif //_LOGMODULE_H_

/*
 *
 * [Revision 1.0   2012-1-20   16:03  Administrator] Created
 *
 */