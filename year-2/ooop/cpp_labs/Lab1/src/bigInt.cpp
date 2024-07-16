#include "bigInt.hpp"

#include <algorithm>
#include <climits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
bool isLess(const std::string& num1, const std::string& num2)
{
  size_t size1 = num1.length();
  size_t size2 = num2.length();

  if (size1 < size2)
  {
    return true;
  }

  if (size1 > size2)
  {
    return false;
  }

  for (size_t i = 0; i < size1; ++i)
  {
    if (num1[i] == num2[i])
    {
      continue;
    }
    if (num1[i] < num2[i])
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  return false;
}

bool isSign(char ch)
{
  return ((ch == '-') || (ch == '+'));
}

bool isMinus(char ch)
{
  return (ch == '-');
}

bool isNumber(char ch)
{
  return (('0' <= ch) && (ch <= '9'));
}

bool isNumber(const std::string& str)
{
  return std::all_of(str.begin(), str.end(), [](char ch) { return isNumber(ch); });
}

void removeLeadingZerosNumber(std::string& num)
{
  if (num.empty())
  {
    return;
  }

  size_t countZeros = 0;

  for (size_t i = 0; i < (num.length() - 1); ++i)
  {
    if (num[i] == '0')
    {
      countZeros++;
    }
    else
    {
      break;
    }
  }

  num.erase(0, countZeros);
}

std::string negationBitString(const std::string& bitNum)
{
  std::string bitRes = "";

  for (size_t i = 0; i < bitNum.length(); ++i)
  {
    bitRes += (bitNum[i] - '0') ? "0" : "1";
  }

  return bitRes;
}

std::string sum(const std::string& num1, const std::string& num2, int scaleNotation = 10)
{
  const std::string& minNum = (isLess(num1, num2)) ? num1 : num2;
  const std::string& maxNum = (minNum == num1) ? num2 : num1;

  size_t minSize = minNum.length();
  size_t maxSize = maxNum.length();

  std::string res = "";
  int nextRank = 0;

  for (size_t i = 0; i < minSize; ++i)
  {
    nextRank += (minNum[minSize - i - 1] - '0') + (maxNum[maxSize - i - 1] - '0');
    res += std::to_string(nextRank % scaleNotation);
    nextRank /= scaleNotation;
  }

  if (minSize != maxSize)
  {
    for (size_t i = minSize; i < maxSize; ++i)
    {
      nextRank += (maxNum[maxSize - i - 1] - '0');
      res += std::to_string(nextRank % scaleNotation);
      nextRank /= scaleNotation;
    }
  }

  if (nextRank)
  {
    res += std::to_string(nextRank);
  }

  std::reverse(res.begin(), res.end());

  return res;
}

// guaranteed that num1 >= num2
std::string difference(const std::string& num1, const std::string& num2, int scaleNotation = 10)
{
  if (num1 == num2)
  {
    return "0";
  }

  std::string res = "";
  int nextRank = 0;

  size_t sizeNum1 = num1.length();
  size_t sizeNum2 = num2.length();

  for (size_t i = 0; i < sizeNum2; ++i)
  {
    int dif = (num1[sizeNum1 - i - 1] - num2[sizeNum2 - i - 1]) - nextRank;
    nextRank = (dif < 0) ? 1 : 0;

    res += std::to_string((dif < 0) ? (scaleNotation + dif) : dif);
  }

  if (sizeNum1 != sizeNum2)
  {
    for (size_t i = sizeNum2; i < sizeNum1; ++i)
    {
      int dif = (num1[sizeNum1 - i - 1] - '0') - nextRank;
      nextRank = (dif < 0) ? 1 : 0;

      res += std::to_string((dif < 0) ? (scaleNotation + dif) : dif);
    }
  }

  std::reverse(res.begin(), res.end());

  size_t countBeginZero = 0;
  while ((countBeginZero < res.length()) && !(res[countBeginZero] - '0'))
  {
    countBeginZero++;
  }
  res.erase(0, countBeginZero);

  return res;
}

std::string product(char num1, char num2)
{
  int res = (num1 - '0') * (num2 - '0');

  return std::to_string(res);
}

std::string product(const std::string& num1, char num2)
{
  if (num2 == '0')
  {
    return std::string("0");
  }

  std::vector<std::string> res(num1.length());

  for (size_t i = 0; i < num1.length(); ++i)
  {
    res[i] = product(num1[num1.length() - i - 1], num2);
    if (res[i] != "0")
    {
      res[i] += std::string(i, '0');
    }
  }

  std::string resSum = "0";
  resSum = std::accumulate
  (
    res.begin(),
    res.end(),
    resSum,
    std::bind(&sum, std::placeholders::_1, std::placeholders::_2, 10)
  );

  return resSum;
}

std::string product(const std::string& num1, const std::string& num2)
{
  if ((num1 == "0") || (num2 == "0"))
  {
    return std::string("0");
  }

  std::vector<std::string> res(num2.length());

  for (size_t i = 0; i < num2.length(); ++i)
  {
    res[i] = product(num1, num2[num2.length() - i - 1]);
    if (res[i] != "0")
    {
      res[i] += std::string(i, '0');
    }
  }

  std::string resSum = "0";
  resSum = std::accumulate
  (
    res.begin(),
    res.end(),
    resSum,
    std::bind(&sum, std::placeholders::_1, std::placeholders::_2, 10)
  );

  return resSum;
}

std::string quotient(const std::string& num1, const std::string& num2)
{
  if (num2 == "1")
  {
    return num1;
  }

  std::string quotient = "0";
  std::string dividend(num1);

  while(isLess(num2, dividend) || (dividend == num2))
  {
    quotient = sum(quotient, "1");
    dividend = difference(dividend, num2);
  }

  return quotient;
}

std::string remainderDivision(const std::string& num1, const std::string& num2)
{
  return difference(num1, product(quotient(num1, num2), num2));
}

std::string remainderDivisionTwo(const std::string& num)
{
  return std::to_string((num[num.length() - 1] - '0') % 2);
}

std::string bitStringNumber(std::string num)
{
  std::string res = "";

  while (num != "0")
  {
    res += remainderDivisionTwo(num);
    num = quotient(num, "2");
  }

  if (res.length() < (sizeof(int) * CHAR_BIT - 1))
  {
    res += std::string((sizeof(int) * CHAR_BIT - 1) - res.length(), '0');
  }
  else if (res.length() % 8)
  {
    res += std::string(8 - (res.length() % 8) - 1, '0');
  }

  std::reverse(res.begin(), res.end());

  return res;
}

std::string bitToNumberString(const std::string& num)
{
  std::string res = "0";

  size_t size = num.length();
  for (size_t i = 0; i < size; ++i)
  {
    if (num[size - i - 1] - '0')
    {
      std::string productRes = "1";
      for (size_t j = 0; j < i; ++j)
      {
        productRes = product(productRes, '2');
      }
      res = sum(res, productRes);
    }
  }

  return res;
}
} // namespace

