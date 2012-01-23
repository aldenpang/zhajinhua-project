#ifndef _MOUSEPROPAGATION_H_
#define _MOUSEPROPAGATION_H_

///  
/// \class MousePropagation
class MousePropagation : public QObject
{
public:
	MousePropagation(QObject* _parent);

	void Enable();
	void Disable();
protected:
	virtual bool eventFilter(QObject *obj, QEvent *event);
private:
	bool mEnable;
};//class MousePropagation
#endif //_MOUSEPROPAGATION_H_
