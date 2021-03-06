#include "MD5.h"
#include <QCryptographicHash>

// make md5
QString ToMD5(QString& _input)
{
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(_input.toUtf8());
	QByteArray dd = hash.result();
	return QString(dd);
}