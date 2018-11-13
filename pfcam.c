//
//  pfcam.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "Usage: %s [v] [c] [tc] (v=nom du navire, c=nbr de camion, tc= tmps nécessaire pour charger une cont) \n", prog);
    exit(EXIT_FAILURE);
}

noreturn void error(char *c)
{
    perror(c);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int c,tc;
    char v;
    prog = argv[0];
    if (argc != 4)
        usage();
    v = argv[1][1];
    c = atoi(argv[2]);
    tc = atoi(argv[3]);
    if (c <= 0 || tc <0 )
        usage();
    return 0;
}