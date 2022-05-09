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

int main(int argc, char** argv) {
	heap = (uint8_t*)malloc(HEAP_SIZE);
	assert(heap != NULL);
	OPTICK_APP("ConsoleApp");
	

	
	int width, height, channels;
	unsigned char* img = stbi_load("128.bmp", &width, &height, &channels, 0);
	if (img == NULL) {
		printf("Error in loading the image\n");
		exit(1);
	
	}
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
	
	size_t img_size = width * height * channels;


	AdjMatrix* graph = create_graph(img_size);

	rgb couleurs;
	Vector2 position;
	for (int i = 0; i < img_size; i += 3)
	{
		if (img[i] == 255 && img[i + 1] == 255 && img[i + 2] == 255)
		{
			couleurs.r = img[i];
			couleurs.g = img[i + 1];
			couleurs.b = img[i + 2];

			position.x = (i / 3) % width;
			position.y = (i / 3) / width;


			add_node(graph, &couleurs, position);
		}
	}
	

	CheckAdjacency(graph);

	Stack path = stack_init(graph->len);
	astar_AdjMatrix(graph, 0, graph->len, &path);



	int test = 0;
	for (int i = 0; i < img_size; i += 3)
	{
		// Si le pixel est blanc, assigne la valeur de couleur du graph à l'image
		if (img[i] == 255 && img[i + 1] == 255 && img[i + 2] == 255)
		{
			img[i] = graph->nodes[test].data.r;
			img[i+1] = graph->nodes[test].data.g;
			img[i+2] = graph->nodes[test].data.b;
			test++;
		}
	}

	// --------------------------------------------------------------------------------------------
	/*
	for (int i = 0; i < img_size; i+= 3)
	{
		img[i] = 255;
	}
	*/
	stbi_write_png("Test123.png", width, height, channels, img, width * channels);
	// --------------------------------------------------------------------------------------------



	
	return 0;
}
