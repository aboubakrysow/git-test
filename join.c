#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
void join_fichier(const char *output_filename, int num_parts, char *parts[]) {
    int output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(-1);
    }
    char buffer[BUFFER_SIZE];
    for (int i = 0; i < num_parts; i++) {
        int part_fd = open(parts[i], O_RDONLY);
        if (part_fd < 0) {
            perror("Erreur d'ouverture d'un fichier de morceau");
            close(output_fd);
            exit(-1);
        }

        ssize_t bytes_read;
        while ((bytes_read = read(part_fd, buffer, sizeof(buffer))) > 0) {
            if (write(output_fd, buffer, bytes_read) < 0) {
                perror("Erreur d'écriture dans le fichier de sortie");
                close(output_fd);
                close(part_fd);
                exit(-1);
            }
        }

        close(part_fd);
    }

    close(output_fd);
    printf("Fichiers joints dans %s.\n", output_filename);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s fichier_de_sortie part1 part2 ...\n", argv[0]);
        return -1;
    }

    const char *output_filename = argv[1];
    int num_parts = argc - 2;
    char **parts = &argv[2];

    join_fichier(output_filename, num_parts, parts);
    return 0;
}
