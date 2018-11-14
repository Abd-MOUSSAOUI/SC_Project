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
    fprintf(stderr, "usage: %s [v] [c] [tc] (v=nom du navire, c=nbr de camion, tc= tmps nécessaire pour charger une cont) \n", prog);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int c,tc, semid;
    char v;
    prog = argv[0];
    if (argc != 4)
        usage();
    v = argv[1][1];
    c = atoi(argv[2]);
    tc = atoi(argv[3]);
    if (c <= 0 || tc <0 )
        usage();
    //struct port *p = get_port(get_shared_memory_id(), true);
    semid = get_semaphore_id();
}