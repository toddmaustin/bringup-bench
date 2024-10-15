
// ripped from geeksforgeeks

// C code to implement Priority Queue
// using Linked List
#include "libmin.h"

// Node
typedef struct node {
    int data;

    // Lower values indicate higher priority
    int priority;

    struct node* next;

} Node;

// Function to Create A New Node
Node* newNode(int d, int p)
{
    Node* temp = (Node*)libmin_malloc(sizeof(Node));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;

    return temp;
}

// Return the value at head
int peek(Node** head)
{
    return (*head)->data;
}

// Return the value at head
int priopeek(Node** head)
{
    return (*head)->priority;
}

// Removes the element with the
// highest priority form the list
void pop(Node** head)
{
    Node* temp = *head;
    (*head) = (*head)->next;
    libmin_free(temp);
}

// Function to push according to priority
void push(Node** head, int d, int p)
{
    Node* start = (*head);

    // Create new Node
    Node* temp = newNode(d, p);

    // Special Case: The head of list has lesser
    // priority than new node. So insert new
    // node before head node and change head node.
    if ((*head)->priority > p) {

        // Insert New Node before head
        temp->next = *head;
        (*head) = temp;
    }
    else {

        // Traverse the list and find a
        // position to insert new node
        while (start->next != NULL &&
               start->next->priority < p) {
            start = start->next;
        }

        // Either at the ends of the list
        // or at required position
        temp->next = start->next;
        start->next = temp;
    }
}

// Function to check is list is empty
int isEmpty(Node** head)
{
    return (*head) == NULL;
}

void
printPQ(Node** pq)
{
  libmin_printf("priorityQ: ");
  while (!isEmpty(pq)) {
      libmin_printf("<prio:%d, val:%d> ", priopeek(pq), peek(pq));
      pop(pq);
  }
  libmin_printf("\n");
}

// Driver code
int main()
{
    // Create a Priority Queue
    // 7->4->5->6
    Node* pq = newNode(4, 1);
    push(&pq, 5, 2);
    push(&pq, 6, 3);
    push(&pq, 7, 0);
    printPQ(&pq);

    libmin_srand(42);
    pq = newNode(4, 1);
    for (int i=0; i < 250; i++)
    {
      int prio = libmin_rand() % 250;
      int val = libmin_rand() % 250;
      push(&pq, prio, val);
    }
    printPQ(&pq);

    libmin_success();
    return 0;
}

