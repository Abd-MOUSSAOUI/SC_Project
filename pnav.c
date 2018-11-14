//
//  pnav.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"
#include <time.h>

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s [v] [c] [ta] [td] (v=nom, c=nbr de conts, ta=tmps d’accostage, td=tmps de dechargement d'une cont \n", prog);
    exit(EXIT_FAILURE);
}

struct navire *create_pnav(char v, int c, int ta, int td)
{
    struct navire *b = get_navire(get_shared_memory_id(), false);
    b->name=v;
    b->number_of_container=c;
    b->time_to_docking=ta;
    b->time_to_discharge_a_container=td;
    return (struct navire *)b;
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 5)
        usage();

    int c; long int ta, td;
    char v;
    v = argv[1][1];
    c = atoi(argv[2]);
    ta = atoi(argv[3]);
    td = atoi(argv[4]);

    if (c <= 0 || ta < 0 || td < 0)
        usage();
    struct navire *n = create_pnav(v,c,ta,td);
    struct port *p = get_port(get_shared_memory_id(), true);
    int semid = get_semaphore_id();

    // Vérifie le nombre de bateau dans le port
    int inside;
    if((inside = semctl(semid, SEM_CAPACITY, GETNCNT)) == -1) error("semctl");
    INFOF("%d bateaux sont dans le port.", inside);
    if (inside >= p->capacity)
    {
        ERROR("Le port est complet ! Ciao.");
        return EXIT_FAILURE;
    }
    else{
        // Demander un numéro de quai et l'autorisation pour rentrer. 
        //TODO
        //Temps d'accostage
        usleep(ta);
        //tente de rentrer 
        SEMOPS(semid, {SEM_CAPACITY, -1, 0}, {SEM_INSIDE, 1, 0}, {SEM_SLEEP, -1, 0});
        INFO("Je suis dans le port, j'attends les camions... ");
    }

    while (n->number_of_container > 0)
    {
        SEMOPS(semid, {SEM_SLEEP, 0, 0}, {SEM_SLEEP, 1, 0});
    }
    // Sortir du port, (IPC_NOWAIT pour éviter d'éventuels hard locks)
    SEMOPS(semid, {SEM_INSIDE, -1, IPC_NOWAIT}, {SEM_SLEEP, -1, 0});
    INFO("Je suis sorti du port! Ciao");
}