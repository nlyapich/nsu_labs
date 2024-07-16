#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <functional>

class BigInt
{
public:
  BigInt();
  explicit BigInt(int);
  explicit BigInt(std::string);
  BigInt(const BigInt&);
  ~BigInt();

  BigInt& operator=(const BigInt&);

  BigInt operator~() const;

  BigInt& operator++();
  const BigInt operator++(int) const;
  BigInt& operator--();
  const BigInt operator--(int) const;

  BigInt& operator+=(const BigInt&);
  BigInt& operator*=(const BigInt&);
  BigInt& operator-=(const BigInt&);
  BigInt& operator/=(const BigInt&);
  BigInt& operator^=(const BigInt&);
  BigInt& operator%=(const BigInt&);
  BigInt& operator&=(const BigInt&);
  BigInt& operator|=(const BigInt&);

  BigInt operator+() const;  // unary +
  BigInt operator-() const;  // unary -

  bool operator==(const BigInt&) const;
  bool operator!=(const BigInt&) const;
  bool operator<(const BigInt&) const;
  bool operator>(const BigInt&) const;
  bool operator<=(const BigInt&) const;
  bool operator>=(const BigInt&) const;

  operator int() const;
  operator std::string() const;

  size_t size() const;  // size in bytes

private:
  std::string unsignedNumber;
  bool isNegative;

  BigInt bitOperation(std::function<bool(bool, bool)>, const BigInt&, const BigInt&) const;
};

BigInt operator+(const BigInt&, const BigInt&);
BigInt operator-(const BigInt&, const BigInt&);
BigInt operator*(const BigInt&, const BigInt&);
BigInt operator/(const BigInt&, const BigInt&);
BigInt operator^(const BigInt&, const BigInt&);
BigInt operator%(const BigInt&, const BigInt&);
BigInt operator&(const BigInt&, const BigInt&);
BigInt operator|(const BigInt&, const BigInt&);

std::ostream& operator<<(std::ostream& o, const BigInt& i);

#endif // BIG_INT_HPP
