#include "stdafx.h"
#include "BaseDialog.h"
#include "LogModule.h"

QWidget* widget;
BaseDialog::BaseDialog( QString& _text, bool _needCancel/*=true*/ )
{
	QUiLoader uiLoader;
	QFile uiFile(QString(":/Client/noticeDialog.ui"));
	widget = uiLoader.load(&uiFile);
	if (!widget)
	{
		LOG_D_ERR("Create Error!");
		return;
	}

	mQDialog = qobject_cast<QDialog*>(widget);
	uiFile.close();
	
	if ( !_needCancel )
	{
		mQDialog->findChild<QPushButton*>("cancelButton")->hide();
		mQDialog->findChild<QPushButton*>("okButton")->move(130, 140);
	}

	mQDialog->findChild<QTextEdit*>("textEdit")->setText(_text);
	mQDialog->findChild<QTextEdit*>("textEdit")->setAlignment(Qt::AlignHCenter);

	connect(mQDialog->findChild<QPushButton*>("okButton"), SIGNAL(clicked()), mQDialog, SLOT(accept()));
	connect(mQDialog->findChild<QPushButton*>("cancelButton"), SIGNAL(clicked()), mQDialog, SLOT(reject ()));

}

BaseDialog::~BaseDialog()
{

}

void BaseDialog::Exec()
{
	mQDialog->exec();
}