BigInt::BigInt() : unsignedNumber{"0"}, isNegative{false}{}

BigInt::BigInt(int a) : unsignedNumber{std::to_string(a)}, isNegative{false}
{
  if (a < 0)
  {
    isNegative = true;
    unsignedNumber.erase(0, 1);
  }
}

BigInt::BigInt(std::string str) : isNegative{false}
{
  if (str.empty())
  {
    unsignedNumber = "0";
    return;
  }

  if (isSign(str[0]))
  {
    if (str.length() == 1)
    {
      throw std::invalid_argument("The number is invalid");
    }

    isNegative = isMinus(str[0]);
    str.erase(0, 1);
  }

  if (!isNumber(str))
  {
    throw std::invalid_argument("The number contains extra characters");
  }

  unsignedNumber = str;
  removeLeadingZerosNumber(unsignedNumber);

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }
}

BigInt::BigInt(const BigInt& a) : unsignedNumber{a.unsignedNumber}, isNegative{a.isNegative}{}

BigInt::~BigInt(){}

BigInt& BigInt::operator=(const BigInt& bi)
{
  unsignedNumber = bi.unsignedNumber;
  isNegative = bi.isNegative;
  return *this;
}

BigInt BigInt::operator~() const
{
  std::string bitNum = bitStringNumber(unsignedNumber);

  if (isNegative)
  {
    bitNum = negationBitString(bitNum);
    bitNum = sum(bitNum, "1", 2);
  }

  bitNum = negationBitString(bitNum);

  if (!isNegative)
  {
    bitNum = difference(bitNum, "1", 2);
    bitNum = negationBitString(bitNum);
  }

  return BigInt(std::string(isNegative ? "" : "-") + bitToNumberString(bitNum));
}

