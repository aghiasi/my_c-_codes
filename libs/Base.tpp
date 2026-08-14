#include <iostream>
using namespace std;
template <class T>
int Base<T>::counter = 0;
template <class T>
void Base<T>::setY(T a)
{
    y = a;
}
template <class T>
void Base<T>::setX(T a)
{
    x = a;
}
template <class T>
Base<T>::Base(T a, T b) : x(a), y(b)
{
    objCounter();
};
template <class T>
Base<T>::~Base()
{
    cout << "cleared\n";
    objDecrise();
};
template <class T>
T Base<T>::getX()
{
    return x;
}
template <class T>
T Base<T>::getY()
{
    return y;
}
template <class T>
Base<T> &Base<T>::operator=(const T a)
{
    x = a, y = a;
    return *this;
};
template <class T>
void Base<T>::objCounter()
{
    counter++;
}
template <class T>
void Base<T>::objDecrise()
{
    counter--;
}
template <class T>
int Base<T>::getObj()
{
    return counter;
}
template <class T>
void Drived<T>::display()
{
    cout << "x is : " << this->getX() << '\n'
         << "y is : " << this->getY() << '\n' 
         << "count is :" << this->getObj() << '\n' ; 
}
template <class T>
Drived<T>::Drived(T a , T b):Base<T>(a,b){}