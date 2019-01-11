#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int main(){
    queue *q = create();
    push(q, 9.5);
    push(q, 8.5);
    print(q);
    printf("%f\n",pop(q) );
    destroy(q);
}