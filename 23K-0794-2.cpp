#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

struct Record
{
    int ID;
    string Name;
    int Age;

    Record()
    {
        ID = 0;
        Name = "";
        Age = 0;
    }
    Record(int id, const string &name, int age)  
    {
        int ID = id;
        string Name = name;
        int Age = age; 
    }
};

Record *generateDummyData(int count)
{
    Record *records = new Record[count];
    for (int i = 0; i < count; i++)
    {
        int id = rand() % 100000;
        string name = "Name" + to_string(id);
        int age = rand() % 82 + 18;
        records[i] = Record(id, name, age);
    }
    return records;
}

class BSTNode
{
public:
    Record data;
    BSTNode *left;
    BSTNode *right;

    BSTNode(const Record &record)
    {
        data = record; 
        left = NULL; 
        right = NULL;
    }
};

class BST
{
private:
    BSTNode *root;

    BSTNode *insertNode(BSTNode *node, const Record &record)
    {
        if (node == NULL)
        {
            return new BSTNode(record);
        }
        if (record.ID < node->data.ID)
        {
            node->left = insertNode(node->left, record);
        }
        else if (record.ID > node->data.ID)
        {
            node->right = insertNode(node->right, record);
        }
        if (record.Age < node->data.Age)
        {
            node->left = insertNode(node->left, record);
        }
        else if (record.Age > node->data.Age)
        {
            node->right = insertNode(node->right, record);
        }
        return node;
    }

    BSTNode *searchNode(BSTNode *node, int id)
    {
        if (node == NULL || node->data.ID == id)
        {
            return node;
        }
        if (id < node->data.ID)
        {
            return searchNode(node->left, id);
        }
        return searchNode(node->right, id);
    }

    BSTNode *getMinNode(BSTNode *node)
    {
        while (node && node->left)
        {
            node = node->left;
        }
        return node;
    }

    BSTNode *deleteNode(BSTNode *node, int id)
    {
        if (node == NULL)
        {
            return NULL;
        }
        if (id < node->data.ID)
        {
            node->left = deleteNode(node->left, id);
        }
        else if (id > node->data.ID)
        {
            node->right = deleteNode(node->right, id);
        }
        else
        {
            if (node->left == NULL)
            {
                BSTNode *temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == NULL)
            {
                BSTNode *temp = node->left;
                delete node;
                return temp;
            }
            BSTNode *temp = getMinNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data.ID);
        }
        return node;
    }


public:
    BST()
    {
        root = NULL;
    }

    void insert(const Record &record)
    {
        root = insertNode(root, record);
    }

    Record *search(int id)
    {
        BSTNode *result = searchNode(root, id);
        return result ? &result->data : nullptr;
    }

    void remove(int id)
    {
        root = deleteNode(root, id);
    }

    void traverse()
    {
        traverseInOrder(root);
    }

    void traverseInOrder(BSTNode *node)
    {
        if (node != NULL)
        {
            traverseInOrder(node->left);
            cout << "ID: " << node->data.ID << ", Name: " << node->data.Name << ", Age: " << node->data.Age << "\n";
            traverseInOrder(node->right);
        }
    }
};

class AVLNode
{
public:
    Record data;
    AVLNode *left;
    AVLNode *right;
    int height;

    AVLNode(const Record &record) 
    {
        data = record; 
        left = NULL;
        right = NULL; 
        height = 1;
    }
};

class AVLTree
{
private:
    AVLNode *root;

    int getHeight(AVLNode *node)
    {
        if(node == NULL)
        {
            return 0;
        }
        else 
        {
            return node->height;
        }
    }

