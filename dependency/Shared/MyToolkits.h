/********************************************************************
	created:	2012/03/02
	created:	2:3:2012   16:10
	filename: 	d:\My Projects\ZJHClient&Server\dependency\Shared\MyToolkits.h
	file path:	d:\My Projects\ZJHClient&Server\dependency\Shared
	file base:	MyToolkits
	file ext:	h
	author:		PangShuo
*********************************************************************/


#ifndef MyToolkits_h__
#define MyToolkits_h__

#define PROPERTY(x, y) public: inline void Set##y(x t){m##y=t;};inline x Get##y(){return m##y;};private: x m##y;

#define SAFE_DELETE(p) {if(p){delete p; p=NULL;}}

#define SAFE_DELETE_ARR(p) {if(p){delete []p; p=NULL;}}

// singleton templete
template <typename T> class Singleton
{
public:
	static T& GetSingleton(void)
	{
		static T s;
		return s;
	}
};

#endif // MyToolkits_h__