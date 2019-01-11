#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

/*
    В родиетльском процессе мы считываем команды и преоразуем,
    в дочернем записываем все в файл
*/

typedef struct{
    char buffer[2][1024];
} RECORD;


int main(int argc, char* argv[]) {
  
    if (argc == 1) {
        fprintf(stderr, "No file detected.\n");
        exit(1); // ошибка в подготовке
    }

    int status;
    pid_t pid;
    int rv;

    RECORD *mapped;

    int fd = open(argv[1], O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR);

    int f = open("record", O_RDWR | O_CREAT, S_IWUSR|S_IRUSR);

    posix_fallocate(f, 0, sizeof(RECORD));

    mapped = (RECORD*)mmap(0, sizeof(RECORD), PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);

    sigset_t set;
    int sig;
    int *sigptr = &sig;
    int ret_val;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);

    
    switch (pid = fork()) {
    case -1:
    {
        perror("cannot fork \n");
        exit(1); // ошибка в подготовке
    }
    case 0:
    {
        
        //процесс - ребенок
        
        while (true) {
            // wait signal
            
            ret_val = sigwait(&set, sigptr);
            

            if(ret_val == -1){
                perror("sigwait failed\n");
            } else {
                if(*sigptr == SIGUSR1){
                    if (strcmp((*mapped).buffer[0], "exit") == 0) break;
                
                    pid_t exec_pid;
                    int exec_back;
                    switch (exec_pid = fork()) {
                        case -1:
                        {
                            perror("cannot fork\n");
                            exit(1); // ошибка в подготовке
                        }
                        case 0:
                        {
                            dup2(fd, STDOUT_FILENO);
                            if ((*mapped).buffer[1][0] != '\0'){
                                execlp((*mapped).buffer[0],(*mapped).buffer[0], (*mapped).buffer[1], (char*)NULL);
                            } else {
                                execlp((*mapped).buffer[0],(*mapped).buffer[0], (char*)NULL);
                            }
                            
                            dup2(STDOUT_FILENO, fd);
                            exit(exec_back);
                        }
                        default:
                        {
                            wait(&exec_back);
                            fflush(stdout);
                        }

                    } // end switch
            
                }
                    
                else
                    printf("sigwait returned with sig: %d\n", *sigptr);
            }
                        
        } // end while 
        wait(&rv);
    
    }
    default:
    {
        
        // процесс - родитель
        // происходит преобразование в команду и параметры

        char buff[1024];
        char argums[2][1024]; // до 4 аргументов

        do {
            bool done = false;
            int swords = 0; // количество слов
            int schars= 0; //количество букв в слове
            int achars = 0; // всего букв  в строке

            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 1024; j++){
                    argums[i][j] = '\0';
                }
            }

            fgets(buff, sizeof (buff), stdin); 
            
            char c = buff[achars];
            while(!done){
                while(c != ' ' && c != '\n' && c != EOF) {
                    argums[swords][schars] = c;
                    schars++;
                    achars++;
                    c = buff[achars];
                } 
                swords++;
                schars = 0;
                achars++;
                c = buff[achars];
                if (buff[achars] == '\0') done = true;
            }
            argums[5][0] = (char)NULL;

            for(int i = 0 ; i < 2; i++){
                for(int j = 0; j < 1024; j++){
                    mapped->buffer[i][j] = argums[i][j];
                }
            }

            msync((void*)mapped, sizeof(RECORD), MS_ASYNC);
            status = kill(pid, SIGUSR1 );
            if ( status <  0)                                                              
                perror("kill failed");

        } while(strcmp(argums[0],"exit") != 0);

        
        exit(rv);
        
    }
    
    }
    munmap((void*)mapped, sizeof(RECORD));
    close(fd);
  
    return 0;

}
