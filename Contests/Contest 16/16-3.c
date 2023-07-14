#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

void printpppdouble(double ***fff){
    if(fff == NULL){
        printf("[0] -> NULL\n");
        return;
    }
    int i = 0;
    for(; fff[i] != NULL; i++){
        printf("[%d] -> ", i);
        for(int j = 0; fff[i][j] != NULL; j++){
            printf("{%.16lf}, ", fff[i][j][0]);
        }
        printf("NULL\n");
    }
    printf("[%d] -> NULL\n", i);
}

void freepppdouble(double ***fff){
    if(fff == NULL){
        return;
    }
    for(int i = 0; fff[i] != NULL; i++){
        for(int j = 0; fff[i][j] != NULL; j++){
            free(fff[i][j]);
        }
        free(fff[i]);
    }
    free(fff);
}

double ***transpose(double ***arr) {

    int rows = 0;
    while (arr[rows++] != NULL) {};

    int *sizes = calloc(rows, sizeof(*sizes));
    int max = 0;
    int j = 0;
    int max_row_index = 0;
    int max_index = 0;


    for (int i = 0; i < rows - 1; i++) {
        j = 0;
        while (arr[i][j++] != NULL) {
            sizes[i]++;
        }
        if (sizes[i] > max) {
            max = sizes[i];
            max_row_index = i;
        }
    }

    int max_len_after = 0;
    for (max_index = rows - 2; max_index >= 0; max_index--) {
        if ((max_index - 1 >= 0)  && (sizes[max_index] > sizes[max_index - 1])) {
            max_len_after = sizes[max_index];
            break;
        }
    }


    double*** matrix = calloc(max + 1, sizeof(double**));
    for (int i = 0; i < max + 1; i++) {
        matrix[i] = calloc(rows, sizeof(double*));
    }

    for (int i = 0; i < rows - 1; i++) {
        j = 0;
        while (arr[i][j] != NULL) {
            matrix[j][i] = calloc(1, sizeof(double));
            *matrix[j][i] = *arr[i][j];
            matrix[j][i + 1] = NULL;
            j++;
        }
    }

    int limit = max;
    for (int i = 0; i < max_index; i++) {
        if (i > max_row_index) {
            limit = max_len_after;
        }
        for (int j = sizes[i]; j < limit; j++) {
            matrix[j][i] = calloc(1, sizeof(double));
            *matrix[j][i] = 0.0;
        }
    }

    for (int i = 0; i < rows; i++) {
        free(matrix[max][i]);
    }
    free(matrix[max]);
    matrix[max] = NULL;

    free(sizes);
    return matrix;
}

int main(void){
    int rows;
    scanf("%d",&rows);
    int cols[rows];
    for(int i = 0; i < rows; i++){
        scanf("%d", &cols[i]);
    }
    double ***fuckfuckfuck = calloc(rows + 1, sizeof(double **));
    for(int i = 0; i < rows; i++){
        double **fuckfuck = calloc(cols[i] + 1, sizeof(double *));
        fuckfuckfuck[i] = fuckfuck;
        for(int j = 0; j < cols[i]; j++){
            double *fuck = calloc(1, sizeof(double));
            fuckfuck[j] = fuck;
            scanf("%lf", fuck);
        }
    }


    double ***fffr = transpose(fuckfuckfuck);
    freepppdouble(fuckfuckfuck);
    printpppdouble(fffr);
    freepppdouble(fffr);
}