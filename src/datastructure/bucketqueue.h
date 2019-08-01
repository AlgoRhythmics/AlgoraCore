/**
 * Copyright (C) 2013 - 2019 : Kathrin Hanauer
 *
 * This file is part of Algora.
 *
 * Algora is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Algora is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Algora.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 *   http://algora.xaikal.org
 */

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

    explicit BucketQueue(const Priority& prio = Priority(),
                         const size_type &limit = 0U)
        : m_size(0), m_top(0), m_bot(0), m_priority(prio), m_limit(limit) { }

    bool empty() const { return m_size == 0UL; }

    void setLimit(const size_type &limit) {
        m_limit = limit;
    }

    size_type size() const { return m_size; }

    const value_type& top() const {
        return m_buckets[m_top].back();
    }

    const value_type& back() const {
        return top();
    }

    const value_type& front() const {
        return m_buckets[m_bot].back();
    }

    void push (const value_type& val) {
        size_type p = m_priority(val);
        if (m_limit > 0U && p > m_limit) {
            p = m_limit;
        }
        if (m_buckets.size() <= p) {
            m_buckets.resize(p+1);
        }
        m_buckets[p].push_back(val);
        if (m_top < p) {
            m_top = p;
        }
        if (m_bot > p || m_size == 0UL) {
            m_bot = p;
        }
        m_size++;
    }

    void push_back (const value_type& val) {
        push(val);
    }

    void pop() {
        m_buckets[m_top].pop_back();
        m_size--;
        while (m_top > 0UL && m_buckets[m_top].empty()) {
            m_top--;
        }
    }

    void pop_back() {
        pop();
    }

    void pop_front() {
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
    size_type m_limit;
};

//template <typename T, typename Priority>
//void swap (BucketQueue<T,Priority>& x, BucketQueue<T,Priority>& y) noexcept(noexcept(x.swap(y)));

}

#endif // BUCKETQUEUE_H
