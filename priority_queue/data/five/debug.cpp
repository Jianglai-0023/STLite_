#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <ctime>
#include <cstdlib>

#include "priority_queue.hpp"
int main(){
    sjtu::priority_queue<int> pq1, pq2;
    for(int i = 1; i <= 3; ++i){
        pq1.push(i);
    }
    std::cout << "here" << std::endl;
    pq1.print(pq1.root);
    std::cout << "end" << std::endl;
    for(int i = 4; i <= 6; ++i){
        pq2.push(i);
    }
    std::cout << "here" << std::endl;
    pq2.print(pq2.root);
    std::cout << "end" << std::endl;
    pq1.merge(pq2);
    pq1.print(pq1.root);
//    pq1.pop();

}
