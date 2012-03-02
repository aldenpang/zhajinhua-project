/********************************************************************
	Copyright (C) 2005 by Bele Group
	@date:	    2012-1-20   16:03
	@file: 		LogModule.h
	@author:    Snake
	@desc:		
*********************************************************************/
#ifndef _LOGMODULE_H_
#define _LOGMODULE_H_
#include "PublicHeader.h"

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
		
protected:
private:
	LogModule();	

	LogLevel mLevel;

	void writeToFile(QString _log);
};
#define LOG LogModule::GetSingleton()

#endif //_LOGMODULE_H_

/*
 *
 * [Revision 1.0   2012-1-20   16:03  Administrator] Created
 *
 */