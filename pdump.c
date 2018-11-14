//
//  pdump.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//
#define LOG_LEVEL 5
#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s\n", prog);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 1)
        usage();
    struct port *p = get_port(get_shared_memory_id(), true);
    int semid = get_semaphore_id();

    int waiting, inside, closed, cur_capacite;
    if( (waiting = semctl(semid, SEM_CAPACITY, GETNCNT)) == -1) error("semctl");
    cur_capacite = get_semaphore_value(semid, SEM_CAPACITY);
    inside = get_semaphore_value(semid, SEM_INSIDE);
    closed = get_semaphore_value(semid, SEM_CLOSED);

    INFOF(" Capacité du port: %d", p->capacity);
    INFOF(" Etat du port: %s", closed ? "fermé" : "ouvert");
    // INFOF(" Numéro de quai : %d", p->number_of_dock);
    // INFOF(" Bateau: %c", p->name);
    // INFOF(" État du bateau: %s", (p->dock ? "à quai" : "en attente"));
    // INFOF(" Nbr de conteneur: %d", p->number_of_container);
    INFOF(" Bateaux en attente: %d", waiting);
    INFOF(" À l'intérieur: %d", inside);
}
