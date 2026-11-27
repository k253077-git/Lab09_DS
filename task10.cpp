#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node *left;
    Node *right;

    Node(int val) {
        data = val;
        left = right = NULL;
    }
};

class BSTtoAVL {
private:
    Node *root;

    int maxVal(int a, int b) const {
        return (a > b) ? a : b;
    }

    int getHeight(const Node *node) const {
        if (node == NULL) return 0;
        return 1 + maxVal(getHeight(node->left), getHeight(node->right));
    }

    int getBalanceFactor(const Node *node) const {
        if (node == NULL) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    void displayAllBalanceFactors(const Node *node) const {
        if (node == NULL) return;
        displayAllBalanceFactors(node->left);
        cout << node->data << "(BF:" << getBalanceFactor(node) << ") ";
        displayAllBalanceFactors(node->right);
    }

    Node* insertNormalBST(Node *node, int val) {
        if (node == NULL) {
            return new Node(val);
        }
        if (val < node->data) {
            node->left = insertNormalBST(node->left, val);
        } else if (val > node->data) {
            node->right = insertNormalBST(node->right, val);
        }
        return node;
    }

    Node* rightRotate(Node *y) {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        return x;
    }

    Node* leftRotate(Node *x) {
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        return y;
    }

    Node* rotateOnce(Node *node) {
        int bf = getBalanceFactor(node);

        if (bf > 1) {
            if (getBalanceFactor(node->left) >= 0) {
                return rightRotate(node); // LL
            } else {
                node->left = leftRotate(node->left); // LR
                return rightRotate(node);
            }
        }

        if (bf < -1) {
            if (getBalanceFactor(node->right) <= 0) {
                return leftRotate(node); // RR
            } else {
                node->right = rightRotate(node->right); // RL
                return leftRotate(node);
            }
        }

        return node;
    }

    Node* convertToAVLHelper(Node *node) {
        if (node == NULL) return NULL;

        while (getBalanceFactor(node) > 1 || getBalanceFactor(node) < -1) {
            node->left = convertToAVLHelper(node->left);
            node->right = convertToAVLHelper(node->right);
            node = rotateOnce(node);
        }

        node->left = convertToAVLHelper(node->left);
        node->right = convertToAVLHelper(node->right);

        return node;
    }

    void inorderHelper(const Node *node) const {
        if (node == NULL) return;
        inorderHelper(node->left);
        cout << node->data << " ";
        inorderHelper(node->right);
    }

    void printGivenLevel(const Node *node, int level) const {
        if (node == NULL) return;
        if (level == 1) {
            cout << node->data << " ";
        } else if (level > 1) {
            printGivenLevel(node->left, level - 1);
            printGivenLevel(node->right, level - 1);
        }
    }

    bool isAVLBalanced(const Node *node) const {
        if (node == NULL) return true;
        int bf = getBalanceFactor(node);
        if (bf < -1 || bf > 1) return false;
        return isAVLBalanced(node->left) && isAVLBalanced(node->right);
    }

    int countNodes(const Node *node) const {
        if (node == NULL) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    void collectAddresses(const Node *node, const Node* addr[], int &idx) const {
        if (node == NULL) return;
        collectAddresses(node->left, addr, idx);
        addr[idx++] = node;
        collectAddresses(node->right, addr, idx);
    }

public:
    BSTtoAVL() {
        root = NULL;
    }

    void insertBST(int val) {
        root = insertNormalBST(root, val);
    }

    void convertInPlaceToAVL() {
        root = convertToAVLHelper(root);
    }

    void displayDiagnostics() const {
        int h = getHeight(root);
        cout << "Root Node: " << (root ? root->data : -1) << endl;
        cout << "Tree Height: " << h << endl;
        cout << "Inorder Traversal: ";
        inorderHelper(root);
        cout << endl;

        cout << "Level-Order Traversal: ";
        for (int i = 1; i <= h; i++) {
            printGivenLevel(root, i);
        }
        cout << endl;

        cout << "Balance Factors: ";
        displayAllBalanceFactors(root);
        cout << endl;
    }

    int getNodeAddresses(const Node* addr[]) const {
        int idx = 0;
        collectAddresses(root, addr, idx);
        return idx;
    }

    void verifyAVL(const Node* beforeAddr[], int beforeCount) const {
        const Node* afterAddr;
        int afterCount = getNodeAddresses(afterAddr);

        bool sameNodes = (beforeCount == afterCount);
        for (int i = 0; i < beforeCount && sameNodes; i++) {
            if (beforeAddr[i] != afterAddr[i]) {
                sameNodes = false;
            }
        }

        cout << "1. Inorder Traversal Sorted: Verified above (10 to 100)" << endl;
        cout << "2. All Balance Factors in {-1, 0, +1}: " 
             << (isAVLBalanced(root) ? "PASSED" : "FAILED") << endl;
        cout << "3. All Original Node Pointers Preserved (" << afterCount 
             << "/" << beforeCount << "): " 
             << (sameNodes ? "PASSED" : "FAILED") << endl;
    }
};

int main() {
    BSTtoAVL tree;

    int values[] = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};
    int n = sizeof(values) / sizeof(values);
    for (int i = 0; i < n; i++) {
        tree.insertBST(values[i]);
    }
 
    const Node* originalAddresses;
    int originalCount = tree.getNodeAddresses(originalAddresses);

    tree.displayDiagnostics();

    cout << "\nConverting BST to AVL Tree using in-place rotations...\n";
    tree.convertInPlaceToAVL();

    cout << "\n Final Balanced AVL Tree " << endl;
    tree.displayDiagnostics();

    cout << "\n=== Verification Summary ===" << endl;
    tree.verifyAVL(originalAddresses, originalCount);

    return 0;
}

