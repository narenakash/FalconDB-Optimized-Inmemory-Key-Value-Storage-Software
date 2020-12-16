// C++ implementation of search and insert operations on Trie 

#include <bits/stdc++.h> 
#include<stdio.h> 
#include<stdlib.h> 

using namespace std;

long long int memory = 0;

// Trie node
struct TrieNode { 
	long int val;
	void *root;
	bool isEndOfWord; 
}; 

struct Node {
    struct Node *next;
    long int key;
    struct TrieNode *address;
};

typedef struct Node Node;
// A utility function to create a new linked list node 
Node *newNode(int item, struct TrieNode *address) {

    Node *temp = (Node *)malloc(sizeof(Node)); 
    memory += sizeof(Node) + 8; //Size of Node + Pointer size
    temp->key = item;
    temp->address = address;
    temp->next = NULL;
    return temp;
}

// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(int value) { 
	struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode)); 
	memory += sizeof(struct TrieNode) + 8; //Size of Node + Pointer size
	pNode->isEndOfWord = false; 
	pNode->root = NULL;
	pNode->val = value;

	return pNode; 
} 


/* A utility function to insert a new node with given key in LL*/
Node* LLInsert(Node* node, int key, struct TrieNode* address) { 
	/* If the list is empty, return a new node */
	if (node == NULL) return newNode(key,address); 

	/* Otherwise, go through the list */
	Node* run = node;
    while(run->next != NULL) {
        run = run->next;
    }

    run->next = newNode(key, address);

	/* Return the (unchanged) node pointer */
	return node; 
} 

Node* LLSearch(Node* start, int key) {
     
    Node* run = start;

    // If not empty
    while(run != NULL) {
        
        if(run->key == key) {
            cout<<(char) (run->key + 'a') << " Key:" << (char)(key + 'a')<< "\n";
            return run;
        }
        run = run->next;
    }

    // If not found or empty list
    return NULL;
}

// If not present, inserts key into trie 
// If the key is prefix of trie node, just  marks leaf node 
void TrieInsert(struct TrieNode *root, string key) { 
	struct TrieNode *pCrawl = root; 

	for (int i = 0; i < key.length(); i++) { 
		int index = key[i] - 'a'; 
		if(!LLSearch((Node *)pCrawl->root,index))
			pCrawl->root = LLInsert((Node *)pCrawl->root,index,getNode(index));

		pCrawl = LLSearch((Node *)pCrawl->root,index)->address;
	} 

	// mark last node as leaf 
	pCrawl->isEndOfWord = true; 
} 

// Returns true if key presents in trie, else 
// false 
bool TrieSearch(struct TrieNode *root, string key) { 
	struct TrieNode *pCrawl = root; 

	for (int i = 0; i < key.length(); i++) { 
		int index = key[i] - 'a'; 
        cout << "KEY: " << index << "\n";
		if(!LLSearch((Node *)pCrawl->root,index)) {
			return false; 
        }
        		
		pCrawl = LLSearch((Node *)pCrawl->root,index)->address; 
	} 

    return (pCrawl != NULL && pCrawl->isEndOfWord);
}

// Returns true if root has no children, else false 
bool isEmpty(TrieNode* root) 
{ 
	Node *ptr = (Node *)root->root;
	if (ptr != NULL && ptr->next == NULL)
		return false;
	return true;
} 
  
void PrintList(Node* root) {
    Node* run = root;
    while(run != NULL) {
        cout << run->key << " ";
    }
}
// Recursive function to delete a key from given Trie 
TrieNode* TrieRemove(TrieNode* root, string key, int depth = 0) 
{ 
    // If tree is empty 
    if (!root) 
        return NULL; 
  
    // If last character of key is being processed 
    if (depth == key.size()) { 
        cout << "Depth: "<< depth << '\n';
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
    LLInsert((Node *)root->root, index, TrieRemove(LLSearch((Node *)root->root,index)->address, key, depth + 1));

    // If root does not have any child (its only child got  
    // deleted), and it is not end of another word. 
    if (isEmpty(root) && root->isEndOfWord == false) { 
        delete (root); 
        root = NULL; 
    } 
  
    return root; 
} 



// int main() { 
//     int n = 10000000;  
//     struct TrieNode* root = getNode(-1); 
//     string key;
  
//     for (int i = 0; i < n; i++) {   
//         cin>>key;
//         TrieInsert(root, key);
//         cout<< i <<" "<< memory <<" "<<sizeof(Node)+8<<" "<<sizeof(struct TrieNode)+8<<endl;
//     } 
//   	cout<<"done\n";
    
//     return 0; 
// } 


int main() 
{ 
    // Input keys (use only 'a' through 'z' 
    // and lower case) 
    string keys[] = { "the", "a", "there", 
                      "answer", "any", "by", 
                      "bye", "their", "hero", "heroplane" }; 
    int n = sizeof(keys) / sizeof(keys[0]); 
  
    struct TrieNode* root = getNode(-1); 
  
    // Construct trie 
    for (int i = 0; i < n; i++) {        
        TrieInsert(root, keys[i]);
    } 
  
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
  
    TrieRemove(root, "heroplane"); 
    TrieSearch(root, "hero") ? cout << "Yes\n" : cout << "No\n"; 

    return 0; 
} 
