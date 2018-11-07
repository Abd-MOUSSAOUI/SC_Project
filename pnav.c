//
//  pnav.c
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#include "helper.h"

noreturn void usage(void)
{
    fprintf(stderr, "Usage: %s [v] [c] [ta] [td] (v=nom, c=nbr de conts, ta=tmps d’accostage, td=tmps de dechargement d'une cont \n", prog);
    exit(CODE_ERREUR);
}

noreturn void error(char *c)
{
    perror(c);
    exit(CODE_ERREUR);
}

void pnav()
{
    int shmid;
    key_t key = 5678;
    char *shm, *s;

    //Locate the segment.
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0)
        error("shmget");

    
    //Now we attach the segment to our data space.
    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        error("shmat");

    
    //Now read what the server put in the memory.
    
    for (s = shm; *s != NULL; s++)
        putchar(*s);
    putchar('\n');

    /*
     * Finally, change the first character of the
     * segment to '*', indicating we have read
     * the segment.
     */
    *shm = '*';
    exit(0);
}

int main(int argc, char *argv[])
{
    int n;
    prog = argv[0];
    if (argc != 5)
        usage();
    n = atoi(argv[1]);
    if (n <= 0)
        usage();
    return 0;
}