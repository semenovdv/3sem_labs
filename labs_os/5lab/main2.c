#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include <dlfcn.h>



int main(){
    void *dl_handle = dlopen("/home/denis/labs_3sem/labs_os/5lab/libqueue.so", RTLD_LAZY );

    if (!dl_handle) {
        printf( "!!! %s\n", dlerror() );
        return 0;
    }
    queue *(*func_create)(void) = dlsym( dl_handle, "create" );
    char *error = dlerror();
    if (error != NULL) {
        printf( "!!! %s\n", error );
        return 0;
    }

    bool (*func_push)(queue *q, const double var) = dlsym( dl_handle, "push" );
    error = dlerror();
    if (error != NULL) {
        printf( "!!! %s\n", error );
        return 0;
    }

    void (*func_print)(const queue *q) = dlsym( dl_handle, "print" );
    error = dlerror();
    if (error != NULL) {
        printf( "!!! %s\n", error );
        return 0;
    }    


    queue *q = (*func_create)();
    (*func_push)(q, 9.5);
    //push(q, 9.5);
    //push(q, 8.5);
    (*func_print)(q);
    //print(q);
    //printf("%f\n",pop(q) );
    //destroy(q);

    dlclose(dl_handle);
    return 0;
}
