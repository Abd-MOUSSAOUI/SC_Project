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
    int i;
    struct port *p = get_port(get_shared_memory_id(), true);
    int semid = get_semaphore_id(p->capacity);
    bool closed = (get_semaphore_value(semid, SEM_CLOSED) != 0);
    int capa = get_semaphore_value(semid, SEM_CAPACITY);

    INFOF(" Nombre de quai du port: %d", p->capacity);
    INFO("--------------");
    
    for(i=0; i<p->capacity; i ++)
    {
        INFOF(" Capacité du port: %d", capa);
        INFO("--------------");
        INFOF(" Etat du port: %s", closed ? "fermé" : "ouvert");
        INFO("--------------");
        INFOF(" Numéro de quai : %d", p->boats[i].number_of_dock);
        INFO("--------------");
        INFOF(" Nom du Navire: %c", p->boats[i].name);
        INFO("--------------");
        INFOF(" État du navire: %s", (p->boats[i].free ? "à quai" : "en attente"));
        INFO("--------------");
        INFOF(" Nbr de conteneur: %d", p->boats[i].number_of_container);
    }
}
