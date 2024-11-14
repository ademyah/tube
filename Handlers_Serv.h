#ifndef HANDLERS_SERV_H
#define HANDLERS_SERV_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Handler pour réveiller le serveur suite à la réception du signal SIGUSR1
void hand_reveil(int sig) {
    printf("Serveur réveillé par SIGUSR1.\n");
}

// Handler pour gérer la fin du serveur suite à un signal quelconque
void fin_serveur(int sig) {
    printf("Fin du serveur suite à la réception du signal %d.\n", sig);
    unlink(FIFO1); // Supprimer le FIFO pour les questions
    unlink(FIFO2); // Supprimer le FIFO pour les réponses
    exit(0);
}

#endif // HANDLERS_SERV_H
