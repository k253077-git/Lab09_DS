#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

class Node {
public:
    int id;
    Node *left;
    Node *right;
    int height;

    Node(int val) {
        id = val;
        left = right = NULL;
        height = 1; // New node starts at height 1
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

    // Right Rotation (used for LL case)
    Node* rightRotate(Node *y) {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    // Left Rotation (used for RR case)
    Node* leftRotate(Node *x) {
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

    Node* rebalance(Node *node, string operation) {
        updateHeight(node);
        int bf = getBalance(node);

        cout << "  Node " << node->id << " -> Balance Factor: " << bf << endl;

        // Left-Heavy Cases (LL or LR)
        if (bf > 1) {
            if (getBalance(node->left) >= 0) {
                cout << "  [!] Imbalance at " << node->id << " during " << operation 
                     << " -> Performing LL Rotation (Right Rotate)" << endl;
                return rightRotate(node);
            } else {
                cout << "  [!] Imbalance at " << node->id << " during " << operation 
                     << " -> Performing LR Rotation (Left-Right Rotate)" << endl;
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        // Right-Heavy Cases (RR or RL)
        if (bf < -1) {
            if (getBalance(node->right) <= 0) {
                cout << "  [!] Imbalance at " << node->id << " during " << operation 
                     << " -> Performing RR Rotation (Left Rotate)" << endl;
                return leftRotate(node);
            } else {
                cout << "  [!] Imbalance at " << node->id << " during " << operation 
                     << " -> Performing RL Rotation (Right-Left Rotate)" << endl;
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
            return node; // Duplicate IDs not allowed
        }

        return rebalance(node, "Insertion");
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

        return rebalance(node, "Deletion");
    }

    void inorderHelper(Node *node) {
        if (node == NULL) return;
        inorderHelper(node->left);
        cout << node->id << " ";
        inorderHelper(node->right);
    }

public:
    AVLTree() {
        root = NULL;
    }

    void insert(int id) {
        cout << "\nInserting Patient ID: " << id << endl;
        root = insertHelper(root, id);
    }

    void remove(int id) {
        cout << "\nDeleting Patient ID: " << id << endl;
        root = deleteHelper(root, id);
    }

    void displayInorder() {
        cout << "Inorder Traversal (Sorted): ";
        inorderHelper(root);
        cout << endl;
    }

    void displayLevelOrder() {
        cout << "Level-Order Traversal: ";
        if (root == NULL) {
            cout << "Empty" << endl;
            return;
        }

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node *curr = q.front();
            q.pop();
            cout << curr->id << " ";

            if (curr->left != NULL) q.push(curr->left);
            if (curr->right != NULL) q.push(curr->right);
        }
        cout << endl;
    }
};

int main() {
    AVLTree hospitalRecords;

    // 1 & 2. Insert patient IDs and check balance/rotations after each
    int insertIDs[] = {45, 25, 65, 15, 35, 55, 75, 10, 20, 30, 40, 5};
    for (int id : insertIDs) {
        hospitalRecords.insert(id);
    }
	cout<<endl;
    cout << "Tree State After All Insertions:\n";
    hospitalRecords.displayInorder();
    hospitalRecords.displayLevelOrder();
    cout << "========================================\n";

    // 3 & 4. Sequentially delete 75, 65, 55 and rebalance if required
    int deleteIDs[] = {75, 65, 55};
    for (int id : deleteIDs) {
        hospitalRecords.remove(id);
    }

    // 5. Display final AVL Tree
    cout<<endl;
    cout << "Final AVL Tree After Deletions:\n";
    hospitalRecords.displayInorder();
    hospitalRecords.displayLevelOrder();
    

    return 0;
}

