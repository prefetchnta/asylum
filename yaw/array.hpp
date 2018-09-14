/*
***************************************
*   Yet Another Wheel @ 2013-12-19
***************************************
*/

#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

/* Asylum Namespace */
namespace asy {

/*********/
/* Array */
/*********/
template<class T>
class array : public asylum
{
private:
    T*      m_list;
    size_t  m_cnts;
    size_t  m_size;

private:
    /* ============== */
    void _clean () const
    {
        size_t  idx = m_cnts;

        while (idx-- != 0)
            m_list[idx].free();
    }

public:
    /* ====== */
    void init ()
    {
        m_cnts = 0;
        m_size = 0;
        m_list = NULL;
    }

    /* ================== */
    bool init (size_t count)
    {
        m_size = 0;
        m_list = NULL;
        if (!this->reserve(count))
            return (false);
        mem_set(m_list, 0, count * sizeof(T));
        m_cnts = count;
        return (true);
    }

    /* ====== */
    void free ()
    {
        if (m_list != NULL) {
            this->_clean();
            mem_free(m_list);
        }
    }

public:
    /* ======= */
    void clear ()
    {
        this->_clean();
        m_cnts = 0;
    }

    /* ========== */
    T* data () const
    {
        return (m_list);
    }

    /* ============== */
    size_t size () const
    {
        return (m_cnts);
    }

    /* ================== */
    size_t capacity () const
    {
        return (m_size);
    }

    /* ================================== */
    T* get (size_t idx, T* obj = NULL) const
    {
        T*  ret = &m_list[idx];

        if (obj != NULL)
            mem_cpy(obj, ret, sizeof(T));
        return (ret);
    }

    /* ======================================= */
    T* get_safe (size_t idx, T* obj = NULL) const
    {
        if (idx >= m_cnts)
            return (NULL);
        return (this->get(idx, obj));
    }

    /* ==================== */
    bool reserve (size_t size)
    {
        T*  temp;

        if (size > m_size) {
            temp = mem_talloc(size, T);
            if (temp == NULL)
                return (false);
            if (m_list != NULL) {
                mem_cpy(temp, m_list, m_cnts * sizeof(T));
                mem_free(m_list);
            }
            m_size = size;
            m_list = temp;
        }
        return (true);
    }

    /* ========= */
    bool no_grow ()
    {
        T*  temp;

        if (m_cnts != m_size) {
            if (m_cnts == 0) {
                mem_free(m_list);
                m_list = NULL;
                m_size = 0;
                return (true);
            }
            temp = mem_talloc(m_cnts, T);
            if (temp == NULL)
                return (false);
            mem_cpy(temp, m_list, m_cnts * sizeof(T));
            mem_free(m_list);
            m_size = m_cnts;
            m_list = temp;
        }
        return (true);
    }

    /* ==================== */
    T* swap (size_t idx) const
    {
        T   tmp, *ret;

        if (idx == 0 || idx >= m_cnts)
            return (NULL);
        ret = &m_list[idx];
        mem_cpy(&tmp, ret - 1, sizeof(T));
        mem_cpy(ret - 1, ret, sizeof(T));
        mem_cpy(ret, &tmp, sizeof(T));
        return (ret);
    }

    /* =============== */
    void del (size_t idx)
    {
        T*  ptr;

        if (idx >= m_cnts)
            return;
        ptr = &m_list[idx];
        ptr->free();
        m_cnts -= 1;
        if (m_cnts > idx)
            mem_cpy(ptr, ptr + 1, (m_cnts - idx) * sizeof(T));
    }

    /* ================== */
    bool pop (T* obj = NULL)
    {
        if (m_cnts == 0)
            return (false);
        m_cnts -= 1;
        if (obj != NULL)
            mem_cpy(obj, &m_list[m_cnts], sizeof(T));
        else
            m_list[m_cnts].free();
        return (true);
    }

    /* ================================================ */
    T* insert (size_t idx, const T* obj, bool grow = true)
    {
        T*  ret;

        if (grow) {
            if (m_cnts >= m_size &&
                !this->reserve(m_size * 2 + 1))
                return (NULL);
        }
        else {
            if (m_cnts >= m_size)
                return (NULL);
        }
        if (idx >= m_cnts) {
            ret = &m_list[m_cnts];
        }
        else {
            ret = &m_list[idx];
            mem_mov(ret + 1, ret, (m_cnts - idx) * sizeof(T));
        }
        m_cnts += 1;
        mem_cpy(ret, obj, sizeof(T));
        return (ret);
    }

    /* ==================================== */
    T* sthead (const T* obj, bool grow = true)
    {
        return (this->insert(0, obj, grow));
    }

    /* ==================================== */
    T* append (const T* obj, bool grow = true)
    {
        T*  ret;

        if (grow) {
            if (m_cnts >= m_size &&
                !this->reserve(m_size * 2 + 1))
                return (NULL);
        }
        else {
            if (m_cnts >= m_size)
                return (NULL);
        }
        ret = &m_list[m_cnts++];
        mem_cpy(ret, obj, sizeof(T));
        return (ret);
    }

    /* ============================================= */
    template<class TRUN>void trav_h2t (void* ctx) const
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TRUN    run;

            unt = m_list;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, unt))
                    return;
                unt += 1;
            }
        }
    }

    /* ============================================= */
    template<class TRUN>void trav_t2h (void* ctx) const
    {
        if (m_cnts != 0)
        {
            T*      unt;
            TRUN    run;

            unt = m_list + m_cnts - 1;
            for (size_t idx = m_cnts; idx != 0; idx--) {
                if (!run.doit(ctx, unt))
                    return;
                unt -= 1;
            }
        }
    }
};

}   /* namespace */

#endif  /* __ARRAY_HPP__ */
