#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>

typedef struct {
    int64_t **matrix;
    uint64_t column;
    uint64_t line;
} Matrix;

Matrix *init_matrix(uint64_t line, uint64_t column) {
    Matrix *new_matrix = (Matrix *) malloc(sizeof(Matrix));
    new_matrix->column = column;
    new_matrix->line = line;
    new_matrix->matrix = (int64_t **) malloc(line * sizeof(int64_t *));
    for (uint64_t i = 0; i < line; ++i)
        new_matrix->matrix[i] = (int64_t *) malloc(column * sizeof(int64_t));
    return new_matrix;
}

void matrix_print(Matrix *cur_matrix) {
    for (uint64_t i = 0; i < cur_matrix->line; i++) {
        for (uint64_t j = 0; j < cur_matrix->column; j++) {
            printf("%lld ", cur_matrix->matrix[i][j]);
        }
        puts("");
    }
}

int main(int argc, char **argv) {
    char *matrix_path = argv[1];
    FILE *fp = fopen(matrix_path, "rt");

    int line = 1, column = 1;
    char letter;
    while ((letter = (char) fgetc(fp)) != '\n') {
        if (letter == ',') {
            column++;
        }
    }

    while ((letter = (char) fgetc(fp)) != EOF) {
        if (letter == '\n') {
            line++;
        }
    }

    Matrix *incidence_matrix = init_matrix(line, column);

    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *) malloc(file_size * sizeof(char));
    char *record;

    for (int i = 0; i < incidence_matrix -> line; i++) {
        (fgets(buffer, file_size, fp));
        record = strtok(buffer, ",");
        for (int j = 0; j < incidence_matrix -> column; j++) {
            incidence_matrix->matrix[i][j] = (unsigned int) atoi(record);
            record = strtok(NULL, ",");
        }
    }
    fclose(fp);

    matrix_print(incidence_matrix);

    FILE *target_fd = fopen("output_data.dot", "wt");
    fprintf(target_fd, "graph {\n");

    int number_flag = 0;

    for (uint64_t k = 0; k < incidence_matrix->column; ++k) {
        for (uint64_t l = 0; l < incidence_matrix->line; ++l) {
            if (incidence_matrix->matrix[l][k] == 1) {
                if (!number_flag){
                    fprintf(target_fd, "%llu -- ", l + 1);
                    number_flag++;
                }
                else
                {
                    fprintf(target_fd, "%llu\n", l + 1);
                    number_flag--;
                }
            }
        }
    }

    fprintf(target_fd, "}");
    fclose(target_fd);

    system("dot -Tpng -O output_data.dot");

    return 0;
}
