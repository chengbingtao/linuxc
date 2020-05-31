/*file:biTree.h*/  
#include <stdlib.h>
#include <string.h>

#ifndef CHIYX_BITREE  
#define CHIYX_BITREE  
#ifndef NULL  
#define NULL 0  
#endif  
typedef char DataType[19];  
 
typedef struct BiTreeNode {  
    DataType data;  
    struct BiTreeNode *parent;  
    struct BiTreeNode *left;  
    struct BiTreeNode *right;  
}BiTreeNode, *BiTree;  
  
//search the first item that equale key,return NULL if can't find  
BiTreeNode *search(BiTree *biTree, DataType key);  
//return the min item
BiTreeNode *minImum(BiTree *biTree);  
//return the max item  
BiTreeNode *maxImum(BiTree *biTree);  
//return the next item of x,if x is the max item ,return NULL  
BiTreeNode *successor(BiTreeNode *x);  
//return the pre item of x,if x is the min item ,then return NULL  
BiTreeNode *predecessor(BiTreeNode *x);  
//insert tree node
void insertNode(BiTree *biTree, DataType data);  
//delete tree node  
void deleteNode(BiTree *biTree, DataType data);  
//travel the tree with mid order
void inorderTraversal(BiTree *biTree, void (*visitor)(BiTreeNode *node));  
#endif  
