
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//quitando el salto de linea
void reemplazar(char *linea){
	while(*linea != 0){
		if(*linea == '\n'){
			*linea = 0;
		}
		linea++;
	}
}

int main()
{
	int fds[2];
	//char *argv[3];
	
	char buf[100] = {0};
	char *args[10] = {0};
	pid_t cpid;
		
	printf("$ ");
	fgets(buf, 100, stdin);
	reemplazar(buf);
	// si es diferente a exit
	while (strcmp(buf, "exit") != 0){
		pid_t cpid=fork();
		pipe(fds);// creando un pipe

		// Si existe un error lo muestra
	    	if (cpid<0) { 
			perror("fork");
			exit(EXIT_FAILURE);
		}	

		//Proceso COMANDO(HIJO)
		if(cpid==0){	
			
			close(fds[1]); //close stdin
			dup(fds[1]);  // redirect standard output to the pipe table
			char read_msg[strlen(buf)+1];
			read(fds[1],read_msg,strlen(buf)+1);
			close(fds[0]);
			int res =execvp(read_msg, args);
			printf("Error de execl %d", res);
			
		} else { //write padre
			int i = 0;
			char *valor;
			args[i] = strdup(strtok(buf, " "));
			i++;		
			while( (valor = strtok(NULL, " ")) != NULL ){
				args[i] = strdup(valor);
				i++;
			}
	
			printf("%s\n", buf);
			fflush(stdout); //limpiando el buffer del teclado

			close(fds[0]); //close stdin
			dup(fds[0]);  // redirect standard input to the pipe table
			write(fds[1],buf,strlen(buf)+1);
			close(fds[1]);

			int stat =-1;
			waitpid(cpid, &stat, 0);

			printf("$ ");
			fgets(buf, 100, stdin);
			reemplazar(buf);  		
			
	    	}
		
		
	}
	
	return 1;
	
}
