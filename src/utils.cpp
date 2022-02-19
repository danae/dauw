#include "utils.h"

using namespace dauw;
using namespace std;


// Split a string by a regex pattern
vector<string> dauw::splitRegex(string source, regex regex)
{
  vector<string> splits;

  sregex_token_iterator token(source.begin(), source.end(), regex, -1);
  sregex_token_iterator end;
  for (; token != end; ++token)
    splits.push_back(*token);

  return splits;
}
