#include <fcntl.h>      // Para open()
#include <pthread.h>    // Para pthread_create(), pthread_join()
#include <string.h>     // Para strlen()
#include <stdio.h>      // Para printf()
#include <stdint.h>     // Para uint64_t
#include <sys/mman.h>   // Para mmap(), mprotect()
#include <sys/types.h>  // Para pid_t
#include <sys/stat.h>   // Para stat()
#include <sys/wait.h>   // Para waitpid()
#include <sys/ptrace.h> // Para ptrace()
#include <unistd.h>     // Para getpid(), kill(), getpagesize()

int f;                  // Descriptor de archivo
void *map;              // Mapeo de memoria
pid_t pid;              // ID del proceso
pthread_t pth;          // Hilo
struct stat st;         // Información del archivo

// Hilo para ejecutar madvise()
void *madviseThread(void *arg) {
    int i, c = 0;
    for (i = 0; i < 200000000; i++) {
        c += madvise(map, 100, MADV_DONTNEED);
    }
    printf("madvise %d\n\n", c);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    // Abrir archivo para lectura
    f = open(argv[1], O_RDONLY);
    fstat(f, &st);

    // Mapear archivo en memoria
    map = mmap(NULL, st.st_size + sizeof(long), PROT_READ, MAP_PRIVATE, f, 0);
    printf("mmap %lx\n\n", (unsigned long) map);

    // Crear proceso hijo
    pid = fork();

    if (pid) { // Padre
        waitpid(pid, NULL, 0);

        int i, o, c = 0, l = strlen("Vaca Moo");
        for (i = 0; i < 10000 / l; i++) {
            for (o = 0; o < l; o++) {
                uint64_t *addr = (uint64_t *) (map + o);
                uint64_t val = *((uint64_t *) ("Vaca Moo" + o));
                val = (val << 8) | (val >> 56);
                c += ptrace(PTRACE_POKEDATA, pid, addr, val);
            }
        }

        printf("ptrace %d\n\n", c);
    } else { // Hijo
        pthread_create(&pth, NULL, madviseThread, NULL);

        // Establecer ptrace en el proceso hijo
        ptrace(PTRACE_TRACEME);

        // Detener el proceso hijo
        kill(getpid(), SIGSTOP);

        // Esperar al hilo
        pthread_join(pth, NULL);
    }

    return 0;
}
