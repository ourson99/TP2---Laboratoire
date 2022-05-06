#include <stdint.h>
#include <assert.h>

void* allocate(size_t size);

typedef struct {
	void** elements;
	int top;
	size_t max;
} Stack;

Stack stack_init(size_t max_size);
void stack_push(Stack* s, void* element);
void* stack_pop(Stack* s);

typedef struct QNode QNode;
typedef struct QNode {
	void* data;
	QNode* prev;
	QNode* next;
	size_t count;
}Queue;


void queue_init(Queue* q);
void queue_push(Queue* q, void* data);
void* queue_pop(Queue* q);


typedef struct {
	int x;
	int y;
} Vector2;


typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;

typedef struct {
	uint8_t cost;
	Vector2 position;

	rgb data;

	uint64_t path_from;
	uint8_t visited;
} Node;
typedef struct AdjMatrix AdjMatrix;

struct AdjMatrix {
	int** adjGraph;// Essentiellement: int[][]
	Node* nodes;
	size_t len;
	size_t max_size;
};


AdjMatrix* create_graph(size_t max_nodes);

void add_node(AdjMatrix* graph, rgb* data, Vector2 pos);

void add_edge(AdjMatrix* graph, int fromNode, int toNode, uint8_t cost);

void astar_AdjMatrix(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath);

void astar_AdjList(AdjMatrix* graph, int startNodeIndex, int endNodeIndex, Stack* solvedPath);

uint32_t CtoEnd(AdjMatrix* graph, Node* from, Node* to);