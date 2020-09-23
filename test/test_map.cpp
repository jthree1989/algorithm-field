#include <map>
#include <string>
#include <iostream>
#include <algorithm>

int main(int argc, char** argv)
{
    std::map<int, std::string, std::less<int>, std::allocator<std::pair<int, std::string>>> map
    { 
        {1, "Hello"},
        {2, "world"},
        {3, "!"}
    };

    std::for_each(map.cbegin(), map.cend(), [](const std::pair<int, std::string>& str){
        std::cout << str.second << " ";         
    });
    
    std::cout << std::flush;
    
    std::for_each(map.rbegin(), map.rend(), [](const std::pair<int, std::string>& str){
        std::cout << str.first << " ";         
    });

    if(!map.empty())
        map.clear();

    auto comp_func = map.value_comp();
        

    return 0;
}