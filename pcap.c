//
//  pcap.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "helper.h"


noreturn void usage(void)
{
    fprintf(stderr, "Usage: %s n (avec n>0 nombre de quais)\n", prog);
    exit(CODE_ERREUR);
}

noreturn void error(char *c)
{
    perror(c);
    exit(CODE_ERREUR);
}


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
}
