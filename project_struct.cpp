#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITRE 100
#define MAX_AUTEUR 100
#define MAX_UTILISATEURS 100
#define MAX_LIVRES 1000

typedef struct {
    int id;
    char titre[MAX_TITRE];
    char auteur[MAX_AUTEUR];
    int estEmprunte;
} Livre;

typedef struct {
    int id;
    char nom[MAX_AUTEUR];
    int livresEmpruntes[MAX_LIVRES];
    int nombreEmprunts;
} Utilisateur;

Livre livres[MAX_LIVRES];
Utilisateur utilisateurs[MAX_UTILISATEURS];
int nombreLivres = 0;
int nombreUtilisateurs = 0;

void ajouterLivre() {
    if (nombreLivres >= MAX_LIVRES) {
        printf("La biblioth�que est pleine, impossible d'ajouter plus de livres.\n");
        return;
    }
    printf("Entrez le titre du livre : ");
    scanf(" %[^\n]%*c", livres[nombreLivres].titre);
    printf("Entrez l'auteur du livre : ");
    scanf(" %[^\n]%*c", livres[nombreLivres].auteur);
    livres[nombreLivres].id = nombreLivres + 1;
    livres[nombreLivres].estEmprunte = 0;
    nombreLivres++;
    printf("Livre ajout� avec succ�s.\n");
}

void ajouterUtilisateur() {
    if (nombreUtilisateurs >= MAX_UTILISATEURS) {
        printf("Nombre maximum d'utilisateurs atteint.\n");
        return;
    }
    printf("Entrez le nom de l'utilisateur : ");
    scanf(" %[^\n]%*c", utilisateurs[nombreUtilisateurs].nom);
    utilisateurs[nombreUtilisateurs].id = nombreUtilisateurs + 1;
    utilisateurs[nombreUtilisateurs].nombreEmprunts = 0;
    nombreUtilisateurs++;
    printf("Utilisateur ajout� avec succ�s.\n");
}

void emprunterLivre() {
    int utilisateurId, livreId;
    printf("Entrez l'ID de l'utilisateur : ");
    scanf("%d", &utilisateurId);
    printf("Entrez l'ID du livre : ");
    scanf("%d", &livreId);

    if (utilisateurId > nombreUtilisateurs || livreId > nombreLivres || livres[livreId - 1].estEmprunte) {
        printf("ID utilisateur ou ID livre invalide, ou livre d�j� emprunt�.\n");
        return;
    }
    
    utilisateurs[utilisateurId - 1].livresEmpruntes[utilisateurs[utilisateurId - 1].nombreEmprunts++] = livreId;
    livres[livreId - 1].estEmprunte = 1;
    printf("Livre emprunt� avec succ�s.\n");
}

void retournerLivre() {
    int utilisateurId, livreId;
    printf("Entrez l'ID de l'utilisateur : ");
    scanf("%d", &utilisateurId);
    printf("Entrez l'ID du livre : ");
    scanf("%d", &livreId);

    if (utilisateurId > nombreUtilisateurs || livreId > nombreLivres || !livres[livreId - 1].estEmprunte) {
        printf("ID utilisateur ou ID livre invalide, ou livre non emprunt�.\n");
        return;
    }

    livres[livreId - 1].estEmprunte = 0;
    for (int i = 0; i < utilisateurs[utilisateurId - 1].nombreEmprunts; i++) {
        if (utilisateurs[utilisateurId - 1].livresEmpruntes[i] == livreId) {
            utilisateurs[utilisateurId - 1].livresEmpruntes[i] = utilisateurs[utilisateurId - 1].livresEmpruntes[--utilisateurs[utilisateurId - 1].nombreEmprunts];
            break;
        }
    }
    printf("Livre retourn� avec succ�s.\n");
}

void listerLivres() {
    printf("Liste de tous les livres :\n");
    for (int i = 0; i < nombreLivres; i++) {
        printf("ID : %d, Titre : %s, Auteur : %s, %s\n", livres[i].id, livres[i].titre, livres[i].auteur, livres[i].estEmprunte ? "Emprunt�" : "Disponible");
    }
}

void sauvegarderDansFichier() {
    FILE *fichier = fopen("bibliotheque.dat", "wb");
    fwrite(&nombreLivres, sizeof(int), 1, fichier);
    fwrite(livres, sizeof(Livre), nombreLivres, fichier);
    fwrite(&nombreUtilisateurs, sizeof(int), 1, fichier);
    fwrite(utilisateurs, sizeof(Utilisateur), nombreUtilisateurs, fichier);
    fclose(fichier);
    printf("Donn�es de la biblioth�que sauvegard�es avec succ�s.\n");
}

void chargerDepuisFichier() {
    FILE *fichier = fopen("bibliotheque.dat", "rb");
    if (fichier == NULL) {
        printf("Aucune donn�e sauvegard�e trouv�e.\n");
        return;
    }
    fread(&nombreLivres, sizeof(int), 1, fichier);
    fread(livres, sizeof(Livre), nombreLivres, fichier);
    fread(&nombreUtilisateurs, sizeof(int), 1, fichier);
    fread(utilisateurs, sizeof(Utilisateur), nombreUtilisateurs, fichier);
    fclose(fichier);
    printf("Donn�es de la biblioth�que charg�es avec succ�s.\n");
}

int main() {
    int choix;

    chargerDepuisFichier();

    do {
        printf("\nSyst�me de gestion de biblioth�que\n");
        printf("1. Ajouter un livre\n");
        printf("2. Ajouter un utilisateur\n");
        printf("3. Emprunter un livre\n");
        printf("4. Retourner un livre\n");
        printf("5. Lister les livres\n");
        printf("6. Sauvegarder et quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterLivre(); break;
            case 2: ajouterUtilisateur(); break;
            case 3: emprunterLivre(); break;
            case 4: retournerLivre(); break;
            case 5: listerLivres(); break;
            case 6: sauvegarderDansFichier(); break;
            default: printf("Choix invalide, veuillez r�essayer.\n");
        }
    } while (choix != 6);

    return 0;
}

