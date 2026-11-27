#include <iostream>
#include <algorithm>
using namespace std;

// Global counter for rotations (single = +1, double = +2)
int totalRotations = 0;

class Node {
public:
    int id;
    Node *left;
    Node *right;
    int height;

    Node(int val) {
        id = val;
        left = right = NULL;
        height = 1;
    }
};

class AVLTree {
private:
    Node *root;

    int getHeight(Node *node) {
        return (node == NULL) ? 0 : node->height;
    }

    int getBalance(Node *node) {
        return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node *node) {
        if (node != NULL) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Single Right Rotation 
    Node* rightRotate(Node *y) {
        totalRotations++;
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Single Left Rotation 
    Node* leftRotate(Node *x) {
        totalRotations++;
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* findMin(Node *node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Node* rebalance(Node *node) {
        updateHeight(node);
        int bf = getBalance(node);

        // Left-Heavy Cases
        if (bf > 1) {
            if (getBalance(node->left) >= 0) {
                cout << "  -> LL Rotation at Node " << node->id << " (+1 rotation)" << endl;
                return rightRotate(node);
            } else {
                cout << "  -> LR Rotation at Node " << node->id << " (+2 rotations)" << endl;
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        // Right-Heavy Cases
        if (bf < -1) {
            if (getBalance(node->right) <= 0) {
                cout << "  -> RR Rotation at Node " << node->id << " (+1 rotation)" << endl;
                return leftRotate(node);
            } else {
                cout << "  -> RL Rotation at Node " << node->id << " (+2 rotations)" << endl;
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }

        return node;
    }

    Node* insertHelper(Node *node, int id) {
        if (node == NULL) {
            return new Node(id);
        }

        if (id < node->id) {
            node->left = insertHelper(node->left, id);
        } else if (id > node->id) {
            node->right = insertHelper(node->right, id);
        } else {
            return node;
        }

        return rebalance(node);
    }

    Node* deleteHelper(Node *node, int id) {
        if (node == NULL) {
            return NULL;
        }

        if (id < node->id) {
            node->left = deleteHelper(node->left, id);
        } else if (id > node->id) {
            node->right = deleteHelper(node->right, id);
        } else {
            if (node->left == NULL || node->right == NULL) {
                Node *temp = (node->left != NULL) ? node->left : node->right;
                delete node;
                return temp;
            }
            Node *temp = findMin(node->right);
            node->id = temp->id;
            node->right = deleteHelper(node->right, temp->id);
        }

        return rebalance(node);
    }

    // Inorder traversal to print every node's balance factor
    void printAllBalanceFactors(Node *node) {
        if (node == NULL) return;
        printAllBalanceFactors(node->left);
        cout << node->id << "(BF:" << getBalance(node) << ") ";
        printAllBalanceFactors(node->right);
    }

public:
    AVLTree() {
        root = NULL;
    }

    void insert(int id, bool showStatus = true) {
        if (showStatus) cout << "\n--- Operation: Insert " << id << " ---" << endl;
        root = insertHelper(root, id);
        if (showStatus) displayStatus();
    }

    void remove(int id, bool showStatus = true) {
        if (showStatus) cout << "\n--- Operation: Delete " << id << " ---" << endl;
        root = deleteHelper(root, id);
        if (showStatus) displayStatus();
    }

    //  Display Root, Height, All BFs, and Rotation Count
    void displayStatus() {
        if (root == NULL) {
            cout << "Tree is empty." << endl;
            return;
        }
        cout << "Root Node: " << root->id << endl;
        cout << "Tree Height: " << getHeight(root) << endl;
        cout << "Balance Factors (Inorder): ";
        printAllBalanceFactors(root);
        cout << endl;
        cout << "Total Rotations Performed: " << totalRotations << endl;
    }
};

int main() {
    AVLTree inventory;

    int initialIDs[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    for (int id : initialIDs) {
        inventory.insert(id, false); 
    }
    cout << "=== Initial Inventory Built ===" << endl;
    inventory.displayStatus();

    int step1[] = {5, 15, 22, 27};
    for (int id : step1) {
        inventory.insert(id);
    }

    inventory.remove(80);

    inventory.insert(90);
    inventory.insert(100);

    inventory.remove(70);
    inventory.remove(60);

    return 0;
}

