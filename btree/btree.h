#ifndef BTREE_H
#define BTREE_H

typedef struct BTreeNode_
{
    void *data;
    struct BTreeNode_ *left;
    struct BTreeNode_ *right;
    struct BTreeNode_ *parent;
}BTreeNode;

typedef struct BTree_
{
    BTreeNode *root;
    int size;
    int (*compare)(const void *key1,const void *key2);
    void (*destroy)(void *data);
}BTree;

void bitree_init(BTree *tree,void (*destroy)(void *data));
void bitree_destroy(BTree *tree);
int bitree_ins_left(BTree *tree,BTreeNode *node,const void *data);
int bitree_ins_right(BTree *tree,BTreeNode *node,const void *data);
void bitree_rem_left(BTree *tree,BTreeNode *node);
void bitree_rem_right(BTree *tree,BTreeNode *node);
int bitree_merge(BTree *merge,BTree *left,BTree *right,const void *data);

#define bitree_size(tree) ((tree)->size)
#define bitree_root(tree) ((tree)->root)
#define bitree_eob(node)  ((node) == NULL)
#define bitree_is_leaf(node)  ((node)->left==NULL && (node)->right==NULL)
#define bitree_data(node)  ((node)->data)
#define bitree_left(node)  ((node)->left)
#define bitree_right(node) ((node)->right)
#endif