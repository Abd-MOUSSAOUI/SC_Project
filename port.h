//
//  port.h
//  SC_Project
//
//  Created by Abderahmane_MOUSSAOUI on 07/11/2018.
//  Copyright © 2018 MOUSSAOUI. All rights reserved.
//

#ifndef PORT_H
#define PORT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

//Sémaphores pour chaque quai
#define SEM_DISCHARGE_NAV 0
#define SEM_DISCHARGE_CAM 1
//Sémaphores générals pour tout le programme
#define SEM_CAPACITY 0
#define SEM_CLOSED 1

char *prog;

#define MAX_BOATS 26 //Nombre des lettres

// SEMOP construit la structure à la volée, et vérifie le code de retour.
#define SEMOPS(semId, ...)                  \
{                                           \
        struct sembuf s[] = {__VA_ARGS__};  \
        if(semop(semId, s, sizeof(s)/sizeof(struct sembuf)) == -1) \
        {                                   \
            perror("semop");                \
            exit(EXIT_FAILURE);}            \
}                                           \

#ifndef LOG_LEVEL
#define LOG_LEVEL get_log_level()
#endif

//Niveau de chaque type de log.
#define LEVEL_FATAL 0
#define LEVEL_ERROR 0
#define LEVEL_WARN 1
#define LEVEL_DEBUG 1
#define LEVEL_INFO 1

// Macros pour log.
#define FATAL(MSG) LOG(FATAL, MSG "\n")
#define FATALF(MSG, ...) LOG(FATAL, MSG "\n", __VA_ARGS__)
#define ERROR(MSG) LOG(ERROR, MSG "\n")
#define ERRORF(MSG, ...) LOG(ERROR, MSG "\n", __VA_ARGS__)
#define WARN(MSG) LOG(WARN, MSG "\n")
#define WARNF(MSG, ...) LOG(WARN, MSG "\n", __VA_ARGS__)
#define INFO(MSG) LOG(INFO, MSG "\n")
#define INFOF(MSG, ...) LOG(INFO, MSG "\n", __VA_ARGS__)
#define DEBUG(MSG) LOG(DEBUG, MSG "\n")
#define DEBUGF(MSG, ...) LOG(DEBUG, MSG "\n", __VA_ARGS__)

// To convert __LINE__ into string literal
#define S(x) #x
#define S_(x) S(x)


#define LEVEL_FMT(LEVEL) #LEVEL
#define LOC_FMT __FILE__ ":" S_(__LINE__)

// Macro pour formatter un message de logs, avec [niveau] 
//[fichier]:[ligne] [message]
#define LOG_FMT(LEVEL, ...) LEVEL_FMT(LEVEL) "\t" LOC_FMT "\t" __VA_ARGS__
#define LOG(LEVEL, ...)                                   \
    {                                                     \
        if (LEVEL_##LEVEL <= LOG_LEVEL)                   \
        {                                                 \
            fprintf(stderr, LOG_FMT(LEVEL, __VA_ARGS__)); \
            fflush(stderr);                               \
        }                                                 \
    }

int MAX(int X, int Y);

int get_log_level(void);

void error(char *c);

key_t key(int i);

struct port *get_port(int shmid, bool readonly);

int create_shared_memory(void);

int get_shared_memory_id(void);

void delete_shared_memory(void);

void create_semaphore(int n);

int get_semaphore_id(int i);

void delete_semaphore(int i);

int get_semaphore_value(int semid, int which);

void set_semaphore_value(int semid, int which, int value);


typedef struct dock
{
    int number_of_dock;
    bool free;
    char name;
    int number_of_container;
    int time_to_discharge_a_container;
    int time_to_charge_a_container;
} dock;

typedef struct port
{
    int capacity;
    int inside;
    dock boats[MAX_BOATS];
} port;



#endif
