/* Practical 4 : Deadlock-free Dining Philosophers using semaphores */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define ROUNDS 2

sem_t fork_sem[N]; /* one semaphore per fork */
sem_t room;        /* at most N-1 philosophers may try to eat -> no deadlock */

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id, right = (id + 1) % N;
    for (int r = 1; r <= ROUNDS; r++) {
        printf("Philosopher %d is THINKING\n", id + 1);
        usleep(200000 + id * 50000);

        printf("Philosopher %d is HUNGRY\n", id + 1);
        sem_wait(&room);
        sem_wait(&fork_sem[left]);
        sem_wait(&fork_sem[right]);

        printf("Philosopher %d is EATING (round %d) using forks %d and %d\n", id + 1, r, left + 1,
               right + 1);
        usleep(400000);

        sem_post(&fork_sem[left]);
        sem_post(&fork_sem[right]);
        sem_post(&room);
        printf("Philosopher %d finished eating, released forks %d and %d\n", id + 1, left + 1, right + 1);
    }
    return NULL;
}

int main(void) {
    pthread_t t[N];
    int id[N];
    for (int i = 0; i < N; i++) sem_init(&fork_sem[i], 0, 1);
    sem_init(&room, 0, N - 1);

    printf("Dining Philosophers (%d philosophers, %d rounds each)\n\n", N, ROUNDS);
    for (int i = 0; i < N; i++) { id[i] = i; pthread_create(&t[i], NULL, philosopher, &id[i]); }
    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);

    printf("\nAll philosophers have eaten. No deadlock occurred.\n");
    return 0;
}
