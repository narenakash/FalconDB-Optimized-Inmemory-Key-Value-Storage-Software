// C++ implementation of search and insert 
// operations on Trie 
#include <bits/stdc++.h> 
#include<stdio.h> 
#include<stdlib.h> 

using namespace std; 

long long int memory = 0;
// trie node 
struct TrieNode 
{ 
	long int val;
	void *root;
	bool isEndOfWord; 
	int descendents;
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
	pNode->descendents=0;

	return pNode; 
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
void TrieInsert(struct TrieNode *root, string key) 
{ 
	struct TrieNode *pCrawl = root; 

	for (int i = 0; i < key.length(); i++) 
	{ 
		int index = key[i] - 'a'; 
		if(!TreeSearch((node *)pCrawl->root,index))
			pCrawl->root = TreeInsert((node *)pCrawl->root,index,getNode(index));
		
		pCrawl->descendents+=1;
		pCrawl = TreeSearch((node *)pCrawl->root,index)->address;

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
		if(!TreeSearch((node *)pCrawl->root,index))
			return false; 
		
		pCrawl = TreeSearch((node *)pCrawl->root,index)->address; 
	} 

	return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 


struct node* TreeSearchN(struct node* root, int *n)
{
	// struct node *temp = root->left;
	// struct node *temp2=root->right;

	if(root==NULL)
		return root;
	TreeSearchN(root->left,n);
	//Middle node work
	int desc = root->address->descendents;
	int v=root->address->val;
	if(root->address->descendents >=*n)
	{
		return(root);
	}
	else
	{
		*n-=root->address->descendents;
	}
	
	TreeSearchN(root->right,n);
} 

TrieNode *TrieSearchN(struct TrieNode *root, int *n) 
{ 
	struct TrieNode *pCrawl = root; 
	int k = pCrawl->val;
	int f=0;
	do
	{ 
		if(pCrawl->isEndOfWord)
		{
			*n-=1;
			if(*n==0)
			{
				f=1;
				break;
			}
				
		}
		
		pCrawl = TreeSearchN((node *)pCrawl->root,n)->address;
		cout<<char(pCrawl->val+'a'); 
	} while(*n>0);

	if(f==1)
		return pCrawl;

	// return (pCrawl != NULL && pCrawl->isEndOfWord); 
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
    if (depth == key.size()) { 
  
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
    if (isEmpty(root) && root->isEndOfWord == false) { 
        delete (root); 
        root = NULL; 
    } 
  
    return root; 
} 

// Driver 
int main() 
{ 
    // Input keys (use only 'a' through 'z' 
    // // and lower case) 
    string keys[] = { "the", "a", "there", 
                      "answer", "any", "by", 
                      "bye", "their", "hero", "heroplane" }; 
	
	// string keys[] = { "a","aab","aac"}; 

	// string keys[]={ "a", "the","an","by","hero","hi","they",};

    int n = sizeof(keys) / sizeof(keys[0]); 
  
    struct TrieNode* root = getNode(-1); 
  
    // Construct trie 
    for (int i = 0; i < n; i++) 
        
        TrieInsert(root, keys[i]); 
  
    // Search for different keys 
    TrieSearch(root, "the") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "there") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "a") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "answer") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "any") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "by") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "bye") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "their") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n"; 
    TrieSearch(root, "heroplane") ? cout << "Yes\n" : cout << "No\n"; 
    
    TrieSearch(root, "these") ? cout << "Yes\n" : cout << "No\n"; 
  
    // TrieRemove(root, "heroplane"); 
    TrieSearch(root, "heroplane") ? cout << "Yes\n" : cout << "No\n"; 

    TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n";
	cout << "----------------------------------------\n\n";

	int k=3;
	struct TrieNode* ans = TrieSearchN(root,&k);
	// cout << "ANSWER : "<<ans->val <<endl;

    return 0; 
} 

// int main() 
// { 
//     int n = 10000000;  
//     struct TrieNode* root = getNode(-1); 
//     string key;
  
//     for (int i = 0; i < n; i++) 
//     {   
//         cin>>key;
//         TrieInsert(root, key);
//         cout<<i<<" "<<memory<<" "<<sizeof(struct node)+8<<" "<<sizeof(struct TrieNode)+8<<endl;
//     } 
//   	cout<<"done\n";
    
//     return 0; 
// } 
