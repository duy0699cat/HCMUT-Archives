/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2022
* Author: Vu Van Tien
* Date: 15.02.2022
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef studyInPink_h
#define studyInPink_h

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////
///STUDENT'S ANSWER BEGINS HERE
///Complete the following functions
///DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

class Point {
private:
    int x;
    int y;
public:
    Point(int x = 0, int y = 0);
    string toString() const;
    int getX();
    int getY();
    int distanceTo(const Point& otherPoint) const;
};

class Node {
private:
    Point point;
    Node* next;

    friend class Path;

public:
    Node(const Point& point = Point(), Node* next = NULL);

    string toString() const;
};

class Path {
private:
    Node* head;
    Node* tail;
    int count;
    int length;

public:
    Path();
    Path(const Path &cpy); //constructor with copy
    ~Path();
    void calPathLen();
    void addPoint(int x, int y);
    string toString() const;
    Point getLastPoint() const;

    /// Extra tasks for Project of Fundamentals Programming
    bool removeLast();
    Path operator+ (const Path& other);
    int getLeng();
};

class Character {
private:
    string name;
    Path* path;

public:
    Character(const string& name = "");
    ~Character();

    string getName() const;
    void setName(const string& name);

    void moveToPoint(int x, int y);
    string toString() const;
    Point getCurPos();
    int getCLeng();
  
};
////answer



inline Point::Point(int xx, int yy)
{
    x = xx;
    y = yy;
}

string Point::toString() const
{
    return "<Point[" + to_string(x) + "," + to_string(y) + "]>";
}

inline int Point::getX()
{
    return this->x;
}

inline int Point::getY()
{
    return this->y;
}

int Point::distanceTo(const Point& otherPoint) const
{
    double d = pow((otherPoint.x-x),2.0) + pow((otherPoint.y-y),2.0);
    return ceil(sqrt(d));
}

inline Node::Node(const Point& pointt, Node* nextt)
{
    point = pointt;
    next = nextt;
}

string Node::toString() const
{
    return "<Node["+point.toString()+ "]>";
}

Path::Path()
{
    head =  NULL;
    tail = NULL;
    count = 0;
    length = -1;

}
//Path::Path(const Path &cpy)
//{
//    if (cpy.head == nullptr) return;
//    length = cpy.length;
//    count = cpy.count;
//    ///////////
//
//    head = new Node(cpy.head->point,nullptr); // Allocate head in memory.
//    if (cpy.head == cpy.tail) this->tail = this->head;
//
//    Node *current = head;   // for the loop
//    Node* tmp = cpy.head;
//    // Move to next item in cpy's list.
//    tmp = tmp->next;
//
//    /// deep copy:
//    /// current ->  tmp
//    ///     *p1=a    *p4=a
//    ///     *p2=b    *p5=b
//    ///     *p3=c    *p6=c
//
//    while (tmp != nullptr)
//    {
//        // Allocate new memory for a new 'node', Copy over the data.
//        current->next = new Node(tmp->point, nullptr);
//        // Move along cpy's list.
//        current = current->next;
//        if (tmp->next == nullptr) this->tail = current;
//        tmp = tmp->next;
//    }
//   
//}
Path::~Path()
{
    Node* current = this->head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    //count = 0;
   // length = 0; delete_scalar.cpp ???
    
}

inline void Path::calPathLen()
{
    if (count < 0) throw("count<0");
    else if (count == 0) length = -1;
    else if(count==1) length = 0;
    else
    {
        int temp=0;
        Node* nt = head;
        while (nt->next != nullptr) {
            Point tp2 = nt->next->point;
            temp = temp + nt->point.distanceTo(tp2);
            nt = nt->next;
        }
        length = temp;
    }
}

void Path::addPoint(int x, int y)
{
    Point temp = Point(x, y);
    Node* tNode = new Node(temp,nullptr);
    if (head == nullptr) {        head = tNode; tail = tNode;    }
    else if (tail == head){head->next= tNode;
    tail = tNode;
    }
    else {
        this->tail->next = tNode;
        this->tail = tNode;
    }
    this->count++;
    this->calPathLen();
}
Path::Path(const Path& cpy) {
    if (cpy.head == nullptr) return; //count = 0
        ///////////
    
    head = new Node(cpy.head->point, nullptr);// Allocate head in memory.
    tail = head;
    count = 1;
        Node* tmp = cpy.head;
        while (tmp->next != nullptr) {
            tmp = tmp->next;
            this->addPoint(tmp->point.getX(), tmp->point.getY());
        }

}
string Path::toString() const
{
    string temp;
    Node *nt = head;
    while (nt != nullptr) {
        temp = temp + nt->toString();
        if (nt->next != nullptr)temp = temp + ",";
        nt = nt->next;
    }
    return "<Path[count:"+to_string(count)+",length:"+to_string(length)+",["+temp+ "]]>";
}

Point Path::getLastPoint() const
{
    return tail->point;
}

bool Path::removeLast()
{
    if (count == 0) return false;
    else if (count == 1) {
        delete head, tail;
    }
    else {
    Node* nt = head;
    while (nt->next != tail) {
        nt = nt->next;
    }
    delete tail;
    nt->next = nullptr;
    tail = nt;
    }
    count--;
    calPathLen();
    return true;
}

Path Path::operator+(const Path& other)
{
    Path temp =  Path(*this);
    Path temp2 = other;
    Node *tNode = temp2.head;
    while (tNode != nullptr) {
        temp.addPoint(tNode->point.getX(), tNode->point.getY());
        tNode = tNode->next;
    }
    temp.calPathLen();
    return temp;
}

inline int Path::getLeng()
{
    return this->length;
}

Character::Character(const string& name)
{
    this->name = name;
    this->path = new Path();
}

Character::~Character()
{
    this->path->~Path();
}

string Character::getName() const
{
    return this->name;
}

void Character::setName(const string& name)
{
    this->name = name;
}

void Character::moveToPoint(int x, int y)
{
    this->path->addPoint(x, y);
}

string Character::toString() const
{
    return "<Character[name:"+this->name+",path:"+this->path->toString()+"]>";
}

inline Point Character::getCurPos()
{
    return this->path->getLastPoint();
}

inline int Character::getCLeng()
{
    return this->path->getLeng();
}

bool rescueSherlock(
    const Character& chMurderer,
    const Character& chWatson,
    int maxLength,
    int maxDistance,
    int& outDistance
) {
    Character c1 = chWatson;
    Character c2 = chMurderer;
    int l1 = c1.getCLeng();
    int l2 = c2.getCLeng();
    outDistance = c1.getCurPos().distanceTo(c2.getCurPos());
    if (l1 - l2 <= maxLength) {
        if (outDistance <= maxDistance)return true;
        else return false;
    }
    else {
        outDistance = -1;
        return false;
    }
}
////////////////////////////////////////////////
///END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* studyInPink_h */