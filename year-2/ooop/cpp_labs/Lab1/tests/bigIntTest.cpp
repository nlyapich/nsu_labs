#include "gtest/gtest.h"

#include <climits>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "./../src/bigInt.hpp"

template <typename T>
struct TestValueExpectedPair
{
  T testValue;
  T expectedValue;
};

template <typename T>
struct TestValuePair
{
  T value1;
  T value2;
};

const std::vector<TestValueExpectedPair<BigInt>> equalTestCase =
{
  {BigInt(), BigInt()},
  {BigInt(0), BigInt()},

  {BigInt(1), BigInt(1)},
  {BigInt(-1), BigInt(-1)},

  {BigInt("0"), BigInt()},
  {BigInt("+0"), BigInt(0)},
  {BigInt("-0"), BigInt(0)},

  {BigInt("12345678910111213"), BigInt("12345678910111213")},
  {BigInt("+12345678910111213"), BigInt("12345678910111213")},
  {BigInt("+12345678910111213"), BigInt("+12345678910111213")},

  {BigInt("-12345678910111213"), BigInt("-12345678910111213")}
};

const std::vector<TestValueExpectedPair<BigInt>> lessTestCase =
{
  {BigInt(), BigInt(1)},
  {BigInt(-1), BigInt()},
  {BigInt(0), BigInt(1)},
  {BigInt(-1), BigInt(0)},
  {BigInt(-1), BigInt(1)},
  {BigInt(1), BigInt(2)},

  {BigInt(), BigInt("1")},
  {BigInt("-1"), BigInt()},
  {BigInt("0"), BigInt(1)},
  {BigInt(-1), BigInt("0")},
  {BigInt("-1"), BigInt(0)},

  {BigInt("-1"), BigInt("1")},
  {BigInt("-12345678910111213"), BigInt("12345678910111213")},
  {BigInt("1"), BigInt("2")},
  {BigInt("12345678910111213"), BigInt("12345678910111214")}
};

const std::vector<TestValueExpectedPair<BigInt>> inequalTestCase =
{
  {BigInt(0), BigInt(-1)},

  {BigInt(1), BigInt(-1)},
  {BigInt(1), BigInt("-1")},

  {BigInt("12345678910111213"), BigInt("-12345678910111213")},
  {BigInt("+12345678910111213"), BigInt("-12345678910111213")}
};

const std::vector<BigInt> incrementDecrementTestCase =
{
  BigInt(),
  BigInt(1),
  BigInt(-1),
  BigInt(-10),
  BigInt("-12345678910111213")
};

const std::vector<TestValueExpectedPair<BigInt>> unaryMinusTestCase =
{
  {BigInt(), BigInt()},
  {BigInt(), BigInt(0)},
  {BigInt(1), BigInt(-1)},
  {BigInt(-1), BigInt(1)},
  {BigInt(10), BigInt(-10)},
  {BigInt("-12345678910111213"), BigInt("12345678910111213")},
  {BigInt("12345678910111213"), BigInt("-12345678910111213")}
};

const std::vector<TestValuePair<int>> logicalOperationTestCase =
{
  {0, 0},
  {0, 1},
  {1, 1},

  {0, -1},
  {-1, -1},

  {123, 32},
  {421, 3254},
  {-53, 653},
  {-53, -653},
};

const std::vector<TestValuePair<int>>& arithmeticOperationTestCase =
{
  {0, 0},
  {0, 1},
  {1, 0},
  {1, 1},
  {-1, 1},

  {0, -1},
  {-1, -1},
  {-1, 0},

  {0, 2},
  {1, 2},
  {2, 2},
  {3, 2},

  {-10, 7},
  {10, -7},
  {10, 7},
  {-10, -7},

  {-10, 5},
  {10, -5},
  {10, 5},
  {-10, -5},

  {123, 32},
  {421, 3254},
  {-53, 653},
  {-53, -653},

  {-999, -9},
  {-999, 1},
  {-991, 9},

  {999, 1},
  {999, 9},
  {999, 10},
  {999, 100},

  {999, -1},
  {999, -9},
  {999, -10},
  {999, -100}
};

