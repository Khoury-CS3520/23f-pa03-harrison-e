#ifndef PHOTOMOSAIC_AVLTREE_H
#define PHOTOMOSAIC_AVLTREE_H
#include <iostream>
#include <cmath>
using std::max, std::cout, std::endl;

template <typename K, typename V>
class AVLTree {
private:
    class AVLNode {
    public:
        K key;
        V val;
        AVLNode* left;
        AVLNode* right;
        int height;

        AVLNode(const K& k, const V& v);
        ~AVLNode() = default;
    };

    AVLNode* root { nullptr };

    // private helper fns for recursive operations
    void insert(AVLNode*& curr, const K& key, const V& val);
    int height(AVLNode* node);
    AVLNode* rotateR(AVLNode*& curRoot);
    AVLNode* rotateL(AVLNode* curRoot);
    AVLNode* search(AVLNode* curr, const K& searchKey);
    void clearTree(AVLNode* curr);
    AVLNode* copyMap(AVLNode* curr);

    // to demo:
    void traverse(AVLNode* curr) {
        if (curr) {
            traverse(curr->left);
            cout << curr->key << endl;
            traverse(curr->right);
        }
    }

public:
    AVLTree() = default;
    ~AVLTree();
    AVLTree(const AVLTree& that);
    AVLTree& operator=(const AVLTree& that);

    void insert(const K& k, const V& v);
    V& search(const K& searchKey);
    int height();

    void traverse() { traverse(root); }
};


//------------NODE--------------
template <typename K, typename V>
AVLTree<K,V>::AVLNode::AVLNode(const K &k, const V &v)
        : key(k), val(v), left(nullptr), right(nullptr), height {}
{}


//------------TREE--------------
// copy ctor
template <typename K, typename V>
AVLTree<K,V>::AVLTree(const AVLTree<K, V> &that) {
    root = copyMap(that.root);
}

// copy assignment op=
template<typename K, typename V>
AVLTree<K,V>& AVLTree<K,V>::operator=(const AVLTree<K, V> &that) {
    if (this != that) {
        clearTree(root);
        root = copyMap(that.root);
    }
    return *this;
}

//---------CLEAR TREE----------
template <typename K, typename V>
void AVLTree<K,V>::clearTree(AVLNode* curr) {
    if (curr != nullptr) {
        clearTree(curr->left);
        clearTree(curr->right);
        delete curr;
    }
}

//------------DTOR--------------
template <typename K, typename V>
AVLTree<K,V>::~AVLTree() {
    clearTree(root);
}

//-----------INSERT-------------
// public interface
template <typename K, typename V>
void AVLTree<K,V>::insert(const K &k, const V &v) {
    insert(root, k, v);
}

// private, recursive impl
template <typename K, typename V>
void AVLTree<K,V>::insert(AVLNode*& curr,
                          const K &key,
                          const V &val) {
    if (curr == nullptr)
        curr = new AVLNode(key, val);

    // proper placement
    if (key < curr->key)
        insert(curr->left, key, val);
    else if (key > curr->key)
        insert(curr->right, key, val);
    else
        curr->val = val;

    // update height
    int h_tL = height(curr->left);
    int h_tR = height(curr->right);
    int balance = h_tL - h_tR;
    curr->height = 1 + max(h_tL, h_tR);

    // left-heavy case
    if (balance > 1) {
        if (key < curr->left->key) {
            // right rotation
            curr = rotateR(curr);
        } else {
            // left-right rotation
            curr->left = rotateL(curr->left);
            curr = rotateR(curr);
        }
    }

    // right-heavy
    if (balance < -1) {
        if (key > curr->right->key) {
            // left rotation
            curr = rotateL(curr);
        } else {
            // right-left rotation
            curr->right = rotateR(curr->right);
            curr = rotateL(curr);
        }
    }
}

//-----------SEARCH-------------
// public interface
template <typename K, typename V>
V& AVLTree<K,V>::search(const K &searchKey) {
    return search(root, searchKey)->val;
}

// private, recursive impl
template <typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K,V>::search(AVLNode* curr,
                                                     const K &searchKey) {
    if (curr->key == searchKey) {
        return curr;
    }
    else if (searchKey < curr->key && curr->left != nullptr) {
        return search(curr->left, searchKey);
    }
    else if (searchKey > curr->key && curr->right != nullptr) {
        return search(curr->right, searchKey);
    }
    else {
        return curr;
    }
}

//------------COPY--------------
template <typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K,V>::copyMap(AVLTree::AVLNode *curr) {
    if (curr == nullptr) {
        return nullptr;
    }

    AVLNode* newNode = new AVLNode(curr->key, curr->val);
    newNode->left = copyMap(curr->left);
    newNode->right = copyMap(curr->right);

    return newNode;
}

//-----------HEIGHT-------------

template <typename K, typename V>
int AVLTree<K,V>::height(AVLNode *node) {
    if (node == nullptr)
        return -1;
    return node->height;
}

template <typename K, typename V>
int AVLTree<K,V>::height() {
    if (root == nullptr)
        return -1;
    return height(root);
}

//----------ROTATION------------
template <typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K,V>::rotateR(AVLNode*& curRoot) {
    AVLNode* newRoot = curRoot->left;
    AVLNode* nR_Right = newRoot->right;

    newRoot->right = curRoot;
    curRoot->left = nR_Right;

    curRoot->height = max(height(curRoot->left),
                          height(curRoot->right)) + 1;
    newRoot->height = max(height(newRoot->left),
                          height(newRoot->right)) + 1;

    return newRoot;
}

template <typename K, typename V>
typename AVLTree<K,V>::AVLNode* AVLTree<K,V>::rotateL(AVLNode* curRoot) {
    AVLNode* newRoot = curRoot->right;
    AVLNode* nR_Left = newRoot->left;

    newRoot->left = curRoot;
    curRoot->right = nR_Left;

    curRoot->height = max(height(curRoot->left),
                          height(curRoot->right)) + 1;
    newRoot->height = max(height(newRoot->left),
                          height(newRoot->right)) + 1;

    return newRoot;
}

#endif //PHOTOMOSAIC_AVLTREE_H
