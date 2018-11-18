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

//ouverture du port: get semid
int open(int n)
{
    DEBUG("On met SEM_CLOSED à 0");
    INFO("Ouverture du port...");
    int semid = get_semaphore_id(n);
    set_semaphore_value(semid, SEM_CLOSED, 0);
    DEBUG("SEM_CLOSED = 0");
    INFO("Le port est ouvert...");
    return semid;
}

//Suppression de shmid et semid
int delete(int n)
{
    int i;
    INFO("Suppression du port...");
    for (i = 0; i <= n; i++)
    {
        delete_semaphore(i);
    }
    delete_shared_memory();
    INFO("Le port est supprimé! Ciao");
    return EXIT_SUCCESS;
}

void initializer(struct port *p)
{
    int i;
    for (i = 0; i < p->capacity; i++)
    {
        p->boats[i].number_of_dock = i;
        p->boats[i].free = true;
        p->boats[i].name = '\0';
        p->boats[i].number_of_container = 0;
        p->boats[i].time_to_discharge_a_container = 0;
        p->boats[i].time_to_charge_a_container = 0;
    }
}

int main(int argc, char *argv[])
{
    int n, shmid, semid;

    prog = argv[0];
    if (argc != 2)
        usage();
    n = atoi(argv[1]);
    if (n <= 0)
        usage();

    INFO("Création de la mémoire partagée…");
    shmid = create_shared_memory();
    INFO("Création de l'ensemble des sémaphores…");
    (void)create_semaphore(n);
    port *p = get_port(shmid, false);
    p->capacity = n;
    p->inside = 0;
    initializer(p);
    DEBUG("Création du port..");
    INFOF("Un port qui contient %d quai(s) a été créé.", n);

    semid = open(n);
    bool closed = false;
    // On attend que le port soit ouvert une première fois
    while (!closed || p->inside > 0)
    {
        //On vérifie que le port n'est pas fermé..
        closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    }
    //set_semaphore_value(semid, SEM_CAPACITY, 0);
    INFO("Supression du port..");
    delete(n);
}
