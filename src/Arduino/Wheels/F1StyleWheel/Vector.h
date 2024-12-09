#pragma once

#include <cstddef>

template<typename storedtype_t>
class Vector {
public:

    Vector() : Vector(16) {}
    Vector(int capacity) : m_capacity(capacity), m_data(new storedtype_t[m_capacity]) {}
    ~Vector() { delete[] m_data; }

    void push_back(storedtype_t value);
    void remove(int index);

    storedtype_t& operator[](int i) { return m_data[i]; }

    size_t getLength() { return m_length; }
    size_t getCapacity() { return m_capacity; }

private:

    storedtype_t* m_data;
    size_t m_length = 0;
    size_t m_capacity;

};

template<typename storedtype_t>
void Vector<storedtype_t>::push_back(storedtype_t value) {
    if (m_length == m_capacity) {
        m_capacity *= 2;
        storedtype_t* newData = new storedtype_t[m_capacity];
        for (int i = 0; i < m_length; i++) {
            newData[i] = m_data[i];
        }
        delete[] m_data;
        m_data = newData;
    }
    m_data[m_length] = value;
    m_length++;
}

template<typename storedtype_t>
void Vector<storedtype_t>::remove(int index) {
    if (index < 0 || index >= m_length) return;
    m_length--;
    for (int i = index; i < m_length; i++) {
        m_data[i] = m_data[i + 1];
    }
}