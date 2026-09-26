#include <stdio.h>

int main() {
    int vida = 100;
    int tesouro = 0; 
    int *pVida = &vida;
    int *pTesouro = &tesouro;
    int dano_recebido;
    int cura_recebida;

    printf("\n=== ESTADO INICIAL ===\n");
    printf("O tesouro esta ativo? %s\n", *pTesouro ? "Sim" : "Nao");
    printf("Vida: %d\n", vida);

    printf("\n=== APLICANDO DANO ===\n");
    printf("Digite a quantidade de dano que o personagem vai sofrer: ");
    scanf("%d", &dano_recebido);

    printf("Vida antes do dano: %d\n", vida);
    *pVida = *pVida - dano_recebido;

    if (*pVida <= 0) {
        *pVida = 0; 
        printf("Vida depois do dano: %d\n", vida);
        printf("ALERTA: O personagem recebeu muito dano e MORREU!\n\n");
    } else {
        printf("Vida depois do dano: %d\n", vida);

        printf("\n=== RESTAURANDO VIDA ===\n");
        printf("Digite a quantidade de cura para o personagem: ");
        scanf("%d", &cura_recebida);

        printf("Vida antes do restauramento: %d\n", vida);
        *pVida = *pVida + cura_recebida;

      
        if (*pVida > 100) {
            *pVida = 100; 
            printf("ALERTA: A cura ultrapassou o limite maximo! Vida ajustada para 100.\n");
        }
        
        printf("Vida depois do restauramento: %d\n\n", vida);
    }
    printf("=== ATIVANDO TESOURO ===\n");
    printf("O tesouro antes: %d\n", *pTesouro);
    *pTesouro = 1;
    printf("Tesouro depois: %d\n", *pTesouro);
    
    return 0;
}