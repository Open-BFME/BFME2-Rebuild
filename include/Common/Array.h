#pragma once
template<class T> class Array
{
public:
 unsigned size() const { return m_end-m_begin; }
 T &operator[](unsigned index) { return m_begin[index]; }
 const T &operator[](unsigned index) const { return m_begin[index]; }
 T *m_begin;
 T *m_end;
 T *m_capacity;
};
