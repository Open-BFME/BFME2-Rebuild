#pragma once
template<class T> class PhaseList
{
public:
 int size() const { return m_finish-m_start; }
 const T *operator[](int i) const { return m_start[i]; }
 void push_back(const T *const &);
 const T **m_start;
 const T **m_finish;
 const T **m_endOfStorage;
};
