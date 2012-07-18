/********************************************************************
	created:	 2012/07/17
	created:	 17:7:2012   15:28
	file name:	 Downloader.h
	author:		 PangShuo
	description:
*********************************************************************/


#ifndef Downloader_h__
#define Downloader_h__
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Downloader : public QObject
{
	Q_OBJECT
public:
	Downloader();
	~Downloader();

	void Start(QString& _from, QString& _to);
	void Stop();
signals:
	void SiStart();
	void SiFinished();
	void SiError(QString& _err);
	void SiRate(quint32 _rate);	// 0~100
protected:
private:
	QNetworkAccessManager*	mHttpMgr;
	QNetworkReply*			mHttpReply;

	QFile					mFile;
	QString					mRawFileName;

private slots:
	void stFinished(QNetworkReply* _reply);
	void stReadyRead();
	void stHttpError(QNetworkReply::NetworkError _err);
	void stProgress(quint64 _done, quint64 _total);
};

#endif // Downloader_h__