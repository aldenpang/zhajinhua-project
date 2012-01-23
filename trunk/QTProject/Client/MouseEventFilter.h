#ifndef _MOUSEEVENTFILTER_H_
#define _MOUSEEVENTFILTER_H_


class MouseEventFilter : public QObject
{
	Q_OBJECT
public:
	enum
	{
		Enable_Drag = 1, ///< 允许拖动
		Enable_Scale = 2 ///< 允许缩放
	};
public:
	/// \Param [in] _wacthedWidget 	受监听的widget
	/// \Param [in] _disWidget 	受影响的widget
	/// \Param [in] _style 		样式
	MouseEventFilter( QWidget* _wacthedWidget = 0, QWidget* _disWidget = 0, int _style = 0 );
	void SetStyle(int _style);
	int GetStyle();
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
private:
	QPoint mOffsetPos;		//鼠标按下时的鼠标相对于激活窗口左上角坐标的偏移
	int  mStyle;			//样式
	bool mIsMouseDown;		//鼠标左键是否按下
	bool mIsArrowInLeft;	//鼠标是否悬停在左边缘
	bool mIsArrowInRight;	//鼠标是否悬停在右边缘
	bool mIsArrowInBottom;	//鼠标是否悬停在下边缘
};
#endif //_MOUSEEVENTFILTER_H_
