#ifndef TUPLE_UTILITY_HPP
#define TUPLE_UTILITY_HPP

#include <tuple>
#include <ostream>

template<std::size_t>
struct int_ {};

template<typename Ch, typename Tr, typename Tuple, size_t PosFromEnd>
auto& printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& tuple, int_<PosFromEnd>)
{
  os << std::get<std::tuple_size<Tuple>::value - PosFromEnd>(tuple) << ", ";
  return printTuple(os, tuple, int_<PosFromEnd - 1>());
}

template<typename Ch, typename Tr, typename Tuple>
auto& printTuple(std::basic_ostream<Ch, Tr>& os, const Tuple& t, int_<1>)
{
  return os << std::get<std::tuple_size<Tuple>::value - 1>(t);
}

template<typename Ch, typename Tr, typename ... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, const std::tuple<Args...>& t)
{
  printTuple(os, t, int_<sizeof...(Args)>());
  return os;
}

#endif // TUPLE_UTILITY_HPP
