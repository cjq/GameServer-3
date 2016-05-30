#ifndef SINGLETON_H
#define SINGLETON_H

template<class T>
class Singleton
{
public:
	static T* instance()
	{
		if (!_instance)
		{
			_instance = new T;
		}
		return _instance;
	}


protected:
	// 使用保护构造是为了用户不能再栈上声明一个实例
	Singleton() {}
private:
	static T* _instance;
};

template <class T> T* Singleton<T>::_instance = NULL;

#endif 