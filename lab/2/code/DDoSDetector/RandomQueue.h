#ifndef RANDOMQUEUE_H
#define RANDOMQUEUE_H

#include <vector>

template<typename T>
class RandomQueue
{
public:
    RandomQueue();
    virtual ~RandomQueue();
    void push(T x);
    T pop();
    bool empty();
    unsigned long size();
    void clear();
    T operator[](int i) const;
private:
    std::vector<T> m_data;
};

template<class T>
RandomQueue<T>::RandomQueue()
{
}

template<typename T>
RandomQueue<T>::~RandomQueue()
{
}

template<typename T>
void RandomQueue<T>::push(T x)
{
    m_data.push_back(x);
}

template<typename T>
T RandomQueue<T>::pop()
{
    typename std::vector<T>::iterator first = m_data.begin();
    m_data.erase(first);
    return *first;
}

template<typename T>
bool RandomQueue<T>::empty()
{
    return m_data.empty();
}

template<typename T>
unsigned long RandomQueue<T>::size()
{
    return m_data.size();
}

template<typename T>
void RandomQueue<T>::clear()
{
    m_data.clear();
}

template<typename T>
T RandomQueue<T>::operator[](int i) const
{
    return m_data[i];
}

#endif /* RANDOMQUEUE_H */
