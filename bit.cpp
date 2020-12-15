#include "bit.h"

bit Not (bit& b)
{
  return bit (!b.get ());
}

int toInt (bit& b)
{
  return b.get () ? 1 : 0;
}

bool operator> (const bit &lhs, const bit &rhs)
{
  return lhs.m_bit && !rhs.m_bit;
}
bool operator<= (const bit &lhs, const bit &rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

bool operator< (const bit &lhs, const bit &rhs)
{
  return !lhs.m_bit && rhs.m_bit;
}
bool operator>= (const bit &lhs, const bit &rhs)
{
  return (lhs > rhs) || (lhs == rhs) ;
}

bool operator== (const bit &lhs, const bit &rhs)
{
  return (lhs.m_bit && rhs.m_bit) || (!lhs.m_bit && !rhs.m_bit);
}
bool operator!= (const bit &lhs, const bit &rhs)
{
  return (lhs.m_bit && !rhs.m_bit) || (!lhs.m_bit && rhs.m_bit);
}

std::ostream& operator<< (std::ostream &out, const bit &b)
{
  out << b.getInt();
  return out;
}

