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
#include <strings.h>
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

int Sendto(int sockfd, const char *buf, int len, int flags,struct sockaddr *to, socklen_t tolen){
    int resultat = sendto(sockfd, buf, len, flags,to,tolen);
    if(resultat == -1){
        perror("Erreur de sendto");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

int Recvfrom(int sockfd, char *buf, int len, int flags,struct sockaddr *from, socklen_t *fromlen){
    int resultat = recvfrom(sockfd, buf, len, flags,from,fromlen);
    if(resultat == -1){
        perror("Erreur de recvfrom");
        exit(EXIT_FAILURE);
    }
    return resultat;
}

int main (int argc, char *argv[]){
    /*
    * Variables du client
    * Déclarer ici les variables suivantes :
    * - sockfd le descripteur de socket
    * - structure d’adresse du serveur
    * - pointeur vers la structure descriptive de machine (hostent)
    * - zone de mémoire destinée à accueillir la chaîne
    * entrée au clavier
    * - taille de la chaîne à envoyer
    */
    int sockfd = Socket(PF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in serveur_udp;
    struct hostent *serveur_info;
    struct sockaddr_in client_udp;
    socklen_t client_udp_size = sizeof(client_udp);
    char buffer[SIZE];
    
    /*
    * Code du client
    *
    * - Ouvrir le socket du client
    * - Récupérer l’adresse IP du serveur à partir de son nom
    * donné en ligne de commande
    * - Remplir la structure d’adresse du serveur
    * - Lire une ligne de l’entrée standard
    * - Envoyer la chaîne lue au serveur
    */
    memset(&serveur_udp, 0, sizeof(serveur_udp));
    serveur_udp.sin_family = PF_INET;
    serveur_udp.sin_port = htons(PORT);
    serveur_udp.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(argc != 2){
        fprintf(stderr, "Utilisation : %s serveur_ip\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    serveur_info = gethostbyname(argv[1]);
    if(serveur_info == NULL){
        fprintf(stderr, "Erreur de l'adresse IP");
        exit(EXIT_FAILURE);
    }
    
    bcopy((char *)serveur_info -> h_addr, (char *)&serveur_udp.sin_addr, serveur_info -> h_length);
    printf("Entrez une chaîne à envoyer: ");
    fgets(buffer, SIZE, stdin);
    
    int n = Sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serveur_udp, sizeof(struct sockaddr));
    n = Recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_udp, &client_udp_size);
    
    write(1, "J'ai reçu un accusé de réception \n",32);
    write(1,buffer, n);
    
    return 0;
}