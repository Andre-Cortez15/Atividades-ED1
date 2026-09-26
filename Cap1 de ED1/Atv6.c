#include <stdio.h>
#include <locale.h>

void aplicar_dano(int *vida, int dano){
    if(vida != NULL){
        printf("O endereço de vida recebido: %p\n", (void*)vida);
        *vida = *vida - dano;

        if(*vida <=0){
            *vida = 0;
            printf("ALERTA! O personagem sofreu muito dano e MORREU!\n");
        }
    }
    else{
        printf("Ponteiro de vida invalido!\n");
    }
}

void restaurar_vida(int *vida, int cura){
    if(vida != NULL){
        printf("Endereço da vida acessado: %p\n", (void *)vida);
        *vida = *vida + cura;
        if(*vida > 100){
            *vida = 100;
            printf("A cura ultrapassou o valor máximo! A vida foi ajustada para 100!\n");
        }
}
    else{
        printf("Ponteiro de vida invalido!\n");
    }
}

void pontuacao_dupla(int *pontuacao){
    if(pontuacao != NULL){
        printf("Endereço de pontuação acessado: %p\n",(void *)pontuacao);
       *pontuacao = *pontuacao * 2; 
    }
    else{
        printf("Ponteiro de pontuação invalido!\n");
    }
}

void ler_mapa(int *mapa, int tamanho) {
    printf("\n--- MODO PROJETISTA: Criando o Mapa ---\n");
    for (int i = 0; i < tamanho; i++) {
        int valor_valido = 0;
        while (valor_valido == 0) {
            printf("Digite os pontos da plataforma %d (entre -50 e 100): ", i);
            scanf("%d", (mapa + i));
            
            if (*(mapa + i) < -50 || *(mapa + i) > 100) {
                printf("Erro: Valor fora dos limites permitidos! Tente novamente.\n");
            } else {
                valor_valido = 1;
            }
        }
    }
}

void mostrar_mapa(int *mapa, int tamanho){
    printf("\n--- MODO PROJETISTA: Revisão do Mapa ---\n");
    for( int i = 0; i < tamanho; i++){
        printf("Plataforma [%d]: %d pontos | Endereco: %p\n", i,*(mapa + i), (void *)(mapa + i));
    }
}

void explorar_mapa(int *mapa, int tamanho, int *pontuacao_atual) {
    printf("\n=== FASE DE EXPLORAÇÃO: MODO CURSOR ===\n");
    
    int *cursor = mapa; 
    int *fim_do_mapa = mapa + tamanho; 

    while (cursor < fim_do_mapa) {
        int posicao_logica = cursor - mapa; 
        *pontuacao_atual += *cursor; 
        printf("Cursor na posição [%d] -> Valor lido: %d | Pontuação atualizada: %d\n", 
               posicao_logica, *cursor, *pontuacao_atual);
        cursor++; 
    }
    printf("\n--- RESUMO DO PERCURSO ---\n");
    printf("Total de plataformas visitadas com sucesso: %d\n", tamanho);
    printf("Pontuação final do jogador após o percurso: %d pontos\n", *pontuacao_atual);
}

// Função utilizando notação de vetor (inventario[i])
void consultar_inventario(int *inventario[], int tamanho) {
    printf("\n--- CONSULTA DE INVENTÁRIO (Notação de Vetor) ---\n");
    for (int i = 0; i < tamanho; i++) {
        // inventario[i] acessa o ponteiro. *inventario[i] acessa o valor apontado.
        printf("Slot [%d]: Endereço %p | Valor guardado do item: %d\n", i, (void*)inventario[i], *inventario[i]);
    }
}

// Função utilizando aritmética de ponteiros (*(inventario + i))
void alterar_inventario(int *inventario[], int tamanho) {
    printf("\n--- ALTERAR INVENTÁRIO (Aritmética de Ponteiros) ---\n");
    for (int i = 0; i < tamanho; i++) {
        int novo_valor;
        // *(inventario + i) pega o ponteiro atual. **(inventario + i) pega o valor final.
        printf("Digite o novo valor para o slot [%d] (Atual: %d): ", i, **(inventario + i));
        scanf("%d", &novo_valor);
        **(inventario + i) = novo_valor; 
    }
    printf("Itens do inventário atualizados com sucesso!\n");
}

int main() {
    setlocale(LC_ALL,"Portuguese");
    
    // Variáveis de estado
    int vida = 100; //
    int tesouro = 0;
    int pontuacao = 150;
    int *pTesouro = &tesouro;

    // Declaração de três itens independentes
    int item_pocao = 50;
    int item_escudo = 100;
    int item_bota = 15;

    // Vetor de ponteiros guardando as referências dos itens
    int *inventario[3] = {&item_pocao, &item_escudo, &item_bota};

    // Variáveis para controle do mapa e menu
    int plataformas_pontos[50]; 
    int tamanho_fase = 0;
    int opcao = -1;

    // Menu principal
    while (opcao != 0) {
        printf("\n==========================================\n");
        printf("              MENU SIMULADOR              \n");
        printf("==========================================\n");
        printf("Estado atual -> Vida: %d | Pontuação: %d\n\n", vida, pontuacao);
        printf("1. Testar Vida (Aplicar Dano / Restaurar)\n");
        printf("2. Testar Pontuação (Bônus Duplo)\n");
        printf("3. Testar Mapa (Criar e Explorar)\n");
        printf("4. Gerenciar Inventário (Consultar / Alterar)\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int dano_recebido, cura_recebida;
                printf("\nDigite o dano a receber: ");
                scanf("%d", &dano_recebido);
                aplicar_dano(&vida, dano_recebido);//
                
                if (vida > 0) {
                    printf("\nDigite a cura a receber: ");
                    scanf("%d", &cura_recebida);
                    restaurar_vida(&vida, cura_recebida);
                }
                break;
            }
            case 2:
                pontuacao_dupla(&pontuacao);
                break;
            case 3:
                printf("\nQuantas plataformas a fase terá? (1 a 50): ");
                scanf("%d", &tamanho_fase);
                if(tamanho_fase > 0 && tamanho_fase <= 50) {
                    ler_mapa(plataformas_pontos, tamanho_fase);
                    mostrar_mapa(plataformas_pontos, tamanho_fase);
                    explorar_mapa(plataformas_pontos, tamanho_fase, &pontuacao);
                } else {
                    printf("Erro: Tamanho de mapa inválido!\n");
                }
                break;
            case 4:
                consultar_inventario(inventario, 3);
                alterar_inventario(inventario, 3);
                
                printf("\n>>> Conferindo alterações finalizadas:\n");
                printf("item_pocao = %d, item_escudo = %d, item_bota = %d\n", item_pocao, item_escudo, item_bota);
                break;
            case 0:
                printf("\nEncerrando o simulador...\n");
                break;
            default:
                printf("\nErro: Opção inválida! Tente novamente.\n");
        }
    }

    return 0;
}