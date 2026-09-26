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

    return 0;
}