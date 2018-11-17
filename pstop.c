//
//  pstop.c
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

int stop(void)
{
    INFO("Fermeture du port..");
    int semid = get_semaphore_id();
    set_semaphore_value(semid, SEM_CLOSED, 1);
    set_semaphore_value(semid, SEM_SLEEP, 0);
    INFO("Le port est fermé.");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 1)
        usage();
    stop();
}