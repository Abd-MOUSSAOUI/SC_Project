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
int create(void)
{
    INFO("Création de la mémoire partagée…");
    int shmid = create_shared_memory();
    INFO("Création de l'ensemble des sémaphores…");
    (void)create_semaphore();
    return shmid;
}

//ouverture du port get semid
int open(void)
{
    INFO("Ouverture du port...");
    int semid = get_semaphore_id();
    set_semaphore_value(semid, SEM_CLOSED, 0);
    set_semaphore_value(semid, SEM_SLEEP, 0);
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
    prog = argv[0];
    if (argc != 2)
        usage();
    n = atoi(argv[1]);
    if (n <= 0)
        usage();
    shmid = create();

    struct port *p = get_port(shmid, false);
    p->capacity = n;
    p->capacity_actual = n;
    INFOF("Un port qui contient %d quai(s) a été créé.", n);

    semid = open();
    while(!closed)
    {
        //set_semaphore_value(semid, SEM_SLEEP, 1);
        closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    }
    delete();

}
