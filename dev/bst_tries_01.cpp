// C++ implementation of search and insert 
// operations on Trie 
#include <bits/stdc++.h> 
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 

using namespace std; 

long long int memory = 0;
char *remn;
bool rem=false;
int point;
// trie node 
struct TrieNode 
{ 
	long int val;
	void *root;
	bool isEndOfWord;
	short descendants; 
	string word;
}; 

struct node 
{ 
	struct node *left, *right;
	long int key;
	struct TrieNode *address; 
}; 

// A utility function to create a new BST node 
struct node *newNode(int item, struct TrieNode *address) 
{ 
	struct node *temp = (struct node *)malloc(sizeof(struct node)); 
	memory += sizeof(struct node) + 8;
	temp->key = item; 
	temp->address = address;
	temp->left = temp->right = NULL; 
	return temp; 
}

// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(int value) 
{ 
	struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 
	memory += sizeof(struct TrieNode) + 8;
	pNode->isEndOfWord = false; 
	pNode->root = NULL;
	pNode->val = value;

	return pNode; 
} 

// Inorder Traversal of Tree
struct TrieNode* inorder(struct node *root, int n, int *current, struct TrieNode *temp = NULL) 
{ 
    if (root != NULL) 
    { 
        if(temp != NULL)
        	return temp;

        temp = inorder(root->left, n, current ,temp);
        
        
        if(root->address->isEndOfWord == true)
        {	
        	*current+=1;
        }
        int sum = *current + root->address->descendants;

        
        
        if(sum < n) 
        	*current = sum;
        else if(sum >= n && temp == NULL)
        {
        	temp = root->address;
        }

        
        if(temp != NULL)
        	return temp;
		
		temp = inorder(root->right, n, current ,temp); 
    } 
} 

void inorderTraversal(struct node *root) 
{ 
    if (root != NULL) 
    { 

        inorderTraversal(root->left);
		cout<<char(root->key + 'a')<<"("<<root->address->descendants<<")";
		inorderTraversal(root->right); 
    } 
    cout<<endl;
} 

/* A utility function to insert a new node with given key in BST */
struct node* TreeInsert(struct node* node, int key, struct TrieNode* address) 
{ 
	/* If the tree is empty, return a new node */
	if (node == NULL) return newNode(key,address); 

	/* Otherwise, recur down the tree */
	if (key < node->key) 
		node->left = TreeInsert(node->left, key, address); 
	else if(key > node->key)
		node->right = TreeInsert(node->right, key, address); 

	/* return the (unchanged) node pointer */
	return node; 
} 

/* Given a non-empty binary search tree, return the node with minimum 
key value found in that tree. Note that the entire tree does not 
need to be searched. */
struct node* minValueNode(struct node* node) 
{ 
	struct node* current = node; 

	/* loop down to find the leftmost leaf */
	while (current && current->left != NULL) 
		current = current->left; 

	return current; 
}

struct node* TreeSearch(struct node* root, int value)
{
	struct node *temp = root;
	while(temp != NULL)
	{
		if(temp->key > value)
			temp = temp->left;
		else if(temp->key < value)
			temp = temp->right;
		else
			return temp;
	}
	return NULL;
} 

/* Given a binary search tree and a key, this function deletes the key 
and returns the new root */
struct node* TreeDeleteNode(struct node* root, int key) 
{ 
	// base case 
	if (root == NULL) return root; 

	// If the key to be deleted is smaller than the root's key, 
	// then it lies in left subtree 
	if (key < root->key) 
		root->left = TreeDeleteNode(root->left, key); 

	// If the key to be deleted is greater than the root's key, 
	// then it lies in right subtree 
	else if (key > root->key) 
		root->right = TreeDeleteNode(root->right, key); 

	// if key is same as root's key, then This is the node 
	// to be deleted 
	else
	{ 
		// node with only one child or no child 
		if (root->left == NULL) 
		{ 
			struct node *temp = root->right; 
			free(root); 
			return temp; 
		} 
		else if (root->right == NULL) 
		{ 
			struct node *temp = root->left; 
			free(root); 
			return temp; 
		} 

		// node with two children: Get the inorder successor (smallest 
		// in the right subtree) 
		struct node* temp = minValueNode(root->right); 

		// Copy the inorder successor's content to this node 
		root->key = temp->key; 

		// Delete the inorder successor 
		root->right = TreeDeleteNode(root->right, temp->key); 
	} 
	return root; 
} 


// If not present, inserts key into trie 
// If the key is prefix of trie node, just 
// marks leaf node 
void TrieInsert(struct TrieNode *root, string key, string word) 
{ 
	struct TrieNode *pCrawl = root; 

	for (int i = 0; i < key.length(); i++) 
	{ 
		int index = key[i] - 'a'; 
		pCrawl->descendants += 1;
		if(!TreeSearch((node *)pCrawl->root,index))
			pCrawl->root = TreeInsert((node *)pCrawl->root,index,getNode(index));

		pCrawl = TreeSearch((node *)pCrawl->root,index)->address;
	} 

	// mark last node as leaf 
	pCrawl->isEndOfWord = true;
	pCrawl->word=word;
} 

