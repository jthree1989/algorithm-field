#include <unordered_set>
#include <vector>
#include <iostream> 
#include <algorithm>

int main(int argc, char** argv)
{
    std::unordered_set<int> uset{3, 1, 5000, 4, 6, 1000, 5, 9, 1000, 2000};

    std::vector<int> ivec{11 ,12 ,13, 14, 15};

    auto print = [](const int& n) { std::cout << n << " "; };
    auto print_empty_line = [](){std::cout << std::endl;};

    auto res = uset.insert(2222);
    std::cout << "insert: " << *res.first << ", " << (res.second ? "success" : "failure") << std::endl;

    res = uset.insert(6);
    std::cout << "insert: " << *res.first << ", " << (res.second ? "success" : "failure") << std::endl;

    uset.insert(ivec.begin(), ivec.end());
    uset.insert({16, 17, 18, 19 ,20, 21, 98});

    uset.emplace(99);

    uset.emplace_hint(uset.cend(), 100);

    std::for_each(uset.cbegin(), uset.cend(), print);
    print_empty_line();

    auto ret = uset.erase(std::next(std::begin(uset)));
    std::cout << "erase element before " << *ret << std::endl;
    auto begin = uset.begin();
    std::advance(begin, 5);
    ret = uset.erase(std::next(uset.cbegin()), begin);
    //std::cout << "erase element before " << *ret << std::endl;
    
    std::for_each(uset.cbegin(), uset.cend(), print);
    print_empty_line();

    return 0;
}