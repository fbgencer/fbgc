#include "fbgc.h"


void init_static_fbgc_queue(struct fbgc_queue * queue, size_t cap, size_t block_size){
	queue->capacity = fbgc_round_to_closest_power_of_two(cap);
	queue->data = fbgc_malloc(queue->capacity*block_size);
	queue->front = queue->data;
	queue->block_size = block_size;
	queue->size = 0;
}

struct fbgc_queue * new_fbgc_queue(size_t cap, size_t block_size){

	struct fbgc_queue * queue =  (struct fbgc_queue *) fbgc_malloc(sizeof(struct fbgc_queue));
	init_static_fbgc_queue(queue,cap,block_size);
	return queue;
}

size_t size_fbgc_queue(const struct fbgc_queue * queue){
	return queue->size;
}

bool is_fbgc_queue_full(const struct fbgc_queue * queue){
	return queue->size == queue->capacity;
}

bool is_fbgc_queue_empty(const struct fbgc_queue * queue){
	return queue->size == 0;
}

static void * get_queue_end(const struct fbgc_queue * queue){
	return (uint8_t*)queue->data + (queue->capacity*queue->block_size);
}


void * fbgc_queue_front(const struct fbgc_queue * queue){
	return queue->front;
}

static void * fbgc_queue_back(const struct fbgc_queue * queue){
	//printf("Calculating back, front :%p|sz:%ld|cap:%ld|bs:%ld = Calculated:%p\n",queue->front,queue->size,queue->capacity,queue->block_size,(uint8_t*)queue->front + ((queue->size%queue->capacity)*queue->block_size) );
	
	void * back =  (uint8_t*)queue->front + ((queue->size%queue->capacity)*queue->block_size);
	if(back == get_queue_end(queue)){
		return queue->data;
	}
	return back;
	
}


void fbgc_queue_push(struct fbgc_queue * queue, const void * item){
	return fbgc_queue_push_back(queue,item);
}

void fbgc_queue_push_back(struct fbgc_queue * queue, const void * item){

	if(is_fbgc_queue_full(queue)){
		//Queue is full, capacity is already power of two,just shift it
		size_t dist = queue->data - queue->front;
		queue->capacity <<= 1;
		queue->data = fbgc_realloc(queue->data,queue->block_size*queue->capacity);
		queue->front = queue->data + dist; 
	}
	//printf("++PUSHBACK Data:%p, old back:%p\n",queue->data,fbgc_queue_back(queue) );
	memcpy(fbgc_queue_back(queue),item,queue->block_size);
	++queue->size;
	//printf("\t\t new back:%p\n",fbgc_queue_back(queue) );
}


void fbgc_queue_pop(struct fbgc_queue * queue){
	return fbgc_queue_pop_front(queue);
}
void fbgc_queue_pop_front(struct fbgc_queue * queue){
	queue->front = (uint8_t*)queue->front + queue->block_size;
	if(queue->front == get_queue_end(queue)){
		queue->front = queue->data;
	}
	//printf("--POP FRONT Data :%p, Now front is :%p\n",queue->data,queue->front);
	--queue->size;
}

void fbgc_queue_swap_with_another_queue(struct fbgc_queue * self,struct fbgc_queue * queue2 ){
	struct fbgc_queue temp;
	memcpy(&temp,self,sizeof(struct fbgc_queue));
	memcpy(self,queue2,sizeof(struct fbgc_queue));
	memcpy(queue2,&temp,sizeof(struct fbgc_queue));
}