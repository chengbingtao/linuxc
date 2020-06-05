#include "biTree.h"
BiTreeNode *search(BiTree *biTree, DataType key) {  
    BiTreeNode *curNode = *biTree;  
    while (curNode != NULL && strcmp(curNode->data , key) != 0 ) {  
        if (strcmp(key , curNode->data) < 0 ) {  
            curNode = curNode->left;  
        } else {  
            curNode = curNode->right;  
        }  
    }  
    return curNode;  
}

//  
BiTreeNode *minImum(BiTree *biTree) {  
    BiTreeNode *curNode = *biTree;  
    while (curNode != NULL && curNode->left != NULL) {  
        curNode = curNode->left;  
    }  
    return curNode;  
}   


//  
BiTreeNode *maxImum(BiTree *biTree) {  
    BiTreeNode *curNode = *biTree;  
    while (curNode != NULL && curNode->right != NULL) {  
        curNode = curNode->right;  
    }  
    return curNode;  
} 


BiTreeNode *successor(BiTreeNode *x) {  
         if (x == NULL) return NULL;  
    if (x != NULL && x->right != NULL) {  
        return minImum(&(x->right));  
    }  
    while (x->parent != NULL && x->parent->right == x) {  
        x = x->parent;  
    }  
    return x->parent; //  
}  

BiTreeNode *predecessor(BiTreeNode *x) {  
         if (x == NULL) return NULL;  
    if (x != NULL && x->left != NULL) {  
        return maxImum(&(x->left));  
    }  
    while (x->parent != NULL && x->parent->left == x) {  
        x = x->parent;  
    }  
    return x->parent; //  
  
}  

void insertNode(BiTree *biTree, DataType data) {  
    //  
    BiTreeNode *targetNode;  
  
    targetNode = (BiTreeNode *)malloc(sizeof(BiTreeNode));  

    if (targetNode == NULL) return;  
    strcpy(targetNode->data,data);  
    targetNode->parent = NULL;  
    targetNode->left = NULL;  
    targetNode->right = NULL;  
  
    BiTreeNode *p, *y;   
    p = *biTree;  
    y = NULL;  
    while (p != NULL ) {  
        y = p;  
        if (strcmp(targetNode->data , p->data) < 0 ) {  
            p = p->left;  
        } else {  
            p = p->right;  
        }  
    }  

    if (y == NULL) {  
        *biTree = targetNode;  
    } else {  
        if (strcmp(targetNode->data , y->data)<0 ) {  
            y->left = targetNode;  
        } else {  
            y->right = targetNode;  
        }  
    }  
    targetNode->parent = y;  
}  


void deleteNode(BiTree *biTree, DataType data) {  
    BiTreeNode *targetNode, *x, *y;  
  
    targetNode = search(biTree, data);  
    if (targetNode == NULL) return;  
    if (targetNode->left == NULL || targetNode->right == NULL) {  
        y = targetNode;  
    } else {  
        y = successor(targetNode);  
    }  
  
    if (y->left != NULL) {  
        x = y->left;  
    } else {  
        x = y->right;  
    }  
  
    if (x != NULL) {  
        x->parent = y->parent;  
    }  
  
    if (y->parent == NULL) {  
        *biTree = x;  
    } else {  
        if (y->parent->right == y) {  
            y->parent->right = x;  
        } else {  
            y->parent->left = x;  
        }  
    }  
  
    if (y != targetNode) {  
        strcpy(targetNode->data , y->data);  
    }   
    free(y);  
} 


void deleteNode2(BiTree *biTree, BiTreeNode* targetNode) {  
    BiTreeNode  *x, *y;  
  
    //targetNode = search(biTree, data);  
    if (targetNode == NULL) return;  
    if (targetNode->left == NULL || targetNode->right == NULL) {  
        y = targetNode;  
    } else {  
        y = successor(targetNode);  
    }  
  
    if (y->left != NULL) {  
        x = y->left;  
    } else {  
        x = y->right;  
    }  
  
    if (x != NULL) {  
        x->parent = y->parent;  
    }  
  
    if (y->parent == NULL) {  
        *biTree = x;  
    } else {  
        if (y->parent->right == y) {  
            y->parent->right = x;  
        } else {  
            y->parent->left = x;  
        }  
    }  
  
    if (y != targetNode) {  
        strcpy(targetNode->data , y->data);  
    }   
    free(y);  
} 


void inorderTraversal(BiTree *biTree, void (*visitor)(BiTreeNode *node)) {  
    BiTreeNode *curNode;  
  
    curNode = *biTree;  
    if (curNode != NULL) {  
        inorderTraversal(&(curNode->left), visitor);  
        visitor(curNode);  
        inorderTraversal(&(curNode->right), visitor);  
    }  
}  