BigInt& BigInt::operator++()
{
  if (isNegative)
  {
    unsignedNumber = difference(unsignedNumber, "1");
  }
  else
  {
    unsignedNumber = sum(unsignedNumber, "1");
  }

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

const BigInt BigInt::operator++(int) const
{
  BigInt copy(*this);
  ++copy;
  return copy;
}

BigInt& BigInt::operator--()
{
  if (isNegative)
  {
    unsignedNumber = sum(unsignedNumber, "1");
  }
  else
  {
    if (unsignedNumber == "0")
    {
      unsignedNumber = sum(unsignedNumber, "1");
      isNegative = true;
    }
    else
    {
      unsignedNumber = difference(unsignedNumber, "1");
    }
  }

  return *this;
}

const BigInt BigInt::operator--(int) const
{
  BigInt copy(*this);
  --copy;
  return copy;
}

BigInt& BigInt::operator+=(const BigInt& num)
{
  if (isNegative == num.isNegative)
  {
    unsignedNumber = sum(unsignedNumber, num.unsignedNumber);
    return *this;
  }

  const std::string minNum = (isLess(unsignedNumber, num.unsignedNumber)) ? unsignedNumber : num.unsignedNumber;
  const std::string maxNum = (minNum == unsignedNumber) ? num.unsignedNumber : unsignedNumber;

  unsignedNumber = difference(maxNum, minNum);
  if (num.unsignedNumber == maxNum)
  {
    isNegative = num.isNegative;
  }

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

BigInt& BigInt::operator-=(const BigInt& num)
{
  if (isNegative != num.isNegative)
  {
    unsignedNumber = sum(unsignedNumber, num.unsignedNumber);
    return *this;
  }

  if (isLess(unsignedNumber, num.unsignedNumber))
  {
    unsignedNumber = difference(num.unsignedNumber, unsignedNumber);
    isNegative = !isNegative;
  }
  else
  {
    unsignedNumber = difference(unsignedNumber, num.unsignedNumber);
  }

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

BigInt& BigInt::operator*=(const BigInt& num)
{
  unsignedNumber = product(unsignedNumber, num.unsignedNumber);
  isNegative = (isNegative != num.isNegative) ? true : false;

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

BigInt& BigInt::operator/=(const BigInt& num)
{
  if (num.unsignedNumber == "0")
  {
    throw std::invalid_argument("Division by zero");
  }

  unsignedNumber = quotient(unsignedNumber, num.unsignedNumber);
  isNegative = (isNegative != num.isNegative) ? true : false;

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

BigInt& BigInt::operator%=(const BigInt& num)
{
  if (num.unsignedNumber == "0")
  {
    throw std::invalid_argument("Division by zero");
  }

  unsignedNumber = remainderDivision(unsignedNumber, num.unsignedNumber);

  if (unsignedNumber == "0")
  {
    isNegative = false;
  }

  return *this;
}

BigInt BigInt::bitOperation(std::function<bool(bool, bool)> operation, const BigInt& num1, const BigInt& num2) const
{
  std::string bitNum1 = bitStringNumber(num1.unsignedNumber);
  if (num1.isNegative)
  {
    bitNum1 = negationBitString(bitNum1);
    bitNum1 = sum(bitNum1, "1", 2);
  }

  std::string bitNum2 = bitStringNumber(num2.unsignedNumber);
  if (num2.isNegative)
  {
    bitNum2 = negationBitString(bitNum2);
    bitNum2 = sum(bitNum2, "1", 2);
  }

  const std::string& minBitNum = (isLess(bitNum1, bitNum2)) ? bitNum1 : bitNum2;
  const std::string& maxBitNum = (minBitNum == bitNum1) ? bitNum2 : bitNum1;

  std::string bitRes = "";

  for (size_t i = 0; i < (maxBitNum.length() - minBitNum.length()); ++i)
  {
    bitRes += (operation(maxBitNum[i] - '0', false)) ? "1" : "0";
  }

  for (size_t i = (maxBitNum.length() - minBitNum.length()); i < maxBitNum.length(); ++i)
  {
    bitRes += (operation(maxBitNum[i] - '0', minBitNum[i - (maxBitNum.length() - minBitNum.length())] - '0') ? "1" : "0");
  }

  if (operation(num1.isNegative,num2.isNegative))
  {
    bitRes = difference(bitRes, "1", 2);
    bitRes = negationBitString(bitRes);
  }

  std::string newUnsignedNumber = bitToNumberString(bitRes);
  bool newIsNegative = (operation(num1.isNegative,num2.isNegative) && (newUnsignedNumber != "0"));

  BigInt resOperation(std::string(newIsNegative ? "-" : "") + newUnsignedNumber);

  return resOperation;
}

BigInt& BigInt::operator^=(const BigInt& bi)
{
  *this = bitOperation([] (bool a, bool b) { return a^b; }, *this, bi);

  return *this;
}

BigInt& BigInt::operator&=(const BigInt& bi)
{
  *this = bitOperation([] (bool a, bool b) { return a&b; }, *this, bi);

  return *this;
}

BigInt& BigInt::operator|=(const BigInt& bi)
{
  *this = bitOperation([] (bool a, bool b) { return a|b; }, *this, bi);

  return *this;
}

BigInt BigInt::operator+() const
{
  return *this;
}

BigInt BigInt::operator-() const
{
  BigInt copy(*this);
  if (copy.unsignedNumber != "0")
  {
    copy.isNegative = !(copy.isNegative);
  }
  return copy;
}

bool BigInt::operator==(const BigInt& a) const
{
  return ((unsignedNumber == a.unsignedNumber) && (isNegative == a.isNegative));
}

bool BigInt::operator!=(const BigInt& a) const
{
  return !(*this == a);
}

bool BigInt::operator<(const BigInt& a) const
{
  if (isNegative != a.isNegative)
  {
    return isNegative;
  }

  if (isNegative)
  {
    return (!isLess(unsignedNumber, a.unsignedNumber) && (unsignedNumber != a.unsignedNumber));
  }

  return isLess(unsignedNumber, a.unsignedNumber);
}

bool BigInt::operator>(const BigInt& a) const
{
  return !(*this <= a);
}

bool BigInt::operator<=(const BigInt& a) const
{
  return ((*this < a) || (*this == a));
}

bool BigInt::operator>=(const BigInt& a) const
{
  return !(*this < a);
}

BigInt::operator int() const
{
  if ((isNegative && isLess(std::to_string(INT_MIN).erase(0, 1), unsignedNumber)) ||
      (!isNegative && isLess(std::to_string(INT_MAX), unsignedNumber)))
  {
      throw std::range_error("The number is not in the int range");
  }

  int num = (isNegative ? -1 : 1) * std::stoi(unsignedNumber);

  return num;
}

BigInt::operator std::string() const
{
  return (std::string(isNegative ? "-" : "") + unsignedNumber);
}

size_t BigInt::size() const
{
  return (unsignedNumber.size() + sizeof(isNegative));
}

BigInt operator+(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy += num2;

  return copy;
}

BigInt operator-(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy -= num2;

  return copy;
}

BigInt operator*(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy *= num2;

  return copy;
}

BigInt operator/(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy /= num2;

  return copy;
}

BigInt operator^(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy ^= num2;

  return copy;
}

BigInt operator%(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy %= num2;

  return copy;
}

BigInt operator&(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy &= num2;

  return copy;
}

BigInt operator|(const BigInt& num1, const BigInt& num2)
{
  BigInt copy(num1);
  copy |= num2;

  return copy;
}

std::ostream& operator<<(std::ostream& o, const BigInt& i)
{
  o << static_cast<std::string>(i);
  return o;
}
