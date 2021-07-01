// DSA_LAB.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "SLinkedList.h"
#include <cassert>
int main()
{ //list.Lprint();
    
    SLinkedList<int> list;
    /*  int size = 8;
    int values[] = { 10,  15,  2,   6,  4,  7,   40,  8 };
    for (int index = 0; index < size; index++) {
        list.add(values[index]);
    }
    */
    for (int i = 0; i < 10; ++i) {
        list.add(i);
    }
    assert(list.get(9) == list.removeAt(9));

   list.Lprint();
     return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
