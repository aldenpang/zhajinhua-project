#include "Downloader.h"
#include "LogModule.h"

Downloader::Downloader()
{

}

Downloader::~Downloader()
{

}

void Downloader::Start( QString& _from, QString& _to )
{
	mRawFileName = _to;
	if ( _from.isEmpty() || _to.isEmpty() )
	{
		LOG_D_ERR(QString("file name is empty! from[%1] to[%2]").arg(_from).arg(_to));
		return;
	}

	// handle file
	mFile.setFileName(_to+QString(".temp"));
	if ( !mFile.open(QIODevice::WriteOnly | QIODevice::Append) )
	{
		LOG_D_ERR(QString("File[%1] created failed!").arg(_to));
		return;
	}
	mHttpMgr = new QNetworkAccessManager();
	connect(mHttpMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(stFinished(QNetworkReply*)));

	QNetworkRequest req;
	req.setUrl(_from);
	//req.setRawHeader("Range", "bytes=0-");

	mHttpReply = mHttpMgr->get(req);
	//connect(mHttpReply, SIGNAL(downloadProgress(quint64, quint64)), this, SLOT(stProgress(quint64, quint64)));
	connect(mHttpReply, SIGNAL(readyRead()), this, SLOT(stReadyRead()));
	connect(mHttpReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(stHttpError(QNetworkReply::NetworkError)));

}

void Downloader::Stop()
{
	if ( mHttpReply )
	{
		mHttpReply->abort();
	}
	if ( mFile.isOpen() )
	{
		mFile.flush();
		mFile.close();
	}
}

void Downloader::stFinished( QNetworkReply* _reply )
{
	if ( mFile.isOpen() )
	{
		mFile.flush();
		mFile.close();

		// overwrite and rename
		QFile::remove(mRawFileName);
		mFile.rename(mRawFileName);

	}
}

void Downloader::stReadyRead()
{
	if ( mFile.isOpen() )
	{
		mFile.write(mHttpReply->readAll());
	}
}

void Downloader::stHttpError( QNetworkReply::NetworkError _err )
{
	LOG_D_ERR(QString("QT Http Error[%1]").arg(_err));
}

void Downloader::stProgress( quint64 _done, quint64 _total )
{

}
