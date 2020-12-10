// C++ implementation of delete
// operations on Trie
#include <bits/stdc++.h>
using namespace std;

struct TrieNode *add = NULL;
// trie node
struct TrieNode
{
	void *root;
	bool isEndOfWord;
	short descendants;
	int val;
};

class Node
{
	public:
		int key;
		Node *left;
		Node *right;
		struct TrieNode *address;
		int height;
};

int max(int a, int b);

int height(Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

Node *newNode(int key, struct TrieNode *address)
{
	Node *node = new Node();
	node->key = key;
	node->left = NULL;
	node->right = NULL;
	node->address = address;
	node->height = 1;
	return (node);
}

// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left),
					height(y->right)) +
				1;
	x->height = max(height(x->left),
					height(x->right)) +
				1;

	// Return new root
	return x;
}

// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left),
					height(x->right)) +
				1;
	y->height = max(height(y->left),
					height(y->right)) +
				1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int AVLgetbalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) -
		   height(N->right);
}

Node *InsertAVL(Node *node, int key, TrieNode *address)
{
	/* 1. Perform the normal BST rotation */
	if (node == NULL)
		return (newNode(key, address));

	if (key < node->key)
		node->left = InsertAVL(node->left, key, address);
	else if (key > node->key)
		node->right = InsertAVL(node->right, key, address);
	else // Equal keys not allowed
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						   height(node->right));

	/* 3. Get the balance factor of this 
		ancestor node to check whether 
		this node became unbalanced */
	int balance = AVLgetbalance(node);

	// If this node becomes unbalanced,
	// then there are 4 cases

	// Left Left Case
	if (balance > 1 && key < node->left->key)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && key > node->right->key)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && key > node->left->key)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && key < node->right->key)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

/* Given a non-empty binary search tree, 
return the node with minimum key value 
found in that tree. Note that the entire 
tree does not need to be searched. */
Node *minValueNode(Node *node)
{
	Node *current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;
}

