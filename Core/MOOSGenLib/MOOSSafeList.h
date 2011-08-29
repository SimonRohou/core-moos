/*
 * MOOSSafeList.h
 *
 *  Created on: Aug 29, 2011
 *      Author: pnewman
 */

#ifndef MOOSSAFELIST_H_
#define MOOSSAFELIST_H_



#include <list>
#include "PocoBits/ScopedLock.h"
#include "PocoBits/Event.h"
#include "PocoBits/Mutex.h"

namespace MOOS
{
/**
 * templated class which makes thread safe, waitable list.
 */
template<class T>
class SafeList
{
public:

    void Push(const T & Element)
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        _List.push_back(Element);
        _PushEvent.set();

    }

    bool Pull(T & Element)
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        if (!_List.empty())
        {
            Element = _List.front();
            _List.pop_front();

            return true;
        } else
            return false;

    }

    bool PeekLatest(T & Element)
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        if (_List.empty())
            return false;

        Element = _List.back();
        return true;
    }


    bool PeekNext(T & Element)
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        if (_List.empty())
            return false;

        Element = _List.front();
        return true;
    }


    bool WaitForPush(long milliseconds = -1)
    {
        if (milliseconds < 0)
        {
            _PushEvent.wait();
            return true;
        }
        else
        {
            return _PushEvent.tryWait(milliseconds);
        }
    }

    unsigned int Size()
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        return _List.size();
    }

    void Clear()
    {
        Poco::FastMutex::ScopedLock Lock(_mutex);
        _List.clear();
    }

private:
    Poco::FastMutex _mutex;
    std::list<T> _List;
    Poco::Event _PushEvent;
};
}




#endif /* MOOSSAFELIST_H_ */
