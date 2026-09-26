#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIDA_MAXIMA 100 

typedef struct {
    int identificador;
    char nome[50];
    int vida;
    int pontuacao;
    float posicao_x;
    float posicao_y;
} Personagem;

// FUNÇÕES AUXILIARES E DE MANIPULAÇÃO DE PERSONAGEM

void exibir_personagem(Personagem p) {
    printf("ID: %02d | Nome: %-15s | Vida: %3d | Pontos: %4d | Posicao: (%.1f, %.1f)\n",
           p.identificador, p.nome, p.vida, p.pontuacao, p.posicao_x, p.posicao_y);
}

void exibir_equipe(Personagem *equipe, int tamanho) {
    printf("\n--- Exibindo Equipe (%d integrantes) ---\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        exibir_personagem(equipe[i]);
    }
    printf("-----------------------------------------\n");
}

Personagem criar_personagem(int id, const char *nome, int vida, int pontuacao, float x, float y) {
    Personagem p;
    p.identificador = id;
    strncpy(p.nome, nome, sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0'; 
    p.vida = (vida > VIDA_MAXIMA) ? VIDA_MAXIMA : ((vida < 0) ? 0 : vida);
    p.pontuacao = pontuacao;
    p.posicao_x = x;
    p.posicao_y = y;
    return p;
}

void alterar_vida(Personagem *p, int delta) {
    if (p == NULL) return;
    p->vida += delta;
    if (p->vida > VIDA_MAXIMA) p->vida = VIDA_MAXIMA;
    if (p->vida < 0) p->vida = 0;
}

void adicionar_pontuacao(Personagem *p, int pontos) {
    if (p == NULL) return; 
    if (pontos > 0) p->pontuacao += pontos; 
}

void mover_personagem(Personagem *p, float dx, float dy) {
    if (p == NULL) return; 
    p->posicao_x += dx; 
    p->posicao_y += dy;
}

// -----------------------------------------------------------------
// INTERCALAÇÃO E ESTRUTURA RECURSIVA (DIVIDIR, RESOLVER, COMBINAR)
// -----------------------------------------------------------------

// Função para intercalar duas partes ordenadas em um único vetor ordenado
void intercalar(Personagem *vetor, int inicio, int meio, int fim, int *comparacoes) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    // Alocação dinâmica para subvetores temporários
    Personagem *L = (Personagem *)malloc(n1 * sizeof(Personagem));
    Personagem *R = (Personagem *)malloc(n2 * sizeof(Personagem));

    if (L == NULL || R == NULL) {
        printf("Erro de alocação nos vetores temporários da intercalação.\n");
        return;
    }

    for (int idx_l = 0; idx_l < n1; idx_l++) L[idx_l] = vetor[inicio + idx_l];
    for (int idx_r = 0; idx_r < n2; idx_r++) R[idx_r] = vetor[meio + 1 + idx_r];

    int i = 0;      
    int j = 0;      
    int k = inicio; 
    while (i < n1 && j < n2) {
        (*comparacoes)++;
        if (L[i].identificador <= R[j].identificador) {
            vetor[k] = L[i];
            i++;
        } else {
            vetor[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        vetor[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vetor[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void merge_sort_equipe(Personagem *vetor, int inicio, int fim, int *comparacoes) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        // ETAPA 1: DIVIDIR
        // O intervalo [inicio...fim] é dividido no ponto médio em duas metades:
        // Metade Esquerda: [inicio ... meio]
        // Metade Direita:  [meio + 1 ... fim]

        // ETAPA 2: RESOLVER
        // Chamadas recursivas para resolver/ordenar cada subproblema/metade
     
        merge_sort_equipe(vetor, inicio, meio, comparacoes);
        merge_sort_equipe(vetor, meio + 1, fim, comparacoes);
        
        // ETAPA 3: COMBINAR
        // Intercala os dois subvetores resolvidos em um único intervalo ordenado
        intercalar(vetor, inicio, meio, fim, comparacoes);
    }
}

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MAIN)
// -----------------------------------------------------------------------------

int main() {
    int capacidade_atual = 6; 
    int total_comparacoes = 0;
    Personagem *equipe = (Personagem *)calloc(capacidade_atual, sizeof(Personagem));
    if (equipe == NULL) {
        printf("Erro na alocação inicial da equipe.\n");
        return 1;
    }

    equipe[0] = criar_personagem(45, "Mago", 80, 150, 2.0f, 3.0f);
    equipe[1] = criar_personagem(12, "Guerreiro", 100, 200, 10.0f, 15.0f);
    equipe[2] = criar_personagem(88, "Arqueiro", 75, 300, 5.0f, 8.0f);
    equipe[3] = criar_personagem(05, "Ladino", 60, 450, 1.0f, 1.0f);
    equipe[4] = criar_personagem(33, "Clérigo", 90, 100, 4.0f, 4.0f);
    equipe[5] = criar_personagem(21, "Paladino", 95, 250, 7.0f, 9.0f);

    printf("=== ATIVIDADE 19 — EQUIPE COMO VETOR DINÂMICO DE ESTRUTURAS ===\n");
    printf("\n[ESTADO INICIAL] Equipe desordenada:");
    exibir_equipe(equipe, capacidade_atual);
    
    merge_sort_equipe(equipe, 0, capacidade_atual - 1, &total_comparacoes);
    printf("\n[ESTADO FINAL] Equipe ordenada por ID (Divisão e Conquista):");
    exibir_equipe(equipe, capacidade_atual);

    printf("\n--- MÉTROLOGIA DE EXECUÇÃO ---");
    printf("\nTotal de comparações realizadas na intercalação: %d\n", total_comparacoes);

    printf("\n--- EXPANDINDO A EQUIPE (REALLOC) ---\n");
    int nova_capacidade = 8;
    Personagem *temp = (Personagem *)realloc(equipe, nova_capacidade * sizeof(Personagem));
    
    if (temp != NULL) {
        equipe = temp;
        equipe[6] = criar_personagem(02, "Bárbaro", 100, 50, 12.0f, 2.0f);
        equipe[7] = criar_personagem(50, "Bardo", 70, 500, 3.0f, 6.0f);
        capacidade_atual = nova_capacidade;

        printf("Nova integrante adicionada! Equipe expandida para %d membros.\n", capacidade_atual);
        total_comparacoes = 0;
        merge_sort_equipe(equipe, 0, capacidade_atual - 1, &total_comparacoes);
        
        printf("\nEquipe reordenada após expansão:");
        exibir_equipe(equipe, capacidade_atual);
        printf("Comparações na nova intercalação: %d\n", total_comparacoes);
    }

    free(equipe);
    printf("\nMemória liberada com sucesso. Fim do programa.\n");

    return 0;
}