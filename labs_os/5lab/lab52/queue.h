#ifndef queue_h
#define queue_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


struct q_el{
	double var;
	struct q_el *next;
};

typedef struct q_el q_el;

typedef struct{
	q_el *head;
	q_el *tail;
}queue;

//push to tail
//take from head

queue *create();
bool isempty(queue *q);
bool push(queue *q, const double var);
double pop(queue *q);
double first(const queue *q);
void destroy(queue *q);
void print(const queue *q);

#endif
