/* A balanced binary search tree(AVLTree) implementation
 * Written by Coleman
 * Released under GNU GPL Version 2 or later(http://www.gnu.org/)
 */
#include "libmin.h"
#include "element.h"
#include "avlcore.h"

#define MAXELEMENTSIZE 500000

void printTree(TreeNode *start, long level)
{
    long i = 0;
    if(start == NULL)
        return;
    libmin_printf("%s -> %ld, Balance %d\n", start->Name,
				*start->Item, start->Balance);
    for(i = 0; i < level; i++)
        libmin_printf(" ");
    libmin_printf("Left: ");
    printTree(start->Left, level + 1);
    libmin_printf("\n");
    for(i = 0; i < level; i++)
        libmin_printf(" ");
    libmin_printf("Right: ");
    printTree(start->Right, level + 1);
    if(level == 0)
        libmin_printf("\n");
}

long breadth(TreeNode *start)
{
    if(start == NULL)
        return (long)0;
    if(start->Left == NULL && start->Right == NULL)
        return (long)1;
    return (long)(breadth(start->Left) + breadth(start->Right));
}

int main(int argc, char** argv)
{
    TreeNode *tree = NULL, *newTree = NULL;
    Element newItem = NULL;
    char *itemName = NULL;
    long i = 0, topsize = 0, tick = 0, last = 0;
    long height = 0, oldheight = 0;
    
    /* get the command argument, used for a custom size parameter */
    topsize = 25;
    tick = topsize / 20;
    
    libmin_srand(42);
    libmin_printf("Start  -->  Finished\n");
    for(i = 0; i < topsize; i++) {
        newItem = libmin_malloc(sizeof(Element));
        itemName = libmin_malloc(sizeof(char) * 11);
        libmin_snprintf(itemName, 11, "Name%ld", i);
        do  {
            *newItem = libmin_rand();
            newTree = Insert(tree, newItem, itemName);
            if(newTree == NULL) {
                if(last == *newItem)    {
                    libmin_free(itemName);
                    libmin_free(newItem);
                    libmin_printf("going to try and skip: %ld\n", *newItem);
                    break;
                } else
                    last = *newItem;
            }
        } while(newTree == NULL);
        if(i % tick == 0)   {
            libmin_printf(".");
        }
        tree = newTree;
    }
    libmin_printf("\n");
    
    /* test out the delete functionality */
    libmin_printf("\nGoing to delete the rootNode: %ld\n", *tree->Item);
    newTree = DeleteByElement(tree, tree->Item);
    if(newTree == NULL)
        libmin_printf("failure!\n");
    else {
        tree = newTree;
        libmin_printf("success! new root is %ld\n", *tree->Item);
    }
    libmin_printf("\nGoing to delete leftMost child... ");
    for(newTree = tree;
        newTree != NULL && newTree->Left != NULL;
        newTree = newTree->Left)    {}
    if(newTree != NULL) {
        libmin_printf("found item: %ld\n", *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            libmin_printf("failure!\n");
        else {
            libmin_printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
    }
    /* duplicated from left side */
    libmin_printf("\nGoing to delete rightMost child... ");
    for(newTree = tree;
        newTree != NULL && newTree->Right != NULL;
        newTree = newTree->Right)   {}
    if(newTree != NULL) {
        libmin_printf("found item: %ld\n", *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            libmin_printf("failure!\n");
        else {
            libmin_printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
    }
    libmin_printf("\n");
	
    if(topsize <= 25)
        printTree(tree, 0);
    libmin_printf("breadth of left tree: %ld right subtree: %ld\n",
           breadth(tree->Left), breadth(tree->Right));
    oldheight = Height(tree->Left);
    libmin_printf("height of left subtree: %ld right subtree: %ld\n",
           oldheight, Height(tree->Right));

    last = (long)(INT_MAX / 2);
    for(i = 0; i < 3; i++)  {
        libmin_printf("\n");
        height = libmin_rand() % oldheight;
        libmin_printf("Going to delete a node at depth (max depth %ld): %ld\n",
                oldheight, height);
        newTree = tree;
        do  {
            if(newTree->Left == NULL)   {
                if(newTree->Right == NULL)
                    break;
                tick = last + 1;
            } else if(newTree->Right == NULL)
                tick = last - 1;
            else
                tick = libmin_rand();
            newTree = (tick >= last ? newTree->Right : newTree->Left);
            height--;
        } while(height > 0);
        libmin_printf("Found a node to delete with Name '%s' and Item %ld\n",
                newTree->Name, *newTree->Item);
        newTree = DeleteByElement(tree, newTree->Item);
        if(newTree == NULL)
            libmin_printf("failure!\n");
        else {
            libmin_printf("success! new root: %ld\n", *newTree->Item);
            tree = newTree;
        }
        oldheight = (int)(oldheight * 0.6);
    }
    
    MakeEmpty(tree);

    libmin_success();
    return 0;
}

