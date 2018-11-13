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
    fprintf(stderr, "Usage: %s n (avec n>0 nombre de quais)\n", prog);
    exit(EXIT_FAILURE);
}

noreturn void error(char *c)
{
    perror(c);
    exit(EXIT_FAILURE);
}

// int create(int nombre_de_quai)
// {
//     INFO("Création de la mémoire partagée…");
//     int shmid = create_shared_memory();
//     INFO("Création de l'ensemble de sémaphores…");
//     create_semaphore();

//     struct port *p = get_port(shmid, false);
//     p->capacity = nombre_de_quai;
//     p->name = '\0';
//     p->number_of_container = 0;
//     p->time_of_docking = 0;
//     p->time_to_discharge_a_container = 0;
//     p->dock = 0;
//     p->number_of_dock = 0;
//     INFOF("Un port de capacité %d a été créé.", nombre_de_quai);

//     return EXIT_SUCCESS;
// }

int main(int argc, char *argv[])
{
    int n;
    prog = argv[0];
    if (argc != 2)
        usage();
    n = atoi(argv[1]);
    if (n <= 0)
        usage();
    return 0;
    //return create(n);
}
