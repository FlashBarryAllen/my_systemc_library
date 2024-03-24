#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int parseNum(string& token)
{
  int num;
  if (token.find("0x") != string::npos) {
      sscanf(token.c_str(), "%x", &num);
  } else {
      num = stoi(token);
  }

  return num;
}

vector<int> parseString(const string& str) {
  vector<int> res;
  stringstream ss(str);
  string token;

  while (getline(ss, token, ',')) {
    if (token.find("..") != string::npos) {
      auto start = token.substr(0, token.find(".."));
      int start_num = parseNum(start);

      auto end = token.substr(token.find("..") + 2);
      int end_num = parseNum(end);

      for (int i = start_num; i <= end_num; ++i) {
        res.push_back(i);
      }
    } else {
        int num = parseNum(token);
        res.push_back(num);
    }
  }

  return res;
}

int main() {
  string str = "0..4,5,6,7,0x111,12,-3..3,0x112..0x116";
  vector<int> res = parseString(str);

  for (int num : res) {
    cout << num << " ";
  }

  cout << endl;

  return 0;
}
