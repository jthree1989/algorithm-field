#include <set>
#include <iostream>
#include <algorithm>
#include <string>

class A {
  public:
    int a;
    void print() const
    {
        std::cout << a << std::endl;
    }
};
A getTemp()
{
    return A();
}

class Copyable {
    public:
        Copyable(){}
        Copyable(const Copyable &o) {
            std::cout << "Copied" << std::endl;
        }
};
Copyable ReturnRvalue() {
    return Copyable(); //返回一个临时对象
}
void AcceptVal(Copyable a) {

}
void AcceptRef(const Copyable& a) {

}


int main(int argc, char** argv)
{
    std::multiset<int> mset {1, 1, 1, 1};

    const int& a = 1; //常量左值引用绑定右值，不会报错
    int&& b = 2;
    // int&& c = b; 编译错误！不能将一个左值复制给一个右值引用

    const A& ca = getTemp();
    ca.print();

    std::cout << "a: " << a << ", b: " << b << std::endl; 

    std::cout << "pass by value: " << std::endl;
    AcceptVal(ReturnRvalue()); // 应该调用两次拷贝构造函数
    std::cout << "pass by reference: " << std::endl;
    AcceptRef(ReturnRvalue()); //应该只调用一次拷贝构造函数

    mset.insert(b);
    mset.insert(3);

    auto print = [](int ele) -> void {
        std::cout << ele << " ";
    };

    auto print_line = []() -> void{
        std::cout << std::endl << std::string(10, '-') << std::endl;
    };

    std::for_each(mset.begin(), mset.end(), print);
    print_line();

    return 0;
}
