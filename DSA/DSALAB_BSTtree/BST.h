#pragma once
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"
template<class T>
class BinarySearchTree
{
public:
    class Node;
private:
    Node* root;
public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree()
    {
        // You have to delete all Nodes in BinaryTree. However in this task, you can ignore it.
    }

    //Helping function

    void add(T value) {
        //TODO
        Node* temp2 = new Node(value);
        
        Node* temp3 = new Node(0);
        bool L;

        if (root == nullptr) root = temp2;
        else {

            temp2 = root;
            while (temp2 != nullptr) {
                
                temp3 = temp2;

                if (value > temp2->value)
                {
                    temp2 = temp2->pRight;
                    L = 0;
                }
                else 
                {
                    temp2 = temp2->pLeft;
                    L = 1;
                }
            }

           // temp2 = temp;   
            temp2 = new Node(value);
            if (L == 0) temp3->pRight = temp2;
            else        temp3->pLeft = temp2;
           // temp2->value = temp->value;
        }
       
    }

    void deleteNode(T value) {
        Node* temp = new Node(0);
        Node* temp2 = new Node(0);
        while (temp != NULL) {
            if (value > temp->value)temp = temp->pRight;
            else if (value < temp->value)temp = temp->pLeft;
            else if (value == temp->value) {
                if (temp->pRight == nullptr) {
                    if (temp->pLeft == nullptr) delete(temp);
                    else {
                        temp->value = temp->pLeft->value;
                        delete(temp->pLeft);
                    }
                }
                else {
                    temp2 = temp;
                    temp2 = temp2->pRight;
                    while (temp2->pLeft != nullptr) temp2 = temp2->pLeft;
                    temp->value = temp2->value;

                    delete(temp2);
                }


            }
        }
    }

    string inOrderRec(Node* root) {
        stringstream ss;
        if (root != nullptr) {
            ss << inOrderRec(root->pLeft);
            ss << root->value << " ";
            ss << inOrderRec(root->pRight);
        }
        return ss.str();
    }

    string inOrder() {
        return inOrderRec(this->root);
    }

    class Node
    {
    private:
        T value;
        Node* pLeft, * pRight;
        friend class BinarySearchTree<T>;
    public:
        Node(T value) : value(value), pLeft(NULL), pRight(NULL) {}
        ~Node() {}
    };
};

