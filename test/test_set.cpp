#include <set>
#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>

int main(int argc, char* argv[])
{
    std::set<int, std::less<int>> set{666, 777, 888};
    std::set<int, std::less<int>> set2{6666, 7777, 8888};

    auto func = set.key_comp();
    set.swap(set2);
    std::vector<int> vec {0,1,2,3,4,5,6,7,8,9,10};
    set.emplace(999);

    auto res = set.insert(555);
    assert(res.first != set.end());
    assert(*res.first == 555);
    assert(res.second);

    set.insert(vec.begin(), vec.end());
    set.insert({10,11,12,13});
    set.emplace(14);

    auto end = set.end();
    std::advance(end, -4);
    auto iter_erase = set.erase(end);
    std::cout << "Iterator points to " << *iter_erase << " after erasing." << std::endl;

    auto print = [](const int& n) { std::cout << " " << n; };
    auto print_empty_line = [](){std::cout << std::endl;};

    auto range = set.equal_range(7000);
    std::cout << *range.first << "\t" << 7000 << "\t" << *range.second << std::endl;

    std::for_each(set.cbegin(), set.cend(), print);
    print_empty_line();

    std::cout << "There is " << set.count(555) << " \"555\" in set" << std::endl;

    set.clear();
    set.empty() ? std::cout << "set is empty." << std::endl
                : std::cout << "set is not empty" << std::endl;

    std::cout << "The size of set is " << set.size() << std::endl;

    std::cout << "The maximum possible size of set is " << set.max_size() << std::endl;


    return 0;
}