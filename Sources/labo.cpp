#include "labo.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

List_Adj* create_list(size_t max_nodes) {
	List_Adj* list = (List_Adj*)allocate(sizeof(List_Adj));
	list->len = 0;
	list->max_size = max_nodes;
	list->nodes = (Node_adj*)allocate(sizeof(Node_adj) * max_nodes);

	

	for (int i = 0; i < max_nodes; ++i) {
		Node_adj* n = &list->nodes[i];
		n->cost = UINT64_MAX;
		n->visited = 0;
		n->len = 0;

		n->position.x = 99999;
		n->position.y = 99999;

		n->data.r = NULL;
		n->data.g = NULL;
		n->data.b = NULL;

		n->path_from = NULL;
	}


	return list;
}

AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = (AdjMatrix*)allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = (Node*)allocate(sizeof(Node) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		Node* n = &graph->nodes[i];
		n->cost = UINT64_MAX;
		n->visited = 0;

		n->data.r = NULL;
		n->data.g = NULL;
		n->data.b = NULL;

		n->path_from = UINT64_MAX;
	}
	graph->adjGraph = (int**)allocate(sizeof(int*) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		graph->adjGraph[i] = (int*)allocate(sizeof(int) * max_nodes);
		for (int y = 0; y < max_nodes; ++y) {
			graph->adjGraph[i][y] = 0;
		}
	}
	return graph;
}


void add_node(AdjMatrix* graph, rgb* data, Vector2 pos) {
	Node* n = &graph->nodes[graph->len++];

	n->data = *data;
	n->position = pos;
}



void add_node_list(List_Adj* list, rgb* data, Vector2 pos) {
	Node_adj* n = &list->nodes[list->len++];

	n->data = *data;
	n->position = pos;
}

void add_adj(List_Adj* list, int fromNode, int toNode, uint8_t cost)
{
	int from_len = list->nodes[fromNode].len;
	list->nodes[fromNode].adj[from_len] = &list->nodes[toNode];
	list->nodes[fromNode].len++;

	int to_len = list->nodes[toNode].len;
	list->nodes[toNode].adj[to_len] = &list->nodes[fromNode];
	list->nodes[toNode].len++;

	// Cost à implementer ? 
}


void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	graph->adjGraph[fromNode][toNode] = cost;
}



double CtoEnd_Matrix(Node* from, Node* to)
{
	
	double x = fabs(from->position.x - to->position.x) * fabs(from->position.x - to->position.x);
	
	double y = fabs(from->position.y - to->position.y) * fabs(from->position.y - to->position.y);

	double square = x + y;

	double c = sqrt(square);



	return c;
}

double CtoEnd_List(Node_adj* from, Node_adj* to)
{

	double x = fabs(from->position.x - to->position.x) * fabs(from->position.x - to->position.x);

	double y = fabs(from->position.y - to->position.y) * fabs(from->position.y - to->position.y);

	double square = x + y;

	double c = sqrt(square);



	return c;
}


void astar_AdjMatrix(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	while (solvedPath->top != -1)
	{
		stack_pop(solvedPath);
	}

	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);

	Node* t = &graph->nodes[startNodeIndex];
	queue_push(q, t);
	graph->nodes[startNodeIndex].cost = 0;
	

	while (t != NULL)
	{

		t = (Node*)queue_pop(q);
		if (t == NULL || (t->position.x == 0 && t->position.y == 0))
		{
			break;
		}
		t->visited = 1;

		if (t == &graph->nodes[endNodeIndex])
		{
			break;
		}

		for (int from = 0; from < graph->len; from++)
		{
			for (int to = 0; to < graph->len; to++)
			{
				// On regarde si le chemin existe et qu'il est un voisin de la node actuelle
				if (graph->adjGraph[from][to] != 0 &&
					t == &graph->nodes[from]) // Je pars de la bonne place et il y a un chemin
				{
					// Si le voisin n'est pas visité et que le coût n'est pas modifié
					// OU 
					// Coût node arrivée > coût node depart + coût chemin
					if ((graph->nodes[to].visited == 0 && 
						graph->nodes[to].cost == UINT64_MAX) ||
						(graph->nodes[to].cost > graph->nodes[from].cost + graph->adjGraph[from][to]))
					{

						double coutPlus = CtoEnd_Matrix(&graph->nodes[to], &graph->nodes[endNodeIndex-1]);

						queue_push(q, &graph->nodes[to]);
						graph->nodes[to].cost = graph->adjGraph[from][to] + t->cost + coutPlus;
						graph->nodes[to].path_from = from;
					}
				}
			}
		}
	}

	t = &graph->nodes[endNodeIndex - 1];
	ColorPathMatrix(t);

	stack_push(solvedPath, t);
	while (t != &graph->nodes[startNodeIndex])
	{
		t = &graph->nodes[t->path_from];

		ColorPathMatrix(t);

		stack_push(solvedPath, t);
	}

}

