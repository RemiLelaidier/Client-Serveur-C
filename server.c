/** Fichier server.c
 *  Cree un serveur pour communiquer avec un ou des clients
 *  V1 : Client unique
 *  V2 : Client multiples */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    int looptime = 0;                                                           /* Numero de la boucle */
    socklen_t ls = sizeof(struct sockaddr_in);                                  /* Taille des adresses */


    /*---- Caracterisation de la socket d'émission ----------*/
    int sd0;                                                                    /* Descripteur  */
    int ps0 = 0;                                                             /* Port         */
    struct sockaddr_in adr0, *padr0 = &adr0;                                    /* Adresse  */

    /*---- Buffers pour Messages -------------------------------*/
    char msg_in[30] = "0";                                                       /* Message recu de "0" a "99" */
    char msg_out[30] = "0";                                                      /* Message a envoyer "0" a "99" */

    /*--- Calcul -----------------------------------------*/
    int ope1 = 0, ope2 = 0;                                                      /* Variables pour les calcul du serveur */
    char cope1[4], cope2[4];
    char operateur, variable = '\0';
    int x = 0, y = 0, z = 0;


    /*---- (0) - Verifications de base ----*/
    if (argc != 2){
        fprintf(stderr,"Syntaxe d'appel : server <port> \n");
        exit(2);
    }
    ps0 = atoi(argv[1]);

    /*---- (1) Concernant la socket d'émission ----
        ---- (a) Creation du socket : Domaine AF_INET, type DGRAM, proto. par defaut ----*/
    if ((sd0=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("[SOCK_DGRAM, AF_INET, 0]");
    else
        printf("socket [SOCK_DGRAM, AF_INET, 0] creee\n");

    /*  ---- (b) Preparation de l'adresse d'attachement ----*/
    adr0.sin_family      = AF_INET;
    adr0.sin_addr.s_addr = htonl(INADDR_ANY);                                   /* Format reseau */
    adr0.sin_port        = htons(ps0);                                          /* Format reseau */

    /*  ---- (c) Demande d'attachement de la socket (specifique serveur)----*/
    if(bind(sd0,(struct sockaddr *)(padr0),ls) == -1) {
        perror("Attachement de la socket impossible");
        close(sd0);                                                             /* Fermeture de la socket */
        exit(2);                                                                /* Le processus se termine anormalement.*/
    }

    /*  ---- (d) Recuperation de l'adresse effective d'attachement ----*/
    getsockname(sd0,(struct sockaddr *)padr0,&ls);

    printf("Le serveur ecoute sur le port %d\n", ntohs(padr0->sin_port));


    /*---- (3) Boucle emission-reception type serveur----*/
    for(;;) {
        int i;
        int res = 0;
        printf("\n------------------\n");

        // Pour savoir quel mode de calcul est necessaire
        // Assigner une variable x y z
        int assign = 0;
        // Calcul sns variable
        int calculNormale = 1;
        variable = '\0';

        /*---- (a) Reception ----*/
        printf("Attente de reception ... ");
        if (recvfrom(sd0,msg_in,sizeof(msg_in),0, (struct sockaddr *)padr0, &ls) == -1)
            printf("inachevee : %s !\n",msg_in);
        else  {
            printf("recue : valeur = %s !\nRecu de %s sur le port %d",msg_in, inet_ntoa(padr0->sin_addr), ntohs(padr0->sin_port));

            // Identification du type de traitement
            int i = 0;
            while(i < sizeof(msg_in)) {
                if(variable != '\0') {
                    if(msg_in[i] == '=')
                        assign = 1;
                }
                else {
                    if (msg_in[i] == 'x' || 'y' || 'z' || 'X' || 'Y' || 'Z') {
                        variable = msg_in[i];
                        calculNormale = 0;
                    }
                }
                i++;
            }

            /*---- (b) Traitement ----*/
            /*---- Assignement de variable ----*/
            if(assign == 1) {
                sscanf(msg_in, "%c %c %d", &variable, &operateur, &ope1);
                switch (variable){
                    case 'x':
                        x = ope1;
                        break;
                    case 'X':
                        x = ope1;
                        break;
                    case 'y':
                        y = ope1;
                        break;
                    case 'Y':
                        y = ope1;
                        break;
                    case 'z':
                        z = ope1;
                        break;
                    case 'Z':
                        z = ope1;
                        break;
                }
                sprintf(msg_out, "%s", "Variable assignee");
            }
                // ---- Si c'est un calcul sans variable
            else {
                if(calculNormale == 1) {
                    sscanf(msg_in, "%d %c %d", &ope1, &operateur, &ope2);
                    switch(operateur){
                        case '+' :
                            res = ope1 + ope2;
                            break;
                        case '-' :
                            res = ope1 - ope2;
                            break;
                        case '*' :
                            res = ope1 * ope2;
                            break;
                        case '/' :
                            res = ope1 / ope2;
                            break;
                        default:
                            perror("Mauvais format");
                            break;
                    }
                }
                    // ---- Si c'est un calcul avec une variable
                else {
                    sscanf(msg_in, "%s %c %s", cope1, &operateur, cope2);
                    if(cope1[0] >= '0' && cope1[0] <= '9') {
                        switch (cope2[0]) {
                            case 'x':
                                if(operateur == '+')
                                    res = x + atoi(cope1);
                                if(operateur == '-')
                                    res = x - atoi(cope1);
                                if(operateur == '*')
                                    res = x * atoi(cope1);
                                if(operateur == '/')
                                    res = x / atoi(cope1);
                                break;
                            case 'X':
                                if(operateur == '+')
                                    res = x + atoi(cope1);
                                if(operateur == '-')
                                    res = x - atoi(cope1);
                                if(operateur == '*')
                                    res = x * atoi(cope1);
                                if(operateur == '/')
                                    res = x / atoi(cope1);
                                break;
                            case 'y':
                                if(operateur == '+')
                                    res = y + atoi(cope1);
                                if(operateur == '-')
                                    res = y - atoi(cope1);
                                if(operateur == '*')
                                    res = y * atoi(cope1);
                                if(operateur == '/')
                                    res = y / atoi(cope1);
                                break;
                            case 'Y':
                                if(operateur == '+')
                                    res = y + atoi(cope1);
                                if(operateur == '-')
                                    res = y - atoi(cope1);
                                if(operateur == '*')
                                    res = y * atoi(cope1);
                                if(operateur == '/')
                                    res = y / atoi(cope1);
                                break;
                            case 'z':
                                if(operateur == '+')
                                    res = z + atoi(cope1);
                                if(operateur == '-')
                                    res = z - atoi(cope1);
                                if(operateur == '*')
                                    res = z * atoi(cope1);
                                if(operateur == '/')
                                    res = z / atoi(cope1);
                                break;
                            case 'Z':
                                if(operateur == '+')
                                    res = z + atoi(cope1);
                                if(operateur == '-')
                                    res = z - atoi(cope1);
                                if(operateur == '*')
                                    res = z * atoi(cope1);
                                if(operateur == '/')
                                    res = z / atoi(cope1);
                                break;
                        }
                    }
                    if(cope2[0] >= '0' && cope2[0] <= '9') {
                        switch (cope1[0]) {
                            case 'x':
                                if(operateur == '+')
                                    res = x + atoi(cope2);
                                if(operateur == '-')
                                    res = x - atoi(cope2);
                                if(operateur == '*')
                                    res = x * atoi(cope2);
                                if(operateur == '/')
                                    res = x / atoi(cope2);
                                break;
                            case 'X':
                                if(operateur == '+')
                                    res = x + atoi(cope2);
                                if(operateur == '-')
                                    res = x - atoi(cope2);
                                if(operateur == '*')
                                    res = x * atoi(cope2);
                                if(operateur == '/')
                                    res = x / atoi(cope2);
                                break;
                            case 'y':
                                if(operateur == '+')
                                    res = y + atoi(cope2);
                                if(operateur == '-')
                                    res = y - atoi(cope2);
                                if(operateur == '*')
                                    res = y * atoi(cope2);
                                if(operateur == '/')
                                    res = y / atoi(cope2);
                                break;
                            case 'Y':
                                if(operateur == '+')
                                    res = y + atoi(cope2);
                                if(operateur == '-')
                                    res = y - atoi(cope2);
                                if(operateur == '*')
                                    res = y * atoi(cope2);
                                if(operateur == '/')
                                    res = y / atoi(cope2);
                                break;
                            case 'z':
                                if(operateur == '+')
                                    res = z + atoi(cope2);
                                if(operateur == '-')
                                    res = z - atoi(cope2);
                                if(operateur == '*')
                                    res = z * atoi(cope2);
                                if(operateur == '/')
                                    res = z / atoi(cope2);
                                break;
                            case 'Z':
                                if(operateur == '+')
                                    res = z + atoi(cope2);
                                if(operateur == '-')
                                    res = z - atoi(cope2);
                                if(operateur == '*')
                                    res = z * atoi(cope2);
                                if(operateur == '/')
                                    res = z / atoi(cope2);
                                break;
                        }
                    }
                    // TODO Si c'est un calcul avec deux variables
                    /*if(cope1[0] == 'x' || 'X' || 'y' || 'Y' || 'z' || 'Z'){
                        if(cope2[0] == 'x' || 'X' || 'y' || 'Y' || 'z' || 'Z'){
                            switch (operateur){
                                
                            }
                        }
                    }*/
                    sprintf(msg_out, "%d", res);
                }
                }
            }

            /*---- (c) Emission ----*/
            printf("\n\nEnvoi(%d) ... ", looptime);
            if (sendto(sd0,msg_out,sizeof(msg_out),0,(struct sockaddr *)padr0, ls) >0)
                printf("termine : message/valeur = %s !\n",msg_out);
            else
                printf("inacheve : %s !\n",msg_out);
            looptime++;
        }
    }