#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* new = (TreeMap*) malloc(sizeof(TreeMap)) ;
    if (new == NULL) return NULL ;
    
    new->root = NULL ;
    new->current = NULL ;
    new->lower_than = lower_than ;
    
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) return ;
    if (searchTreeMap(tree, key) != NULL ) return ;

    TreeNode* new = createTreeNode(key, value) ;
    if (new == NULL) return ;

    TreeNode* parent = NULL ;
    TreeNode* current = tree->root ;

    while(current != NULL) {
        parent = current ;

        if (is_equal(tree, key, current->pair->key)) return ;

        if (tree->lower_than(key, current->pair->key)) {
        current = current->left ;
        } else {
            current = current->right ;
        }
    }

    new->parent = parent ;

    if (parent == NULL) tree->root = new ;
    else if (tree->lower_than(key, parent->pair->key)) parent->left = new ;
    else parent->right = new ;

    tree->current = new ;
}

TreeNode * minimum(TreeNode * x){
    while (x != NULL && x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return ;

    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) tree->root = NULL ;
        else if (node->parent->left == node) node->parent->left = NULL ;
        else node->parent->right = NULL;
    }
    
    else if (node->left == NULL || node->right == NULL) {
        TreeNode * child = node->left ? node->left : node->right ;
        child->parent = node->parent ;

        if (node->parent == NULL) tree->root = child ;
        else if (node->parent->left == node) node->parent->left = child ;
        else node->parent->right = child ;
    }

    else {
        TreeNode* min = minimum(node->right) ;
        node->pair = min->pair ;
        removeNode(tree, min) ;
        return ;
    }

    free(node) ;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;

    TreeNode* node = tree->current;
    removeNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * current = tree->root ;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current ;
            return current->pair ;
        }

        if (tree->lower_than(key, current->pair->key)) {
            current = current->left ;
        } else {
            current = current->right ;
        }
    }

    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* current = tree->root ;
    TreeNode* ubNode = NULL ;

    while (current != NULL) {
        if (is_equal(tree, key, current->pair->key)) {
            tree->current = current ;
            return current->pair ;
        }

        if (tree->lower_than(key, current->pair->key)) {
            ubNode = current ;
            current = current->left ;
        } else {
            current = current->right ;
        }
    }
    
    if (ubNode != NULL) {
        tree->current = ubNode ;

        return ubNode->pair ;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL ; 

    tree->current = minimum(tree->root) ;
    if (tree->current == NULL) return NULL ; 
    
    return tree->current->pair ;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL ; 

    TreeNode* current = tree->current ;

    if (current->right != NULL) {
        current = minimum(current->right) ;
        tree->current = current ;
        return current->pair ;
    }

    TreeNode* parent = current->parent ;
    while (parent != NULL && current == parent->right) {
        current = parent ;
        parent = parent->parent ;
    }

    tree->current = parent ;
    if (parent == NULL) return NULL ;

    return parent->pair ;
}
