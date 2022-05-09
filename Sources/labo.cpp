#include "labo.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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

	// ----------------------------------------------------------------- Ajouté un * avant data
	n->data = *data;
	n->position = pos;
}


void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	graph->adjGraph[fromNode][toNode] = cost;
}



double CtoEnd(AdjMatrix* graph, Node* from, Node* to)
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

						double coutPlus = CtoEnd(graph, &graph->nodes[to], &graph->nodes[endNodeIndex-1]);

						queue_push(q, &graph->nodes[to]);
						graph->nodes[to].cost = graph->adjGraph[from][to] + t->cost + coutPlus;
						graph->nodes[to].path_from = from;
					}
				}
			}
		}
	}

	t = &graph->nodes[endNodeIndex - 1];
	ColorPath(t);

	stack_push(solvedPath, t);
	while (t != &graph->nodes[startNodeIndex])
	{
		t = &graph->nodes[t->path_from];

		ColorPath(t);

		stack_push(solvedPath, t);
	}

}

void astar_AdjList(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{

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

void ColorPath(Node* node)
{
	node->data.r = 255;
	node->data.g = 0;
	node->data.b = 0;
}
