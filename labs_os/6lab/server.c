#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <zmq.h>

#define MIN_ACC -1000

char *s_recv (void *socket) {
    char buffer [256];
    int size = zmq_recv (socket, buffer, 255, 0);
    if (size == -1)
        return NULL;
    if (size > 255)
        size = 255;
    buffer [size] = '\0';
    return strndup(buffer, sizeof(buffer)-1); //in *nix */
    //return strdup (buffer);
}

int main(){
    void* context = zmq_ctx_new();
	void* respond = zmq_socket(context, ZMQ_REP);
	
	int rc = zmq_bind(respond, "tcp://*:4040");
	assert (rc == 0);

	printf("Starting…\n");

	tnode *tree = NULL;

    for(;;) 
	{
		char buffer[256];
		char *d = buffer;
		
		d = s_recv(respond);

		
		printf("Recieved %s\n", d);
		if(strcmp("", d)){
		
			char sep[10] = " /";

			char *nstr;
			nstr = strtok(d, sep);

			// getting operation 
			if(nstr == NULL){
				printf("OK\n");
			} else
			if (!strcmp(nstr, "+")){
				nstr = strtok(NULL, sep);

				//getting sum 
				char *ptr;
				long sum_to_append = strtol(nstr, &ptr, 10);
				printf("Summ to be added: %ld\n", sum_to_append);

				// getting account
				nstr = strtok(NULL, sep);
				if (!strcmp(nstr, "d")) {
					printf("Account to add: d");
					nstr = strtok(NULL, sep);
					int userid = atoi(nstr);
					tnode **d = find_node(&tree, userid);
					if(d == NULL){
						zmq_send(respond, "No user with such id", 20, 0);
					} else {
						(*d)->debet_acc += sum_to_append;
						zmq_send(respond, "Added", 5, 0);
					}
			
				} else if(!strcmp(nstr, "c")) {
					printf("Account to add: c");
					nstr = strtok(NULL, sep);
					int userid = atoi(nstr);
					tnode **d = find_node(&tree, userid);
					if(d == NULL){
						zmq_send(respond, "No user with such id", 20, 0);
					} else {
						(*d)->credit_acc += sum_to_append;
						zmq_send(respond, "Added", 5, 0);
					}
				}else {
					zmq_send(respond, "No such account (chech d/c)", 27, 0);
				}

				printf("OK\n");
			} else if(!strcmp(nstr, "!")) {
				nstr = strtok(NULL, sep);
				int idinsert = atoi(nstr);
				tnode **d = find_node(&tree, idinsert);
				if (d == NULL){
					add(&tree, idinsert);
					zmq_send(respond, "User added", 10, 0);
					printf("User added\n");
				} else {
					zmq_send(respond, "Such id is already in system", 28 ,0);
					printf("Already in system\n");
				}
				printf("OK\n");
			} else if(!strcmp(nstr, "?")) {
				nstr = strtok(NULL, sep);
				if (!strcmp(nstr, "d")){
					nstr = strtok(NULL, sep);
					int idinsert = atoi(nstr);
					printf("id is: %d\n", idinsert);
					tnode **d = find_node(&tree, idinsert);

					const int n = snprintf(NULL, 0, "There is %ld on your d account", (*d)->debet_acc);
					assert(n > 0);
					char buf[n+1];
					int c = snprintf(buf, n+1, "There is %ld on your d account", (*d)->debet_acc);
					assert(buf[n] == '\0');
					assert(c == n);

					zmq_send(respond, buf, n, 0);
					printf("OK, sent\n");
				} else if (!strcmp(nstr, "c")) {
					nstr = strtok(NULL, sep);
					int idinsert = atoi(nstr);
					tnode **d = find_node(&tree, idinsert);

					const int n = snprintf(NULL, 0, "There is %ld on your c account", (*d)->credit_acc);
					assert(n > 0);
					char buf[n+1];
					int c = snprintf(buf, n+1, "There is %ld on your c account", (*d)->credit_acc);
					assert(buf[n] == '\0');
					assert(c == n);

					zmq_send(respond, buf, n, 0);
				} else {
					zmq_send(respond, "No such account (chech d/c)", 27, 0);
				}
			} else if (!strcmp(nstr, "-")) {
				nstr = strtok(NULL, sep);
				char *ptr;
				long sum_to_minus = strtol(nstr, &ptr, 10);
				nstr = strtok(NULL, sep);

				if (!strcmp("d", nstr)) {
					nstr = strtok(NULL, sep);
					int idinsert = atoi(nstr);
					printf("id is: %d\n", idinsert);
					tnode **d = find_node(&tree, idinsert);
					if(d == NULL){
						zmq_send(respond, "No user with such id", 20, 0);
					} else {
						if ((*d)->debet_acc == MIN_ACC){
							zmq_send(respond, "Reached the minimum value of the acc", 36, 0);
						} else {
							if (((*d)->debet_acc - sum_to_minus) < MIN_ACC){
								zmq_send(respond, "Impossible to do, limit will be reached", 39, 0);
							} else {
								(*d)->debet_acc -= sum_to_minus;
								zmq_send(respond, "Done!!!!!!!!!!", 4, 0);
							}
						}
						
					}
				} else if (!strcmp("c", nstr)) {
					nstr = strtok(NULL, sep);
					int idinsert = atoi(nstr);
					printf("id is: %d\n", idinsert);
					tnode **d = find_node(&tree, idinsert);
					if(d == NULL){
						zmq_send(respond, "No user with such id", 20, 0);
					} else {
						if((*d)->credit_acc - sum_to_minus < 0){
							long ostatok = (*d)->credit_acc - sum_to_minus; // otricatelinoye
							if ((*d)->debet_acc == MIN_ACC){
								zmq_send(respond, "Reached the minimum value of the acc", 36, 0);
							} else {
								if (((*d)->debet_acc + ostatok) < MIN_ACC){
									zmq_send(respond, "Impossible to do, limit will be reached", 39, 0);
								} else {
									(*d)->debet_acc += ostatok;
									(*d)->credit_acc = 0;
									zmq_send(respond, "Done!!!!!!!", 4, 0);
								}
							}
						} else {
							(*d)->credit_acc = 0;
							zmq_send(respond, "Done!!!!!!!!!11", 4, 0);
						}

					}
				} else {
					zmq_send(respond, "No such account (chech d/c)", 27, 0);
				}

			}
		}
		
		
	}
	zmq_close(respond);
	zmq_ctx_destroy(context);
	
	return 0;
}
