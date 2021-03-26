#pragma once
#include <string>
#include <sstream>
#include <algorithm>

//TODO[EAB][Mettre ca dans ONDTLib]

class VersionUtils
{
public:
  static bool IsEqualOrLess(const std::string& a_, const std::string& b_)
  {
    if (a_ == b_) {
      return true;
    }

    int parsedA[3], parsedB[3];
    Parse(parsedA, a_);
    Parse(parsedB, b_);
    return std::lexicographical_compare(parsedB, parsedB + 3, parsedA, parsedA + 3);
  }

private:
  static void Parse(int result[3], const std::string& input_)
  {
    std::istringstream parser(input_);
    parser >> result[0];
    for (int idx(1); idx < 3; idx++)
    {
      parser.get(); //Skip period
      parser >> result[idx];
    }
  }
};
