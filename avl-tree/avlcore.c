/*
 * A free AVLTree implementation written by Andrew Coleman
 * find out more cool things and licensing at http://penguincoder.org
 */
#include "libmin.h"
#include "avlcore.h"

TreeNode *FindByElement(TreeNode *rootNode, Element Data)
{
    short comp;
    TreeNode *Return;
    
    /* sanity checks */
    if(Data == NULL)
        return NULL;
    else if(rootNode == NULL || rootNode->Item == NULL)  {
        /* reached a leaf, and the data was not found */
        return NULL;
    }
    
    /* non recursive search because it really isn't neccessary right here */
    Return = rootNode;
    do    {
        comp = Compare(Return->Item, Data);
        if(comp < 0)
            Return = Return->Left;
        else if(comp > 0)
            Return = Return->Right;
    } while(Return != NULL && comp != 0);
    return Return;
}

short IsEmpty(TreeNode *rootNode)
{
    return (rootNode == NULL);
}

TreeNode *SingleRightRotation(TreeNode *rootNode)
{
    TreeNode *newRoot = NULL;
    newRoot = rootNode->Left;
    rootNode->Left = newRoot->Right;
    newRoot->Right = rootNode;
    return newRoot;
}

TreeNode *SingleLeftRotation(TreeNode *rootNode)
{
    TreeNode *newRoot = NULL;
    newRoot = rootNode->Right;
    rootNode->Right = newRoot->Left;
    newRoot->Left = rootNode;
    return newRoot;
}

TreeNode *DoubleRightRotation(TreeNode *rootNode)
{
    TreeNode *newRoot = SingleLeftRotation(rootNode->Left);
    rootNode->Left = newRoot;
    newRoot = SingleRightRotation(rootNode);
    return newRoot;
}

TreeNode *DoubleLeftRotation(TreeNode *rootNode)
{
    TreeNode *newRoot = SingleRightRotation(rootNode->Right);
    rootNode->Right = newRoot;
    newRoot = SingleLeftRotation(rootNode);
    return newRoot;
}

TreeNode *CheckTreeNodeRotation(TreeNode *tnode,
                                TreeNode *subtree, short *finished)
{
    /* this function will check to see if tnode's Balance factor indicates
        that a rotation is needed. Subtree is the child link that was either
        just added or possibly changed */
    switch(tnode->Balance)  {
        case 0:
            /* nothing special, but we have balanced out properly, yay */
            *finished = 1;
            break;
        case 2:
            /* right heavy */
            *finished = 1;
            if(subtree->Balance == -1)   {
                /* update balance factors */
                if(subtree->Left->Balance == 1)    {
                    subtree->Balance = 0;
                    tnode->Balance = -1;
                } else if(subtree->Left->Balance == -1)    {
                    subtree->Balance = 1;
                    tnode->Balance = 0;
                } else
                    subtree->Balance = tnode->Balance = 0;
                subtree->Left->Balance = 0;
                tnode = DoubleLeftRotation(tnode);
            } else if(subtree->Balance == 1)   {
                /* update balance factors */
                subtree->Balance = tnode->Balance = 0;
                tnode = SingleLeftRotation(tnode);
            }
            break;
        case -2:
            /* left heavy */
            *finished = 1;
            if(subtree->Balance == 1)  {
                /* update balance factors */
                if(subtree->Right->Balance == -1)    {
                    subtree->Balance = 0;
                    tnode->Balance = 1;
                } else if(subtree->Right->Balance == 1)    {
                    subtree->Balance = -1;
                    tnode->Balance = 0;
                } else
                    subtree->Balance = tnode->Balance = 0;
                subtree->Right->Balance = 0;
                tnode = DoubleRightRotation(tnode);
            } else if(subtree->Balance == -1)   {
                /* update balance factors */
                subtree->Balance = tnode->Balance = 0;
                tnode = SingleRightRotation(tnode);
            }
    }
    return tnode;
}

TreeNode *InsertNode(TreeNode *tnode, Element Item,
                     char *Name, short *finished)
{
    TreeNode *subtree = NULL;
    short comp = 0;
    
    if(tnode == NULL)    {
        /* found the spot that the TreeNode belongs */
        tnode = libmin_malloc(sizeof(TreeNode));
        /* malloc problem, return NULL */
        if(tnode == NULL)
            return NULL;
        else    {
            /* default settings for a new node */
            tnode->Left = NULL;
            tnode->Right = NULL;
            tnode->Item = Item;
            tnode->Name = Name;
            tnode->Balance = 0;
        }
    } else {
        /* need to find the new node, don't check for a 0,
           cos its already in teh tree, foo */
        comp = Compare(Item, tnode->Item);
        if(comp < 0)    {
            /* go left */
            subtree = InsertNode(tnode->Left, Item, Name, finished);
            /* if this is NULL, there is problem. quit now */
            if(subtree == NULL)
                return NULL;
            /* you are not finished until Balance == 0, reached the root
                or a rotation has occured */
            if(!*finished)
                tnode->Balance--;
            tnode->Left = subtree;
        } else if(comp > 0)    {
            /* go right */
            subtree = InsertNode(tnode->Right, Item, Name, finished);
            if(subtree == NULL)
                return NULL;
            if(!*finished)
                tnode->Balance++;
            tnode->Right = subtree;
        } else
            /* got a dupe, return NULL */
            return NULL;

        /* check if a rotation is neccessary */
        if(!*finished)  {
            tnode = CheckTreeNodeRotation(tnode, subtree, finished);
        }
    }
    return tnode;
}

