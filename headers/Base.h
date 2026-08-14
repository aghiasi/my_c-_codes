template <class T>
class Base
{
private:
    T y, x;
    static int counter;

public:
    Base(T, T);
    virtual ~Base();
    T getY();
    T getX();
    void setY(T);
    void setX(T);
    Base<T> &operator=(const T);
    static void objCounter();
    static void objDecrise();
    static int getObj();
    virtual void display() = 0;
};
template <class T>
class Drived : Base<T>
{
public:
    Drived(T,T);
    void display() override;
};
#include "../libs/Base.tpp"