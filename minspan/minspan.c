#include "libmin.h"

//
// Compute the minimal spanning tree of a randomly generated graph.
// This program implements Kruscal's algorithm (https://en.wikipedia.org/wiki/Kruskal%27s_algorithm)
// over a graph represented as an adjacency matrix (https://en.wikipedia.org/wiki/Adjacency_matrix).
// This algorithm works on graphs with multiple components that are not connected.
// 

// V represents the number of vertex in the graph G
const int V = 10;

// this are the names(representation) of each vertex
const char *vertName[] = {"Home","z-mall","st.pet","office","school","motel","restr.","library","airport","barber"};

// find the vertex with min distance from the unknown vertexes
int32_t
minVal(int32_t *dist, int *known)
{
	int32_t min = -1;
	int32_t distVal = INT_MAX;
			
	for (int i=0; i<V; i++)
  {
		if (distVal>dist[i] && !known[i])
    {
			distVal = dist[i];
			min = i;
		}					
	}
				
  if (min != -1)
    known[min] = TRUE;
  return min;
}

// find the shortest path from the source to all other vertexes
void
minSpanTree(int32_t graph[V][V], int32_t path[V])
{
	int32_t dist[V];

  // KNOWN[I] set to true when the algorithm has linked node I into the minimal spanning tree being built
	int known[V];
	int32_t min = 0;
	
	for (int i=0; i<V; i++)
  {
		if (min == i)
    {
			dist[i] = 0;
			known[i] = TRUE;
		}
    else
    {
			dist[i] = INT_MAX;
			known[i] = FALSE;
		}
	}
		
	for (int i=0;i<V;i++)
  {
		for (int j = 0;j<V;j++)
    {
			if (!known[j] && (graph[min][j] != 0) && (graph[min][j] < dist[j]))
      {
				dist[j] = graph[min][j];
				path[j] = min;				
			}
		}
		min = minVal(dist, known);
	}
}

//Used to initialize the graph
void
initializeData(int32_t graph[V][V])
{
	libmin_srand(10);
	for (int i=0; i < V; i++)
  {
		for (int j=0;j<V;j++)
    {
			if (i>j)
				graph[i][j] = graph[j][i];
			else if (i==j)
				graph[i][j] = 0;
			else
      {
				if (libmin_rand() % 5 == libmin_rand() % 5)
					graph[i][j] = 0;	
				else
					graph[i][j] = libmin_rand() % 10;			
			}
		}
	}
}

void
displayGraph(int32_t graph[V][V])
{
	int index = 0;
	for (int i=-1;i<V;i++)
  {
		for (int j=-1;j<V;j++)
    {
			if (i==-1)
      {
				if (j==-1)
          libmin_printf("       ");
				else
          libmin_printf("%8s", vertName[j]);
			}
      else
      {
				if(j==-1)
        {
          libmin_printf("%8s", vertName[index]);
					index++;
				}
        else
          libmin_printf("%8d", graph[i][j]);
			}
		}
    libmin_printf("\n");
	}
  libmin_printf("\n\n");
}

void
displayGraph1(int32_t graph[V][V], int32_t path[V])
{
	int index = 0;
	for (int i=-1;i<V;i++)
  {
		for (int j=-1;j<V;j++)
    {
			if (i==-1)
      {
				if (j==-1)
          libmin_printf("       ");
				else
          libmin_printf("%8s", vertName[j]);
			}
      else
      {
				if(j==-1)
        {
          libmin_printf("%8s", vertName[index]);
					index++;
				}
        else
          libmin_printf("%8d / %d", graph[i][j], path[i]);
			}
		}
    libmin_printf("\n");
	}
  libmin_printf("\n\n");
}

//Displays the path from source to destination
void
displayPath(int32_t source, int32_t dest, int32_t path[V])
{
	static int count = 0;
	
	int currPath = dest;
	
	if (count == 0)
  {
    libmin_printf("\nSource: %s\tDestination: %s\n\n", vertName[source], vertName[currPath]);
    libmin_printf("Path: %s", vertName[source]);
		count++;
	}	
	if (path[currPath] != source)
  {
		displayPath(source,path[currPath],path);
	}
  libmin_printf("-> %s", vertName[currPath]);
	if (currPath == dest)
  {
    libmin_printf("\n");
	}
}

// display the minimum spanning tree
void
displayTree(int32_t graph[V][V], int32_t path[V])
{
  int cost = 0;
  libmin_printf("minimum spanning tree:\n");
  for (int i=1; i < V; i++)
  {
    libmin_printf("  %8s <-%d-> %8s\n", vertName[i], graph[i][path[i]], vertName[path[i]]);
    cost += graph[i][path[i]];
  }
  libmin_printf("total cost = %d\n", cost);
}

int
main()
{
	// int32_t source = 0;
	// int32_t destination = 1;
	int32_t graph[V][V];
	int32_t path[V];	
	for(int i=0;i<V;i++){
		path[i]=-1;
	}
	initializeData(graph);	
	displayGraph(graph);
  minSpanTree(graph,path);

	// displayPath(source,destination,path);
	// displayGraph1(graph, path);
	displayTree(graph, path);

  libmin_success();
  return 0;
}

