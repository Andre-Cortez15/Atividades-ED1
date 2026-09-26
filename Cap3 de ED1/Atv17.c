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

// 1. Função que recebe por valor (Consulta sem alterar o original)
void exibir_personagem(Personagem p) {
    printf("ID: %d | Nome: %s | Vida: %d | Pontos: %d | Posicao: (%.1f, %.1f)\n",
           p.identificador, p.nome, p.vida, p.pontuacao, p.posicao_x, p.posicao_y);
}

/*
 * 2. Função de demonstração de passagem por valor com alteração.
 * EFEITO DA CÓPIA: Como a estrutura 'Personagem' é passada por valor, 
 * a função trabalha em uma cópia local isolada. As alterações feitas em 'p' 
 * dentro desta função não afetam a variável original que foi passada na main.
 * 
 * CUSTO DA CÓPIA: A passagem por valor de estruturas grandes (que contêm 
 * arrays, como o char nome[50]) exige que todos os bytes da struct sejam 
 * copiados para a pilha (stack) de execução. Isso consome mais memória e 
 * processamento em comparação com a passagem por referência (ponteiro), 
 * que copiaria apenas o endereço de memória (normalmente 4 ou 8 bytes).
 */
void demonstrar_passagem_por_valor(Personagem p) {
    p.vida = 9999;
    p.pontuacao = -500;
    strncpy(p.nome, "Copia Alterada", sizeof(p.nome) - 1);
    
    printf("-> [Estado Interno na Funcao] Os dados da COPIA foram alterados:\n");
    printf("   ");
    exibir_personagem(p);
}

Personagem criar_personagem(int id, const char *nome, int vida, int pontuacao, float x, float y) {
    Personagem p;
    p.identificador = id;
    strncpy(p.nome, nome, sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0'; 
    
    p.vida = vida;
    p.pontuacao = pontuacao;
    p.posicao_x = x;
    p.posicao_y = y;
    
    return p;
}

void alterar_nome(Personagem *p, const char *novo_nome) {
    strncpy(p->nome, novo_nome, sizeof(p->nome) - 1);
    p->nome[sizeof(p->nome) - 1] = '\0';
}

int main() {
    int capacidade_atual = 5; 
    int nova_capacidade;
    
    Personagem *catalogo = (Personagem *)calloc(capacidade_atual, sizeof(Personagem));
    if (catalogo == NULL) {
        printf("Erro na alocação inicial de memória.\n");
        return 1;
    }

    Personagem exemplo_designado = {
        .identificador = 99,
        .nome = "Mago Exemplo",
        .vida = 80,
        .pontuacao = 100,
        .posicao_x = 0.0f,
        .posicao_y = 0.0f
    };
    printf("\n--- Exemplo de Inicializacao Designada ---\n");
    exibir_personagem(exemplo_designado);
    catalogo[0] = criar_personagem(1, "Guerreiro", 100, 0, 10.0f, 15.5f);

    printf("\n--- Estado Inicial do Personagem no Catalogo ---\n");
    exibir_personagem(catalogo[0]);

    char buffer_nome[100];
    printf("\nDigite um novo nome para o personagem: ");
    if (fgets(buffer_nome, sizeof(buffer_nome), stdin) != NULL) {
        buffer_nome[strcspn(buffer_nome, "\n")] = '\0';
        alterar_nome(&catalogo[0], buffer_nome);
    }

    int dano_sofrido = 120;
    catalogo[0].vida -= dano_sofrido;
    if (catalogo[0].vida < 0) catalogo[0].vida = 0;

    catalogo[0].pontuacao += 250;
    catalogo[0].posicao_x += 5.0f;

    printf("\n--- Registro Completo Apos as Alteracoes ---\n");
    exibir_personagem(catalogo[0]);
    printf("\n=================================================\n");
    printf("--- Teste de Passagem por Valor (Atividade 17) ---\n");
    
    printf("\n-> [Estado Externo ANTES] Personagem na main:\n");
    printf("   ");
    exibir_personagem(catalogo[0]);
    demonstrar_passagem_por_valor(catalogo[0]);
    
    printf("-> [Estado Externo DEPOIS] Personagem na main (intacto):\n");
    printf("   ");
    exibir_personagem(catalogo[0]);
    printf("\n=================================================\n");

    printf("\nIntroduza a nova capacidade pretendida para o catálogo: ");
    
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
        printf("Nova capacidade: %d\n", nova_capacidade);\
    }

    free(catalogo);
    printf("\nMemória libertada com sucesso. A encerrar o programa.\n");

    return 0;
}