#include "labo.h"
#include <math.h>
#include <stdlib.h>


AdjMatrix* create_graph(size_t max_nodes) {
	AdjMatrix* graph = (AdjMatrix*)allocate(sizeof(AdjMatrix));
	graph->len = 0;
	graph->max_size = max_nodes;
	graph->nodes = (Node*)allocate(sizeof(Node) * max_nodes);
	for (int i = 0; i < max_nodes; ++i) {
		Node* n = &graph->nodes[i];
		n->cost = 1;
		n->visited = 0;

		n->data.r = NULL;
		n->data.g = NULL;
		n->data.b = NULL;

		n->path_from = UINT8_MAX;
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



uint32_t CtoEnd(AdjMatrix* graph, Node* from, Node* to)
{
	
	uint32_t x = abs(from->position.x - to->position.x) * abs(from->position.x - to->position.x);
	
	uint32_t y = abs(from->position.y - to->position.y) * abs(from->position.y - to->position.y);

	uint32_t square = x + y;

	uint32_t c = sqrt(square);



	return c;
}

void astar_AdjMatrix(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{
	while (solvedPath->top != -1) // Vide la stack
	{
		stack_pop(solvedPath);
	}

	// Init et allocate Q
	Queue* q = (Queue*)allocate(sizeof(Queue));
	queue_init(q);

	// Node assignation et Coût ini à 0
	Node* t = &graph->nodes[startNodeIndex];
	queue_push(q, t); // push
	graph->nodes[startNodeIndex].cost = 0;




	while (t != NULL)
	{
		t = (Node*)queue_pop(q); // pop
		if (t == NULL)
		{
			break;
		}
		t->visited = 1;

		if (t == &graph->nodes[endNodeIndex]) // 'continue' va skip la prochaine itération de for a la ligne 108
		{
			break;
		}

		for (int e = 0; e < graph->len; e++)
		{
			for (int j = 0; j < graph->len; j++)
			{
				// On regarde si le chemin existe et qu'il est un voisin de la node actuelle
				if (graph->adjGraph[e][j] != 0 && t == &graph->nodes[e]) // Je pars de la bonne place et il y a un chemin
				{
					// Si le voisin n'est pas visité et que le coût n'est pas modifié
					// OU 
					// Coût node arrivée > coût node depart + coût chemin
					if ((graph->nodes[j].visited == 0 && graph->nodes[j].cost == UINT8_MAX) || (graph->nodes[j].cost > graph->nodes[e].cost + graph->adjGraph[e][j]))
					{
						uint32_t coutPlus = CtoEnd(graph, &graph->nodes[j], &graph->nodes[endNodeIndex]);
						// Pousse le prochain voisin non visité dans la queue, update son cost et ajuste son chemin
						queue_push(q, &graph->nodes[j]);
						graph->nodes[j].cost = graph->adjGraph[e][j] + t->cost + coutPlus;
						graph->nodes[j].path_from = e;
					}
				}
			}
		}
	}

	// Assigne t a valeur de fin pour remplir la stack dans le bon sens (À travers les rev paths)
	t = &graph->nodes[endNodeIndex];

	stack_push(solvedPath, t);
	while (t != &graph->nodes[startNodeIndex])
	{
		t = &graph->nodes[t->path_from];

		// colorie en rouge
		t->data.r = 255;
		t->data.g = 0;
		t->data.b = 0;

		stack_push(solvedPath, t);

	}
}

void astar_AdjList(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{

}
