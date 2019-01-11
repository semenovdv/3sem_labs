#include "queue.h"


queue *create(){
	queue *q = (queue*)malloc(sizeof(queue));
	if(q == NULL) return NULL;
	
	q->head = NULL;
	q->tail = NULL;

	return q;
}

bool isempty(queue *q){
	if(q->head == NULL) return true;
	return false;
}

bool push(queue *q, const double var){
	q_el *el = (q_el*)malloc(sizeof(q_el));
	
	if(el == NULL) return false;
	
	el->var = var;
	el->next = NULL; // put to end of queue

	if(q->head == NULL){
	       	q->head = el;
		q->tail = el;
	} else{
		q->tail->next = el;
		q->tail = el;
	}
	return true;
}

double tq_first(const queue *q){
	return q->head->var;
}

double pop(queue *q){
	q_el *el = q->head;
	double var = el->var;
	q->head = q->head->next;
	free(el);
	return var;
}

void destroy(queue *q){
	q_el *el = q->head;
	while(el != NULL){
		q_el *nel = el->next;
		free(el);
		el = nel;
	}
}

void print(const queue *q){
	q_el *el = q->head;
	while(el != NULL){
		printf("%f\n", el->var);
		el = el->next;
	}
}

