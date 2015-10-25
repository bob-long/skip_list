/*
 * skiplist.cpp
 * 
 * Driver program that verifies skip_list.
 */

#include <iostream>

#include "skiplist.h"

using namespace std;

int main(int argc, char *argv[]) {
    skip_list<int> lst;
    
    lst.insert(0);lst.insert(5);
    
    cout << lst.search(0) << endl;
    cout << lst.search(8) << endl;
    cout << lst.search(5) << endl;
    
    lst.remove(5);
    cout << lst.search(0) << endl;
    cout << lst.search(5) << endl;
    
    lst.remove(0);
    cout << lst.search(0) << endl;
    cout << lst.search(5) << endl;
    
    return 0;
}