TEST(initializationTest, constructorTest)
{
  EXPECT_NO_THROW(BigInt());
  EXPECT_NO_THROW(BigInt(0));
  EXPECT_NO_THROW(BigInt(1));
  EXPECT_NO_THROW(BigInt(-1));
  EXPECT_NO_THROW(BigInt(""));
  EXPECT_NO_THROW(BigInt("0"));
  EXPECT_NO_THROW(BigInt("1"));
  EXPECT_NO_THROW(BigInt("-1"));
  EXPECT_NO_THROW(BigInt("+123"));

  EXPECT_NO_THROW(BigInt("000000123"));
  EXPECT_NO_THROW(BigInt("000000"));

  EXPECT_ANY_THROW(BigInt("-"));
  EXPECT_ANY_THROW(BigInt("+"));
  EXPECT_ANY_THROW(BigInt("+-"));
  EXPECT_ANY_THROW(BigInt("-+"));

  EXPECT_ANY_THROW(BigInt("@#$"));
}

TEST(comparativeTest, equalTest)
{
  for (auto [testValue, expectedValue] : equalTestCase)
  {
    EXPECT_EQ(testValue, expectedValue);
  }
}

TEST(comparativeTest, inequalTest)
{
  for (auto [testValue, expectedValue] : inequalTestCase)
  {
    EXPECT_NE(testValue, expectedValue);
  }
}

TEST(comparativeTest, lessTest)
{
  for (auto [testValue, expectedValue] : lessTestCase)
  {
    EXPECT_LT(testValue, expectedValue);
  }
}

TEST(comparativeTest, moreTest)
{
  for (auto [testValue, expectedValue] : lessTestCase)
  {
    EXPECT_GT(expectedValue, testValue);
  }
}

TEST(comparativeTest, lessEqualTest)
{
  // check of equality

  for (auto [testValue, expectedValue] : equalTestCase)
  {
    EXPECT_LE(testValue, expectedValue);
  }

  // check of less

  for (auto [testValue, expectedValue] : lessTestCase)
  {
    EXPECT_LE(testValue, expectedValue);
  }
}

TEST(comparativeTest, moreEqualTest)
{
  // check of equality

  for (auto [testValue, expectedValue] : equalTestCase)
  {
    EXPECT_GE(testValue, expectedValue);
  }

  // check of more

  for (auto [testValue, expectedValue] : lessTestCase)
  {
    EXPECT_GE(expectedValue, testValue);
  }
}

TEST(arithmeticTest, preIncrementTest)
{
  auto preIncrementTestFunction = [](BigInt bi)
  {
    EXPECT_EQ(++bi, bi);
    EXPECT_EQ(bi, ++bi);
    EXPECT_EQ(++bi, ++bi);
  };

  for (auto el : incrementDecrementTestCase)
  {
    preIncrementTestFunction(el);
  }
}

TEST(arithmeticTest, postIncrementTest)
{
  auto postIncrementTestFunction = [](BigInt bi)
  {
    EXPECT_GT(bi++, bi);
    EXPECT_LT(bi, bi++);
    EXPECT_EQ(bi++, bi++);
  };

  for (auto el : incrementDecrementTestCase)
  {
    postIncrementTestFunction(el);
  }
}

TEST(arithmeticTest, preDecrementTest)
{
  auto preDecrementTestFunction = [](BigInt bi)
  {
    EXPECT_EQ(--bi, bi);
    EXPECT_EQ(bi, --bi);
    EXPECT_EQ(--bi, --bi);
  };

  for (auto el : incrementDecrementTestCase)
  {
    preDecrementTestFunction(el);
  }
}

