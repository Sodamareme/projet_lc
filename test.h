
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

//FONCTIONS MENUS
void afficherMenuAdmin() {
    printf("\n---------------MENU ADMIN--------------\n");
    printf("1--------GESTION DES ÉTUDIANTS\n");
    printf("2--------GÉNÉRATION DE FICHIERS----\n");
    printf("3--------MARQUER LES PRÉSENCES\n");
    printf("4--------ENVOYER UN MESSAGE\n");
    printf("5--------DÉCONNEXION\n");
}

void afficherMenuApprenant() {
    printf("\n---------------MENU APPRENANT--------------\n");
    printf("1------------MARQUER MA PRÉSENCE\n");
    printf("2------------NOMBRE DE MINUTES D'ABSENCE\n");
    printf("3------------MES MESSAGES\n");
    printf("4------------DECONNEXION\n");
}

// Fonction pour masquer la saisie avec des étoiles
void masquerSaisie() {
    struct termios old, new_term;
    if (tcgetattr(STDIN_FILENO, &old) != 0) {
        perror("Erreur lors de la récupération des attributs du terminal");
        exit(EXIT_FAILURE);
    }
    new_term = old;
    new_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON); 
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0) {
        perror("Erreur lors de la configuration des attributs du terminal");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour restaurer l'écho de la saisie
void restaurerSaisie() {
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    old.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL | ICANON); 
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}
//FONCTION CONNEXION
int fonctionConnexion() {
    int passwordAdmin = 123;
    int reponseAdmin; 
    int passwordApp = 456;
    int reponseApp; 
    char c;
    int i;

    do {
        reponseAdmin = 0; 
        reponseApp = 0; 

        printf("Entrez un code : ");
        fflush(stdout);
        masquerSaisie(); 
        i = 0;
        while (1) {
            c = getchar();
            if (c == '\n') {
                break;
            } else if (c == 127 || c == 8) { // Gestion de la suppression arrière (backspace)
                if (i > 0) {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    i--;
                }
            } else {
                putchar('*');
                reponseAdmin = reponseAdmin * 10 + (c - '0');
                i++;
            }
        }
        restaurerSaisie();
        printf("\n");

        if (reponseAdmin == passwordAdmin) {
            afficherMenuAdmin();
            break;
        } else {
            printf("Erreur de code\n");
        }

        masquerSaisie(); 
        i = 0;
        while (1) {
            c = getchar();
            if (c == '\n') {
                break;
            } else if (c == 127 || c == 8) { 
                if (i > 0) {
                    putchar('\b');
                    putchar(' ');
                    putchar('\b');
                    i--;
                }
            } else {
                putchar('*');
                reponseApp = reponseApp * 10 + (c - '0');
                i++;
            }
        }
        restaurerSaisie(); 
        printf("\n");

        if (reponseApp == passwordApp) {
            afficherMenuApprenant();
            break;
        } else {
            printf("Erreur de code\n");
        }

    } while ((reponseAdmin != passwordAdmin) || (reponseApp != reponseApp));

    return 0;
}


int deconnexion() {
    char choix;
    printf("Voulez-vous vous déconnecter ? (o/n): ");
    fflush(stdout);
    scanf(" %c", &choix);
    if (choix == 'o' || choix == 'O') {
        printf("Déconnexion réussie.\n");
        return 1; 
    } else {
        printf("Retour au menu principal.\n");
        return 0; 
    }
}
void marquerPresence() {
    FILE *fichierEtudiantsPtr; // Renommé pour éviter la confusion
    char nomClasse[50];
    int choixClasse, choixEtudiant, presence;
    char etudiant[50];
    char fichierEtudiants[100]; // Tableau pour stocker le nom du fichier

    printf("Liste des classes :\n");
    printf("1. Classe A\n");
    printf("2. Classe B\n");
    printf("3. Classe C\n");
    printf("Entrez le numéro de la classe : ");
    scanf("%d", &choixClasse);

    switch (choixClasse) {
        case 1:
            strcpy(nomClasse, "Classe A");
            break;
        case 2:
            strcpy(nomClasse, "Classe B");
            break;
        case 3:
            strcpy(nomClasse, "Classe C");
            break;
        default:
            printf("Choix invalide.\n");
            return;
    }

    sprintf(fichierEtudiants, "liste_etudiants_%s.txt", nomClasse);
    fichierEtudiantsPtr = fopen(fichierEtudiants, "r"); 
    if (fichierEtudiantsPtr == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    printf("\nListe des étudiants de la classe %s :\n", nomClasse);
    while (fgets(etudiant, sizeof(etudiant), fichierEtudiantsPtr) != NULL) {
        printf("%s", etudiant);
    }
    fclose(fichierEtudiantsPtr);

    printf("\nEntrez le numéro de l'étudiant à marquer : ");
    scanf("%d", &choixEtudiant);

    printf("Marquer la présence de l'étudiant (1: présent, 0: absent) : ");
    scanf("%d", &presence);

    if (presence == 1) {
        printf("La présence de l'étudiant a été marquée.\n");
    } else {
        printf("L'absence de l'étudiant a été marquée.\n");
    }
}


int fonctionOption(){
    int choix;
    int continuer = 1;

    do {
        fonctionConnexion();
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("Gestion des étudiants");
                break;
            case 2: {
                // char nomClasse[20];
                printf("génération de fichiers: ");
                // scanf("%s", nomClasse);
                // genererListeEtudiants(nomClasse);
                break;
            }
            case 3:
                marquerPresence();
                break;
            case 4:
                printf("envoyer un message.\n");
                break;
            case 5:
                continuer = deconnexion();
                break;
            default:
                printf("Choix invalide.\n");
                break;
        }
    } while (continuer);



    return 0;
}