// Recursive function to delete a node
// with given key from subtree with
// given root. It returns root of the
// modified subtree.
Node *AVLDeleteNode(Node *root, int key)
{

	// STEP 1: PERFORM STANDARD BST DELETE
	if (root == NULL)
		return root;

	// If the key to be deleted is smaller
	// than the root's key, then it lies
	// in left subtree
	if (key < root->key)
		root->left = AVLDeleteNode(root->left, key);

	// If the key to be deleted is greater
	// than the root's key, then it lies
	// in right subtree
	else if (key > root->key)
		root->right = AVLDeleteNode(root->right, key);

	// if key is same as root's key, then
	// This is the node to be deleted
	else
	{
		// node with only one child or no child
		if ((root->left == NULL) ||
			(root->right == NULL))
		{
			Node *temp = root->left ? root->left : root->right;

			// No child case
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else			   // One child case
				*root = *temp; // Copy the contents of
							   // the non-empty child
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			Node *temp = minValueNode(root->right);

			// Copy the inorder successor's
			// data to this node
			root->key = temp->key;

			// Delete the inorder successor
			root->right = AVLDeleteNode(root->right,
									 temp->key);
		}
	}

	// If the tree had only one node
	// then return
	if (root == NULL)
		return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + max(height(root->left),
						   height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF
	// THIS NODE (to check whether this
	// node became unbalanced)
	int balance = AVLgetbalance(root);

	// If this node becomes unbalanced,
	// then there are 4 cases

	// Left Left Case
	if (balance > 1 &&
		AVLgetbalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 &&
		AVLgetbalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 &&
		AVLgetbalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 &&
		AVLgetbalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

Node *SearchAVL(Node *root, int value)
{
	if (root != NULL)
	{
		Node *temp = root;
		while (temp != NULL && (temp->left != NULL || temp->right != NULL))
		{
			if (temp->key < value)
				temp = temp->right;
			else if (temp->key > value)
				temp = temp->left;
			else
				return temp;
		}
		if (temp == NULL)
			return NULL;
		if (temp->key == value)
			return temp;
		else
			return NULL;
	}
}
void inorder(struct Node *root, int n, int *current) 
{ 
    if (root != NULL && add == NULL) 
    { 
        if(add != NULL)
        	return;

        inorder(root->left, n, current);
        
        
        if(root->address->isEndOfWord == true)
        {	
        	*current += 1;
        }
        int sum = *current + root->address->descendants;

        
        
        if(sum < n) 
        	*current = sum;
        else if(sum >= n && add == NULL)
        {
        	add = root->address;
        }

        
        if(add != NULL)
			return;

		inorder(root->right, n, current); 
    } 
} 

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(int value)
{
	struct TrieNode *pNode = new TrieNode;
	pNode->isEndOfWord = false;
	pNode->root = NULL;
	pNode->val = value;
	pNode->descendants = 0;

	return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void TrieInsert(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		pCrawl->descendants += 1;
		Node *temp = SearchAVL((Node *)pCrawl->root, index);
		if (!temp)
			pCrawl->root = InsertAVL((Node *)pCrawl->root, index, getNode(index));

		temp = SearchAVL((Node *)pCrawl->root, index);
		pCrawl = temp->address;
	}

	// mark last node as leaf
	pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else
// false
bool TrieSearch(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';
		if (!SearchAVL((Node *)pCrawl->root, index))
			return false;

		Node *temp = SearchAVL((Node *)pCrawl->root, index);
		pCrawl = temp->address;
	}

	return (pCrawl != NULL && pCrawl->isEndOfWord);
}

void TrieSearchN(struct TrieNode *root, int n) 
{ 
	int x = 0, i = 0;
	int *current = &x;
	
	struct TrieNode *pCrawl = root; 
	struct TrieNode *temp = NULL; 

	// cout<<"aaa"<<endl;
	while(*current < n)
	{
		add = NULL;
		// cout<<"ITERATION "<<i<<endl;
		// inorderTraversal((node *)pCrawl->root);
		inorder((Node *)pCrawl->root, n, current);
		pCrawl = add;
		cout<<char('a'+pCrawl->val);
		// cout<<"ccc "<<*current<<endl;
		i++;
	}
	// cout<<"bbb"<<endl;
	cout<<endl;
} 

// Returns true if root has no children, else false
bool isEmpty(TrieNode *root)
{
	Node *ptr = (Node *)root->root;
	if (ptr != NULL)
		return false;
	return true;
}

// Recursive function to delete a key from given Trie
TrieNode *TrieRemove(TrieNode *root, string key, int depth = 0)
{
	// If tree is empty
	if (!root)
		return NULL;

	// If last character of key is being processed
	if (depth == key.size())
	{

		// This node is no more end of word after
		// removal of given key
		if (root->isEndOfWord)
			root->isEndOfWord = false;

		// If given is not prefix of any other word
		if (isEmpty(root))
		{
			delete (root);
			root = NULL;
		}

		return root;
	}

	// If not last character, recur for the child
	// obtained using ASCII value
	int index = key[depth] - 'a';
	Node *ptr = SearchAVL((Node *)root->root, index);
	InsertAVL((Node *)root->root, index, TrieRemove(ptr->address, key, depth + 1));

	// If root does not have any child (its only child got
	// deleted), and it is not end of another word.
	if (isEmpty(root) && root->isEndOfWord == false)
	{
		delete (root);
		root = NULL;
	}
	else
		root->descendants -= 1;

	return root;
}
int random_key_size()
{
	return 4 + rand() % 60;
}
char *tosearch;
// Driver
int main()
{
	// Input keys (use only 'a' through 'z'
	// and lower case)
	// string keys[] = {"the", "a", "there",
	// 				 "answer", "any", "by",
	// 				 "bye", "their", "hero", "heroplane"};
	// int n = sizeof(keys) / sizeof(keys[0]);

	struct TrieNode *root = getNode(-1);

	// for (int i = 0; i < n; i++) 
        
    //     TrieInsert(root, keys[i]); 
  
    // // Search for different keys 
    // TrieSearch(root, "the") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "there") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "a") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "answer") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "any") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "by") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "bye") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "their") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "heroplane") ? cout << "Yes\n" : cout << "No\n"; 
    // TrieSearch(root, "these") ? cout << "Yes\n" : cout << "No\n";
    // // TrieRemove(root, "heroplane"); 
    // TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n"; 
	
	// for(int k = 1; k <= 10 ; k++)
    // 	TrieSearchN(root,k);

	int n=100001, x = 0;
	string keys[n];

	for(int i=0; i<n; i++)
	{
		cin >> keys[i];
		TrieInsert(root,keys[i]);
	}

	for(int i=0; i<n; i++)
	{
		// cin >> keys[i];
		// TrieSearch(root, keys[i]) ? cout << "Yes\n" : cout << "No\n";
		TrieSearchN(root, i);
		// cout<<i<<endl;
	}

	// for(int i=0; i<n; i++)
	// {
	// 	// cin >> keys[i];
	// 	// TrieSearch(root, keys[i]) ? cout << "Yes\n" : cout << "No\n";
	// 	TrieSearch(root, keys[i])? cout << "Yes\n" : cout << "No\n";
	// 	// cout<<i<<endl;
	// }

	// TrieSearch(root, "hello")? cout << "Yes\n" : cout << "No\n";


	return 0;
}