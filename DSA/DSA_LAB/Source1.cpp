SLinkedList<int> list;
std::cout << "size0: " << list.size() << "\n";

int size = 10;
for (int idx = 0; idx < size; idx++) {
    list.add(idx);
}
std::cout << "size1: " << list.size() << " with input size:" << size << "\n";

std::cout << "Lprint: ";
list.Lprint();
std::cout << " \n";
int values[] = { 10,  15,  2,   6,  4,  7,   40,  8 };
//                0    1    2    3   4   5    6    7
int index[] = { 0,   1,   5,   3,  2,  1,   1,   0 };

/*                10,  15,  2,   6,  4,  7,   40,  8 //initial list
    *                15,  2,   6,   4,  7,  40,  8      //after removeAt 0
    *                15,  6,   4,   7,  40, 8      //after removeAt 1
    *                15,  6,   4,   7,  40   //after removeAt 5
    *                15,  6,   4,   40   //after removeAt 3
    *                15,  6,   40   //after removeAt 2
    *                15,  40   //after removeAt 1
    *                15,   //after removeAt 1
    *                {}  //after removeAt 0
    */
int expvalues[][8] = {
    {15,  2,   6,   4,  7,  40,  8},
    {15,  6,   4,   7,  40, 8},
    {15,  6,   4,   7,  40},
    {15,  6,   4,   40 },
    {15,  6,   40},
    {15,  40},
    {15},
    {}
};

list.clear();
std::cout << "size2 should be 0: " << list.size() << "\n";

std::cout << "is list empty: " << list.empty() << "\n";
for (int idx = 0; idx < 8; idx++)
{
    std::cout << "Lprint in add: ";
    list.Lprint();
    std::cout << " \n";
    std::cout << "value to add is: " << values[idx] << "\n";
    int temm = values[idx];
    std::cout << "wtf temm: " << temm << "\n";


    list.add(2);
}
std::cout << "size3 should be 8: " << list.size() << "\n";

std::cout << "Lprint: ";
list.Lprint();
std::cout << " \n";

//removeAt:
int count = 8;
for (int idx = 0; idx < 8; idx++) {

    std::cout << "Lprint: ";
    list.Lprint();
    std::cout << " \n";


    int idxRemoved = index[idx];
    list.removeAt(idxRemoved);
    assert(list.size() == (count - idx - 1));
    //check expected values
    for (int c = 0; c < (count - idx - 1); c++) {
        int exp = expvalues[idx][c];
        std::cout << "assert: " << exp << " == " << list.get(c) << "\n";
    }
}
