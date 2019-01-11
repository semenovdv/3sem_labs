#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <zmq.h>

void help_page(){
	printf("\nc - credit account\n");
	printf("d - debet account\n");
	printf("\n");
	printf("TO ask for money print \? (d/c)\n");
	printf("To send money print \"+ (amount of money) (c/d) (user id to sent)\"\n");
	printf("To enrich your account print \"+ (amunt of money) (c/d)\"\n");
	printf("To get modey print \"- (amount of modey) (c/d)\"\n");
}


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

int main (int argc, char const *argv[]) 
{
	void* context = zmq_ctx_new();
	printf("Client Starting….\n");
	
	void* request = zmq_socket(context, ZMQ_REQ);
	zmq_connect(request, "tcp://localhost:4040");

	char initbuf[256];
	char *d = initbuf;
	int count = 0;
	int id = 0;
	char idchar[10] = {'\0'};

	do{
		initbuf[0] = '!';
		initbuf[1] = ' ';

		for(int i = 2; i < 256; i++){
			initbuf[i] = '\0';
		}

		printf("Enter your id: ");
		scanf("%s",idchar);

		id = atoi(idchar);
		for(int i = 0; i < 260; i++){
        	initbuf[i+2] = idchar[i];
		}
		zmq_send(request, initbuf, 256, 0);
		
		d = s_recv(request);
		printf("%s\n", d);
	
	} while (!strcmp(d, "Such id is already in system"));
	

	//zmq_send(request, buff, )
	getchar();
	
	for(;;) 
	{
		printf("Enter new command: ");
		char buffer[256] = {'\0'};
		fgets(buffer, 256, stdin);
		if(buffer[0] == '\0'){
			return 0;
		}
		
		if (!strcmp(buffer, "help\n")) {
			help_page();
		} else if(buffer[0] == '?'){
			char newbuffer[256] = {'\0'};
			newbuffer[0] = buffer[0];
			newbuffer[1] = buffer[1];
			newbuffer[2] = buffer[2];
			newbuffer[3] = ' ';
			for(int i = 4; i < 14; i++){
				newbuffer[i] = idchar[i-4];
			}
			zmq_send(request, newbuffer, 256, 0);
			zmq_recv(request, newbuffer, 256, 0);
			printf("%s\n", newbuffer);

		} else if(buffer[0] == '-'){
			
			int i = 0;
			while(buffer[i] != '\n'){
				i++;
			}
			buffer[i] = ' ';
			i++;
			for( int j = 0; j < 10; i++, j++){
				buffer[i] = idchar[j];
			}
			
			
			zmq_send(request, buffer, 256, 0);
			zmq_recv(request, buffer, 256, 0);
			printf("%s\n", buffer);
		} else if(buffer[0] == '+'){
			char sep[10] = " /\n";
			char send_buffer[256];
			for(int i = 0; i < 256; i++){
				send_buffer[i] = buffer[i];
			}
			char *d = buffer;
			char *nstr = strtok(d, sep);
			nstr = strtok(NULL, sep); // modey
			nstr = strtok(NULL, sep); // c/d
			nstr = strtok(NULL, sep); // id user
			int i = 0;
			if(nstr == NULL){
				while(send_buffer[i++] != '\0');
				send_buffer[i-2] = ' ';
				i--;
				for(int j = 0; j < 10; j++){
					send_buffer[i + j] = idchar[j];
				}
			} 
			zmq_send(request, send_buffer, 256, 0);
			zmq_recv(request, send_buffer, 256, 0);
			printf("%s\n", send_buffer);

		} else {
			zmq_send(request, buffer, 256, 0);
			zmq_recv(request, buffer, 256, 0);
			printf("%s\n", buffer);
			sleep(1);
			count++;
		}
		
	}
	// We never get here though.
	zmq_close(request);
	zmq_ctx_destroy(context);
	
	return 0;
}