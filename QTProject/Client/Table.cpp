#include "stdafx.h"
#include "Table.h"

Table::Table()
// : QObject(NULL)
: mID(0)
{

}

Table::~Table()
{

}

void Table::Init()
{
	QLabel* tableID = mWidget->findChild<QLabel*>("tableID");
	tableID->setText(QString("%1").arg(mID+1));
}

void Table::StSit( quint32 _tableID, quint32 _seatID, quint32 _accountID )
{

}