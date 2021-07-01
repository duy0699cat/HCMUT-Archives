#include "Cache.h"
Data* getData(string s) {
	stringstream ss(s);
	int idata;
	float fdata;
	if (ss >> idata)
		return new Int(idata);
	else if (ss >> fdata)
		return new Float(fdata);
	else if (s.compare("true") || s.compare("false"))
		return new Bool(s.compare("true"));
	else {
		s.resize(s.size() - 1);
		return new Address(stoi(s));
	}

}
int main()
{
	int choice, item;
	string tstr;
	Cache avl;
	while (1)
	{
		cout << "\n---------------------" << endl;
		cout << "AVL Tree Implementation" << endl;
		cout << "\n---------------------" << endl;
		cout << "1.Insert Element into the tree" << endl;
		cout << "2.Delete Element into the tree" << endl;
		cout << "3.Display Balanced AVL Tree" << endl;
		cout << "4.InOrder traversal" << endl;
		cout << "5.PreOrder traversal" << endl;
		cout << "6.PostOrder traversal" << endl;
		cout << "8.Find with addr" << endl;
		cout << "9.Write element" << endl;
		cout << "10.Print task" << endl;
		cout << "7.Exit" << endl;
		cout << "Enter your Choice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cout << "Enter value to be inserted: ";
			cin >> item>>tstr;
			//root = avl.insert(root, item, getData(tstr));
			avl.put(item, getData(tstr));
			break;
		case 2:
			cout << "Enter value to be deleted: ";
			cin >> item;
			root = avl.remove(root, item);
			break;
		case 3:
			if (root == NULL)
			{
				cout << "Tree is Empty" << endl;
				continue;
			}
			cout << "Balanced AVL Tree:" << endl;
			avl.display(root, 1);
			break;
		case 4:
			cout << "Inorder Traversal:" << endl;
			avl.inOrder();
			cout << endl;
			break;
		case 5:
			cout << "Preorder Traversal:" << endl;
			avl.preOrder();
			cout << endl;
			break;
		case 6:
			cout << "Postorder Traversal:" << endl;
			avl.postorder(root);
			cout << endl;
			break;
		case 8:
			cout << "Search with input addr:" << endl;
			cin >> item;
			;
			if(avl.read(item)!=NULL) {
				cout << "Data is: ";
				cout << avl.read(item)->getValue() << endl;
			}
			else cout << "cannot find \n ";
			break;
		case 9:
			cout << "Enter elem to be write: ";
			cin >> item >> tstr;
			
			avl.write(item, getData(tstr));
			break;
		case 10:
			cout << "Print task: ";
			avl.print();
			cout << endl;
			break;
		case 7:
			exit(1);
			break;
		default:
			cout << "Wrong Choice" << endl;
		}
	}
	return 0;
}

