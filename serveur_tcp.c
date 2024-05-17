#include <stdio.h>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <errno.h> //Pour faire une exception des erreurs
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

/* Port local du serveur */
#define PORT 9600

int Socket(int domain, int type, int protocol){
    int resultat = socket(domain,type,protocol);
    if(resultat == -1){
        perror("Erreur de socket");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

void Bind(int sockfd, struct sockaddr *addr, int addrlen) {
    int resultat = bind(sockfd, addr, addrlen);
    if (resultat == -1) {
        perror("Erreur de bind");
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockfd, int backlog) {
    int resultat = listen(sockfd, backlog);
    if (resultat == -1) {
        perror("Erreur de listen");
        exit(EXIT_FAILURE);
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
    int resultat = accept(sockfd, addr, addrlen);
    if (resultat == -1) {
        perror("Erreur de accept");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

int main() {
    /*
    * Variables du serveur
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d’adresse locale du serveur
    * - structure d’adresse du client
    * - taille de l’adresse du client
    */
    int sockfd = Socket(PF_INET, SOCK_STREAM, 0); ;
    struct sockaddr_in serveur_ad = {0};
    struct sockaddr_in client_ad;
    socklen_t serveur_ad_size = sizeof(serveur_ad);
    /*
    * Code du serveur
    * - Ouvrir le socket du serveur
    * - Remplir la structure d’adresse locale du serveur :
    * - la famille d’adresse
    * - l’adresse IP
    * - le port
    * - Spécifier l’adresse locale du socket du serveur
    */ 
    memset(&serveur_ad, 0, sizeof(serveur_ad));
    serveur_ad.sin_family = AF_INET; 
    serveur_ad.sin_port = htons(PORT);
    serveur_ad.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(sockfd, (struct sockaddr*)&serveur_ad, serveur_ad_size);
    Listen(sockfd, 5);
    while(1){
        int acc = Accept(sockfd, (struct sockaddr*)&client_ad, &serveur_ad_size);
        char buffer[256];
        ssize_t n = read(acc, buffer, 256);
        if(n == -1){
            perror("Erreur de read");
            exit(EXIT_FAILURE);
        }
        if(n == 0){
            printf("Fin de fichier\n");
        }
        write(STDOUT_FILENO, buffer, n);
        write(acc, buffer, n);
        close(acc);
    }
    close(sockfd);
    return 0;
}