#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int identificador;
    char nome[50];
    int vida;
    int pontuacao;
    float posicao_x;
    float posicao_y;
} Personagem;

void exibir_personagem(Personagem p) {
    printf("ID: %d | Nome: %s | Vida: %d | Pontos: %d | Posicao: (%.1f, %.1f)\n",
           p.identificador, p.nome, p.vida, p.pontuacao, p.posicao_x, p.posicao_y);
}

int main() {
    int capacidade_atual = 5; 
    int nova_capacidade;
    
    Personagem *catalogo = (Personagem *)calloc(capacidade_atual, sizeof(Personagem));
    if (catalogo == NULL) {
        printf("Erro na alocação inicial de memória.\n");
        return 1;
    }

    catalogo[0].identificador = 1;
    strcpy(catalogo[0].nome, "Guerreiro");
    catalogo[0].vida = 100;
    catalogo[0].pontuacao = 0;
    catalogo[0].posicao_x = 10.0f;
    catalogo[0].posicao_y = 15.5f;

    printf("\n--- Estado Inicial do Personagem ---\n");
    exibir_personagem(catalogo[0]);

    printf("\n--- A Aplicar Alteracoes... ---\n");
    
    int dano_sofrido = 120;
    catalogo[0].vida -= dano_sofrido;
    if (catalogo[0].vida < 0) {
        catalogo[0].vida = 0; 
    }
    printf("> O personagem sofreu %d de dano.\n", dano_sofrido);

    int pontos_ganhos = 250;
    catalogo[0].pontuacao += pontos_ganhos;
    printf("> O personagem ganhou %d pontos.\n", pontos_ganhos);

    catalogo[0].posicao_x += 5.0f;
    catalogo[0].posicao_y -= 2.5f;
    printf("> O personagem moveu-se.\n");
    printf("\n--- Estado Apos as Alteracoes ---\n");
    exibir_personagem(catalogo[0]);

    printf("\n=================================================\n");
    printf("Introduza a nova capacidade pretendida para o catálogo: ");
    
    if (scanf("%d", &nova_capacidade) != 1 || nova_capacidade <= 0) {
        printf("Erro: A capacidade deve ser um número inteiro positivo.\n");
        free(catalogo);
        return 1;
    }

    Personagem *temp = (Personagem *)realloc(catalogo, nova_capacidade * sizeof(Personagem));

    if (temp == NULL) {
        printf("\nFalha ao redimensionar a memória. O bloco original foi preservado.\n");
    } else {
        if (nova_capacidade > capacidade_atual) {
            Personagem personagem_vazio = {0}; 
            for (int i = capacidade_atual; i < nova_capacidade; i++) {
                temp[i] = personagem_vazio;
            }
        }
        
        catalogo = temp;

        printf("\n--- Redimensionamento Concluido ---\n");
        printf("Capacidade anterior: %d\n", capacidade_atual);
        printf("Nova capacidade: %d\n", nova_capacidade);
        
        capacidade_atual = nova_capacidade;
    }

    free(catalogo);
    printf("\nMemória libertada com sucesso. A encerrar o programa.\n");

    return 0;
}