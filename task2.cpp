#include <iostream>
using namespace std;

class lib {
public:
    int ids;

    lib() {
        ids = 0;
    }

    
    lib(int r) {
        ids = r;
    }
};

class Node {
public:
    lib data;
    Node *left;
    Node *right;

    Node(lib d) {
        data = d; 
        left = right = NULL;
    }
};

class bst {
private:
    Node *root;

    Node* insert(Node *node, lib data) {
        if (node == NULL) {
            cout << "Node inserted: " << data.ids << endl;
            return new Node(data);
        } else if (data.ids < node->data.ids) {
            node->left = insert(node->left, data); 
        } else if (data.ids > node->data.ids) { 
            node->right = insert(node->right, data); 
        }
        return node;
    }

    void inorder(Node *node) {
        if (node == NULL) {
            return;
        }
        inorder(node->left);
        cout << node->data.ids << " "; 
        inorder(node->right);
    }
    
    void valsearch(Node *node, int data) {
        if (node == NULL) {
            cout << "Not found" << endl;
            return;
        }
        if (data < node->data.ids) { 
            valsearch(node->left, data);
        } else if (data > node->data.ids) { 
            valsearch(node->right, data);
        } else {
            cout << "Data found: " << data << endl;
        }
    }

public:
    bst() {
        root = NULL;
    }

    void insert(int data) {
        root = insert(root, lib(data));
    }

    void inorder() {
        inorder(root);
        cout << endl;
    }

    void search(int data) {
        valsearch(root, data);
    }
};

int main() {
    int data;
    bst b;

    b.insert(45);
    b.insert(25);
    b.insert(65);
    b.insert(15);
    b.insert(35);
    b.insert(55);

    cout << "Inorder traversal: ";
    b.inorder();

    cout << "Enter data: ";
    cin >> data;
    b.search(data);

    return 0;
}

