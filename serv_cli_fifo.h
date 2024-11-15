#ifndef SERV_CLI_FIFO_H
#define SERV_CLI_FIFO_H

// Inclusion des bibliothèques nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

// Constantes et macros
#define NMAX 40               // Nombre maximum pour la demande
#define FIFO1 "/tmp/fifo1"      // Nom du FIFO pour les questions
#define FIFO2 "/tmp/fifo2"      // Nom du FIFO pour les réponses

// Structure de la question envoyée par le client
typedef struct {
    pid_t client_pid;           // PID du client pour l'identifier
    int n;                      // Nombre de valeurs demandées
} Question;

// Structure de la réponse envoyée par le serveur
typedef struct {
    pid_t client_pid;           // PID du client destinataire
    int values[NMAX];           // Liste des valeurs générées
    int count;                  // Nombre de valeurs envoyées
} Reponse;

#endif // SERV_CLI_FIFO_H