void astar_AdjList(List_Adj* list, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	while (solvedPath->top != -1)
	{
		stack_pop(solvedPath);
	}

	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);

	Node_adj* t = &list->nodes[startNodeIndex];
	queue_push(q, t);
	list->nodes[startNodeIndex].cost = 0;

	int a = 0;
	int b = 0;
	while (t != NULL)
	{
		printf("Pop #%d\n", a++);
		t = (Node_adj*)queue_pop(q);
		if (t == NULL || (t->position.x == 0 && t->position.y == 0))
		{
			break;
		}
		t->visited = 1;

		if (t == &list->nodes[endNodeIndex])
		{
			break;
		}

		
		for (int i = 0; i < t->len; i++)
		{
			if ((t->adj[i]->visited == 0 &&
				t->adj[i]->cost == UINT64_MAX) ||
				t->adj[i]->cost > t->cost)
			{
				double coutPlus = CtoEnd_List(t->adj[i], &list->nodes[endNodeIndex - 1]);

				queue_push(q, t->adj[i]);
				t->adj[i]->cost = t->cost + coutPlus;
				t->adj[i]->path_from = t;
			}
		}

	}

	t = &list->nodes[endNodeIndex-1];
	ColorPathList(t);

	stack_push(solvedPath, t);
	while (t != &list->nodes[startNodeIndex])
	{
		t = t->path_from;

		ColorPathList(t);

		stack_push(solvedPath, t);
	}
}

void CheckAdjacencyMatrix(AdjMatrix* graph)
{
	for (int i = 0; i < graph->len; i++)
	{
		//left Adj
		if (i != 0 && // Si pas pixel blanc #1 (Erreur except levée)
			graph->nodes[i].position.x - 1 == graph->nodes[i - 1].position.x &&
			graph->nodes[i].position.y == graph->nodes[i - 1].position.y)
		{
			add_edge(graph, i, i - 1, 1);
			add_edge(graph, i - 1, i, 1);
		}

		//top Adj
		if (graph->nodes[i].position.y != 0) // Si pas 1er etage
		{
			int pos = i;
			while (pos != -1 &&
				graph->nodes[pos].position.y != -1)
			{
				if (graph->nodes[i].position.x == graph->nodes[pos].position.x &&
					graph->nodes[i].position.y - 1 == graph->nodes[pos].position.y)
				{
					add_edge(graph, i, pos, 1);
					add_edge(graph, pos, i, 1);
				}

				pos--;
			}
		}
	}
}

void CheckAdjacentNode(List_Adj* list)
{
	for (int i = 0; i < list->len; i++)
	{
		//left Adj
		if (i != 0 && // Si pas pixel blanc #1 (Erreur except levée)
			list->nodes[i].position.x - 1 == list->nodes[i - 1].position.x &&
			list->nodes[i].position.y == list->nodes[i - 1].position.y)
		{
			add_adj(list, i, i - 1, 1);
		}

		//top Adj
		if (list->nodes[i].position.y != 0) // Si pas 1er etage
		{
			int pos = i;
			while (pos != -1 &&
				list->nodes[pos].position.y != -1)
			{
				if (list->nodes[i].position.x == list->nodes[pos].position.x &&
					list->nodes[i].position.y - 1 == list->nodes[pos].position.y)
				{
					add_adj(list, i, pos, 1);
				}

				pos--;
			}
		}
	}
}

void ColorPathMatrix(Node* node)
{
	node->data.r = 255;
	node->data.g = 0;
	node->data.b = 0;
}

void ColorPathList(Node_adj* node)
{
	node->data.r = 255;
	node->data.g = 0;
	node->data.b = 0;
}
