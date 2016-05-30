
#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class Mutex
{
    public:
        Mutex()
        {
            pthread_mutex_init(&_mutexid, NULL);
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&_mutexid);
        }
        void lock()
        {
            pthread_mutex_lock(&_mutexid);
        }
        void unlock()
        {
            pthread_mutex_unlock(&_mutexid);
        }

        pthread_mutex_t* getPthreadMutex()
        {
            return &_mutexid;       
        }
    private:
        pthread_mutex_t _mutexid;
};

class MutexGuard
{
    public:
        MutexGuard(Mutex& mutex)
            :_mutex(mutex)
        {
            _mutex.lock();        
        }
        ~MutexGuard()
        {
            _mutex.unlock();        
        }
    private:
        Mutex& _mutex;
};

#endif
