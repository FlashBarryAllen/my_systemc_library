#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <memory>
#include <sstream>

using namespace std;

int parseNum(string& token)
{
  int num = 0;
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
  /*
  string str = "0..4,5,6,7,0x111,12,-3..3,0x112..0x116";
  vector<int> res = parseString(str);

  for (int num : res) {
    cout << num << " ";
  }

  cout << endl;
  */

  int a[] = {1, 2, 3, 1};
  auto my_set = std::set<int>(a, a + 4);

  std::shared_ptr<void> ptr = nullptr;
  auto p1 = std::make_shared<int>(10);
  ptr = p1;
  std::cout << "*ptr = " << *(static_pointer_cast<int>(ptr)) << std::endl;

  std::map<int, char> test_map;
  test_map.insert(std::make_pair(10, 'a'));
  test_map[10] = 'b';
  //test_map.insert(std::make_pair(10, 'b'));

  std::multimap<int, int> mmp;
  mmp.insert(std::make_pair(1, 2));
  mmp.insert(std::make_pair(1, 2));

  vector<deque<shared_ptr<int>>> sg;
  sg.resize(4);
  auto b = make_shared<int>(100);
  sg[1].push_back(b);

	/*
	while (1) {
		//std::shared_ptr<int[]> arr(new int[10]);
		auto arr = new int[10];
		arr[0] = 0;
		arr[9] = 1;
	}
	*/

	std::vector<int> vec = {1, 2, 3};

  // 使用下标访问
  int num1 = vec[1]; // 安全
  int num2 = vec[10]; // 越界，程序崩溃

  // 使用 at() 函数
  //try {
    int num3 = vec.at(1); // 安全
    int num4 = vec.at(10); // 越界，抛出异常
  //} catch (const std::out_of_range& e) {
  //  std::cout << e.what() << std::endl;
//}

  vector<vector<float>> nested_vec(2, vector<float>(2));
  nested_vec[0][0] = 1.2;
  nested_vec[1][1] = 2.2;

  nested_vec[0].resize(5);

  std::shared_ptr<int[]> arr(new int[10]);

  // 访问数组元素
  for (int i = 0; i < 10; i++) {
    arr[i] = i + 100;
  }

  return 0;
}
