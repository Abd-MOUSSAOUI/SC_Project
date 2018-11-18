//
//  pnav.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s [v] [c] [ta] [td] (v=nom, c=nbr des conteneurs, ta=tmps d’accostage, td=tmps de déchargement d'un conteneur \n", prog);
    exit(EXIT_FAILURE);
}

void create_pnav(char v, int c, int td, struct port *p, int quai)
{
    p->boats[quai].name = v;
    p->boats[quai].number_of_container = c;
    p->boats[quai].time_to_discharge_a_container = td;
}

int get_free_dock(struct port *p)
{
    int i;
    for (i = 0; i < p->capacity; i++)
    {
        if (p->boats[i].free)
        {
            p->boats[i].free = false;
            p->inside++;
            return i;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 5)
        usage();

    int c; long int ta, td;
    char v;
    v = argv[1][0];
    c = atoi(argv[2]);
    ta = atoi(argv[3]);
    td = atoi(argv[4]);

    if (c <= 0 || ta < 0 || td < 0)
        usage();

    struct port *p = get_port(get_shared_memory_id(), false);
    int semid = get_semaphore_id(p->capacity);

    //Vérifie si le port n'est pas fermé
    bool closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    if (closed)
    {
        ERROR("Le port est fermé! Je me casse.. Ciao");
        return EXIT_FAILURE;
    }

    //tente de rentrer
    SEMOPS(semid, {SEM_CAPACITY, -1, 0});

    //Vérifie si le port est toujours ouvert..
    closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    if (closed)
    {
        ERROR("Le port est fermé! Je me casse.. Ciao");
        return EXIT_FAILURE;
    }

    int quai = get_free_dock(p);

    //Temps d'accostage
    INFO("Le bateau est entrain d'accoster");
    usleep(ta*1000);
    create_pnav(v, c, td, p, quai);
    INFOF("quai num:%d",p->boats[quai].number_of_dock);

    DEBUG("bateau dans le port, il attend les camions... ");
    
    int sem_id = get_semaphore_id(quai);
    //Commencer le déchargement...
    INFO("Déchargement..");
    int tc = 0;
    while (p->boats[quai].number_of_container > 0)
    {
        //attendre les camions pour le déchargement...
        SEMOPS(sem_id, {SEM_DISCHARGE_NAV, -1, 0});
        //Temps de déchargement..
        usleep(MAX(tc, td) * 1000);
        p->boats[quai].number_of_container--;
        td = p->boats[quai].time_to_charge_a_container;
        SEMOPS(sem_id, {SEM_DISCHARGE_CAM, 1, 0});
    }
    INFO("Déchargement fini..");
    // Sortir du port
    p->boats[quai].name = '\0';
    p->boats[quai].free = true;
    SEMOPS(semid, {SEM_CAPACITY, 1, IPC_NOWAIT});
    p->inside--;
    INFO("Je suis sorti du port! Ciao");
    return 0;
}