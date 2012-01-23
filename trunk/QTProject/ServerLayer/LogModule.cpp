#include "LogModule.h"

//------------------------------------------------------------------------------
LogModule::LogModule()
: mLevel(LL_INFO)
{

}
//------------------------------------------------------------------------------
void LogModule::StInfo(const QString& _text)
{
	qDebug()<<QString("INFO:")<<_text;
}
//------------------------------------------------------------------------------
void LogModule::StWarn(const QString& _text)
{
	qDebug()<<QString("WARN:")<<_text;
}

//------------------------------------------------------------------------------
void LogModule::StError(const QString& _text)
{
	qDebug()<<QString("ERROR:")<<_text;
}
