#include <stdio.h>
#include <locale.h>
#include <string.h>

const int MAX_NOME = 50;
const int MAX_APELIDO = 20;
const int MAX_SENHA = 30;
const int MAX_EQUIPE = 30;
const int MAX_EXIBICAO = 60;
const int QTD_JOGADORES = 5;

void remover_newline(char *str) {
    int tamanho = strlen(str);
    if (tamanho > 0 && str[tamanho - 1] == '\n') {
        str[tamanho - 1] = '\0';
    }
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrar_jogador() {
    char nome[MAX_NOME];
    char apelido[MAX_APELIDO];
    char senha[MAX_SENHA];
    char confirma_senha[MAX_SENHA];
    int dados_validos = 0;

    printf("\n--- MODO CADASTRO DE JOGADOR ---\n");
    limpar_buffer(); 

    printf("Digite o seu nome completo: ");
    fgets(nome, MAX_NOME, stdin);
    remover_newline(nome);

    while (!dados_validos) {
        int erro = 0;

        printf("Digite um apelido (máximo %d caracteres): ", MAX_APELIDO - 1);
        fgets(apelido, MAX_APELIDO, stdin);
        remover_newline(apelido);

        if (strlen(apelido) == 0) {
            printf(">> ERRO: O apelido não pode ficar em branco. Precisa ser corrigido.\n");
            erro = 1;
        }

        printf("Digite uma senha: ");
        fgets(senha, MAX_SENHA, stdin);
        remover_newline(senha);

        printf("Confirme a sua senha: ");
        fgets(confirma_senha, MAX_SENHA, stdin);
        remover_newline(confirma_senha);


        if (strcmp(senha, confirma_senha) != 0) {
            printf(">> ERRO: As senhas digitadas não coincidem. Precisa ser corrigido.\n");
            erro = 1;
        }

        if (erro == 0) {
            dados_validos = 1;
            printf("\nCadastro concluído com sucesso!\n");
            printf("Bem-vindo(a), %s (Apelido: %s).\n", nome, apelido);
        } else {
            printf("\n--- Por favor, insira os dados novamente ---\n");
        }
    }
}

void gerenciar_equipe_e_lista() {
    char apelido[MAX_APELIDO];
    char equipe[MAX_EQUIPE];
    char nome_exibicao[MAX_EXIBICAO];
    char separador[] = " | Equipe: ";
    char lista_jogadores[5][50] = {
        "ShadowHunter",
        "DragonSlayer",
        "PhoenixRise",
        "CyberKnight",
        "VortexMage"
    };

    printf("\n--- MODO EQUIPE E LISTA DE PARTICIPANTES ---\n");
    limpar_buffer();

    printf("Digite o seu apelido: ");
    fgets(apelido, MAX_APELIDO, stdin);
    remover_newline(apelido);

    printf("Digite o nome da sua equipe: ");
    fgets(equipe, MAX_EQUIPE, stdin);
    remover_newline(equipe);

    int tamanho_necessario = strlen(apelido) + strlen(separador) + strlen(equipe) + 1;

    if (tamanho_necessario <= MAX_EXIBICAO) {

        strcpy(nome_exibicao, apelido);
        strcat(nome_exibicao, separador);
        strcat(nome_exibicao, equipe);

        printf("\n[Nome de Exibição Gerado]: %s\n", nome_exibicao);
    } else {
        printf("\n>> ERRO: O tamanho combinado ultrapassa a capacidade máxima (%d caracteres)!\n", MAX_EXIBICAO);
    }

    printf("\n--- LISTA DE JOGADORES INSCRITOS ---\n");
    for (int i = 0; i < QTD_JOGADORES; i++) {
        printf("Posição [%d]: %s\n", i, lista_jogadores[i]);
    }
    char busca[50];
    printf("\nDigite o nome de um jogador para buscar na lista: ");
    fgets(busca, 50, stdin);
    remover_newline(busca);

    int encontrado = 0;
    for (int i = 0; i < QTD_JOGADORES; i++) {
        if (strcmp(lista_jogadores[i], busca) == 0) {
            printf(">> SUCESSO: Jogador '%s' ENCONTRADO na posição [%d]!\n", busca, i);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf(">> AVISO: Jogador '%s' NÃO foi encontrado na lista.\n", busca);
    }
}

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

void consultar_inventario(int *inventario[], int tamanho) {
    printf("\n--- CONSULTA DE INVENTÁRIO (Notação de Vetor) ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Slot [%d]: Endereço %p | Valor guardado do item: %d\n", i, (void*)inventario[i], *inventario[i]);
    }
}

void alterar_inventario(int *inventario[], int tamanho) {
    printf("\n--- ALTERAR INVENTÁRIO (Aritmética de Ponteiros) ---\n");
    for (int i = 0; i < tamanho; i++) {
        int novo_valor;
        printf("Digite o novo valor para o slot [%d] (Atual: %d): ", i, **(inventario + i));
        scanf("%d", &novo_valor);
        **(inventario + i) = novo_valor; 
    }
    printf("Itens do inventário atualizados com sucesso!\n");
}

int main() {
    setlocale(LC_ALL,"Portuguese");
    
    // Variáveis de estado
    int vida = 100;
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
        printf("5. Cadastrar Jogador (Validação de Dados)\n");
        printf("6. Gerenciar Equipes e Lista (Cópia, Concatenação e Busca)\n");
        printf("0. Sair\n");
        printf("------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int dano_recebido, cura_recebida;
                printf("\nDigite o dano a receber: ");
                scanf("%d", &dano_recebido);
                aplicar_dano(&vida, dano_recebido);
                
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
            case 5:
                cadastrar_jogador();
                break;
            case 6:
                gerenciar_equipe_e_lista();
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