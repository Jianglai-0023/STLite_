#include<type_traits>
#include <iostream>
using namespace std;
int main(){
     is_void<int> a;
    if(!a) cout << "NO" << endl;
}

