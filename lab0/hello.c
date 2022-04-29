#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define LINESIZE 256

void read_file(char filename[], char buffer[], unsigned buffer_size)
{
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fread(buffer, 1, buffer_size, f);
        fclose(f);
    }
}

int main(int argc, char *argv[])
{
    int nbytes, fd[2];
    pid_t pid;
    char line[LINESIZE];

    if (pipe(fd) < 0)
    {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        /* parent */
        close(fd[READ_END]); // fechar leitura pq so podemos fazer uma coisa de cada vez: ler ou escrever
        printf("Parent process with pid %d\n", getpid());
        printf("Messaging the child process (pid %d):\n", pid);

//READ FILE
        char buf[256]; // temos de usar um buffer auxiliar e n o line[] ja existente pq n funciona
        read_file("txt.txt", buf, 256); // ler de um ficheiro para o buffer 256 posicoes de memoria (o limite do pipe)

        snprintf(line, LINESIZE, "Hello! I'm your parent pid %d!\n", getpid());

        if ((nbytes = write(fd[WRITE_END], buf, strlen(line))) < 0) // escrever no pipe (aqui troca se line por buf --> não funciona com o line diretamente)
        {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }

        close(fd[WRITE_END]); // ja escrevemos ent podemos fechar a escrita

        /* wait for child and exit */
        if (waitpid(pid, NULL, 0) < 0)
        {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
        }


        exit(EXIT_SUCCESS);
    }

    else
    {
        /* child */
        close(fd[WRITE_END]); // vamos querer ler, ent fechamos a escrita
        printf("Child process with pid %d\n", getpid());
        printf("Receiving message from parent (pid %d):\n", getppid());
        if ((nbytes = read(fd[READ_END], line, LINESIZE)) < 0) // ler pipe
        {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd[READ_END]); // podemos fechar agora a leitura pq ja n estamos a usar

        /* write message from parent */
        write(STDOUT_FILENO, line, nbytes); // dar print do conteudo de LINE
        printf("\n");

        // printf("%s",line); // equivalente ao write(...);
        /* exit gracefully */
        exit(EXIT_SUCCESS);
    }
}

/*
SIGNALS, PIPES, SOCKETS --> Metodos de comunicacao entre processos. Há mais como files e fifos mas n vamos ver.

SIGNALS

enviar informação (parecido com interrupts)

o handle destes signals costuma já estar definido. Por ex, ctrl-c já esta definido.

signal é limitado. Só dá para mandar flags especificas predefinidas.

sinais tmb são assincronos --> podem causar race conditions

signal usam pipes (unidirecionais)




Sockets permitem comunicação verbosa entre processos



fifos sao mto parecidos. Pipes são meio que fifos anonimos

*/
