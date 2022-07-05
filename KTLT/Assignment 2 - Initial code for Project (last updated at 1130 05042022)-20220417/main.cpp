/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 2
* Programming Fundamentals Spring 2022
* Author: Vu Van Tien
* Date: 15.02.2022
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "studyInPink3b.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // Point: simple test
    Point p1(0, 5);
    cout << p1.toString() << endl;
    Point p2(9, 3);
    cout << p2.toString() << endl;

    // Node: simple test
    Node node1(Point(1, 2));
    Node node2(Point(2,3), &node1);
    cout << "node 1:" <<  node1.toString() << endl;
    cout << "node 2:" << node2.toString() << endl;

    // Path: simple test
    Path path1;
    path1.addPoint(0, 1);
    path1.addPoint(1, 3);
    cout << path1.toString() << endl;

    // Character: simple test
    Character chWatson("Watson");
    cout << chWatson.toString() << endl;
    chWatson.moveToPoint(2, 7);
    cout << chWatson.toString() << endl;

    return 0;
}