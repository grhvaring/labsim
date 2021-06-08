#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
  string key[] = {"one", "two", "three"};
  // for (int i = 0; i < 3; i++) s[i] = to_string(i + 1);
  vector<string> s;
  for (string k : key) s.push_back(k);
  // for (string k : s) cout << k << endl;

  map<string, double> m;
  m["one"] = 1;
  m["two"] = 2;
  // for (auto mi : m) cout << mi.second << endl;

  int *p = nullptr;
  int q[] = {0, 0, 0, 1};
  p = q;
  // for (int i = 0; i < 4; i++) cout << p[i];
  string w = "hola";
  cout << (w[0] == 'h') << endl;
  return 0;
}