#include <iostream>
#include "double_chain.hpp"
struct Test
{
    int number;
    ~Test()
    {

    }
};
int main()
{
    double_chain<Test> chain;
    Test t = {5};
    Test t1 = {1};
    Test t2 = {2};
    Test t3 = {3};
    chain.push_back(t);
    chain.push_back(t1);
    chain.push_back(t2);
    chain.push_back(t3);
    for(auto iter=chain.begin();iter!=chain.end();iter++)
    {
        std::cout<<(*iter).number<<std::endl;
        (*iter).number = 100;
    }
//    for(auto& iter:chain)
//    {
//        std::cout<<iter.number<<std::endl;
//    }
    return 0;
}