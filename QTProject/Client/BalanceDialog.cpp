#include "stdafx.h"
#include "BalanceDialog.h"

BalanceDialog::BalanceDialog( QWidget* _base)
{
	mMain = _base;
	mMain->hide();


	regConnections();
}

BalanceDialog::~BalanceDialog()
{

}

void BalanceDialog::SetAndShow( QMap<int, int>& _result, QMap<int, TablePlayer>& _players )
{
	QMap<int, int>::Iterator itr;
	int i = 0;
	for ( itr = _result.begin(); itr != _result.end(); itr++)
	{
		if ( itr.value() != 0 )
		{
			QString nickName = _players[itr.key()].mNickName;
			int score = itr.value();
			int tableMoney = _players[itr.key()].mTableMoney;

			QLabel* nickNameLabel = mMain->findChild<QLabel*>(QString("bl_nickname_%1").arg(i));
			nickNameLabel->setText(nickName);
			QLabel* scoreLabel = mMain->findChild<QLabel*>(QString("bl_winscore_%1").arg(i));
			scoreLabel->setText(QString("%1").arg(score));
			QLabel* totalScoreLabel = mMain->findChild<QLabel*>(QString("bl_totalscore_%1").arg(i));
			totalScoreLabel->setText(QString("%1").arg(tableMoney));

			i++;
		}
	}

	mMain->show();
}

void BalanceDialog::StHide()
{
	for ( int i = 0; i<MAX_PLAYER; i++ )
	{
		QLabel* nickName = mMain->findChild<QLabel*>(QString("bl_nickname_%1").arg(i));
		nickName->setText("---");

		QLabel* winScore = mMain->findChild<QLabel*>(QString("bl_winscore_%1").arg(i));
		winScore->setText("0");

		QLabel* totalScore = mMain->findChild<QLabel*>(QString("bl_totalscore_%1").arg(i));
		totalScore->setText("0");

	}

	mMain->hide();
}

void BalanceDialog::regConnections()
{
	connect(mMain->findChild<QPushButton*>("blbtn_continue"), SIGNAL(clicked()), this, SLOT(StHide()));
	connect(mMain->findChild<QPushButton*>("blbtn_continue"), SIGNAL(clicked()), this, SIGNAL(SiContinue()));
	connect(mMain->findChild<QPushButton*>("blbtn_back"), SIGNAL(clicked()), this, SIGNAL(SiBack()));
	connect(mMain->findChild<QPushButton*>("blbtn_back"), SIGNAL(clicked()), this, SLOT(StHide()));
}
