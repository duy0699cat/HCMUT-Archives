
#include "main.h"


struct avl_node
{
	Elem *data= new Elem(NULL,NULL,true);
	struct avl_node* left = NULL;
	struct avl_node* right = NULL;
}*root;

/*
* Class Declaration
*/
class Cache
{ private:
	vector<int> age;

	int p = 0;
	size_t Msize;
public:
	//base tree cache func
	int height(avl_node*);
	int diff(avl_node*);
	avl_node* rr_rotation(avl_node*);
	avl_node* ll_rotation(avl_node*);
	avl_node* lr_rotation(avl_node*);
	avl_node* rl_rotation(avl_node*);
	avl_node* balance(avl_node*);
	avl_node* insert(avl_node* root, int value /*addr*/, Data* cont);
	void display(avl_node*, int);
	void inorder(avl_node*);
	void preorder(avl_node*);
	void postorder(avl_node*);
	avl_node* remove(avl_node* t, int x);
	avl_node* findMin(avl_node*);
	avl_node* findMax(avl_node*);
	Cache()
	{
		root = NULL;
		Msize = MAXSIZE;
	}
	//////////////////src code///////////////////////////
	Cache(int s) {
		root = NULL;
		Msize = s;
	}
	~Cache() {
		//nothing
	}
	Data* read(int addr);
	Elem* put(int addr, Data* cont);
	Elem* write(int addr, Data* cont);
	void print();
	void preOrder();
	void inOrder();
	/// <summary>
	/// support func
	/// </summary>
	/// <param name="t"></param>
	/// <param name="Addr"></param>
	/// <returns></returns>
	avl_node* findWithAddr(avl_node* t, int Addr);
};


/*Height of AVL Tree
*/
int Cache::height(avl_node* temp)
{
	int h = 0;
	if (temp != NULL)
	{
		int l_height = height(temp->left);
		int r_height = height(temp->right);
		int max_height = max(l_height, r_height);
		h = max_height + 1;
	}
	return h;
}

