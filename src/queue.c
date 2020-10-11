// This file is part of fbgc

// fbgc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// fbgc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with fbgc.  If not, see <https://www.gnu.org/licenses/>.
#include "fbgc.h"


void init_static_fbgc_queue_with_allocater(struct fbgc_queue * queue, size_t cap, size_t block_size, void * (*allocator)(size_t )){
	queue->content =(uint8_t*)allocator(fbgc_round_to_closest_power_of_two(cap)*block_size);
	queue->front = queue->content;
	queue->block_size = block_size;
	queue->size = 0;
}

void init_static_fbgc_queue(struct fbgc_queue * queue, size_t cap, size_t block_size){
	assert(queue);
	queue->content =(uint8_t*)fbgc_malloc(fbgc_round_to_closest_power_of_two(cap)*block_size);
	queue->front = queue->content;
	queue->block_size = block_size;
	queue->size = 0;
}

struct fbgc_queue * new_fbgc_queue(size_t cap, size_t block_size){
	struct fbgc_queue * queue =  (struct fbgc_queue *) fbgc_malloc(sizeof(struct fbgc_queue));
	init_static_fbgc_queue(queue,cap,block_size);
	return queue;
}

struct fbgc_queue * from_array_new_fbgc_queue(const void * array_ptr, size_t array_len, size_t block_size){
	struct fbgc_queue * queue =  (struct fbgc_queue *) fbgc_malloc(sizeof(struct fbgc_queue));
	init_static_fbgc_queue(queue,array_len,block_size);
	queue->size = array_len;
	memcpy(queue->content,array_ptr,array_len*block_size);
	return queue;
}


size_t capacity_fbgc_queue(const struct fbgc_queue * queue){
	return capacity_fbgc_raw_memory(queue->content,queue->block_size);
}

size_t size_fbgc_queue(const struct fbgc_queue * queue){
	return queue->size;
}

bool is_full_fbgc_queue(const struct fbgc_queue * queue){
	return queue->size == capacity_fbgc_queue(queue);
}

bool is_empty_fbgc_queue(const struct fbgc_queue * queue){
	return queue->size == 0;
}

static void * get_queue_end(const struct fbgc_queue * queue){
	return queue->content + (capacity_fbgc_queue(queue)*queue->block_size);
}


void * front_fbgc_queue(const struct fbgc_queue * queue){
	return queue->front;
}

void * back_fbgc_queue(const struct fbgc_queue * queue){	
	void * back =  (uint8_t*)queue->front + ((queue->size%capacity_fbgc_queue(queue))*queue->block_size);
	if(back == get_queue_end(queue)){
		return queue->content;
	}
	return back;
}


void push_fbgc_queue(struct fbgc_queue * queue, const void * item){
	return push_back_fbgc_queue(queue,item);
}

void push_back_fbgc_queue(struct fbgc_queue * queue, const void * item){

	if(is_full_fbgc_queue(queue)){
		//Queue is full, capacity is already power of two,just shift it
		size_t dist = queue->content - (uint8_t*)queue->front;
		queue->content = (uint8_t*)fbgc_realloc(queue->content,queue->block_size*(capacity_fbgc_queue(queue)<<1));
		queue->front = queue->content + dist; 
	}
	//printf("++PUSHBACK Data:%p, old back:%p\n",queue->content,fbgc_queue_back(queue) );
	memcpy(back_fbgc_queue(queue),item,queue->block_size);
	++queue->size;
	//printf("\t\t new back:%p\n",fbgc_queue_back(queue) );
}


void pop_fbgc_queue(struct fbgc_queue * queue){
	return pop_front_fbgc_queue(queue);
}
void pop_front_fbgc_queue(struct fbgc_queue * queue){
	queue->front = (uint8_t*)queue->front + queue->block_size;
	if(queue->front == get_queue_end(queue)){
		queue->front = queue->content;
	}
	//printf("--POP FRONT Data :%p, Now front is :%p\n",queue->content,queue->front);
	--queue->size;
}

void swap_with_another_queue_fbgc_queue(struct fbgc_queue * self,struct fbgc_queue * queue2 ){
	//If queue2 is null return 
	if(queue2 == NULL) return;

	struct fbgc_queue temp;
	memcpy(&temp,self,sizeof(struct fbgc_queue));
	memcpy(self,queue2,sizeof(struct fbgc_queue));
	memcpy(queue2,&temp,sizeof(struct fbgc_queue));
}


struct fbgc_queue * copy_queue_fbgc_queue(struct fbgc_queue * self){
	struct fbgc_queue * ret = new_fbgc_queue(capacity_fbgc_queue(self),self->block_size);
	if(ret){
		memcpy(ret->content,self->content,self->block_size*self->size);
	}
	return ret;
}

bool iterator_fbgc_queue_front_to_back(struct fbgc_queue * queue, void ** it){

	printf("f:%p b:%p|*it:%p\n",front_fbgc_queue(queue),back_fbgc_queue(queue),*it);

	if(it == NULL) return 0;
	if(*it == NULL){
		*it = queue->front;
		return 1;
	}
	if(*it == back_fbgc_queue(queue))
		return 0;

	//Assuming it is already assigned
	*it = (uint8_t*)*it + queue->block_size;
	if(*it == get_queue_end(queue)){
		*it = queue->content;
	}

	if(*it == back_fbgc_queue(queue))
		return 0;
	
	return 1;
}


void queue_test(){

	double arr[] = {3.123,50.23,20,30,40};
	struct fbgc_queue * q = from_array_new_fbgc_queue(arr,5,sizeof(double));
	pop_fbgc_queue(q);
	push_fbgc_queue(q,arr);
	
	while(!is_empty_fbgc_queue(q)){
		double * a = (double*)front_fbgc_queue(q);
		printf("Data : %g\n",*a );
		pop_fbgc_queue(q);
	}	
}