
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
// Déclaration des fonctions
void afficherMenuAdmin();
void afficherMenuApprenant();
void masquerSaisie();
void restaurerSaisie();
void marquerPresence();
void genererListeClasses();
void genererListeEtudiants(char *nomClasse);
int fonctionConnexion();
int deconnexion();
int fonctionOption();
// Fonctions pour afficher les menus
void afficherMenuAdmin() {
    printf("\n---------------MENU ADMIN--------------\n");
    printf("1--------GESTION DES ÉTUDIANTS\n");
    printf("2--------GÉNÉRATION DE FICHIERS----\n");
    printf("3--------MARQUER LES PRÉSENCES\n");
    printf("4--------ENVOYER UN MESSAGE\n");
    printf("5--------QUITTER\n");
}

void afficherMenuApprenant() {
    printf("\n---------------MENU APPRENANT--------------\n");
    printf("1------------MARQUER MA PRÉSENCE\n");
    printf("2------------NOMBRE DE MINUTES D'ABSENCE\n");
    printf("3------------MES MESSAGES\n");
    printf("4------------QUITTER\n");
}

// Fonction pour masquer la saisie avec des étoiles
void masquerSaisie() {
    struct termios old, new_term;
    if (tcgetattr(STDIN_FILENO, &old) != 0) {
        perror("Erreur lors de la récupération des attributs du terminal");
        exit(EXIT_FAILURE);
    }
    new_term = old;
    new_term.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON); // Désactive l'écho des caractères saisis
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) != 0) {
        perror("Erreur lors de la configuration des attributs du terminal");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour restaurer l'écho de la saisie
void restaurerSaisie() {
    struct termios old;
    tcgetattr(STDIN_FILENO, &old);
    old.c_lflag |= (ECHO | ECHOE | ECHOK | ECHONL | ICANON); // Réactive l'écho des caractères saisis
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

// Fonction pour marquer la présence
void marquerPresence() {
    printf("Fonctionnalité non implémentée pour l'instant.\n");
}

// Fonction pour générer le fichier contenant la liste des classes
void genererListeClasses() {
    FILE *fichier;
    fichier = fopen("liste_classes.txt", "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "Liste des classes :\n");
    fprintf(fichier, "1. Classe A\n");
    fprintf(fichier, "2. Classe B\n");
    fprintf(fichier, "3. Classe C\n");

    fclose(fichier);
    printf("Fichier de liste des classes généré.\n");
}

// Fonction pour afficher le contenu d'un fichier
void afficherContenuFichier(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        printf("%s", ligne);
    }

    fclose(fichier);
}

// Fonction pour générer la liste des étudiants d'une classe donnée
void genererListeEtudiants(char *nomClasse) {
    FILE *fichier;
    char nomFichier[50];
    sprintf(nomFichier, "liste_etudiants_%s.txt", nomClasse);
    fichier = fopen(nomFichier, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "Liste des étudiants de la classe %s :\n", nomClasse);
    fprintf(fichier, "1. Étudiant 1\n");
    fprintf(fichier, "2. Étudiant 2\n");
    fprintf(fichier, "3. Étudiant 3\n");

    fclose(fichier);
    printf("Fichier de liste des étudiants de la classe %s généré.\n", nomClasse);

    // Afficher le contenu du fichier après sa génération
    afficherContenuFichier(nomFichier);
}


// Fonction de connexion
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

// Fonction de déconnexion
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


int fonctionOption() {
    int choix;
    int continuer = 1;

    do {
        fonctionConnexion();
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                genererListeClasses();
                break;
            case 2: {
                char nomClasse[20];
                printf("Entrez le nom de la classe : ");
                scanf("%s", nomClasse);
                genererListeEtudiants(nomClasse);
                break;
            }
            case 3:
                marquerPresence();
                break;
            case 4:
                printf("Fonctionnalité non implémentée pour l'instant.\n");
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