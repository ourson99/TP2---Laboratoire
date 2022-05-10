#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "labo.h"

#define OPTICK_ENABLE_GPU (0)
#include "optick.h"


Stack stack_init(size_t max_size) {
	Stack s;
	s.max = max_size;
	s.top = -1;
	s.elements = (void**)allocate(sizeof(void*) * s.max);
	return s;
}

void stack_push(Stack* s, void* element) {
	if (s->max != s->top + 1) {
		s->top++;
		s->elements[s->top] = element;
	}
}

void* stack_pop(Stack* s) {
	if (s->top > -1) {
		void* elem = s->elements[s->top];
		s->elements[s->top] = NULL;
		--s->top;
		return elem;
	}
	return NULL;
}
void queue_init(Queue* q) {
	q->next = q->prev = NULL;
	q->count = 0;
}
void queue_push(Queue* q, void* data) {
	QNode* n = (QNode*)allocate(sizeof(Node));
	n->data = data;
	n->prev = n->next = NULL;
	if (q->prev == NULL) {
		q->prev = q->next = n;
	}
	else {
		QNode* temp = q->next;
		q->next = temp->prev = n;
		n->next = temp;
	}
}
void* queue_pop(Queue* q) {
	if (q->prev != NULL) {
		QNode* temp = q->prev;
		if(temp->prev != NULL)
			temp->prev->next = NULL;
		q->prev = temp->prev;
		q->count++;
		return temp->data;
	}
}


#define HEAP_SIZE (UINT32_MAX * UINT32_MAX) * 9999999999
static uint8_t* heap = NULL;
static size_t heap_top = 0;
void* allocate(size_t size) {
	size_t old_top = heap_top;
	heap_top += size;
	assert(heap_top <= HEAP_SIZE);
	return &heap[old_top];
}


/*
	Les deux méthodes utilisées dans ce tp pour créer le chemin de sortie du labyrinthe ne sont pas d'égale performance dans ce cas-ci.

	La matrice d'adjacence vient comparer chaque node du graph (Celui-ci étant de len = pixels blancs de l'image) avec tout les autres nodes,
	astar avec cette méthode vient donc créer une execution de n x n.

	De l'autre côté, la liste d'adjacence ne créer des voisins que si elle est en contact physique avec. Dans le cas du labyrinthe, cela veut dire que
	c'est un maximum de 4 voisins. Astar dans ce cas-ci vient créer une execution de n. (avec quelques surplus, mais ce sont des chiffres statiques, donc on enlève)

	De ce point de vue, il est plus favorable d'utiliser la liste d'adjacence dans notre situation.

*/


int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");
	
	// ------------------------------------------------------------------ IMPORTANT, Matrix = 0, List = 1
	bool Matrix_Or_List = 1;
	
	if (Matrix_Or_List == 0) // -------------------------------------------Matrice--------------------------------------------------------------------------------------
	{
		int width_matrix, height_matrix, channels_matrix;
		unsigned char* img_matrix = stbi_load("31.bmp", &width_matrix, &height_matrix, &channels_matrix, 0);
		if (img_matrix == NULL) {
			printf("Error in loading the image\n");
			exit(1);

		}
		printf("Loaded img_matrix with a width of %dpx, a height of %dpx and %d channels\n", width_matrix, height_matrix, channels_matrix);

		size_t img_size_matrix = width_matrix * height_matrix * channels_matrix;


		AdjMatrix* graph = create_graph(img_size_matrix);


		rgb couleurs;
		Vector2 position;
		for (int i = 0; i < img_size_matrix; i += 3)
		{
			if (img_matrix[i] == 255 && img_matrix[i + 1] == 255 && img_matrix[i + 2] == 255)
			{
				couleurs.r = img_matrix[i];
				couleurs.g = img_matrix[i + 1];
				couleurs.b = img_matrix[i + 2];

				position.x = (i / 3) % width_matrix;
				position.y = (i / 3) / width_matrix;


				add_node(graph, &couleurs, position);
				//add_node_list(list, &couleurs, position);
			}
		}
		CheckAdjacencyMatrix(graph);
		//CheckAdjacentNode(list);
		graph->len;
		Stack path = stack_init(graph->len);
		astar_AdjMatrix(graph, 0, graph->len, &path);

		int cM = 0;
		for (int i = 0; i < img_size_matrix; i += 3)
		{
			// Si le pixel est blanc, assigne la valeur de couleur du graph à l'image
			if (img_matrix[i] == 255 && img_matrix[i + 1] == 255 && img_matrix[i + 2] == 255)
			{
				img_matrix[i] = graph->nodes[cM].data.r;
				img_matrix[i + 1] = graph->nodes[cM].data.g;
				img_matrix[i + 2] = graph->nodes[cM].data.b;
				cM++;
			}
		}

		stbi_write_png("Matrice_Adj.png", width_matrix, height_matrix, channels_matrix, img_matrix, width_matrix * channels_matrix);
	}
	else // -------------------------------------------List--------------------------------------------------------------------------------------
	{
		int width_list, height_list, channels_list;
		unsigned char* img_list = stbi_load("perfect2k.png", &width_list, &height_list, &channels_list, 0);
		if (img_list == NULL) {
			printf("Error in loading the image\n");
			exit(1);

		}
		printf("Loaded img_list with a width of %dpx, a height of %dpx and %d channels\n", width_list, height_list, channels_list);
		
		size_t img_size_list = width_list * height_list * channels_list;

		List_Adj* list = create_list(img_size_list);

		rgb couleurs;
		Vector2 position;
		for (int i = 0; i < img_size_list; i += 3)
		{
			if (img_list[i] == 255 && img_list[i + 1] == 255 && img_list[i + 2] == 255)
			{
				couleurs.r = img_list[i];
				couleurs.g = img_list[i + 1];
				couleurs.b = img_list[i + 2];

				position.x = (i / 3) % width_list;
				position.y = (i / 3) / width_list;


				add_node_list(list, &couleurs, position);
			}
		}
		printf("Pre checkAdjNode\n");

		CheckAdjacentNode(list);

		printf("Post checkAdjNode\n");

		Stack path = stack_init(list->len);

		printf("Pre astar\n");
		astar_AdjList(list, 0, list->len, &path);
		printf("Post astar\n");

		int cL = 0;
		for (int i = 0; i < img_size_list; i += 3)
		{
			// Si le pixel est blanc, assigne la valeur de couleur du graph à l'image
			if (img_list[i] == 255 && img_list[i + 1] == 255 && img_list[i + 2] == 255)
			{
				img_list[i] = list->nodes[cL].data.r;
				img_list[i + 1] = list->nodes[cL].data.g;
				img_list[i + 2] = list->nodes[cL].data.b;
				cL++;
			}
		}

		stbi_write_png("List_Adj.png", width_list, height_list, channels_list, img_list, width_list * channels_list);
	}
	
	
	return 0;
}
