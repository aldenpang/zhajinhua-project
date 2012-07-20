#include "stdafx.h"
#include "Patcher.h"
#include "LogModule.h"
#include <QDomDocument>
#include "crc/crc.h"
#include "MyToolkits.h"

Patcher::Patcher()
{

}

Patcher::~Patcher()
{

}

void Patcher::Start( QString& _file )
{
	parse(_file);

	QVector<PatchItem> tobe = compareFiles(mPatchItems);

	download(tobe);
}

void Patcher::parse( QString& _file )
{
	QFile file(_file);
	if ( !file.open(QIODevice::ReadOnly) )
	{
		LOG_D_ERR(QString("Load [%1] failed").arg(_file));
		return;
	}

	QDomDocument doc;
	if ( !doc.setContent(&file) )
	{
		file.close();
		LOG_D_ERR(QString("Create QDomDocument for [%1] failed").arg(_file));
		return;
	}

	QDomElement root = doc.documentElement();
	QDomNode node = root.firstChild();
	while ( !node.isNull() )
	{
		PatchItem patchItem;
		QString name = node.nodeName();
		QDomNamedNodeMap attrMap = node.attributes();
		patchItem.mSize = attrMap.item(0).nodeValue().toUInt();
		patchItem.mPath = attrMap.item(1).nodeValue();
		patchItem.mCRC = attrMap.item(2).nodeValue();

		mPatchItems.push_back(patchItem);
		node = node.nextSibling();
	}

	return;
}

QVector<PatchItem> Patcher::compareFiles( QVector<PatchItem>& _allItems )
{
	QVector<PatchItem> tobeDownloaded;

	for ( int i = 0; i<mPatchItems.size(); i++)
	{
		if(!isSameFile(mPatchItems[i].mPath, mPatchItems[i].mSize, mPatchItems[i].mCRC))
		{
			tobeDownloaded.push_back(mPatchItems[i]);
		}
	}

	return tobeDownloaded;
}

void Patcher::download( QVector<PatchItem>& _items )
{

}

bool Patcher::isSameFile(const QString& _file, const quint64 _size, const QString& _crc)
{
	QFile file(_file);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDataStream in(&file);
	quint64 fileSize = file.size();
	if ( _size != fileSize )
		return false;
	unsigned char* data = new unsigned char[fileSize];
	int res = in.readRawData((char*)data, fileSize);
	quint32 crc = crc32c_sb8(NULL, data, file.size(), 0, MODE_ALIGN);
	SAFE_DELETE_ARR(data);
	QString& crcHex = QString("%1").arg(crc, 8, 16, QLatin1Char('0'));
	return (_crc == crcHex);
}