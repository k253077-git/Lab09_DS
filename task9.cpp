#include <iostream>
using namespace std;

class Node {
public:
    int ip;
    Node *left;
    Node *right;
    int height;

    Node(int val) {
        ip = val;
        left = right = NULL;
        height = 1;
    }
};

class RouterAVL {
private:
    Node *root;

    int maxVal(int a, int b) const {
        return (a > b) ? a : b;
    }

    int getHeight(const Node *node) const {
        return (node == NULL) ? 0 : node->height;
    }

    int getBalance(const Node *node) const {
        return (node == NULL) ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    void updateHeight(Node *node) {
        if (node != NULL) {
            node->height = 1 + maxVal(getHeight(node->left), getHeight(node->right));
        }
    }

    Node* rightRotate(Node *y) {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

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

    Node* rebalance(Node *node) {
        updateHeight(node);
        int bf = getBalance(node);

        if (bf > 1) {
            if (getBalance(node->left) >= 0) {
                return rightRotate(node);
            } else {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }

        if (bf < -1) {
            if (getBalance(node->right) <= 0) {
                return leftRotate(node);
            } else {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }

        return node;
    }

    Node* insertHelper(Node *node, int ip) {
        if (node == NULL) {
            return new Node(ip);
        }

        if (ip < node->ip) {
            node->left = insertHelper(node->left, ip);
        } else if (ip > node->ip) {
            node->right = insertHelper(node->right, ip);
        } else {
            return node;
        }

        return rebalance(node);
    }

    Node* deleteHelper(Node *node, int ip) {
        if (node == NULL) {
            return NULL;
        }

        if (ip < node->ip) {
            node->left = deleteHelper(node->left, ip);
        } else if (ip > node->ip) {
            node->right = deleteHelper(node->right, ip);
        } else {
            if (node->left == NULL || node->right == NULL) {
                Node *temp = (node->left != NULL) ? node->left : node->right;
                delete node;
                return temp;
            }
            Node *temp = findMin(node->right);
            node->ip = temp->ip;
            node->right = deleteHelper(node->right, temp->ip);
        }

        return rebalance(node);
    }

    void inorderHelper(const Node *node) const {
        if (node == NULL) return;
        inorderHelper(node->left);
        cout << node->ip << " ";
        inorderHelper(node->right);
    }

    void printGivenLevel(const Node *node, int level) const {
        if (node == NULL) return;
        if (level == 1) {
            cout << node->ip << " ";
        } else if (level > 1) {
            printGivenLevel(node->left, level - 1);
            printGivenLevel(node->right, level - 1);
        }
    }

    void collectLeaves(const Node *node, int arr[], int &count) const {
        if (node == NULL) return;
        collectLeaves(node->left, arr, count);
        if (node->left == NULL && node->right == NULL) {
            arr[count++] = node->ip;
        }
        collectLeaves(node->right, arr, count);
    }

public:
    RouterAVL() {
        root = NULL;
    }

    Node* getRoot() {
        return root;
    }

    void insert(int ip) {
        root = insertHelper(root, ip);
    }

    void remove(int ip) {
        root = deleteHelper(root, ip);
    }

    // Requirement 2: Search function
    Node* search(Node* curr, int key) {
        if (curr == NULL || curr->ip == key) {
            return curr;
        }
        if (key < curr->ip) {
            return search(curr->left, key);
        }
        return search(curr->right, key);
    }

    void deleteRoot() {
        if (root != NULL) {
            cout << "Deleting root node (" << root->ip << ")..." << endl;
            remove(root->ip);
        }
    }

    int getLeafNodes(int leaves[]) const {
        int count = 0;
        collectLeaves(root, leaves, count);
        return count;
    }

    void deleteLeavesGreaterThan60() {
        int leaves[100];
        int count = getLeafNodes(leaves);

        for (int i = 0; i < count; i++) {
            if (leaves[i] > 60) {
                cout << "Deleting leaf node > 60: " << leaves[i] << endl;
                remove(leaves[i]); // Rebalances after every deletion
            }
        }
    }
    
    void displayTree() const {
        cout << "Inorder Traversal (Sorted): ";
        inorderHelper(root);
        cout << endl;

        cout << "Level-Order Traversal: ";
        int h = getHeight(root);
        if (h == 0) {
            cout << "Empty";
        } else {
            for (int i = 1; i <= h; i++) {
                printGivenLevel(root, i);
            }
        }
        cout << endl;
        cout << "Tree Height: " << h << endl;
    }
};

int main() {
    RouterAVL router;

    int initialIPs[] = {40, 20, 60, 10, 30, 50, 70, 25, 35};
    int initialSize = sizeof(initialIPs) / sizeof(initialIPs[0]);
    for (int i = 0; i < initialSize; i++) {
        router.insert(initialIPs[i]);
    }
    router.displayTree();

    cout << "\n=== Step 3: Search Operations ===" << endl;
    int searchKeys[] = {25, 55, 70};
    for (int i = 0; i < 3; i++) {
        Node *result = router.search(router.getRoot(), searchKeys[i]);
        if (result != NULL) {
            cout << "Search " << searchKeys[i] << ": Found (Node IP = " << result->ip << ")" << endl;
        } else {
            cout << "Search " << searchKeys[i] << ": Not Found" << endl;
        }
    }

    cout << "\n=== Step 4: Delete Root Node ===" << endl;
    router.deleteRoot();
    router.displayTree();

    cout << "\n=== Step 5: Insert 15, 5, 65, 75, 80 ===" << endl;
    int newIPs[] = {15, 5, 65, 75, 80};
    int newSize = sizeof(newIPs) / sizeof(newIPs[0]);
    for (int i = 0; i < newSize; i++) {
        router.insert(newIPs[i]);
    }
    router.displayTree();

    cout << "\n=== Step 6 & 7: Delete Leaf Nodes > 60 ===" << endl;
    router.deleteLeavesGreaterThan60();

    cout << "\n=== Step 8: Final AVL Tree ===" << endl;
    router.displayTree();

    cout << "\n=== Challenge: Remaining Leaf Nodes (Ascending) ===" << endl;
    int remainingLeaves[100];
    int leafCount = router.getLeafNodes(remainingLeaves);
    for (int i = 0; i < leafCount; i++) {
        cout << remainingLeaves[i] << " ";
    }
    cout << endl;

    return 0;
}

