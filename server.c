// serveur.c
#include "serv_cli_fifo.h"
#include "Handlers_Serv.h"
#include <time.h>
#include <sys/wait.h>

#define MAX_CLIENTS 8           // Nombre maximum de clients simultanés

int client_count = 0;           // Compteur pour le nombre de clients actifs

// Handler pour gérer la fin des processus enfants et décrémenter le compteur de clients
void handle_child_exit(int sig) {
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        client_count--;         // Décrémenter le compteur de clients actifs
    }
}

int main() {
    int fifo1_fd, fifo2_fd;
    Question question;
    
    // Création des tubes nommés
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Configuration des handlers de signaux
    signal(SIGUSR1, hand_reveil);
    signal(SIGINT, fin_serveur);
    signal(SIGCHLD, handle_child_exit);  // Gérer la terminaison des processus enfants

    // Ouverture des tubes
    fifo1_fd = open(FIFO1, O_RDONLY);
    if (fifo1_fd == -1) {
        perror("Erreur lors de l'ouverture de FIFO1");
        exit(1);
    }

    fifo2_fd = open(FIFO2, O_WRONLY);
    if (fifo2_fd == -1) {
        perror("Erreur lors de l'ouverture de FIFO2");
        exit(1);
    }

    while (1) {
        // Attendre qu'il y ait moins de clients actifs
        if (client_count >= MAX_CLIENTS) {
            printf("Nombre maximum de clients atteint. En attente...\n");
            pause();
            continue;
        }

        // Lecture de la question d'un client
        if (read(fifo1_fd, &question, sizeof(Question)) > 0) {
            printf("Question reçue de %d pour %d nombres.\n", question.client_pid, question.n);

            // Création d'un processus enfant pour gérer la réponse
            pid_t pid = fork();
            if (pid == 0) {  // Processus enfant
                Reponse reponse;
                srand(time(NULL) ^ question.client_pid);
                reponse.client_pid = question.client_pid;
                reponse.count = question.n;

                printf("Nombres générés pour le client %d :\n", question.client_pid);
                for (int i = 0; i < question.n; i++) {
                    reponse.values[i] = rand() % 100;
                    printf("nb[%d] = %d\n", i, reponse.values[i]);
                }

                // Envoi de la réponse au client
                write(fifo2_fd, &reponse, sizeof(Reponse));
                printf("Réponse envoyée à %d.\n", question.client_pid);

                // Envoi du signal au client pour le réveiller
                kill(question.client_pid, SIGUSR1);

                close(fifo1_fd);
                close(fifo2_fd);
                exit(0);  // Terminaison du processus enfant
            } else if (pid > 0) {  // Processus parent
                client_count++;  // Incrémenter le compteur de clients actifs
            } else {
                perror("Erreur de fork");
            }
        }
    }

    close(fifo1_fd);
    close(fifo2_fd);
    return 0;
}
