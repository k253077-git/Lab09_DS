#include <iostream>
using namespace std;

class student{
	public:
	int rollnum;
	student(){
		rollnum=0;
	}
	student(int r) {
        rollnum = r;
    }
};
class Node {
public:
    student data;
    Node *left;
    Node *right;

    Node(student d) {
        data = d; 
        left = right = NULL;
    }
};

class bst {
private:
    Node *root;
    Node* insert(Node *node, student s) {
        if (node == NULL) {
            cout << "Node inserted: " << s.rollnum << endl;
            return new Node(s);
        } else if (s.rollnum < node->data.rollnum) {
            node->left = insert(node->left, s);
        } else if (s.rollnum > node->data.rollnum) {
            node->right = insert(node->right, s);
        }
        return node;
    }

    void inorder(Node *node) {
        if (node == NULL) {
            return;
        }
        inorder(node->left);
        cout << node->data.rollnum << " ";
        inorder(node->right);
    }

public:
    bst() {
        root = NULL;
    }

    void insert(student s) {
        root = insert(root, s);
    }

    void inorder() {
        inorder(root);
        cout << endl;
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
    b.insert(data);

    cout << "inorder traversal: ";
    b.inorder();

    return 0;
}