/*
* Height Difference
*/
int Cache::diff(avl_node* temp)
{
	int l_height = height(temp->left);
	int r_height = height(temp->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

/*
* Right- Right Rotation
*/
avl_node* Cache::rr_rotation(avl_node* parent)
{
	avl_node* temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}
/*
* Left- Left Rotation
*/
avl_node* Cache::ll_rotation(avl_node* parent)
{
	avl_node* temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

/*
* Left - Right Rotation
*/
avl_node* Cache::lr_rotation(avl_node* parent)
{
	avl_node* temp;
	temp = parent->left;
	parent->left = rr_rotation(temp);
	return ll_rotation(parent);
}

/*
* Right- Left Rotation
*/
avl_node* Cache::rl_rotation(avl_node* parent)
{
	avl_node* temp;
	temp = parent->right;
	parent->right = ll_rotation(temp);
	return rr_rotation(parent);
}

/*
* Balancing AVL Tree
*/
avl_node* Cache::balance(avl_node* temp)
{
	int bal_factor = diff(temp);
	if (bal_factor > 1)
	{
		if (diff(temp->left) > 0)
			temp = ll_rotation(temp);
		else
			temp = lr_rotation(temp);
	}
	else if (bal_factor < -1)
	{
		if (diff(temp->right) > 0)
			temp = rl_rotation(temp);
		else
			temp = rr_rotation(temp);
	}
	return temp;
}

/*
* Insert Element into the tree
*/
avl_node* Cache::insert(avl_node* root, int value /*addr*/, Data* cont)
{
	if (root == NULL)
	{
		root = new avl_node;
		root->data->addr = value;
		root->data->data = cont;
		root->left = NULL;
		root->right = NULL;
		return root;
	}
	else if (value < root->data->addr)
	{
		root->left = insert(root->left, value, cont);
		root = balance(root);
	}
	else if (value > root->data->addr) //can include =
	{
		root->right = insert(root->right, value, cont);
		root = balance(root);
	}
	return root;
}

/*
* Display AVL Tree
*/
void Cache::display(avl_node* ptr, int level)
{
	int i;
	if (ptr != NULL)
	{

		display(ptr->right, level + 1);
		printf("\n");
		if (ptr == root)
			cout << "Root -> ";
		for (i = 0; i < level && ptr != root; i++)
			cout << "       ";
		cout << ptr->data->addr << "/" << ptr->data->data->getValue()<<"/"<<ptr->data->sync;

		display(ptr->left, level + 1);
	}
}

/*
* Inorder Traversal of AVL Tree
*/
void Cache::inorder(avl_node* tree)
{
	if (tree == NULL)
		return;
	inorder(tree->left);
	tree->data->print();
	inorder(tree->right);
}
/*
* Preorder Traversal of AVL Tree
*/
void Cache::preorder(avl_node* tree)
{
	if (tree == NULL)
		return;
	tree->data->print();
	preorder(tree->left);
	preorder(tree->right);

}
avl_node* Cache::findMin(avl_node* t) {
	if (t == NULL) return NULL;
	else if (t->left == NULL) return t; // if element traverse on max left then return
	else return findMin(t->left); // or recursively traverse max left
}
avl_node* Cache::findMax(avl_node* t) {
	if (t == NULL) return NULL;
	else if (t->right == NULL) return t;
	else return findMax(t->right);
}
/*
* Postorder Traversal of AVL Tree
*/
void Cache::postorder(avl_node* tree)
{
	if (tree == NULL)
		return;
	postorder(tree->left);
	postorder(tree->right);
	tree->data->print();
}
avl_node* Cache::remove(avl_node* t, int x /*addr*/) {
	avl_node* temp;
	// element not found
	if (t == NULL) return NULL;
	// searching element
	else if (x < t->data->addr) t->left = remove(t->left, x);
	else if (x > t->data->addr) t->right = remove(t->right, x);

	// element found 
	// element has 2 children
	else if (t->left && t->right) {
		temp = findMin(t->right);
		t->data = temp->data;
		t->right = remove(t->right, t->data->addr);
	}
	// if element has 1 or 0 child
	else {
		temp = t;
		if (t->left == NULL) t = t->right;
		else if (t->right == NULL) t = t->left;
		delete temp;
	}
	if (t == NULL) return t;
	// check balanced

	t = balance(t);
	return t;

}
//
/////////////////this should be in cache.cpp//////////////////////////////////////////////////////////////
//
Data* Cache::read(int addr) {
	
	if (findWithAddr(root, addr) != NULL) return findWithAddr(root, addr)->data->data;
	else return NULL;
}
Elem* Cache::put(int addr, Data* cont)
{
if (age.size() < Msize)	// cache not full
{
	age.push_back(addr);	// store this to caculate mem age
	root = insert(root, addr, cont);
	findWithAddr(root, addr)->data->sync = true;
	return NULL;
}
else {	// cache full
	int tempAddr = age[0]; //oldest mem
	Elem* tempData = findWithAddr(root, tempAddr)->data;
	age.erase(age.begin()); // FIFO, clear oldest mem with vector func
	root = remove(root, tempAddr);

	age.push_back(addr); //add new addr to mem age vct
	root = insert(root, addr, cont); //add mem to cache
	findWithAddr(root, addr)->data->sync = true;

	return tempData; //elem data that got removed
}
///func should not reach here
}
Elem* Cache::write(int addr, Data* cont) {
	//search addr
	avl_node* temp = findWithAddr(root, addr);
	//addr found
	if ( temp != NULL) {
		delete temp->data->data;
		temp->data->data = cont;
		temp->data->sync = false;
		////////refresh age?
		//vector<int>::iterator position = std::find(age.begin(), age.end(), addr);
		//if (position != age.end()) // == myVector.end() means the element was not found
		//	age.erase(position);

		//age.push_back(addr);
		/////////////////////////
		return NULL;
	}
	else {//addr not found
		Elem* temp2 = this->put(addr, cont);
		findWithAddr(root, addr)->data->sync = false;
		return temp2;
	}
}
void Cache::print() {
	for (int i = age.size() - 1; i >= 0; i--)
	{
		avl_node* foundd = findWithAddr(root, age[i]);

		foundd->data->print();
	}

}
void Cache::preOrder() {
	preorder(root);
}
void Cache::inOrder() {
	inorder(root);
}
///////////////////////////////////////////////////////////////////////////////////////
/////////////addition support func:
////////////////////////////////////////////////
avl_node* Cache::findWithAddr(avl_node* t, int Addr) {
	if (t == NULL) return NULL;
	else if (Addr == t->data->addr)
	{
		return t;
	}
	else if (Addr > t->data->addr) //can include = ?
	{

		return findWithAddr(t->right, Addr);
	}
	return findWithAddr(t->left, Addr);
}