#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10000
#define INICIO 4000
#define FIN 6000

typedef struct {
    int id;
    sem_t *mutex;
} Datos;

void* contar(void* arg);

int main() {
    pthread_t hilo1, hilo2;
    sem_t mutex;

    Datos datos1 = {1, &mutex};
    Datos datos2 = {2, &mutex};
    if (sem_init(&mutex, 0, 1) != 0) {
        perror("Error al inicializar el semaforo");
        return EXIT_FAILURE;
    }
    if (pthread_create(&hilo1, NULL, contar, &datos1) != 0) {
        perror("Error al crear hilo 1");
        sem_destroy(&mutex);
        return EXIT_FAILURE;
    }
    if (pthread_create(&hilo2, NULL, contar, &datos2) != 0) {
        perror("Error al crear hilo 2");
        pthread_join(hilo1, NULL);
        sem_destroy(&mutex);
        return EXIT_FAILURE;
    }
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);
    sem_destroy(&mutex);

    return 0;
}

void* contar(void* arg) {
    Datos* datos = (Datos*)arg;
    for (int i = 1; i <= N; i++) {
        if (i == INICIO) {
            sem_wait(datos->mutex);
        }
        printf("Hilo %d: %d\n", datos->id, i);
        if (i == FIN) {
            sem_post(datos->mutex);
        }
    }

    return NULL;
}