//
//  helper.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 10/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "helper.h"

noreturn void error(char *c)
{
    perror(c);
    exit(CODE_ERREUR);
}

int get_log_level(void)
{
    char *env = getenv("DEBUG_PORT");
    if (env == NULL)
        return 0;
    int level = atoi(env);
    return level >= 0 ? level : 0;
}

key_t key(void)
{
    key_t k;
    if ((k = ftok("/etc/passwd", 0)) == -1)
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
    key_t k = key();
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
    //DEBUGF("shmid = %d", shmid);
    return shmid;
}

int get_shared_memory_id(void)
{
    key_t k = key();
    int shmid;
    if ((shmid = shmget(k, 0, 0)) == -1)
        error("shmget");
    //DEBUGF("shmid = %d", shmid);
    return shmid;
}

void delete_shared_memory_id(void)
{
    int shmid = get_shared_memory_id();
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
        error("shmctl");
}

int create_semaphore(void)
{
    key_t k = key();
    int semid = semget(k, 0, 0);
    if (semid > -1)
    {
        fprintf(stderr, "Un ensemble de sémaphores existe déjà, supression de l'ancien ensemble.");
        fflush(stderr);
        if (semctl(semid, 0, IPC_RMID) == -1)
            error("semctl");
    }
    else
    {
        if (errno != ENOENT)
            error("semget");
    }

    if ((semid = semget(k, 4, IPC_CREAT | 0666)) == -1)
        error("semget");
    if (semctl(semid, 0, SETVAL, 1) == -1)
        error("semctl"); // 1 = Closed
    //DEBUGF("semid = %d", semid);
    return semid;
}

int get_semaphore_id(void)
{
    key_t k = key();
    int semid;
    if ((semid = semget(k, 0, 0)) == -1)
        error("semget");
    //DEBUGF("semid = %d", semid);
    return semid;
}

void delete_semaphore(void)
{
    int semid = get_semaphore_id();
    if (semctl(semid, 0, IPC_RMID) == -1)
        error("semctl");
}

int get_semaphore_value(int semid, unsigned short which)
{
    int val;
    if ((val = semctl(semid, which, GETVAL)) == -1)
        error("semctl");
    return val;
}

void set_semaphore_value(int semid, unsigned short which, unsigned short value)
{
    if (semctl(semid, which, SETVAL, value) == -1)
        error("semctl");
}

