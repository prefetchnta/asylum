/*
***************************************
*   CPU STUFF @ 2014-04-05
***************************************
*/

#ifndef __MTPOOL_HPP__
#define __MTPOOL_HPP__

/* Asylum Namespace */
namespace asy {

/***************/
/* CPU Monitor */
/***************/
template<class T>
struct monitor
{
    T               user;
    events          evts;
    volatile int    quit;

    /* ======= */
    void _done ()
    {
        this->evts.wait();
    }
};

/***************/
/* Thread Pool */
/***************/
template<class T, size_t N>
class mtpool : public asylum
{
private:
    size_t      m_num;
    monitor<T>  m_mon;
    asy_thrd_t  m_cpu[N];

public:
    /* ============================================================================== */
    void init (thrd_main_t start, const T* user, size_t count = N, size_t stacksize = 0)
    {
        if (count > N)
            count = N;
        m_num = count;
        m_mon.quit = 0;
        m_mon.evts.init();
        mem_cpy(&m_mon.user, user, sizeof(T));
        for (size_t idx = 0; idx < count; idx++)
            m_cpu[idx] = thread_run(start, &m_mon, stacksize);
    }

    /* ====== */
    void free ()
    {
        atom_inc_fetch32(&m_mon.quit);
        m_mon.evts.fire(true);
        for (size_t idx = 0; idx < m_num; idx++) {
            if (m_cpu[idx] != NULL)
                thread_end(m_cpu[idx]);
        }
        m_mon.evts.free();
        m_mon.user.free();
    }

public:
    /* ======== */
    void wakeup ()
    {
        m_mon.evts.fire(false);
    }
};

}   /* namespace */

#endif  /* __MTPOOL_HPP__ */
