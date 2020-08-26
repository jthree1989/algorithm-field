#include <set>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>

int main(int argc, char* argv[])
{
    std::set<int, std::greater<int>> set{666, 777, 888};
    set.emplace(999);

    auto res = set.insert(555);
    assert(res.first != set.end());
    assert(*res.first == 555);
    assert(res.second);


    auto print = [](const int& n) { std::cout << " " << n; };
    auto print_empty_line = [](){std::cout << std::endl;};

    std::for_each(set.cbegin(), set.cend(), print);
    print_empty_line();

    set.clear();
    set.empty() ? std::cout << "set is empty." << std::endl
                : std::cout << "set is not empty" << std::endl;

    std::cout << "The size of set is " << set.size() << std::endl;

    std::cout << "The maximum possible size of set is " << set.max_size() << std::endl;


    return 0;
}