TEST(arithmeticTest, postDecrementTest)
{
  auto postDecrementTestFunction = [](BigInt bi)
  {
    EXPECT_LT(bi--, bi);
    EXPECT_GT(bi, bi--);
    EXPECT_EQ(bi--, bi--);
  };

  for (auto el : incrementDecrementTestCase)
  {
    postDecrementTestFunction(el);
  }
}

TEST(arithmeticTest, assignmentPlusTest)
{
  auto assignmentPlusTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) += BigInt(num2),
      BigInt(num1 + num2)
    ) << num1 << " " << num2;
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    assignmentPlusTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, assignmentMinusTest)
{
  auto assignmentMinusTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) -= BigInt(num2),
      BigInt(num1 - num2)
    );
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    assignmentMinusTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, assignmentProductTest)
{
  auto assignmentProductTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) *= BigInt(num2),
      BigInt(num1 * num2)
    );
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    assignmentProductTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, assignmentQuotientTest)
{
  auto assignmentQuotientTestFunction = [](int num1, int num2)
  {
    if (num2 != 0)
    {
      EXPECT_EQ
      (
        BigInt(num1) /= BigInt(num2),
        BigInt(num1 / num2)
      ) << num1 << " " << num2;
    }
    else
    {
      EXPECT_ANY_THROW
      (
        {BigInt(num1) /= BigInt(num2);}
      );
    }
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    assignmentQuotientTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, assignmentRemainderTest)
{
  auto assignmentRemainderTestFunction = [](int num1, int num2)
  {
    if (num2 != 0)
    {
      EXPECT_EQ
      (
        BigInt(num1) %= BigInt(num2),
        BigInt(num1 % num2)
      );
    }
    else
    {
      EXPECT_ANY_THROW
      (
        BigInt(num1) %= BigInt(num2)
      );
    }
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    assignmentRemainderTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, logicalNegationTest)
{
  const std::vector<int> logicalNegationTestCase =
  {
    10, -10, 0, 1, -1, 999, -999, 1000, -1000//, 12345678, -12345678, INT_MAX, INT_MIN
  };

  auto logicalNegationTestFunction = [](int num)
  {
    EXPECT_EQ
    (
      ~BigInt(num),
      BigInt(~num)
    ) << num;
  };

  for (auto el : logicalNegationTestCase)
  {
    logicalNegationTestFunction(el);
  }
}

TEST(bitOperationTest, assignmentXorTest)
{
  auto assignmentXorTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) ^= BigInt(num2),
      BigInt(num1 ^ num2)
    ) << num1 << " " << num2;
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    assignmentXorTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, assignmentLogicalAndTest)
{
  auto assignmentLogicalAndTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) &= BigInt(num2),
      BigInt(num1 & num2)
    );
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    assignmentLogicalAndTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, assignmentLogicalOrTest)
{
  auto assignmentLogicalOrTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) |= BigInt(num2),
      BigInt(num1 | num2)
    );
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    assignmentLogicalOrTestFunction(value1, value2);
  }
}

TEST(unarySignTest, unaryPlusTest)
{
  for (auto [testValue, expectedValue] : unaryMinusTestCase)
  {
    EXPECT_EQ(+testValue, testValue);
  }
}

TEST(unarySignTest, unaryMinusTest)
{
  for (auto [testValue, expectedValue] : unaryMinusTestCase)
  {
    EXPECT_EQ(-testValue, expectedValue);
  }
}

TEST(castTest, intCastTest)
{
  const std::map<BigInt, int> intCastTestCase =
  {
    {BigInt(), 0},
    {BigInt(0), 0},
    {BigInt(1), 1},
    {BigInt(-1), -1},
    {BigInt("12345"), 12345},
    {BigInt("-12345"), -12345}
  };

  for (auto el : intCastTestCase)
  {
    EXPECT_EQ(static_cast<int>(el.first), el.second);
  }

  const std::vector<BigInt> exceptionIntCastTestCase =
  {
    BigInt(std::to_string(INT_MAX) + "0"),
    BigInt(std::to_string(INT_MIN) + "0")
  };

  for (auto el : exceptionIntCastTestCase)
  {
    EXPECT_ANY_THROW(static_cast<int>(el));
  }
}

