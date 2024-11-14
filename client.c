#include "serv_cli_fifo.h"
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t reveil = 0;  // Variable pour détecter le signal du serveur

// Handler de réveil du client par le serveur
void reveil_client(int sig) {
    reveil = 1;
}

// Handler pour gérer l'interruption du client (SIGINT)
void gestion_SIGINT(int sig) {
    printf("\nFin du client suite à la réception du signal SIGINT (%d).\n", sig);
    exit(0);  // Quitter le client proprement
}

// Handler pour gérer le signal SIGQUIT
void gestion_SIGQUIT(int sig) {
    printf("\nFin du client suite à la réception du signal SIGQUIT (%d).\n", sig);
    exit(0);  // Quitter le client proprement
}

int main() {
    int fifo1_fd, fifo2_fd;
    Question question;
    Reponse reponse;

    // Configuration des handlers
    signal(SIGUSR1, reveil_client);  // Handler pour réveil du client par le serveur
    signal(SIGINT, gestion_SIGINT);  // Handler pour gérer SIGINT (Ctrl+C)
    signal(SIGQUIT, gestion_SIGQUIT);  // Handler pour gérer SIGQUIT (Ctrl+\)

    // Ouverture des tubes
    fifo1_fd = open(FIFO1, O_WRONLY);
    fifo2_fd = open(FIFO2, O_RDONLY);

    // Préparation de la question
    question.client_pid = getpid();
    srand(time(NULL) ^ question.client_pid);
    question.n = rand() % NMAX + 1;

    // Envoi de la question au serveur
    write(fifo1_fd, &question, sizeof(Question));
    printf("Question envoyée pour %d nombres.\n", question.n);

    // Attente du réveil par le serveur
    while (!reveil) pause();

    // Lecture de la réponse du serveur
    while (read(fifo2_fd, &reponse, sizeof(Reponse)) > 0) {
        if (reponse.client_pid == question.client_pid) {
            printf("Réponse reçue :\n");
            for (int i = 0; i < reponse.count; i++) {
                printf("nb[%d] = %d\n", i, reponse.values[i]);
            }

            // Envoyer un signal au serveur indiquant que la réponse a été lue
            kill(getppid(), SIGUSR1);

            printf("Réponse complètement reçue.\n");

            // Attente de la touche pour terminer le client
            printf("Appuyez sur une touche pour terminer le client.\n");
            getchar();  // Attente d'une touche
            break;
        }
    }

    close(fifo1_fd);
    close(fifo2_fd);
    return 0;
}
