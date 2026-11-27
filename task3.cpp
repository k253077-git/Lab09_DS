#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node *left;
    Node *right;

    Node(int data) {
        this->data = data; 
        left = right = NULL;
    }
};

class bst {
private:
    Node *root;
    Node* insert(Node *node, int data) {
        if (node == NULL) {
            cout << "Node inserted: " << data << endl;
            return new Node(data);
        } else if (data < node->data) {
            node->left = insert(node->left, data);
        } else if (data > node->data) {
            node->right = insert(node->right, data);
        }
        return node;
    }

    void inorder(Node *node) {
        if (node == NULL) {
            return;
        }
        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }
    Node* deletenode(Node *node, int data){
        if (node == NULL)
            return NULL;
        if (data < node->data){
        	node->left = deletenode(node->left, data);
		} else if (data > node->data){
		 node->right = deletenode(node->right, data);
		} else{
            if (node->left == NULL &&
                node->right == NULL){
                delete node;
                return NULL;
            }

        return node;
    }
}

public:
    bst() {
        root = NULL;
    }

    void insert(int data) {
        root = insert(root, data);
    }

    void inorder() {
        inorder(root);
        cout << endl;
    }
    void dlt(int data){
    	root=deletenode(root,data);
    	cout<<"delete successfully\n";
	}
};

int main() {
    int data;
    bst b;

    b.insert(50);
    b.insert(30);
    b.insert(70);
    b.insert(20);
    b.insert(40);
    b.insert(60);

    cout << "Inorder traversal: ";
    b.inorder();

    cout << "Enter data: ";
    cin >> data;
    b.dlt(data);

    cout << "inorder traversal: ";
    b.inorder();

    return 0;
}

