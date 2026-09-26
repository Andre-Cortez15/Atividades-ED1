#include <stdio.h>
#include <locale.h>

void aplicador_dano(int *vida, int dano) {
    if(vida != NULL) {
        printf("O endereço de vida recebido: %p\n", (void*)vida);
        *vida = *vida - dano;
        
        if (*vida <= 0) {
            *vida = 0;
            printf("ALERTA: O personagem recebeu muito dano e MORREU!\n");
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
            printf("A cura ultrapassou o valor máximo! A vida foi ajustada para 100! \n");
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

void mostrar_mapa(const int *mapa, int tamanho) {
    printf("\n--- MODO PROJETISTA: Revisão do Mapa ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Plataforma [%d]: %d pontos | Endereco: %p\n", i, *(mapa + i), (void*)(mapa + i));
    }
}

int main() {
    setlocale(LC_ALL,"Portuguese");
    int vida = 100;
    int tesouro = 0;
    int pontuacao = 150;

    int *pTesouro = &tesouro;
    
    int dano_recebido;
    int cura_recebida;

    printf("\n=== ESTADO INICIAL ===\n");
    printf("Vida: %d | Endereço na main: %p\n", vida, (void *)&vida);
    printf("Pontuação: %d | Endereço na main: %p\n", pontuacao, (void *)&pontuacao);
    printf("O tesouro está ativo? %s\n", *pTesouro ? "Sim" : "Não");

    printf("\n=== APLICANDO DANO ===\n");
    printf("Digite a quantidade de dano que o personagem vai sofrer: ");
    scanf("%d", &dano_recebido);
    printf("Vida antes do dano: %d\n", vida);
    aplicador_dano(&vida, dano_recebido);
    printf("Vida na main depois do dano: %d\n", vida);

    if (vida > 0) {
        printf("\n=== RESTAURANDO VIDA ===\n");
        printf("Digite a quantidade de cura para o personagem: ");
        scanf("%d", &cura_recebida);
        printf("Vida antes do restauramento: %d\n", vida);
        restaurar_vida(&vida, cura_recebida);
        printf("Vida depois do restauramento: %d\n", vida);
    }

    printf("\n=== APLICANDO BONUS DE PONTUACAO ===\n");
    printf("Pontuacao antes do bonus: %d\n", pontuacao);
    pontuacao_dupla(&pontuacao);
    printf("Pontuacao na main depois do bonus: %d\n", pontuacao);

    printf("\n=== ATIVANDO TESOURO ===\n");
    printf("O tesouro antes: %d\n", *pTesouro);
    *pTesouro = 1;
    printf("Tesouro depois: %d\n", *pTesouro);

    // ====================================================
    // SISTEMA DE PLATAFORMAS (SEM TAMANHO FIXO)
    // ====================================================

    int tamanho_fase = 0;

    printf("\n=== CONFIGURAÇÃO DE FASE ===\n");
    while(tamanho_fase <= 0){
        printf("Quantas plataformas a fase terá? (Digite um valor positivo): ");
        scanf("%d", &tamanho_fase);
    }
    
    // Criação do vetor com base no tamanho digitado
    int plataformas_pontos[tamanho_fase];

    // Chamada das funções
    ler_mapa(plataformas_pontos, tamanho_fase);
    mostrar_mapa(plataformas_pontos, tamanho_fase);

    printf("\n=== FASE DE EXPLORAÇÃO: INICIADA ===\n");
    
    /* EXPLICAÇÃO OBRIGATÓRIA 
     * O deslocamento *(vetor + i) funciona e respeita o tipo do ponteiro porque o 
     * compilador sabe exatamente o tamanho em bytes do tipo de dado que está sendo 
     * apontado (por exemplo, um 'int' geralmente ocupa 4 bytes)...
    */

    // Cálculo utilizando apenas aritmética de ponteiros
    for (int i = 0; i < tamanho_fase; i++){
        pontuacao += *(plataformas_pontos + i);
        
        printf("Plataforma [%d] explorada. Pontuação atualizada: %d\n", i, pontuacao);
    }

    printf("\n=== RESULTADO FINAL DA EXPLORACAO ===\n");
    printf("Pontuação total do jogador: %d pontos\n", pontuacao);
    
    return 0;
}