#include <iostream>
using namespace std;

class Patient
{
public:
    int emergencyID;

    Patient()
    {
        emergencyID = 0;
    }
};

// AVL NODE
class Node
{
public:
    Patient data;
    int height;
    Node *left;
    Node *right;

    Node(Patient p)
    {
        data = p;
        height = 1;
        left = NULL;
        right = NULL;
    }
};

class AVL{
private:
    Node *root;

    int getHeight(Node *node){
        if (node == NULL)
            return 0;

        return node->height;
    }

    int getBalance(Node *node)
    {
        if (node == NULL)
            return 0;

        return getHeight(node->left) -
               getHeight(node->right);
    }

    int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    Node* rightRotate(Node *y)
    {
        Node *x = y->left;
        Node *temp = x->right;

        x->right = y;
        y->left = temp;

        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        return x;
    }

    Node* leftRotate(Node *x)
    {
        Node *y = x->right;
        Node *temp = y->left;

        y->left = x;
        x->right = temp;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));

        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }

    // INSERTION
    Node* insertNode(Node *node, Patient p){
        int id = p.emergencyID;

        if (node == NULL){
            cout << "Inserted " << id
                 << " BF = 0" << endl;

            return new Node(p);
        }

        if (id < node->data.emergencyID)
            node->left = insertNode(node->left, p);

        else if (id > node->data.emergencyID)
            node->right = insertNode(node->right, p);

        else
            return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int balance = getBalance(node);

        cout << "Node " << node->data.emergencyID
             << " BF = " << balance << endl;

        if (balance > 1 && id < node->left->data.emergencyID){
            cout << "LL Rotation at "<< node->data.emergencyID << endl;

            return rightRotate(node);
        }

        // RR CASE
        if (balance < -1 && id > node->right->data.emergencyID){
            cout << "RR Rotation at "
                 << node->data.emergencyID << endl;

            return leftRotate(node);
        }

        // LR CASE
        if (balance > 1 && id > node->left->data.emergencyID){
            cout << "LR Rotation at "<< node->data.emergencyID << endl;

            node->left = leftRotate(node->left);

            return rightRotate(node);
        }

        // RL CASE
        if (balance < -1 && id < node->right->data.emergencyID){
            cout << "RL Rotation at "
                 << node->data.emergencyID << endl;

            node->right = rightRotate(node->right);

            return leftRotate(node);
        }
        return node;
    }

    // SEARCHING
    bool searchNode(Node *node, int id)
    {
        if (node == NULL)
            return false;

        if (id == node->data.emergencyID)
            return true;

        if (id < node->data.emergencyID)
            return searchNode(node->left, id);

        return searchNode(node->right, id);
    }

    Node* findMin(Node *node)
    {
        while (node->left != NULL)
            node = node->left;

        return node;
    }

    Node* deleteNode(Node *node, int id)
    {
        if (node == NULL)
            return NULL;

        if (id < node->data.emergencyID)
        {
            node->left = deleteNode(node->left, id);
        }
        else if (id > node->data.emergencyID)
        {
            node->right = deleteNode(node->right, id);
        }
        else
        {
            if (node->left == NULL)
            {
                Node *temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == NULL)
            {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            else
            {
                Node *temp = findMin(node->right);

                node->data = temp->data;

                node->right =
                    deleteNode(node->right,
                               temp->data.emergencyID);
            }
        }

        node->height = 1 + max(getHeight(node->left),
                               getHeight(node->right));

        int balance = getBalance(node);

        cout << "Node " << node->data.emergencyID
             << " BF = " << balance << endl;

        // LL CASE
        if (balance > 1 &&
            getBalance(node->left) >= 0)
        {
            cout << "LL Rotation at "
                 << node->data.emergencyID << endl;

            return rightRotate(node);
        }

        // LR CASE
        if (balance > 1 &&
            getBalance(node->left) < 0)
        {
            cout << "LR Rotation at "
                 << node->data.emergencyID << endl;

            node->left = leftRotate(node->left);

            return rightRotate(node);
        }

        // RR CASE
        if (balance < -1 &&
            getBalance(node->right) <= 0)
        {
            cout << "RR Rotation at "
                 << node->data.emergencyID << endl;

            return leftRotate(node);
        }

        // RL CASE
        if (balance < -1 &&
            getBalance(node->right) > 0)
        {
            cout << "RL Rotation at "
                 << node->data.emergencyID << endl;

            node->right = rightRotate(node->right);

            return leftRotate(node);
        }

        return node;
    }

    // INORDER
    void inorder(Node *node)
    {
        if (node == NULL)
            return;

        inorder(node->left);

        cout << node->data.emergencyID << " ";

        inorder(node->right);
    }

    // DISPLAY LEVEL ORDER USING ARRAY
    void levelOrder(Node *node)
    {
        if (node == NULL)
            return;

        Node *q[100];

        int front = 0;
        int rear = 0;

        q[rear++] = node;

        while (front < rear)
        {
            Node *current = q[front++];

            cout << current->data.emergencyID << " ";

            if (current->left != NULL)
                q[rear++] = current->left;

            if (current->right != NULL)
                q[rear++] = current->right;
        }
    }

public:

    AVL()
    {
        root = NULL;
    }

    void insert(int id)
    {
        cout << "\nInserting " << id << endl;

        Patient p(id);

        root = insertNode(root, p);
    }

    bool search(int id)
    {
        return searchNode(root, id);
    }

    void remove(int id)
    {
        cout << "\nDeleting " << id << endl;

        root = deleteNode(root, id);
    }

    void displayInorder()
    {
        inorder(root);
        cout << endl;
    }

    void displayLevelOrder()
    {
        levelOrder(root);
        cout << endl;
    }
};

int main()
{
    AVL tree;

    int ids[] = {
        45, 25, 65, 15, 35, 55,
        75, 10, 20, 30, 40, 5
    };

    int n = 12;
    cout << "INSERTION" << endl;

    for (int i = 0; i < n; i++)
    {
        tree.insert(ids[i]);
    }

    cout << "\nSEARCHING " << endl;

    if (tree.search(45))
        cout << "Patient 45 Found" << endl;
    else
        cout << "Patient 45 Not Found" << endl;

    cout << "\nDELETION" << endl;

    tree.remove(75);
    tree.remove(65);
    tree.remove(55);

    cout << "Inorder: ";
    tree.displayInorder();

    cout << "Level Order: ";
    tree.displayLevelOrder();

    return 0;
}

