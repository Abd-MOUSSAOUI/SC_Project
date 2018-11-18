//
//  port.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 10/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

int MAX(int X, int Y)
{
    return ((X) > (Y)) ? (X) : (Y);
}

noreturn void error(char *c)
{
    perror(c);
    exit(EXIT_FAILURE);
}

int get_log_level(void)
{
    char *env = getenv("DEBUG_PORT");
    if (env == NULL)
        return 0;
    int level = atoi(env);
    return level >= 0 ? level : 0;
}

key_t key(int i)
{
    key_t k;
    if ((k = ftok("/etc/passwd", i)) == -1)
        error("ftok");
    return k;
}


struct port *get_port(int shmid, bool readonly)
{
    int shmflg = readonly ? SHM_RDONLY : 0;
    void *p;
    if ((p = shmat(shmid, NULL, shmflg)) == (void *)-1)
        error("shmat");
    return (struct port *)p;
}


int create_shared_memory(void)
{
    key_t k;
    if ((k = ftok("/etc/passwd", 999)) == -1)
        error("ftok");
    int shmid = shmget(k, 0, 0);
    if (shmid > -1)
    {
        fprintf(stderr, "Un espace de mémoire partagé existe déjà, suppression de l'ancien espace.");
        fflush(stderr);
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
            error("shmctl");
    }
    else
    {
        if (errno != ENOENT)
            error("shmget");
    }
    if ((shmid = shmget(k, sizeof(struct port), IPC_CREAT | 0666)) == -1)
        error("shmget");
    DEBUGF("shmid = %d", shmid);
    return shmid;
}

int get_shared_memory_id(void)
{
    key_t k;
    if ((k = ftok("/etc/passwd", 999)) == -1)
        error("ftok");
    int shmid = shmget(k, 0, 0);
    if (shmid > -1)
    {
        return shmid;
    }
    return -1;
}

void delete_shared_memory(void)
{
    int shmid = get_shared_memory_id();
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        error("shmctl");
}

void create_semaphore(int n)
{
    int i, semid;
    key_t k, k1;
    k1 = key(n);
    if ((semid = semget(k1, 2, IPC_CREAT | 0666)) == -1) error("semget");

    if (semctl(semid, SEM_CAPACITY, SETVAL, n) == -1) error("semctl");
    if (semctl(semid, SEM_CLOSED, SETVAL, 1) == -1) error("semctl");

    for (i = 0; i < n; i++)
    {
        k = key(i);
        if ((semid = semget(k, 2, IPC_CREAT | 0666)) == -1) error("semget");
        if (semctl(semid, SEM_DISCHARGE_NAV, SETVAL, 0) == -1) error("semctl");
        if (semctl(semid, SEM_DISCHARGE_CAM, SETVAL, 1) == -1) error("semctl");
    }

    DEBUGF("semid = %d", semid);
}

int get_semaphore_id(int i)
{
    key_t k = key(i);
    int semid;
    if ((semid = semget(k, 0, 0)) == -1)
        error("semget");
    DEBUGF("semid = %d", semid);
    return semid;
}

void delete_semaphore(int i)
{
    int semid = get_semaphore_id(i);
    if (semctl(semid, 0, IPC_RMID) == -1)
        error("semctl");
}

int get_semaphore_value(int semid, int which)
{
    int val;
    if ((val = semctl(semid, which, GETVAL, 0)) == -1)
        error("semctl");
    return val;
}

void set_semaphore_value(int semid, int which, int value)
{
    if (semctl(semid, which, SETVAL, value) == -1)
        error("semctl");
}
