#ifndef BUCKETQUEUE_H
#define BUCKETQUEUE_H

#include <vector>

namespace Algora {

template<typename T, typename Priority>
class BucketQueue
{
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    explicit BucketQueue(const Priority& prio = Priority()) : m_size(0), m_top(0), m_bot(0), m_priority(prio) { }

    bool empty() const { return m_size == 0; }

    size_type size() const { return m_size; }

    const value_type& top() const {
        return m_buckets[m_top].back();
    }

    const value_type& bot() const {
        return m_buckets[m_bot].back();
    }

    void push (const value_type& val) {
        size_type p = m_priority(val);
        if (m_buckets.size() <= p) {
            m_buckets.resize(p+1);
        }
        m_buckets[p].push_back(val);
        if (m_top < p) {
            m_top = p;
        }
        if (m_bot > p || m_size == 0) {
            m_bot = p;
        }
        m_size++;
    }

    void pop() {
        m_buckets[m_top].pop_back();
        m_size--;
        while (m_top > 0 && m_buckets[m_top].empty()) {
            m_top--;
        }
    }

    void popBot() {
        m_buckets[m_bot].pop_back();
        m_size--;
        while (m_bot < m_buckets.size() && m_buckets[m_bot].empty()) {
            m_bot++;
        }
    }

private:
    std::vector<std::vector<value_type>> m_buckets;
    size_type m_size;
    size_type m_top;
    size_type m_bot;
    Priority m_priority;
};

//template <typename T, typename Priority>
//void swap (BucketQueue<T,Priority>& x, BucketQueue<T,Priority>& y) noexcept(noexcept(x.swap(y)));

}

#endif // BUCKETQUEUE_H
