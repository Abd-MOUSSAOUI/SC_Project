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
    fprintf(stderr, "Usage: %s (ne prends aucun arg) \n", prog);
    exit(EXIT_FAILURE);
}

noreturn void error(char *c)
{
    perror(c);
    exit(EXIT_FAILURE);
}

int stop(void)
{
    INFO("Suppression du port ...");
    delete_shared_memory();
    delete_semaphore();
    INFO("Le port est supprimé.");
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 1)
        usage();
    stop();
}