TreeNode *Insert(TreeNode *tnode, Element Item, char *Name)
{
    /* flag for the recursive function */
    short done = 0;
    /* run and return the recursive insert */
    return InsertNode(tnode, Item, Name, &done);
}

void MakeEmpty(TreeNode *rootNode)
{
    /* this function is pretty simple, just traverses the tree and starts
        free'ing from the bottom up */
    if(rootNode == NULL)
        return;
    MakeEmpty(rootNode->Left);
    MakeEmpty(rootNode->Right);
    FreeElement(rootNode->Item);
    libmin_free(rootNode->Name);
    libmin_free(rootNode);
}

TreeNode *DeleteLeftMost(TreeNode **deletedNode, TreeNode *rootNode,
                    short *finished)
{
    TreeNode *subtree = rootNode->Right;
    if(rootNode == NULL)    {
        libmin_printf("bailing!\n");
        return NULL;
    }
    if(rootNode->Left == NULL)  {
        libmin_free((*deletedNode)->Name);
        FreeElement((*deletedNode)->Item);
        (*deletedNode)->Name = rootNode->Name;
        (*deletedNode)->Item = rootNode->Item;
        libmin_free(rootNode);
        rootNode = subtree;
    } else {
        rootNode->Left = DeleteLeftMost(deletedNode, rootNode->Left, finished);
        if(!*finished)  {
            rootNode->Balance++;
            if(rootNode->Left != NULL)
                rootNode = CheckTreeNodeRotation(rootNode, rootNode->Left,
                                finished);
        }
    }
    return rootNode;
}

TreeNode *DeleteByElementRecursive(TreeNode *rootNode,
                                   Element Item, short *finished)
{
    TreeNode *subtree = NULL;
    short comp = 0;
    
    /* found a leaf */
    if(rootNode == NULL)
        return NULL;
    /* figure out where the node in question is in relation to the current */
    comp = Compare(rootNode->Item, Item);
    if(comp > 0)    {
        /* buried somewhere in the right subtree */
        subtree = DeleteByElementRecursive(rootNode->Right, Item, finished);
        rootNode->Right = subtree;
        if(!*finished && subtree != NULL)  {
            rootNode->Balance--;
            rootNode = CheckTreeNodeRotation(rootNode, subtree, finished);
        }
    } else if(comp < 0) {
        subtree = DeleteByElementRecursive(rootNode->Left, Item, finished);
        rootNode->Left = subtree;
        if(!*finished && subtree != NULL)  {
            rootNode->Balance++;
            rootNode = CheckTreeNodeRotation(rootNode, subtree, finished);
        }
    } else {
        /* found what we were looking for */
        if(rootNode->Right == NULL) {
            /* rootNode is replaced by the left child; can be null */
            subtree = rootNode->Left;
            libmin_free(rootNode->Name);
            FreeElement(rootNode->Item);
            libmin_free(rootNode);
            rootNode = subtree;
        } else if(rootNode->Right->Left == NULL)  {
            /* rootNode is replaced by Right */
            subtree = rootNode->Right;
            subtree->Balance = rootNode->Balance;
            subtree->Left = rootNode->Left;
            libmin_free(rootNode->Name);
            FreeElement(rootNode->Item);
            libmin_free(rootNode);
            rootNode = subtree;
        } else {
            /* rootNode is replaced by inorder successor */
            /* re-using comp as another flag since it is 0 */
            rootNode->Right = DeleteLeftMost(&rootNode, rootNode->Right, &comp);
        }
    }
    return rootNode;
}

TreeNode *DeleteByElement(TreeNode *rootNode, Element Item)
{
    short finished = 0;
    TreeNode *newRoot = NULL;
    newRoot = DeleteByElementRecursive(rootNode, Item, &finished);
    return newRoot;
}

long Height(TreeNode *rootNode)
{
    /* recursive function to determine the height by comparing the height of
        the left and right child and returning the larger value */
    long lresult = 0, rresult = 0;
    /* either the tree is empty or we found a leaf */
    if(rootNode == NULL)
        return 0;
    lresult = Height(rootNode->Left);
    rresult = Height(rootNode->Right);
    /* minimum height is 1, but that's probably correct */
    return (lresult > rresult ? lresult : rresult) + 1;
}

