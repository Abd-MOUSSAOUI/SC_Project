//
//  pcap.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s n (avec n>0 nombre de quais)\n", prog);
    exit(EXIT_FAILURE);
}

//Creation de shmid et l'ensemble des sémaphores
int create(int n)
{
    INFO("Création de la mémoire partagée…");
    int shmid = create_shared_memory();
    INFO("Création de l'ensemble des sémaphores…");
    (void)create_semaphore(n);
    return shmid;
}

//ouverture du port: get semid
int open(void)
{
    DEBUG("On met SEM_CLOSED && SEM_SLEEP à 0");
    INFO("Ouverture du port...");
    int semid = get_semaphore_id();
    set_semaphore_value(semid, SEM_CLOSED, 0);
    set_semaphore_value(semid, SEM_SLEEP, 0);
    DEBUG("SEM_CLOSED && SEM_SLEEP = 0");
    INFO("Le port est ouvert...");
    return semid;
}

//Suppression de shmid et semid
int delete(void)
{
    INFO("Suppression du port...");
    delete_shared_memory();
    delete_semaphore();
    INFO("Le port est supprimé! Ciao");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int n, shmid, semid;

    bool closed=false;
    short inside = 0;
    prog = argv[0];
    if (argc != 2)
        usage();
    n = atoi(argv[1]);
    if (n <= 0)
        usage();
    shmid = create(n);
    port *p = get_port(shmid, false);
    p->capacity = n;
    initializer(p);
    DEBUG("Creation du port..");
    INFOF("Un port qui contient %d quai(s) a été créé.", n);

    semid = open();
    // On attend que le port soit ouvert une première fois
    while(!closed || inside > 0)
    {
        DEBUG("Attend les requêtes de pnav et pfcam");
        WARN("le pcap et en mode sleep");
        SEMOPS(semid, {SEM_SLEEP, 1, 0}, {SEM_SLEEP, 1, 0});

        DEBUG("On vérifie que le port n'est pas fermé..");
        closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
        inside = (short)get_semaphore_value(semid, SEM_INSIDE);

        
        
    }
    delete();
}
