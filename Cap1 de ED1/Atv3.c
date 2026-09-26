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
    // SISTEMA DE PLATAFORMAS COM ARITMÉTICA DE PONTEIROS
    // ====================================================

    printf("\n=== Fase de exploração: Plataformas ===\n");
    // 1. Criar um vetor com pelo menos cinco valores (pontos e alturas)
    int plataforma_pontos[5] = {10, -5, 20, 50, -10};
    float plataforma_alturas[5] = {1.5, 2.0, 3.0, 4.5, 2.5};
    float altura_total = 0.0;

    /* EXPLICAÇÃO OBRIGATÓRIA - DESLOCAMENTO E TIPO DO PONTEIRO:
   * O deslocamento *(vetor + i) funciona e respeita o tipo do ponteiro porque o 
     * compilador sabe exatamente o tamanho em bytes do tipo de dado que está sendo 
     * apontado (por exemplo, um 'int' geralmente ocupa 4 bytes). Quando fazemos 
     * (vetor + 1), o endereço de memória não avança apenas 1 byte físico, mas sim 
     * o equivalente a "1 * sizeof(tipo)". Isso garante que o ponteiro pule a 
     * quantidade exata de bytes para acessar precisamente o próximo elemento do vetor.
    */

    for (int i=0; i<5; i++){
        // 2. Acessar os elementos com *(vetor + i) (SEM COLCHETES)
        // 3. Calcular pontuação e altura total (acumulando na pontuação da main)
        pontuacao += *(plataforma_pontos + i);
        altura_total += *(plataforma_alturas + i);

        // 4. Mostrar índice, endereço e conteúdo de cada posição
        printf("Plataforma [Indice: %d]:\n",i);
        printf("[Pontos]  Endereço: %p |valor: %d\n", (void *)(plataforma_pontos + i), *(plataforma_pontos + i));
        printf("[Alturas] Endereço: %p|valor: %.2f\n\n", (void *)(plataforma_alturas + i), *(plataforma_alturas + i));
    }

    printf("\n=== RESULTADO FINAL DA EXPLORACAO ===\n");
    printf("Pontuação total do jogador: %d pontos\n", pontuacao);
    printf("Altura toatl percorrida no trajeto: %.2f metros\n", altura_total);
    return 0;
}