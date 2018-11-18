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
    fprintf(stderr, "usage: %s [v] [c] [tc] (v=nom du navire, c=nbr de camion, tc= tmps nécessaire pour charger un conteneur) \n", prog);
    exit(EXIT_FAILURE);
}

int get_number_of_dock(struct port *p, char v)
{
    int i;
    for (i = 0; i < p->capacity; i++)
        {
        if (p->boats[i].name == v)
            return p->boats[i].number_of_dock;
        }
    return -1;
}


int main(int argc, char *argv[])
{
    int c,tc, semid;
    char v;
    prog = argv[0];
    if (argc != 4)
        usage();
    v = argv[1][0];
    c = atoi(argv[2]);
    tc = atoi(argv[3]);
    if (c <= 0 || tc <0 )
        usage();

    struct port *p = get_port(get_shared_memory_id(), false);

    int quai;
    if ((quai = get_number_of_dock(p, v)) == -1)
    {
        ERROR("Le navire n'est pas à quai ! je me casse.. Ciao");
        return EXIT_FAILURE;
    }
    semid = get_semaphore_id(quai);

    int td=0;
    while (c > 0 && p->boats[quai].number_of_container > 0)
    {
        //Demander un conteneur
        SEMOPS(semid, {SEM_DISCHARGE_NAV, 1, 0});
        p->boats[quai].time_to_charge_a_container = tc;
        usleep(MAX(tc,td) * 1000);
        c--;
        td = p->boats[quai].time_to_discharge_a_container;
        SEMOPS(semid, {SEM_DISCHARGE_CAM, -1, 0});
    }
    return 0;
}