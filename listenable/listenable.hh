#ifndef LISTENABLE_H
#define LISTENABLE_H
#include <functional>
#include <iterator>
#include <list>

using namespace std;

template <class T>

/// For Single datatype i.e int, bool,etc (not array)
class listenable
{
private:
    list<function<void()>> listeners;

public:
// For array, list, vector,etc. Modification should be 
// done by direct access to i and call notifyListener if the 
// changes should be reflected in the listeners

    T i;
public:
    listenable(T i);
    
    void set(T i);

    T get();

    void change(T i);

    int listen(function<void()> func);

    void notifyListener();

    void removeListener(int subscriptionId);
};

#endif
