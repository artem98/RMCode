#ifndef BIT_H
#define BIT_H

#include <iostream>
#include <vector>

class bit
{
private:
  bool m_bit;
public:
  int getInt () const { return m_bit ? 1 : 0; }
  bool get () const { return m_bit; }
  void set (bool value) { m_bit = value; }
  void inverse () { m_bit = !m_bit; }

  bit (bool value = false) { m_bit = value; }

  const bit operator+ (const bit& rhs) const {
      return bit ((m_bit && !rhs.get ()) || (!m_bit && rhs.get ()));
  }

  const bit operator* (const bit& rhs) const {
      return bit (m_bit && rhs.get () );
  }

  bit& operator= (const bit& rhs) {
      if (this == &rhs) {
          return *this;
      }
      m_bit = rhs.get ();
      return *this;
  }

  friend bool operator> (const bit &lhs, const bit &rhs);
  friend bool operator<= (const bit &lhs, const bit &rhs);

  friend bool operator< (const bit &lhs, const bit &rhs);
  friend bool operator>= (const bit &lhs, const bit &rhs);

  friend bool operator== (const bit &lhs, const bit &rhs);
  friend bool operator!= (const bit &lhs, const bit &rhs);

  friend std::ostream& operator<< (std::ostream &out, const bit &b);
};

using bit_array = std::vector<bit>;

#endif //BIT_H
