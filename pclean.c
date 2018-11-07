//
//  pclean.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "helper.h"

noreturn void usage(void)
{
    fprintf(stderr, "Usage: %s \n", prog);
    exit(CODE_ERREUR);
}

noreturn void error(char *c)
{
    perror(c);
    exit(CODE_ERREUR);
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 1)
        usage();
    return 0;
}