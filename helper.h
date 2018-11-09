//
//  pcap.h
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <stdbool.h>


#define CODE_ERREUR 1
#define SHMSZ 27

char *prog;

struct port *get_port(int shmid, bool readonly);

struct port
{
    int dock_number;
    char boat_name;
};

#endif /* helper_h */
