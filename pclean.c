//
//  pclean.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s (ne prends aucun arg) \n", prog);
    exit(EXIT_FAILURE);
}

void delete_sem_if_exist(int i)
{
    key_t k = key(i);
    int semid = semget(k, 0, 0);
    if (semid > -1)
    {
        INFO("Un ensemble de sémaphores existe encore, supression...");
        if (semctl(semid, 0, IPC_RMID) == -1)
            error("semctl");
    }
    else
        INFO("Supression des sémaphores: SUCCESS")
}

void delete_shm_if_exist(void)
{
    key_t k = key(999);
    int shmid = shmget(k, 0, 0);
    if (shmid > -1)
    {
        INFO("Un espace de mémoire partagé existe encore, suppression...");
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
            error("shmctl");
    }
    else
        INFO("Supression de mémoire partagé: SUCCESS")
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 1)
        usage();
    int id = get_shared_memory_id();
    int i;

    if (id != -1)
    {
        struct port *p = get_port(id, true);
        for (i = 0; i <= p->capacity; i++)
        {
            delete_sem_if_exist(i);
        }
        delete_shm_if_exist();
    }
    
    return 0;
}