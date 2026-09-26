#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIDA_MAXIMA 100 

typedef enum {
    MAGO = 1,
    GUERREIRO,
    ARQUEIRO,
    LADINO,
    CLERIGO,
    PALADINO
} ClassePersonagem;

typedef struct {
    float x;
    float y;
} Posicao;

typedef struct {
    int identificador;
    char nome[50];
    ClassePersonagem classe;
    int vida;
    int pontuacao;
    Posicao posicao;         
} Personagem;

typedef struct {
    char nome_equipe[50];
    Personagem *catalogo;
    int quantidade;
    int capacidade;
} Equipe;

const char* obter_nome_classe(ClassePersonagem c) {
    switch(c) {
        case MAGO: return "Mago";
        case GUERREIRO: return "Guerreiro";
        case ARQUEIRO: return "Arqueiro";
        case LADINO: return "Ladino";
        case CLERIGO: return "Clerigo";
        case PALADINO: return "Paladino";
        default: return "Desconhecido";
    }
}

void exibir_personagem(Personagem p) {
    printf("ID: %02d | Nome: %-15s | Classe: %-10s | Vida: %3d | Pontos: %4d | Pos: (%.1f, %.1f)\n",
           p.identificador, p.nome, obter_nome_classe(p.classe), p.vida, p.pontuacao, p.posicao.x, p.posicao.y);
}

void listar_equipe(Equipe *eq) {
    printf("\n--- Equipe: %s (%d/%d integrantes) ---\n", eq->nome_equipe, eq->quantidade, eq->capacidade);
    if (eq->quantidade == 0) {
        printf("A equipe esta vazia.\n");
    } else {
        for (int i = 0; i < eq->quantidade; i++) {
            exibir_personagem(eq->catalogo[i]);
        }
    }
    printf("--------------------------------------------------------------------------------------\n");
}

Personagem criar_personagem(int id, const char *nome, ClassePersonagem classe, int vida, int pontuacao, float x, float y) {
    Personagem p;
    p.identificador = id;
    strncpy(p.nome, nome, sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0'; 
    p.classe = classe;
    p.vida = (vida > VIDA_MAXIMA) ? VIDA_MAXIMA : ((vida < 0) ? 0 : vida);
    p.pontuacao = pontuacao;
    p.posicao.x = x;
    p.posicao.y = y;
    return p;
}

void cadastrar_personagem(Equipe *eq, Personagem p) {
    if (eq->quantidade >= eq->capacidade) {
        int nova_capacidade = eq->capacidade * 2;
        Personagem *temp = (Personagem *)realloc(eq->catalogo, nova_capacidade * sizeof(Personagem));
        if (temp == NULL) {
            printf("Erro ao redimensionar equipe.\n");
            return;
        }
        eq->catalogo = temp;
        eq->capacidade = nova_capacidade;
    }
    eq->catalogo[eq->quantidade] = p;
    eq->quantidade++;
    printf("Personagem '%s' cadastrado com sucesso!\n", p.nome);
}

int buscar_personagem_idx(Equipe *eq, int id) {
    for (int i = 0; i < eq->quantidade; i++) {
        if (eq->catalogo[i].identificador == id) {
            return i;
        }
    }
    return -1;
}

void intercalar(Personagem *vetor, int inicio, int meio, int fim, int *comparacoes) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;
    Personagem *L = (Personagem *)malloc(n1 * sizeof(Personagem));
    Personagem *R = (Personagem *)malloc(n2 * sizeof(Personagem));

    for (int i = 0; i < n1; i++) L[i] = vetor[inicio + i];
    for (int i = 0; i < n2; i++) R[i] = vetor[meio + 1 + i];

    int i = 0, j = 0, k = inicio; 
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
    while (i < n1) { vetor[k] = L[i]; i++; k++; }
    while (j < n2) { vetor[k] = R[j]; j++; k++; }

    free(L); free(R);
}

void merge_sort_equipe(Personagem *vetor, int inicio, int fim, int *comparacoes) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;
        merge_sort_equipe(vetor, inicio, meio, comparacoes);
        merge_sort_equipe(vetor, meio + 1, fim, comparacoes);
        intercalar(vetor, inicio, meio, fim, comparacoes);
    }
}