// Returns true if key presents in trie, else 
// false 
bool TrieSearch(struct TrieNode *root, string key) 
{ 
	struct TrieNode *pCrawl = root; 

	for (int i = 0; i < key.length(); i++) 
	{ 
		int index = key[i] - 'a'; 
		if(!TreeSearch((node *)pCrawl->root,index))
			return false; 
		
		pCrawl = TreeSearch((node *)pCrawl->root,index)->address; 
	} 

	return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 

char * TrieSearchN(struct TrieNode *root, int n) 
{ 
	int x = 0, i = 0;
	int *current = &x;
	
	struct TrieNode *pCrawl = root; 
	struct TrieNode *temp = NULL; 

	// cout<<"aaa"<<endl;
	while(*current < n)
	{
		temp = NULL;
		// cout<<"ITERATION "<<i<<endl;
		// inorderTraversal((node *)pCrawl->root);
		temp = inorder((node *)pCrawl->root, n, current, temp);
		pCrawl = temp;
		if(rem)
		{
			// *(remn+point++)=char('a'+pCrawl->val);
			*(remn+point++)=char('a'+pCrawl->val);

		}
		// cout<<char('a'+pCrawl->val);
		// cout<<"ccc "<<*current<<endl;
		i++;
	}
	// cout<<"bbb"<<endl;
	// cout<<endl;
	return remn;
} 

// Returns true if root has no children, else false 
bool isEmpty(TrieNode* root) 
{ 
	node *ptr = (node *)root->root;
	if (ptr != NULL)
		return false;
	return true;
} 
  
// Recursive function to delete a key from given Trie 
TrieNode* TrieRemove(TrieNode* root, string key, int depth = 0) 
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
        if (isEmpty(root)) { 
            delete (root); 
            root = NULL; 
        } 
  
        return root; 
    } 
  
    // If not last character, recur for the child 
    // obtained using ASCII value 
    int index = key[depth] - 'a'; 
    TreeInsert((node *)root->root, index, TrieRemove(TreeSearch((node *)root->root,index)->address, key, depth + 1));

    // If root does not have any child (its only child got  
    // deleted), and it is not end of another word. 
    if (isEmpty(root) && root->isEndOfWord == false) 
    { 
        delete (root); 
        root = NULL; 
    } 
    else
    	root->descendants-=1;
  
    return root; 
} 


TrieNode* TrieRemoveN(TrieNode* root, int n)
{
	point = 0;
	rem = true; 
	remn = (char *)malloc(sizeof(char)*64);
	TrieSearchN(root,n);
	rem = false;
	TrieRemove(root,remn);
	free(remn);
}
 


// Driver 
int main() 
{ 
    // Input keys (use only 'a' through 'z' 
    // and lower case) 
    // string keys[] = { "the", "a", "there", 
    //                   "answer", "any", "by", 
    //                   "bye", "their", "hero", "heroplane" }; 
    // int n = sizeof(keys) / sizeof(keys[0]); 
  	srand (time(NULL));
    int n = 1000;
    struct TrieNode* root = getNode(-1); 
  
    // Construct trie 
    for (int i = 0; i < n; i++) 
    {	
    	string key;
    	cout<<"Getting value "<<i<<endl;
    	cin>>key;
    	cout<<"Entering value "<<i<<" "<<key<<endl;
       	TrieInsert(root, key, string("Mallika"));
    	cout<<"Entered value "<<i<<endl;
    } 
  	
  	cout<<"INSERTION DONE"<<endl;
  	cout<<"BEGINNING CHECK"<<endl;

    for(int k = 1; k <= n ; k++)
    {
    	char temp1[64], temp2[64];
    	strcmp(temp1,TrieSearchN(root,k));
    	if(k>2)
    	{	
    		if(strcmp(temp1,temp2)<0)
    			cout<<"failed at iteration "<<k<<endl;
    	}
    	strcmp(temp2,temp1);
    }

    cout<<"CHECK DONE"<<endl;
    int del = rand() % n;
    cout<<"DELETING "<<del<<" VALUES"<<endl;
    for(int k = 0 ; k<del ; k++)
    {
    	int val = rand()%n;
    	TrieRemoveN(root,val);
    	n--;
    }

    cout<<"BEGINNING CHECK"<<endl;

    for(int k = 1; k <= n ; k++)
    {
    	char temp1[64], temp2[64];
    	strcmp(temp1,TrieSearchN(root,k));
    	if(k>2)
    	{	
    		if(strcmp(temp1,temp2)<0)
    			cout<<"failed at iteration "<<k<<endl;
    	}
    	strcmp(temp2,temp1);
    }

    cout<<"CHECK DONE"<<endl;




    // Search for different keys 
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
    // TrieRemove(root, "heroplane"); 
    // TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n"; 


    for(int k = 1; k <= n ; k++)
    	TrieSearchN(root,k);

	// TrieRemoveN(root,7);
    // TrieSearch(root, "heroplane") ? cout << "Yes\n" : cout << "No\n"; 

    return 0; 
} 

