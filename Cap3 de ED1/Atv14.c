#include <stdio.h>
#include <stdlib.h>

int main() {
    int capacidade_atual = 5; 
    int nova_capacidade;
    int *catalogo = (int *)calloc(capacidade_atual, sizeof(int));
    if (catalogo == NULL) {
        printf("Erro na alocação inicial de memória.\n");
        return 1;
    }

    printf("--- Capacidade Inicial: %d ---\n", capacidade_atual);
    printf("Estado inicial do vetor (posições zeradas):\n");
    for (int i = 0; i < capacidade_atual; i++) {
        printf("catalogo[%d] = %d\n", i, catalogo[i]);
    }

    printf("\nIntroduza a nova capacidade pretendida para o catálogo: ");
    
    if (scanf("%d", &nova_capacidade) != 1 || nova_capacidade <= 0) {
        printf("Erro: A capacidade deve ser um número inteiro positivo.\n");
        free(catalogo);
        return 1;
    }

    int *temp = (int *)realloc(catalogo, nova_capacidade * sizeof(int));

    if (temp == NULL) {
        printf("\nFalha ao redimensionar a memória. O bloco original foi preservado.\n");
    } else {
        if (nova_capacidade > capacidade_atual) {
            for (int i = capacidade_atual; i < nova_capacidade; i++) {
                temp[i] = 0; 
            }
        }
        
        catalogo = temp;

        printf("\n--- Redimensionamento Concluído ---\n");
        printf("Capacidade anterior: %d\n", capacidade_atual);
        printf("Nova capacidade: %d\n", nova_capacidade);
        
        capacidade_atual = nova_capacidade;
        
        printf("\nEstado do vetor após o redimensionamento:\n");
        for (int i = 0; i < capacidade_atual; i++) {
            printf("catalogo[%d] = %d\n", i, catalogo[i]);
        }
    }

    free(catalogo);
    printf("\nMemória libertada com sucesso. A encerrar o programa.\n");

    return 0;
}