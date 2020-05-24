#include <stdlib.h>
#include <stdio.h>
#include "btree.h"
/*
初始化二叉树
*/
void bitree_init(BTree *tree,void (*destroy)(void *data))
{
    tree->root=(BTreeNode*)malloc(sizeof(BTreeNode));
    if(tree->root==NULL) return;
    tree->size=0;
    tree->destroy=destroy;
    return;
}
/*
销毁二叉树
*/
void bitree_destroy(BTree *tree)
{
    bitree_rem_left(tree,NULL);
    bitree_rem_right(tree,NULL);
    if(tree->destroy!=NULL)
    {
        tree->destroy(tree->root->data);
    }
    free(tree->root);
    tree->size--;
    tree->root=NULL;
    return;
}
/*
在node的左节点插入元素
*/
int bitree_ins_left(BTree *tree,BTreeNode *node,const void *data)
{
    if(tree->size==0 && node==NULL)
    {
        tree->root->data=(void*)data;
        tree->root->left=NULL;
        tree->root->right=NULL;
        tree->size++;
        return 0;
    }
    if(node->left!=NULL) return -1;
    BTreeNode *left=(BTreeNode*)malloc(sizeof(BTreeNode));
    if(left==NULL) return -1;
    left->data=(void*)data;
    node->left=left;
    left->parent=node;
    left->left = left->right =NULL;
    tree->size++;
    return 0;
}

/*
在node的右节点插入元素
*/
int bitree_ins_right(BTree *tree,BTreeNode *node,const void *data)
{
    if(tree->size==0 && node==NULL)
    {
        tree->root->data=(void*)data;
        tree->root->left=NULL;
        tree->root->right=NULL;
        tree->size++;
        return 0;
    }
    if(node->right!=NULL) return -1;
    BTreeNode *right=(BTreeNode*)malloc(sizeof(BTreeNode));
    if(right==NULL) return -1;
    right->data=(void*)data;
    node->right=right;
    right->parent=node;
    right->left = right->right =NULL;
    tree->size++;
    return 0;
}
/*
删除一个节点下的所有子元素，包括该节点
*/
void treenode_remove(BTree *tree,BTreeNode *node)
{
    if(node->left!=NULL)
        treenode_remove(tree,node->left);
    if(node->right!=NULL)
        treenode_remove(tree,node->right);
    if(tree->destroy!=NULL)
        tree->destroy(node->data);
    if(node->parent->right == node)
    {
        node->parent->right=NULL;
    }
    if(node->parent->left == node){
        node->parent->left=NULL;
    }
    free(node);
    tree->size--;
}

/*
前序遍历：先访问根节点，然后左节点，最后右节点
*/
void preorder(BTreeNode *node)
{
    if(node==NULL) return;
    printf("%d ",(int)node->data);
    if(node->left!=NULL){
        postorder(node->left);
    }
    if(node->right!=NULL)
    {
        postorder(node->right);
    }
}
/*
中序遍历：先访问左节点，然后根节点，最后右节点
*/
void inorder(BTreeNode *node)
{
    if (node==NULL) return;
    if(node->left!=NULL){
        postorder(node->left);
    }
    printf("%d ",(int)node->data);
    if(node->right!=NULL)
    {
        postorder(node->right);
    }
}
/*
后序遍历：先访问左节点，然后右节点，最后根节点
*/
void postorder(BTreeNode *node)
{
    if(node==NULL) return;
    if(node->left!=NULL){
        postorder(node->left);
    }
    if(node->right!=NULL)
    {
        postorder(node->right);
    }
    printf("%d ",(int)node->data);
}
/*
删除该节点的左节点，如果node为空，删除根节点的左节点
*/
void bitree_rem_left(BTree *tree,BTreeNode *node)
{
    if(node==NULL)
    {
        if(tree->root->left)
            treenode_remove(tree,tree->root->left);
        return;
    }
    if(node->left!=NULL)
    {
        treenode_remove(tree,node->left);
        return;
    }
    
}
/*
删除该节点的右节点，如果node为空，删除根节点的右节点
*/
void bitree_rem_right(BTree *tree,BTreeNode *node)
{
    if(node==NULL )
    {
        if(tree->root->right) 
            treenode_remove(tree,tree->root->right);
        return;
    }
    if(node->right!=NULL)
    {   
        treenode_remove(tree,node->right);
        return;
    }
}
/*
合并两棵树，data是merge的根节点元素值
*/
int bitree_merge(BTree *merge,BTree *left,BTree *right,const void *data)
{
    bitree_init(merge,left->destroy);
    merge->root->data=(void*)data;
    merge->root->left=left->root;
    merge->root->right=right->root;
    merge->size = left->size + right->size + 1;
    left->root=NULL;
    right->root=NULL;
    left->size=0;
    right->size=0;
    return 0;
}

int main(int argc, char const *argv[])
{
    BTree tree,tree1,merge;
    bitree_init(&tree,NULL);
    bitree_init(&tree1,NULL);
    bitree_init(&merge,NULL);
    bitree_ins_left(&tree,NULL,1);
    bitree_ins_left(&tree,tree.root,3);
    bitree_ins_right(&tree,tree.root,2);
    bitree_ins_left(&tree,tree.root->left,4);
    bitree_ins_left(&tree,tree.root->right,5);
    bitree_rem_left(&tree,NULL);
    
    return 0;
}
