/********************************************************************
	Copyright (C) 2012 by Alden Pang
	@date:	    13:8:2012   22:51
	@file: 		BaseDialog.h
	@author:    Alden Pang
	@desc:		
*********************************************************************/

#ifndef BaseDialog_h__
#define BaseDialog_h__

class QDialog;
class BaseDialog : QObject
{
	Q_OBJECT
public:
	BaseDialog(QString& _text, bool _needCancel=true);
	~BaseDialog();

	void Exec();
protected:
private:
	QDialog* mQDialog;
};

#endif // BaseDialog_h__