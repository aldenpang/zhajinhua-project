#include "stdafx.h"
#include "Table.h"
#include "LogModule.h"
#include "Setting.h"

Table::Table()
// : QObject(NULL)
: mID(0)
, mCurrentSeatID(0)
{

}

Table::~Table()
{

}

void Table::Init()
{
	mBringMoneyDlg = mWidget->findChild<QWidget*>("bringMoney");
	mBringMoneyDlg->hide();

	QLabel* tableID = mWidget->findChild<QLabel*>("tableID");
	tableID->setText(QString("%1").arg(mID+1));

	// 顺序是按照顺时针方向，上0，右1，下2，左3
	//	   [0]
	//[3]		[1]
	//	   [2]
	mSeat[0] = mWidget->findChild<QPushButton*>("seat0");
	mSeat[1] = mWidget->findChild<QPushButton*>("seat1");
	mSeat[2] = mWidget->findChild<QPushButton*>("seat2");
	mSeat[3] = mWidget->findChild<QPushButton*>("seat3");

	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		mSeat[i]->setIconSize(mSeat[i]->size());
	}

	mNameLabel[0] = mWidget->findChild<QTextEdit*>("name0");
	mNameLabel[1] = mWidget->findChild<QTextEdit*>("name1");
	mNameLabel[2] = mWidget->findChild<QTextEdit*>("name2");
	mNameLabel[3] = mWidget->findChild<QTextEdit*>("name3");

	regConnection();
}

void Table::UpdateTableInfo( quint32 _tableID, TableData _data)
{
	if ( _tableID != mID )
	{
		LOG_D_ERR(QString("Wrong TableID[%1], self id is[%2]").arg(_tableID).arg(mID));
		return;
	}

	QMap<int, TablePlayer> players = _data.GetPlayers();
	QMap<int, TablePlayer>::iterator itr;
	for( itr = players.begin(); itr != players.end(); itr++ )
	{
		int seatID = itr.key();
		UpdatePlayer(seatID, itr.value());

		//if ( seatID < 0 || seatID >3 )
		//{
		//	LOG_D_ERR(QString("Invaild seatID[%1]").arg(seatID));
		//	break;
		//}
		//if ( itr.value().mNickName != EMPTY_SEAT )
		//{
		//	mNameLabel[seatID]->setText(itr.value().mNickName);
		//}
	}

}

void Table::regConnection()
{
	for ( int i = 0; i<MAX_PLAYER; i++ )
		connect(mSeat[i], SIGNAL(clicked()), this, SLOT(stSit()));

	QPushButton* btn0 = mWidget->findChild<QPushButton*>("bringMoneyBtn0");
	connect(btn0, SIGNAL(clicked()), this, SLOT(stBringMoney()));

	QPushButton* btn1 = mWidget->findChild<QPushButton*>("bringMoneyBtn1");
	connect(btn1, SIGNAL(clicked()), this, SLOT(stBringMoney()));

	QPushButton* btn2 = mWidget->findChild<QPushButton*>("bringMoneyBtn2");
	connect(btn2, SIGNAL(clicked()), this, SLOT(stBringMoney()));

	QPushButton* btn3 = mWidget->findChild<QPushButton*>("bringMoneyBtn3");
	connect(btn3, SIGNAL(clicked()), this, SLOT(stBringMoneyInput()));

}

void Table::stShowBringMoneyDlg()
{
	mBringMoneyDlg->show();
	
	// show amount of money on buttons, according to minimum amount of bring money
	mWidget->findChild<QPushButton*>("bringMoneyBtn0")->setText(QString("%1").arg(SETTINGS.GetRoomInfo().mMinMoney*2));
	mWidget->findChild<QPushButton*>("bringMoneyBtn1")->setText(QString("%1").arg(SETTINGS.GetRoomInfo().mMinMoney*3));
	mWidget->findChild<QPushButton*>("bringMoneyBtn2")->setText(QString("%1").arg(SETTINGS.GetRoomInfo().mMinMoney*4));

}


void Table::stBringMoney()
{
	mBringMoneyDlg->hide();
	int amount = ((QPushButton*)sender())->text().toInt();
	emit SiBringMoney(mID, mCurrentSeatID, amount);
}


void Table::stBringMoneyInput()
{
	mBringMoneyDlg->hide();
	// get amount of money from user input
	int amount = mWidget->findChild<QLineEdit*>("inputMoney")->text().toInt();
	emit SiBringMoney(mID, mCurrentSeatID, amount);
}

void Table::stSit()
{
	int seatID = -1;
	for ( int i = 0; i<MAX_PLAYER; i++)
	{
		if ( sender() == mSeat[i] )
		{
			seatID = i;
			break;
		}
	}
	if ( seatID == -1 )
	{
		LOG_D_ERR(QString("SeatID[%1] is not found").arg(seatID));
		return;
	}
	else
	{
		LOG_D_INFO(QString("Player click on seatID[%1]").arg(seatID));
		mCurrentSeatID = seatID;
	}

	emit SiSit(mID, mCurrentSeatID);
}


void Table::UpdatePlayer( quint32 _seatID, TablePlayer _player )
{
	if ( _seatID < 0 || _seatID > 3 )
	{
		LOG_D_ERR(QString("Invaild seatID[%1]").arg(_seatID));
		return;
	}
	if ( _player.mNickName != EMPTY_SEAT )
	{
		mNameLabel[_seatID]->setText(_player.mNickName);
		mSeat[_seatID]->setIcon(QIcon(QString(":/Portraits/Media/Portrait/%1.png").arg(_player.mProtraitID)));
	}

	//// if this is player self, then show bring money dialog
	//if ( _player.mNickName == SETTINGS.GetPlayer().GetNickName() )
	//{
	//	if ( SETTINGS.GetRoomInfo().mMoneyType == 1 )	// 1 is gold coin romm
	//	{
	//		stShowBringMoneyDlg();
	//	}
	//	else
	//	{
	//		emit SiBringMoney(mID, mCurrentSeatID, 0);
	//	}
	//}
}

void Table::PlayerLeave(TablePlayer _player)
{
	quint32 selfTableID = mID;
	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		QString ddd = mNameLabel[i]->toPlainText();
		if ( mNameLabel[i]->toPlainText() == _player.mNickName )
		{
			mNameLabel[i]->setText(EMPTY_SEAT);
			mSeat[i]->setIcon(QIcon());
		}
	}
	
}

