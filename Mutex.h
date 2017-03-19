#ifndef __MUTEX__
#define __MUTEX__

#include <pthread.h>
#include <cstdlib>

//Allow processes to share mutex
#ifndef _POSIX_THREAD_PROCESS_SHARED
#warning "Shared mutex not supported by this platform"
#endif

/**
 * @version 0.1
 * @since March 9, 2012
 * @class Mutex
 * @brief This class is a pthread mutex wrapper
 */
class Mutex
{
  private:

  /** A pthread mutex pointer **/
  pthread_mutex_t* mutex;

  public:

  /**
   * Class constructor Allocates and initializes the pthread mutex pointer
   */
  Mutex()
  {
    mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if(mutex) pthread_mutex_init(mutex, NULL);
  };

  /**
   * Locks the critical section
   */
  void lock()   { pthread_mutex_lock(mutex); };

  /**
   * Try locking the critical section without waiting
   *
   * @return true if a lock is aquired, false otherwise
   */
  inline bool trylock() { return !pthread_mutex_trylock(mutex); };

  /**
   * Unlocks the critical section
   */
  void unlock() { pthread_mutex_unlock(mutex); };

  /**
   * Destroys an allocated mutex after unlocking it
   */
  ~Mutex()
  {
    if(mutex)
    {
      unlock();
      pthread_mutex_destroy(mutex);
      free(mutex);
      mutex = NULL;
    }
  };
};

#endif//__MUTEX__