    int getBalanceFactor(AVLNode *node)
    {
        if(node == NULL)
        {
            return 0;
        }
        else
        {
            return getHeight(node->left) - getHeight(node->right);
        }
    }
    int getmax(int a, int b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    AVLNode *rotateRight(AVLNode *y)
    {
        AVLNode *x = y->left;
        AVLNode *Temp = x->right;

        x->right = y;
        y->left = Temp;

        y->height = getmax(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = getmax(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    AVLNode *rotateLeft(AVLNode *x)
    {
        AVLNode *y = x->right;
        AVLNode *Temp = y->left;

        y->left = x;
        x->right = Temp;

        x->height = getmax(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = getmax(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    AVLNode *insertNode(AVLNode *node, const Record &record)
    {
        if (node == NULL)
        {
            return new AVLNode(record);
        }
        if (record.ID < node->data.ID)
        {
            node->left = insertNode(node->left, record);
        }
        else if (record.ID > node->data.ID)
        {
            node->right = insertNode(node->right, record);
        }
        else
            return node;

        node->height = getmax(getHeight(node->left), getHeight(node->right)) + 1;

        int balance = getBalanceFactor(node);

        if (balance > 1 && record.ID < node->left->data.ID)
        {
            return rotateRight(node);
        }
        if (balance < -1 && record.ID > node->right->data.ID)
        {
            return rotateLeft(node);
        }
        if (balance > 1 && record.ID > node->left->data.ID)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && record.ID < node->right->data.ID)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    AVLNode *searchNode(AVLNode *node, int id)
    {
        if (node == NULL || node->data.ID == id)
        {
            return node;
        }
        if (id < node->data.ID)
        {
            return searchNode(node->left, id);
        }
        return searchNode(node->right, id);
    }

    AVLNode *deleteNode(AVLNode *node, int id)
    {
        if (node == NULL)
        {
            return node;
        }
        if (id < node->data.ID)
        {
            node->left = deleteNode(node->left, id);
        }
        else if (id > node->data.ID)
        {
            node->right = deleteNode(node->right, id);
        }
        else
        {
            if (node->left == NULL || node->right == NULL)
            {
                AVLNode *temp = node->left ? node->left : node->right;
                if (temp == NULL)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                AVLNode *temp = getMinValueNode(node->right);
                node->data = temp->data;
                node->right = deleteNode(node->right, temp->data.ID);
            }
        }
        if (node == NULL)
        {
            return node;
        }
        node->height = getmax(getHeight(node->left), getHeight(node->right)) + 1;

        int balance = getBalanceFactor(node);
        if (balance > 1 && getBalanceFactor(node->left) >= 0)
        {
            return rotateRight(node);
        }
        if (balance > 1 && getBalanceFactor(node->left) < 0)
        {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) <= 0)
        {
            return rotateLeft(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) > 0)
        {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    AVLNode *getMinValueNode(AVLNode *node)
    {
        while (node->left)
        {
            node = node->left;
        }
        return node;
    }

public:
    AVLTree() 
    {
        root = NULL;
    }

    void insert(const Record &record)
    {
        root = insertNode(root, record);
    }

    Record *search(int id)
    {
        AVLNode *result = searchNode(root, id);
        return result ? &result->data : nullptr;
    }

    void remove(int id)
    {
        root = deleteNode(root, id);
    }

    void traverse()
    {
        traverseInOrder(root);
    }

    void traverseInOrder(AVLNode *node)
    {
        if (node != NULL)
        {
            traverseInOrder(node->left);
            cout << "ID: " << node->data.ID << ", Name: " << node->data.Name << ", Age: " << node->data.Age << "\n";
            traverseInOrder(node->right);
        }
    }
};

class BTree
{
private:
    class BTreeNode
    {
    public:
        Record *keys;
        int t;
        BTreeNode **children;
        int n;
        bool leaf;

        BTreeNode(int degree, bool isLeaf)
            : t(degree), leaf(isLeaf), n(0)
        {
            keys = new Record[2 * t - 1];
            children = new BTreeNode *[2 * t];
        }

        void traverse()
        {
            for (int i = 0; i < n; i++)
            {
                if (!leaf)
                {
                    children[i]->traverse();
                }
                cout << "ID: " << keys[i].ID << ", Name: " << keys[i].Name << ", Age: " << keys[i].Age << "\n";
            }
            if (!leaf)
            {
                children[n]->traverse();
            }
        }

        BTreeNode *search(int id)
        {
            int i = 0;
            while (i < n && id > keys[i].ID)
                i++;
            if (i < n && keys[i].ID == id)
                return this;
            if (leaf)
                return nullptr;
            return children[i]->search(id);
        }

        void insertNonFull(const Record &record)
        {
            int i = n - 1;
            if (leaf)
            {
                while (i >= 0 && keys[i].ID > record.ID)
                {
                    keys[i + 1] = keys[i];
                    i--;
                }
                keys[i + 1] = record;
                n++;
            }
            else
            {
                while (i >= 0 && keys[i].ID > record.ID)
                    i--;
                if (children[i + 1]->n == 2 * t - 1)
                {
                    splitChild(i + 1, children[i + 1]);
                    if (keys[i + 1].ID < record.ID)
                        i++;
                }
                children[i + 1]->insertNonFull(record);
            }
        }

        void splitChild(int i, BTreeNode *y)
        {
            BTreeNode *z = new BTreeNode(y->t, y->leaf);
            z->n = t - 1;
            for (int j = 0; j < t - 1; j++)
                z->keys[j] = y->keys[j + t];
            if (!y->leaf)
            {
                for (int j = 0; j < t; j++)
                    z->children[j] = y->children[j + t];
            }
            y->n = t - 1;
            for (int j = n; j >= i + 1; j--)
                children[j + 1] = children[j];
            children[i + 1] = z;
            for (int j = n - 1; j >= i; j--)
                keys[j + 1] = keys[j];
            keys[i] = y->keys[t - 1];
            n++;
        }

        void remove(int id)
        {
            int idx = findKey(id);
            if (idx < n && keys[idx].ID == id)
            {
                if (leaf)
                {
                    removeFromLeaf(idx);
                }
                else
                {
                    removeFromNonLeaf(idx);
                }
            }
            else
            {
                if (leaf)
                {
                    cout << "Record with ID " << id << " not found.\n";
                    return;
                }

                bool flag = (idx == n);
                if (children[idx]->n < t)
                {
                    fill(idx);
                }

                if (flag && idx > n)
                {
                    children[idx - 1]->remove(id);
                }
                else
                {
                    children[idx]->remove(id);
                }
            }
        }

    private:
        int findKey(int id)
        {
            int idx = 0;
            while (idx < n && keys[idx].ID < id)
            {
                ++idx;
            }
            return idx;
        }

        void removeFromLeaf(int idx)
        {
            for (int i = idx + 1; i < n; i++)
            {
                keys[i - 1] = keys[i];
            }
            n--;
        }

        void removeFromNonLeaf(int idx)
        {
            Record record = keys[idx];
            if (children[idx]->n >= t)
            {
                Record pred = getPredecessor(idx);
                keys[idx] = pred;
                children[idx]->remove(pred.ID);
            }
            else if (children[idx + 1]->n >= t)
            {
                Record succ = getSuccessor(idx);
                keys[idx] = succ;
                children[idx + 1]->remove(succ.ID);
            }
            else
            {
                merge(idx);
                children[idx]->remove(record.ID);
            }
        }

        Record getPredecessor(int idx)
        {
            BTreeNode *cur = children[idx];
            while (!cur->leaf)
                cur = cur->children[cur->n];
            return cur->keys[cur->n - 1];
        }

        Record getSuccessor(int idx)
        {
            BTreeNode *cur = children[idx + 1];
            while (!cur->leaf)
                cur = cur->children[0];
            return cur->keys[0];
        }

        void merge(int idx)
        {
            BTreeNode *child = children[idx];
            BTreeNode *sibling = children[idx + 1];

            child->keys[t - 1] = keys[idx];
            for (int i = 0; i < sibling->n; i++)
            {
                child->keys[i + t] = sibling->keys[i];
            }

            if (!child->leaf)
            {
                for (int i = 0; i <= sibling->n; i++)
                {
                    child->children[i + t] = sibling->children[i];
                }
            }

            for (int i = idx + 1; i < n; i++)
            {
                keys[i - 1] = keys[i];
            }

            for (int i = idx + 2; i <= n; i++)
            {
                children[i - 1] = children[i];
            }

            child->n += sibling->n + 1;
            n--;
            delete sibling;
        }

        void fill(int idx)
        {
            if (idx != 0 && children[idx - 1]->n >= t)
            {
                borrowFromPrev(idx);
            }
            else if (idx != n && children[idx + 1]->n >= t)
            {
                borrowFromNext(idx);
            }
            else
            {
                if (idx != n)
                {
                    merge(idx);
                }
                else
                {
                    merge(idx - 1);
                }
            }
        }

        void borrowFromPrev(int idx)
        {
            BTreeNode *child = children[idx];
            BTreeNode *sibling = children[idx - 1];

            for (int i = child->n - 1; i >= 0; i--)
            {
                child->keys[i + 1] = child->keys[i];
            }

            if (!child->leaf)
            {
                for (int i = child->n; i >= 0; i--)
                {
                    child->children[i + 1] = child->children[i];
                }
            }

            child->keys[0] = keys[idx - 1];
            if (!child->leaf)
            {
                child->children[0] = sibling->children[sibling->n];
            }

            keys[idx - 1] = sibling->keys[sibling->n - 1];
            child->n += 1;
            sibling->n -= 1;
        }

        void borrowFromNext(int idx)
        {
            BTreeNode *child = children[idx];
            BTreeNode *sibling = children[idx + 1];

            child->keys[child->n] = keys[idx];
            if (!child->leaf)
            {
                child->children[child->n + 1] = sibling->children[0];
            }

            keys[idx] = sibling->keys[0];
            for (int i = 1; i < sibling->n; i++)
            {
                sibling->keys[i - 1] = sibling->keys[i];
            }

            if (!sibling->leaf)
            {
                for (int i = 1; i <= sibling->n; i++)
                {
                    sibling->children[i - 1] = sibling->children[i];
                }
            }

            child->n += 1;
            sibling->n -= 1;
        }
    };

    BTreeNode *root;
    int t;

public:
    BTree(int degree) : root(NULL), t(degree) {}

    void insert(const Record &record)
    {
        if (root == NULL)
        {
            root = new BTreeNode(t, true);
            root->keys[0] = record;
            root->n = 1;
        }
        else
        {
            if (root->n == 2 * t - 1)
            {
                BTreeNode *newRoot = new BTreeNode(t, false);
                newRoot->children[0] = root;
                newRoot->splitChild(0, root);
                root = newRoot;
            }
            root->insertNonFull(record);
        }
    }

    void traverse()
    {
        if (root)
        {
            root->traverse();
        }
    }

    BTreeNode *search(int id)
    {
        return root ? root->search(id) : NULL;
    }

    void remove(int id)
    {
        if (root)
        {
            root->remove(id);
            if (root->n == 0)
            {
                BTreeNode *oldRoot = root;
                if (root->leaf)
                {
                    root = NULL;
                }
                else
                {
                    root = root->children[0];
                }
                delete oldRoot;
            }
        }
    }
};

template <typename TreeType>
void testTree(TreeType& tree, Record* records, int count) 
{
    using namespace std;
    clock_t start, end;

    int randomIndices[20];
    for (int i = 0; i < 20; i++) 
    {
        randomIndices[i] = rand() % count;
    }
    start = clock();
    for (int i = 0; i < 20; i++) 
    {
        tree.insert(records[randomIndices[i]]);
    }
    end = clock();
    cout << "20 Insertions Time: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

    start = clock();
    for (int i = 0; i < 20; i++) 
    {
        tree.search(records[randomIndices[i]].ID);
    }
    end = clock();
    cout << "20 Searches Time: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

    start = clock();
    for (int i = 0; i < 20; i++) 
    {
        tree.remove(records[randomIndices[i]].ID);
    }
    end = clock();
    cout << "20 Deletions Time: " << double(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";
}


int main()
{
    srand(static_cast<unsigned int>(time(0)));

    int MAX_RECORDS;
    cout << "Enter number of records to generate: ";
    cin >> MAX_RECORDS;

    Record *records = generateDummyData(MAX_RECORDS);

    cout << "\nTesting Binary Search Tree (BST):\n";
    BST bst;
    testTree(bst, records, MAX_RECORDS);

    cout << "\nTesting AVL Tree:\n";
    AVLTree avl;
    testTree(avl, records, MAX_RECORDS);

    cout << "\nTesting B-Tree:\n";
    BTree btree(3);
    testTree(btree, records, MAX_RECORDS);

    delete[] records;

}