TEST(castTest, stringCastTest)
{
  const std::map<BigInt, std::string> stringCastTestCase =
  {
    {BigInt(), "0"},
    {BigInt(0), "0"},
    {BigInt(1), "1"},
    {BigInt(-1), "-1"},
    {BigInt("+12345678910111213"), "12345678910111213"},
    {BigInt("-12345678910111213"), "-12345678910111213"}
  };

  for (auto el : stringCastTestCase)
  {
    EXPECT_EQ(static_cast<std::string>(el.first), el.second);
  }
}

TEST(arithmeticTest, plusTest)
{
  auto plusTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) + BigInt(num2),
      BigInt(num1 + num2)
    );
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    plusTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, minusTest)
{
  auto minusTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) - BigInt(num2),
      BigInt(num1 - num2)
    );
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    minusTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, productTest)
{
  auto productTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) * BigInt(num2),
      BigInt(num1 * num2)
    );
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    productTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, quotientTest)
{
  auto quotientTestFunction = [](int num1, int num2)
  {
    if (num2 != 0)
    {
      EXPECT_EQ
      (
        BigInt(num1) / BigInt(num2),
        BigInt(num1 / num2)
      ) << num1 << " " << num2;
    }
    else
    {
      EXPECT_ANY_THROW
      (
        BigInt(num1) / BigInt(num2)
      );
    }
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    quotientTestFunction(value1, value2);
  }
}

TEST(arithmeticTest, remainderTest)
{
  auto remainderTestFunction = [](int num1, int num2)
  {
    if (num2 != 0)
    {
      EXPECT_EQ
      (
        BigInt(num1) % BigInt(num2),
        BigInt(num1 % num2)
      );
    }
    else
    {
      EXPECT_ANY_THROW
      (
        BigInt(num1) % BigInt(num2)
      );
    }
  };

  for (auto [value1, value2] : arithmeticOperationTestCase)
  {
    remainderTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, xorTest)
{
  auto xorTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) ^ BigInt(num2),
      BigInt(num1 ^ num2)
    );
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    xorTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, logicalAndTest)
{
  auto logicalAndTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) & BigInt(num2),
      BigInt(num1 & num2)
    );
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    logicalAndTestFunction(value1, value2);
  }
}

TEST(bitOperationTest, logicalOrTest)
{
  auto logicalOrTestFunction = [](int num1, int num2)
  {
    EXPECT_EQ
    (
      BigInt(num1) | BigInt(num2),
      BigInt(num1 | num2)
    );
  };

  for (auto [value1, value2] : logicalOperationTestCase)
  {
    logicalOrTestFunction(value1, value2);
  }
}

TEST(streamTest, outputTest)
{
  const std::vector<BigInt> outputTestCase =
  {
    BigInt(),
    BigInt(0),

    BigInt(1),
    BigInt(-1),

    BigInt("1"),
    BigInt("-1"),

    BigInt("12345678910111213"),
    BigInt("-12345678910111213")
  };

  auto outputTestFunction = [](BigInt& bi)
  {
    std::ostringstream output;
    output << bi;
    EXPECT_EQ(output.str(), static_cast<std::string>(bi));
  };

  for (auto el : outputTestCase)
  {
    outputTestFunction(el);
  }
}

TEST(sizeNumberTest, sizeTest)
{
  const std::map<BigInt, int> sizeTestCase =
  {
    {BigInt(), 2},
    {BigInt(0), 2},
    {BigInt(1), 2},
    {BigInt(-1), 2},
    {BigInt("12345"), 6},
    {BigInt("-12345"), 6},
    {BigInt("12345678910111213"), 18},
    {BigInt("-12345678910111213"), 18}
  };

  for (auto el : sizeTestCase)
  {
    EXPECT_EQ(el.first.size(), el.second);
  }
}
