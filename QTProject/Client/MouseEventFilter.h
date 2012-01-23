#ifndef _MOUSEEVENTFILTER_H_
#define _MOUSEEVENTFILTER_H_


class MouseEventFilter : public QObject
{
	Q_OBJECT
public:
	enum
	{
		Enable_Drag = 1, ///< �����϶�
		Enable_Scale = 2 ///< ��������
	};
public:
	/// \Param [in] _wacthedWidget 	�ܼ�����widget
	/// \Param [in] _disWidget 	��Ӱ���widget
	/// \Param [in] _style 		��ʽ
	MouseEventFilter( QWidget* _wacthedWidget = 0, QWidget* _disWidget = 0, int _style = 0 );
	void SetStyle(int _style);
	int GetStyle();
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
private:
	QPoint mOffsetPos;		//��갴��ʱ���������ڼ�������Ͻ������ƫ��
	int  mStyle;			//��ʽ
	bool mIsMouseDown;		//�������Ƿ���
	bool mIsArrowInLeft;	//����Ƿ���ͣ�����Ե
	bool mIsArrowInRight;	//����Ƿ���ͣ���ұ�Ե
	bool mIsArrowInBottom;	//����Ƿ���ͣ���±�Ե
};
#endif //_MOUSEEVENTFILTER_H_
