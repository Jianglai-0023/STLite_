#include "../five/map.hpp"
#include <iostream>
#include <cassert>
#include <string>

int main(){
    sjtu::map<int,int> m;
    for(int i = 1; i <= 5; ++i)m[i] = i;
    m.Intraverse();
    sjtu::map<int, int> :: const_iterator itt;
    itt = m.cbegin();
    std::cout <<(itt)->first << std::endl;
    ++itt;
    ++itt;
    std::cout <<(itt)->first << std::endl;
    ++itt;
    ++itt;
    ++itt;
    if(itt==m.cend()) std::cout <<"right" << std::endl;
    std::cout <<(itt)->first << std::endl;
//    for(int i = 1; i <= 20; ++i){
//        std::cout << i<<std::endl;
//        if(!m.count(i))return 0;
//        std::cout << m.count(i)<<std::endl;
//        m.erase(m.find(i));
//        if(m.count(i))return 0;
//        std::cout << m.count(i)<<std::endl;
//    }
//    for(int i = 2; i <= 5; ++i){
//        m.Intraverse();
//        std::cout << i<<std::endl;
//        if(!m.count(i))return 0;
//        std::cout << m.count(i)<<std::endl;
//        m.erase(m.find(i));
//        if(m.count(i))return 0;
//        std::cout << m.count(i)<<std::endl;
//    }
//    m.Intraverse();

//    m.Intraverse();
//    exit(0);
//    m.erase(m.find(1));
//    m.Intraverse();
//    for(int i = 50; i >= 1; --i){
//        std::cout << m.find(i)->second << std::endl;
//         m.erase(m.find(i));
//    }

}