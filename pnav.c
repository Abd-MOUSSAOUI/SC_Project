//
//  pnav.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "port.h"

noreturn void usage(void)
{
    fprintf(stderr, "usage: %s [v] [c] [ta] [td] (v=nom, c=nbr de conts, ta=tmps d’accostage, td=tmps de dechargement d'une cont \n", prog);
    exit(EXIT_FAILURE);
}


void create_navire(char v, int c, int ta, int td)
{
    struct port *p = get_port(get_shared_memory_id(), false);
    p->name=v;
    p->number_of_container=c;
    p->time_of_docking=ta;
    p->time_to_discharge_a_container=td;
    p->dock=0;
}

int main(int argc, char *argv[])
{
    prog = argv[0];
    if (argc != 5)
        usage();

    int c, ta, td;
    char v;
    v = argv[1][1];
    c = atoi(argv[2]);
    ta = atoi(argv[3]);
    td = atoi(argv[4]);

    if (c <= 0 || ta < 0 || td < 0)
        usage();
    create_navire(v, c, ta, td);
    return 0;
}