#ifndef CSV_PARSER_HPP
#define CSV_PARSER_HPP

#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>

#include "tupleUtility.hpp"

namespace csv
{
  const char defaultLineDelimiter = '\n';
  const char defaultValueDelimiter = ',';
  const char defaultFieldDelimiter = '\"';

  template<class ... Args>
  class CSVReader
  {
  public:
    CSVReader(char lineDelimiter = defaultLineDelimiter, char valueDelimiter = defaultValueDelimiter, char fieldDelimiter = defaultFieldDelimiter) :
      _lineDelimiter{lineDelimiter}, _valueDelimiter{valueDelimiter}, _fieldDelimiter{fieldDelimiter} {}

    virtual ~CSVReader() {}

    void readInTuple(std::istream& input, std::tuple<Args...>& tuple)
    {
      readLine(input, tuple, std::index_sequence_for<Args...>());
    }

    bool canRead(std::istream &input)
    {
      return (input.good() && (input.peek() != EOF));
    }

  private:
    char _lineDelimiter;
    char _valueDelimiter;
    char _fieldDelimiter;

    std::string _nextLine;
    size_t _countReadLines = 0;
    size_t _curValuePos = 0;
    std::vector<std::string> _lineFields;
    std::vector<size_t> _delimitersPos;
    size_t _curFieldIndex = 0;

    enum class CSVState
    {
      UnescapedField,
      EscapedField,
      EscapedEscape,
    };

    void splitIntoStrings()
    {
      CSVState state = CSVState::UnescapedField;
      _lineFields.clear();
      _lineFields.emplace_back("");
      _delimitersPos.clear();
      size_t curField = 0;
      size_t curPos = 0;
      for (auto c : _nextLine)
      {
        switch (state)
        {
          case CSVState::UnescapedField:
          {
            if (c == _valueDelimiter)
            {
              _lineFields.emplace_back("");
              _delimitersPos.push_back(curPos);
              curField++;
            }
            else if (c == _fieldDelimiter)
            {
              state = CSVState::EscapedField;
            }
            else
            {
              _lineFields[curField].push_back(c);
            }
            break;
          }
          case CSVState::EscapedField:
          {
            if (c == _fieldDelimiter)
            {
              state = CSVState::EscapedEscape;
            }
            else
            {
              _lineFields[curField].push_back(c);
            }
            break;
          }
          case CSVState::EscapedEscape:
          {
            if (c == _valueDelimiter)
            {
              _lineFields.emplace_back("");
              _delimitersPos.push_back(curPos);
              curField++;
              state = CSVState::UnescapedField;
            }
            else if (c == _fieldDelimiter)
            {
              _lineFields[curField].push_back(_fieldDelimiter);
              state = CSVState::EscapedField;
            }
            else
            {
              state = CSVState::UnescapedField;
            }
            break;
          }
        }
        curPos++;
      }
    }

    template<size_t ... index>
    void readLine(std::istream& input, std::tuple<Args...>& tuple, std::index_sequence<index...>)
    {
      std::getline(input, _nextLine, _lineDelimiter);
      _countReadLines++;
      _curValuePos = 0;
      _curFieldIndex = 0;
      if (_nextLine.empty())
      {
        throw std::invalid_argument("Line is empty: Line = " + std::to_string(_countReadLines + 1) + " Column = " + std::to_string(_curValuePos) + "\n");
      }

      splitIntoStrings();

      (..., (readValue(std::get<index>(tuple))));

      if (_curFieldIndex != _lineFields.size())
      {
        std::stringstream s;
        s << "Unexpected value! " << _lineFields[_curFieldIndex];
        throw std::invalid_argument(s.str() + "Line = " + std::to_string(_countReadLines) + " Column = " +
          std::to_string(_curValuePos + 1) + "\n");
      }
    }

    template<typename T>
    void readValue(T& t)
    {
      std::string curField = _lineFields[_curFieldIndex];
      std::stringstream convert{curField};
      if ((convert >> t).fail() || !(convert >> std::ws).eof())
      {
        std::stringstream s;
        s << "Couldn't parse value " << curField;
        throw std::invalid_argument(s.str() + "Line = " + std::to_string(_countReadLines) + " Column = " + std::to_string(_curValuePos + 1) + "\n");
      }
      _curValuePos = _delimitersPos[_curFieldIndex] + 1;
      _curFieldIndex++;
    }
  };

  template<class ... Args>
  class CSVIterator
  {
  public:
    CSVIterator(std::istream& input, char lineDelimiter = defaultLineDelimiter, char valueDelimiter = defaultValueDelimiter, char fieldDelimiter = defaultFieldDelimiter) :
      _input{input}, _reader{lineDelimiter, valueDelimiter, fieldDelimiter}, _nextTuple{}, _canRead{input.good()}
    {
      ++(*this);
    }

    CSVIterator(std::istream& input, bool isEnd) : _input{input}, _reader{}, _nextTuple{}, _canRead{!isEnd} {}

    virtual ~CSVIterator() {}

    CSVIterator operator++()
    {
      _canRead = _reader.canRead(_input);
      if (_canRead)
      {
        _reader.readInTuple(_input, _nextTuple);
      }
      return *this;
    }

    CSVIterator operator++(int)
    {
      CSVIterator copy{*this};
      ++(*this);
      return copy;
    }

    const std::tuple<Args...> &operator*() const
    {
      return _nextTuple;
    }

    const std::tuple<Args...> *operator->() const
    {
      return &_nextTuple;
    }

    bool operator==(const CSVIterator &other) const
    {
      return ((this == &other) || (_canRead == other._canRead));
    }

    bool operator!=(const CSVIterator &other) const
    {
      return !((*this) == other);
    }

  private:
    std::istream& _input;
    CSVReader<Args...> _reader;
    std::tuple<Args...> _nextTuple;
    bool _canRead = false;
  };

  template<class ... Args>
  class CSVParser
  {
  public:
    CSVParser(std::istream& input, size_t offset = 0, char lineDelimiter = defaultLineDelimiter, char valueDelimiter = defaultValueDelimiter, char fieldDelimiter = defaultFieldDelimiter) :
      _input{input}, _offset{offset}, _lineDelimiter{lineDelimiter}, _valueDelimiter{valueDelimiter}, _fieldDelimiter{fieldDelimiter} {}

    virtual ~CSVParser() {}

    CSVIterator<Args...> begin() const
    {
      CSVIterator<Args...> iter{_input, _lineDelimiter, _valueDelimiter, _fieldDelimiter};
      for (int i = 0; i < _offset; ++i)
      {
        iter++;
      }
      return iter;
    }

    CSVIterator<Args...> end() const
    {
      return CSVIterator<Args...> {_input, true};
    }

  private:
    std::istream& _input;
    size_t _offset;
    const char _lineDelimiter;
    const char _valueDelimiter;
    const char _fieldDelimiter;
  };
} // namespace csv
#endif // CSV_PARSER_HPP
