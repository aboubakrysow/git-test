#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#define dimension 100
void creer_matrix_aleatoir(int dim, int matrix[dimension][dimension]) 
{
    srand(time(NULL));
    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            matrix[i][j] = rand() % 100; 
        }
    }
}
void sauvegarde_matrix_binaire(const char *filename, int dim, int matrix[dimension][dimension]) 
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(-1);
    }
    fwrite(&dim, sizeof(int), 1, file);
    fwrite(matrix, sizeof(int), dim * dim, file);
    fclose(file);
}
void sauvegarde_matrix_txt(const char *filename, int dim, int matrix[dimension][dimension]) 
{
    FILE *file = fopen(filename, "w");
    if (!file) 
    {
        perror("Erreur d'ouverture du fichier");
        exit(-1);
    }
    fprintf(file, "%d\n", dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}
void affich_matrix_binaire(const char *filename) 
{
    FILE *file = fopen(filename, "rb");
    if (!file) 
    {
        perror("Erreur d'ouverture du fichier");
        exit(-1);
    }
    int dim;
    fread(&dim, sizeof(int), 1, file);
    int matrix[dimension][dimension];
    fread(matrix, sizeof(int), dim * dim, file);
    fclose(file);

    printf("Matrice (%dx%d) :\n", dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void affich_matrix_txt(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(-1);
    }
    int dim;
    fscanf(file, "%d", &dim);
    int matrix[dimension][dimension];
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);

    printf("Matrice (%dx%d) :\n", dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
int main(int argc, char *argv[]) 
{
    int opt;
    int creer = 0, affichage = 0, dim = 0, binaire = 1;
    char *filename = NULL;
    while ((opt = getopt(argc, argv, "cad:f:bt")) != -1) {
        switch (opt) {
        case 'c':
            creer = 1;
            break;
        case 'a':
            affichage = 1;
            break;
        case 'd':
            dim = atoi(optarg);
            if (dim <= 0 || dim > dimension) {
                fprintf(stderr, "Dimension invalide\n");
                return -1;
            }
            break;
        case 'f':
            filename = optarg;
            break;
        case 'b':
            binaire = 1;
            break;
        case 't':
            binaire = 0;
            break;
        default:
            fprintf(stderr, "Usage: ./matrix -c|-a -d dimension -f nomfichier [-b|-t]\n");
            return -1;
        }
    }

    if (!filename) {
        fprintf(stderr, "Le nom du fichier est requis (-f)\n");
        return -1;
    }

    int matrix[dimension][dimension];

    if (creer) {
        creer_matrix_aleatoir(dim, matrix);
        if (binaire) {
            sauvegarde_matrix_binaire(filename, dim, matrix);
        } else {
            sauvegarde_matrix_txt(filename, dim, matrix);
        }
    } else if (affichage) {
        if (binaire) {
            affich_matrix_binaire(filename);
        } else {
            affich_matrix_txt(filename);
        }
    } else {
        fprintf(stderr, "Veuillez spécifier -c (création) ou -a (affichage)\n");
        return -1;
    }

    return 0;
}
