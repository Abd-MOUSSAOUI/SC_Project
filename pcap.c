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


void pcap()
{
    char c;
    int shmid;
    key_t key = 5678;

    char *shm, *s;

    //Create the segment.
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
        error("shmget");

    //Now we attach the segment to our data space.
    if ((shm = shmat(shmid, NULL, 0)) == (char *)-1)
        error("shmat");

    
    //Now put some things into the memory for the
    //other process to read.
    s = shm;

    for (c = 'a'; c <= 'z'; c++)
        *s++ = c;
    *s = NULL;

    /*
     * Finally, we wait until the other process
     * changes the first character of our memory
     * to '*', indicating that it has read what
     * we put there.
     */
    while (*shm != '*')
        sleep(1);

    putchar(*shm);
    putchar('\n');
    exit(0);
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
