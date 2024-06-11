// C Program to implement Topological Sorting 
#include "libmin.h"

// Structure to represent a stack 
struct Stack { 
    int data; 
    struct Stack* next; 
}; 
  
struct Graph { 
    int V; // No. of vertices 
    // Pointer to an array containing adjacency lists 
    struct List* adj; 
}; 
  
// Structure to represent a list (adjacency list) 
struct List { 
    int data; 
    struct List* next; 
}; 
  
// Create a new node for the stack 
struct Stack *
createStackNode(int data) 
{ 
  struct Stack* newNode = (struct Stack*)libmin_malloc(sizeof(struct Stack)); 
  newNode->data = data; 
  newNode->next = NULL; 
  return newNode; 
} 
  
// Create a new node for the adjacency list 
struct List *
createListNode(int data) 
{ 
  struct List* newNode = (struct List*)libmin_malloc(sizeof(struct List)); 
  newNode->data = data; 
  newNode->next = NULL; 
  return newNode; 
} 
  
// Function to initialize a graph with V vertices 
struct Graph *
createGraph(int V) 
{ 
  struct Graph* graph = (struct Graph*)libmin_malloc(sizeof(struct Graph)); 
  graph->V = V; 
  graph->adj = (struct List*)libmin_malloc(V * sizeof(struct List)); 
  for (int i = 0; i < V; ++i)
    graph->adj[i].next = NULL; 
  return graph; 
} 
  
// Function to add an edge to the graph 
void
addEdge(struct Graph* graph, int v, int w) 
{ 
  struct List* newNode = createListNode(w); 
  newNode->next = graph->adj[v].next; 
  graph->adj[v].next = newNode; 
} 
  
// A recursive function used by topologicalSort 
void
topologicalSortUtil(struct Graph* graph, int v, int visited[], struct Stack** stack) 
{ 
  visited[v] = TRUE; 
  
  struct List* current = graph->adj[v].next; 
  while (current != NULL)
  { 
    int adjacentVertex = current->data; 
    if (!visited[adjacentVertex]) 
      topologicalSortUtil(graph, adjacentVertex, visited, stack); 
    current = current->next; 
  } 
  
  // Push the current vertex to stack which stores the 
  // result 
  struct Stack* newNode = createStackNode(v); 
  newNode->next = *stack; 
  *stack = newNode; 
} 
  
// The function to do Topological Sort. It uses recursive 
// topologicalSortUtil 
void topologicalSort(struct Graph* graph) 
{ 
    struct Stack* stack = NULL; 
  
    // Mark all the vertices as not visited 
    int* visited = (int*)libmin_malloc(graph->V * sizeof(int)); 
    for (int i = 0; i < graph->V; ++i) { 
        visited[i] = FALSE; 
    } 
  
    // Call the recursive helper function to store 
    // Topological Sort starting from all vertices one by 
    // one 
    for (int i = 0; i < graph->V; ++i) { 
        if (!visited[i]) { 
            topologicalSortUtil(graph, i, visited, &stack); 
        } 
    } 
  
    // Print contents of stack 
    while (stack != NULL) { 
        libmin_printf("%d ", stack->data); 
        struct Stack* temp = stack; 
        stack = stack->next; 
        libmin_free(temp); 
    } 
  
    // Free allocated memory 
    libmin_free(visited); 
    libmin_free(graph->adj); 
    libmin_free(graph); 
} 
  
// Driver program to test above functions 
int
main(void) 
{ 
  // Create a graph given in the above diagram 
  struct Graph* g = createGraph(6); 
  addEdge(g, 5, 2); 
  addEdge(g, 5, 0); 
  addEdge(g, 4, 0); 
  addEdge(g, 4, 1); 
  addEdge(g, 2, 3); 
  addEdge(g, 3, 1); 
  
  libmin_printf("Topological Sorting Order: "); 
  topologicalSort(g); 
  libmin_printf("\n");
  
  libmin_success();
  return 0; 
}
