#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        
        sort(intervals.begin(), intervals.end(), [](vector<int>& a, vector<int>& b){
            return a[0] <= b[0];
        });
        
        vector<vector<int>> merged;
        merged.push_back(intervals[0]);

        for(size_t i = 1; i < intervals.size(); i++)
        {
            auto& last = merged.back();
            if(!intervals[i].size()) continue;
            if(intervals[i][0] > last[1])
                merged.push_back(intervals[i]);
            else
                last[1] = intervals[i][1];
        }

        return merged;
    }

    void foo() = delete;
};

// void Solution::foo(){
  
// }

class foo{
public:
  foo(std::string name):name_(name){}
  void test_foo(){
    static int a = 0;
    std::cout << name_ << ++a << std::endl;
  }
private:
  std::string name_;
};

int main(int argc, char** argv)
{
    cout << argv[0] << endl;
    std::vector<int> a(10), b;
    std::cout << std::boolalpha << (a.begin()!= b.begin()) << std::endl;
    std::cout << a.back() << std::endl;

    foo foo_a("a"), foo_b("b");
    for(size_t i = 0; i < 100 ; i++){
      foo_a.test_foo();
      foo_b.test_foo();
    }

    return 0;
}