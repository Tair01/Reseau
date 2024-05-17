/*
* Code du client
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define SIZE 100
#define PORT 9600
#define h_addr h_addr_list[0]

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

void Connect(int sockfd, struct sockaddr *addr, int addrlen){
    int resultat = connect(sockfd, addr, addrlen);
    if(resultat == -1){
        perror("Erreur de connexion");
        exit(EXIT_FAILURE);
    }
}


int main (int argc, char *argv[]){
    /*
    * Variables du client
    *
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d’adresse du serveur
    * - pointeur vers la structure descriptive de machine (hostent)
    * - zone de mémoire destinée à accueillir la chaîne
    * entrée au clavier
    * - taille de la chaîne à envoyer
    */
    int sockfd = Socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serveur_ad;
    struct hostent *serveur_info;
    char buffer[SIZE];
    
    /*
    * Code du client
    * - Ouvrir le socket du client
    * - Récupérer l’adresse IP du serveur à partir de son nom
    * donné en ligne de commande
    * - Remplir la structure d’adresse du serveur
    * - Lire une ligne de l’entrée standard
    * - Envoyer la chaîne lue au serveur
    */
    memset(&serveur_ad, 0, sizeof(serveur_ad));
    serveur_ad.sin_family = PF_INET;
    serveur_ad.sin_port = htons(PORT);
    serveur_ad.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(argc != 2){
        fprintf(stderr, "Utilisation : %s serveur_ip\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    serveur_info = gethostbyname(argv[1]);
    if(serveur_info == NULL){
        fprintf(stderr, "Erreur de l'adresse IP");
        exit(EXIT_FAILURE);
    }
    memcpy(&serveur_ad.sin_addr, serveur_info->h_addr, serveur_info->h_length);
    Connect(sockfd,(struct sockaddr*)&serveur_ad, sizeof(serveur_ad));
    printf("Entrez une chaîne à envoyer: ");
    fgets(buffer, SIZE, stdin);
    
    ssize_t n = write(sockfd, buffer, strlen(buffer));
    if(n == -1){
        perror("Erreur de write");
        exit(EXIT_FAILURE);
    }
    
    close(sockfd);
    return 0;
}