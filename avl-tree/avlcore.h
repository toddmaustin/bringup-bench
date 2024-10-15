#ifndef __Tree_H

#define __Tree_H

#include "element.h"

/* the basic tree structure
 * the root of the complete tree is defined where Parent is NULL
 */
typedef struct TreeNodeTag	{
	struct TreeNodeTag *Left;
	struct TreeNodeTag *Right;
	Element Item;
	char *Name;
	short Balance;
} TreeNode;

/*
 * This function will find a particular TreeNode with the given
 * Element as the Item.
 */
TreeNode *FindByElement(TreeNode *rootNode, const Element item);

/*
 * This function will return a boolean and determine if the given tree
 * (or subtree) is empty.
 */
short IsEmpty(TreeNode *rootNode);

/*
 * This function will insert a given Element into the tree (subtree) and
 * perform all neccessary adjustments to the tree. Returns the new root
 * node.
 */
TreeNode *Insert(TreeNode *rootNode, Element Item, char *Name);

/*
 * This function will remove all items from the give tree or subtree
 */
void MakeEmpty(TreeNode *rootNode);

/*
 * Deletes a given TreeNode and will perform all neccessary adjustments
 * to the tree. Returns the new root node. Note: the Name field will be
 * free'd with the TreeNode.
 */
TreeNode *DeleteByElement(TreeNode *rootNode, Element Item);

/*
 * Determines the height of the given tree. Should be used with caution with
 * large trees.
 */
long Height(TreeNode *rootNode);

#endif