int main() {
    Equipe minha_equipe;
    strcpy(minha_equipe.nome_equipe, "Herois de Lumnia");
    minha_equipe.quantidade = 0;
    minha_equipe.capacidade = 2; 
    minha_equipe.catalogo = (Personagem *)malloc(minha_equipe.capacidade * sizeof(Personagem));

    cadastrar_personagem(&minha_equipe, criar_personagem(45, "Gandalf", MAGO, 80, 150, 2.0f, 3.0f));
    cadastrar_personagem(&minha_equipe, criar_personagem(12, "Arthur", GUERREIRO, 100, 200, 10.0f, 15.0f));
    cadastrar_personagem(&minha_equipe, criar_personagem(88, "Legolas", ARQUEIRO, 75, 300, 5.0f, 8.0f));

    int opcao;
    do {
        printf("\n==================================\n");
        printf("       MENU DA EQUIPE             \n");
        printf("==================================\n");
        printf("1. Cadastrar Personagem\n");
        printf("2. Listar Equipe\n");
        printf("3. Buscar Personagem por ID\n");
        printf("4. Alterar Vida de Personagem\n");
        printf("5. Ordenar Equipe por ID (MergeSort)\n");
        printf("0. Sair\n");
        printf("==================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int id, vida, pontos, classe_in;
                char nome[50];
                float x, y;

                printf("ID: "); scanf("%d", &id);
                printf("Nome: "); scanf("%s", nome);
                printf("Classe (1-Mago, 2-Guerreiro, 3-Arqueiro, 4-Ladino, 5-Clerigo, 6-Paladino): ");
                scanf("%d", &classe_in);
                printf("Vida (0 a 100): "); scanf("%d", &vida);
                printf("Pontuacao inicial: "); scanf("%d", &pontos);
                printf("Posicao X e Y (Ex: 10.5 5.0): "); scanf("%f %f", &x, &y);

                Personagem novo = criar_personagem(id, nome, (ClassePersonagem)classe_in, vida, pontos, x, y);
                cadastrar_personagem(&minha_equipe, novo);
                break;
            }
            case 2:
                listar_equipe(&minha_equipe);
                break;
            case 3: {
                int id;
                printf("Digite o ID para busca: ");
                scanf("%d", &id);
                int idx = buscar_personagem_idx(&minha_equipe, id);
                if (idx != -1) {
                    printf("\nPersonagem Encontrado:\n");
                    exibir_personagem(minha_equipe.catalogo[idx]);
                } else {
                    printf("\nPersonagem nao encontrado na equipe.\n");
                }
                break;
            }
            case 4: {
                int id, dano_cura;
                printf("Digite o ID do personagem: ");
                scanf("%d", &id);
                int idx = buscar_personagem_idx(&minha_equipe, id);
                if (idx != -1) {
                    printf("Digite a alteracao de vida (negativo p/ dano, positivo p/ cura): ");
                    scanf("%d", &dano_cura);
                    minha_equipe.catalogo[idx].vida += dano_cura;
                    
                    if(minha_equipe.catalogo[idx].vida > VIDA_MAXIMA) minha_equipe.catalogo[idx].vida = VIDA_MAXIMA;
                    if(minha_equipe.catalogo[idx].vida < 0) minha_equipe.catalogo[idx].vida = 0;
                    
                    printf("Vida atualizada! Novo estado:\n");
                    exibir_personagem(minha_equipe.catalogo[idx]);
                } else {
                    printf("\nPersonagem nao encontrado.\n");
                }
                break;
            }
            case 5: {
                if(minha_equipe.quantidade > 0) {
                    int comps = 0;
                    merge_sort_equipe(minha_equipe.catalogo, 0, minha_equipe.quantidade - 1, &comps);
                    printf("Equipe ordenada com sucesso! (Comparacoes: %d)\n", comps);
                    listar_equipe(&minha_equipe);
                } else {
                    printf("Equipe vazia.\n");
                }
                break;
            }
            case 0:
                printf("Saindo e liberando memoria...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    free(minha_equipe.catalogo);
    return 0;
}