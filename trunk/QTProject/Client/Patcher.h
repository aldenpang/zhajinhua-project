/********************************************************************
	created:	 2012/07/19
	created:	 19:7:2012   15:07
	file name:	 Patcher.h
	author:		 PangShuo
	description: load PatchFileList.xml, compare then download if needed
*********************************************************************/


#ifndef Patcher_h__
#define Patcher_h__

#include <QObject>

struct PatchItem 
{
	QString mPath;
	quint32 mSize;
	QString mCRC;
};

class Patcher : public QObject
{
Q_OBJECT
public:
	Patcher();
	~Patcher();

	// load PatchFileList.xml which already downloaded from remote
	void Start(QString& _file);

signals:
	void SiProgress(quint64 _done, quint64 _total);

private:
	QVector<PatchItem> mPatchItems;

	void parse(QString& _file);
	// compare file's size and crc code then decide which file will be downloaded
	QVector<PatchItem> compareFiles(QVector<PatchItem>& _allItems);

	void download(QVector<PatchItem>& _items);

	bool isSameFile(const QString& _file, const QString& _crc);
};



#endif // Patcher_h__