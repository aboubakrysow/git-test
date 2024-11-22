#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void split_fichier(const char *fichiername, size_t chunk_size) {
    int input_fd = open(fichiername, O_RDONLY);
    if (input_fd < 0) {
        perror("Erreur d'ouverture du fichier source");
        exit(-1);
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    int part_num = 1;

    while ((bytes_read = read(input_fd, buffer, chunk_size)) > 0) {
        char part_name[256];
        snprintf(part_name, sizeof(part_name), "part%d", part_num++);

        int part_fd = open(part_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (part_fd < 0) {
            perror("Erreur d'ouverture d'un fichier de sortie");
            close(input_fd);
            exit(-1);
        }

        if (write(part_fd, buffer, bytes_read) < 0) {
            perror("Erreur d'écriture dans un fichier de sortie");
            close(input_fd);
            close(part_fd);
            exit(-1);
        }

        close(part_fd);
    }

    close(input_fd);
    printf("Fichier %s découpé en %d parties.\n", fichiername, part_num - 1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s nomfichier taille\n", argv[0]);
        return -1;
    }

    const char *fichiername = argv[1];
    size_t chunk_size = atoi(argv[2]);

    if (chunk_size <= 0) {
        fprintf(stderr, "La taille doit être un entier positif.\n");
        return -1;
    }

    split_fichier(fichiername, chunk_size);
    return 0;
}
