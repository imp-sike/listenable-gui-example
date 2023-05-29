#include "listenable.hh"
#include <functional>
#include <iterator>
#include <list>
#include <thread>

using namespace std;

template <class T>
listenable<T>::listenable(T i)
{
    set(i);
}

template <class T>
void listenable<T>::set(T i)
{
    this->i = i;
}

template <class T>
T listenable<T>::get()
{
    return i;
}

template <class T>
void listenable<T>::change(T i)
{
    this->i = i;
    notifyListener();
}

template <class T>
int listenable<T>::listen(function<void()> func)
{
    // Add the provided function to the list of listeners
    listeners.push_back(func);

    // Return the index of the added listener, which can be used to remove the listener later
    return listeners.size() - 1;
}

template <class T>
void listenable<T>::notifyListener()
{

    vector<thread> threads;
    threads.reserve(listeners.size());

    // Create a new thread for each listener function and store them in the 'threads' vector
    for (const auto& listener : listeners)
    {
        threads.emplace_back(listener);
    }

    // Wait for all the threads to finish executing
    for (auto& thread : threads)
    {
        thread.join();
    }
}

template <class T>
void listenable<T>::removeListener(int subscriptionId)
{
    // Find the iterator pointing to the listener to be removed
    list<function<void()>>::iterator itr = listeners.begin();
    advance(itr, subscriptionId);

    // Erase the listener from the list
    listeners.erase(itr);
}
