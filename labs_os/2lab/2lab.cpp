#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
/*
    В родиетльском процессе мы считываем команды и преоразуем,
    в дочернем записываем все в файл
*/


int main(int argc, char* argv[]) {
  
    int status;
    pid_t pid;
    int rv;

    int file_pipes[2];
    char buffer[2][1024];

    int fd = open(argv[1], O_CREAT|O_WRONLY, S_IWUSR|S_IRUSR);


    if (argc == 1) {
        fprintf(stderr, "No file detected.\n");
        exit(1); // ошибка в подготовке
    } 

    if (pipe(file_pipes) != 0) {
        perror("Pipes: ");
        exit(1); // ошибка в подготовке
    }

    
    
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
            read(file_pipes[0], buffer, sizeof(buffer));
            if (strcmp(buffer[0], "exit") == 0) break;
         
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
                    if (buffer[1][0] != '\0'){
                        execlp(buffer[0],buffer[0], buffer[1], (char*)NULL);
                    } else {
                        execlp(buffer[0],buffer[0], (char*)NULL);
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

            write(file_pipes[1], argums, sizeof(argums));
        } while(strcmp(argums[0],"exit") != 0);

        
        exit(rv);
        
    }
    
    }
    close(fd);
  
    return 0;

}