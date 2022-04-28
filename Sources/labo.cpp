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
		n->cost = UINT8_MAX;
		n->graph_group = UINT8_MAX;
		n->visited = 0;
		n->data = "NONE";
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

void add_node(AdjMatrix* graph, void* data, Vector2 pos) {
	Node* n = &graph->nodes[graph->len++];
	n->data = data;
	n->position = pos;
}

void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost)
{
	graph->adjGraph[fromNode][toNode] = cost;
}


/*
* Programmer l'algorithme de fibonacci.
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/
long long fibonacci(int n)
{
	if (n == 0 || n == 1)
	{
		return n;
	}
	return fibonacci(n-1) + fibonacci(n-2);
}

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long [].
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647
*/

long long fibonacci_memoization(int n)
{
	static 	long long tab[100];
	if (n == 0) return 0;
	if (n == 1) return 1;

	if (tab[n] != 0)
	{
		return tab[n];
	}


	tab[n] = fibonacci(n - 1) + fibonacci(n - 2);

	return tab[n];
}

/*
* Programmer l'algorithme de fibonacci avec la memoization en utilisant une variable static fib_cache, implémenter dans la fonction. Son type sera long long**.
* Allouer de la memoire pour fib_cache en utilisant allocate. Après avoir trouver un resultat, pour le mettre dans fib_cache, allouer un int* avec malloc puis ajouter le a fib_cache.
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
* Faire attention lorsque vous utilisez un type pour une variable qui représente un nombre, int ne dépasse pas 2^31-1 ou 2,147,483,647.
*/
long long fibonacci_memoization_malloc(int n)
{
	if (n == 0 || n == 1)
	{
		return n;
	}

	static 	long long ** tab = NULL; // Creation tableau de pointeurs long long de 100 pos
	if (tab == NULL)
	{
		tab = (long long**)allocate(sizeof(long long*) * 100);
	}


	long long* rslt = (long long*)malloc(sizeof(long long*)); // Creation d'un pointeur long long
	*rslt = fibonacci_memoization_malloc(n - 1) + fibonacci_memoization_malloc(n - 2); // Assignation au déréférencement du pointeur
	tab[n] = rslt; // On pousse l'adresse dans la table.


	return *rslt;
}
/*
	L'idée ici est que l'on crée un tableau de pointeurs de 100 positions.
	Par la suite, on insère des pointeurs qui eux, pointent dans des endroits non-linéaires dans la donnée.
	Cela va venir ralentir le système considérablement.
*/







/*
* Construire les groupes de nodes
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/
/*
void build_groups(AdjMatrix* graph)
{

}
*/
/*
* Aller chercher votre implémentation d'astar. Faire des test de profiling avec votre implémentation.
* Essayer de changer le packing de la struct Node et voir si cela a un impact.
* Essayer d'utiliser la fonction avec build_groups et sans build_groups
* Utiliser OPTICK_EVENT(); pour enregistrer la fonction dans le profiler
*/
/*
void astar(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath)
{

}
*/