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
    fprintf(stderr, "usage: %s [v] [c] [ta] [td] (v=nom, c=nbr des conteneurs, ta=tmps d’accostage, td=tmps de déchargement d'un conteneur \n", prog);
    exit(EXIT_FAILURE);
}

void create_pnav(char v, int c, int td, struct port *p, int quai)
{
    p->boats[quai].free=false;
    p->boats[quai].name = v;
    p->boats[quai].number_of_container = c;
    p->boats[quai].time_to_discharge_a_container = td;
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
    int semid = get_semaphore_id();

    //Vérifie si le port n'est pas fermé
    bool closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    if (closed)
    {
        ERROR("Le port est fermé! Je me casse.. Ciao");
        return EXIT_FAILURE;
    }
    
    //On regarde s'il y a un quai libre..
    int quai;
    if((quai = get_free_dock(p)) == -1) 
    {
        INFO("Le port est complet je dois attendre...");
        WARN("Le port est complet");
        set_semaphore_value(semid, SEM_SLEEP, 1);       
    }
    create_pnav(v, c, td, p, quai);

    //tente de rentrer 
    SEMOPS(semid,{SEM_CAPACITY,-1,0},{SEM_INSIDE,1,0}, {SEM_SLEEP,-1,0});
    //Temps d'accostage
    INFO("Le bateau est entrain d'accoster");
    usleep(ta);
    INFOF("Je suis dans le port,au quai numéro: %d",p->boats[quai].number_of_dock);
    DEBUG("bateau dans le port, il attend les camions... ");

    //attendre les camions pour le déchargement..
    //set_semaphore_value(semid, SEM_SLEEP_PNAV, 1);
    //Commencer le déchargement...
    INFO("Les camions sont là, Déchargement..");
    while (p->boats[quai].number_of_container > 0)
    {
        //Temps de déchargement:
        usleep(td);
        p->boats[0].number_of_container--;
    }
    // Sortir du port, (IPC_NOWAIT pour éviter d'éventuels hard locks)
    SEMOPS(semid, {SEM_INSIDE, -1, IPC_NOWAIT}, {SEM_SLEEP, -1, 0});
    INFO("Je suis sorti du port! Ciao");
}