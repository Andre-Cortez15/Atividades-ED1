#include <stdio.h>
#include <stdlib.h> 
#include <locale.h>
#include <string.h>

const int MAX_NOME = 50;
const int MAX_APELIDO = 20;
const int MAX_SENHA = 30;
const int MAX_EQUIPE = 30;
const int MAX_EXIBICAO = 60;
const int QTD_JOGADORES = 5;
const int MAPA_LINHAS = 5;
const int MAPA_COLUNAS = 5;


typedef struct {
    char apelido[20];
} Jogador;

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


void inicializar_mapa_matricial(int mapa[MAPA_LINHAS][MAPA_COLUNAS]) {
    for (int i = 0; i < MAPA_LINHAS; i++) {
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            mapa[i][j] = 0; 
        }
    }
}

void exibir_mapa_matricial(int mapa[MAPA_LINHAS][MAPA_COLUNAS]) {
    printf("\n--- VISUALIZAÇÃO DA FORMAÇÃO ---\n");
    printf("     ");
    for (int j = 0; j < MAPA_COLUNAS; j++) printf("C%d   ", j);
    printf("\n");
    for (int i = 0; i < MAPA_LINHAS; i++) {
        printf("L%d: ", i);
        for (int j = 0; j < MAPA_COLUNAS; j++) {
            if (mapa[i][j] == 0) {
                printf("[ - ]");
            } else {
                printf("[%3d]", mapa[i][j]); 
            }
        }
        printf("\n");
    }
}

int validar_coordenadas(int l, int c) {
    return (l >= 0 && l < MAPA_LINHAS && c >= 0 && c < MAPA_COLUNAS);
}

void posicionar_jogador(int mapa[MAPA_LINHAS][MAPA_COLUNAS]) {
    int id, l, c;
    printf("\nDigite o ID do jogador (número inteiro): ");
    scanf("%d", &id);
    printf("Digite a linha (0 a %d): ", MAPA_LINHAS - 1);
    scanf("%d", &l);
    printf("Digite a coluna (0 a %d): ", MAPA_COLUNAS - 1);
    scanf("%d", &c);

    if (!validar_coordenadas(l, c)) {
        printf(">> ERRO: Coordenadas fora dos limites do mapa!\n");
        return;
    }

    if (mapa[l][c] == 0) {
        mapa[l][c] = id;
        printf(">> SUCESSO: Jogador %d posicionado na célula (L%d, C%d).\n", id, l, c);
    } else {
        printf(">> ERRO: A célula (L%d, C%d) já está ocupada pelo jogador %d.\n", l, c, mapa[l][c]);
    }
}

void remover_posicao(int mapa[MAPA_LINHAS][MAPA_COLUNAS]) {
    int l, c;
    printf("\nDigite a linha para remover jogador (0 a %d): ", MAPA_LINHAS - 1);
    scanf("%d", &l);
    printf("Digite a coluna para remover jogador (0 a %d): ", MAPA_COLUNAS - 1);
    scanf("%d", &c);

    if (!validar_coordenadas(l, c)) {
        printf(">> ERRO: Coordenadas fora dos limites do mapa!\n");
        return;
    }

    if (mapa[l][c] != 0) {
        printf(">> SUCESSO: Jogador %d removido da célula (L%d, C%d).\n", mapa[l][c], l, c);
        mapa[l][c] = 0;
    } else {
        printf(">> AVISO: A célula selecionada já está livre.\n");
    }
}

void reposicionar_jogador(int mapa[MAPA_LINHAS][MAPA_COLUNAS]) {
    int l_orig, c_orig, l_dest, c_dest;
    printf("\n--- REPOSICIONAMENTO ---\n");
    printf("Coordenadas de ORIGEM (linha e coluna separadas por espaço): ");
    scanf("%d %d", &l_orig, &c_orig);

    if (!validar_coordenadas(l_orig, c_orig)) {
        printf(">> ERRO: Coordenadas de origem inválidas!\n");
        return;
    }
    if (mapa[l_orig][c_orig] == 0) {
        printf(">> ERRO: Nenhuma jogador encontrado na origem.\n");
        return;
    }

    printf("Coordenadas de DESTINO (linha e coluna separadas por espaço): ");
    scanf("%d %d", &l_dest, &c_dest);

    if (!validar_coordenadas(l_dest, c_dest)) {
        printf(">> ERRO: Coordenadas de destino inválidas!\n");
        return;
    }
    if (mapa[l_dest][c_dest] != 0) {
        printf(">> ERRO: Célula de destino já ocupada!\n");
        return;
    }

    mapa[l_dest][c_dest] = mapa[l_orig][c_orig];
    mapa[l_orig][c_orig] = 0;
    printf(">> SUCESSO: Jogador movido para (L%d, C%d).\n", l_dest, c_dest);
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


void gerenciar_cadastro_dinamico() {
    int qtd_jogadores;
    
    printf("\n--- MODO CADASTRO DINÂMICO DE EQUIPE ---\n");
    printf("Quantos jogadores a equipe terá? ");
    scanf("%d", &qtd_jogadores);
    limpar_buffer();

    if (qtd_jogadores <= 0) {
        printf(">> ERRO: A quantidade de jogadores deve ser maior que zero.\n");
        return;
    }
    
    // Documentação solicitada: Vantagem de usar sizeof(*ponteiro)
    // O uso de sizeof(*equipe) evita erros caso o tipo da estrutura Jogador seja alterado futuramente.
    // O compilador deduz automaticamente o tamanho do tipo base para o qual o ponteiro aponta.
    
    // Alocação dinâmica calculada
    Jogador *equipe = (Jogador *)malloc(qtd_jogadores * sizeof(*equipe));
    
    // Verificar o retorno de malloc antes de acessar o ponteiro
    if (equipe == NULL) {
        printf(">> ERRO CRÍTICO: Falha na alocação de memória. Operação encerrada.\n");
        return;
    }
    
    // Preencher as posições válidas
    for (int i = 0; i < qtd_jogadores; i++) {
        printf("Digite o apelido do jogador %d: ", i + 1);
        fgets(equipe[i].apelido, MAX_APELIDO, stdin);
        remover_newline(equipe[i].apelido);
    }
    
    // Exibir o mapa do cadastro dinâmico
    printf("\n--- REGISTROS ALOCADOS ---\n");
    for (int i = 0; i < qtd_jogadores; i++) {
        printf("Posição [%d]: Apelido: %s\n", i, equipe[i].apelido);
    }
    
    // Liberar o bloco quando não for mais necessário
    free(equipe);
    printf(">> Memória do cadastro dinâmico liberada com sucesso.\n");
}

int main() {
    setlocale(LC_ALL,"Portuguese");

    int vida = 100;
    int tesouro = 0;
    int pontuacao = 150;
    int *pTesouro = &tesouro;
    int item_pocao = 50;
    int item_escudo = 100;
    int item_bota = 15;
    int *inventario[3] = {&item_pocao, &item_escudo, &item_bota};
    int plataformas_pontos[50]; 
    int tamanho_fase = 0;
    int matriz_equipe[MAPA_LINHAS][MAPA_COLUNAS];
    inicializar_mapa_matricial(matriz_equipe);

    int opcao = -1;
    while (opcao != 0) {
        printf("\n==========================================\n");
        printf("              MENU SIMULADOR              \n");
        printf("==========================================\n");
        printf("Estado atual -> Vida: %d | Pontuação: %d\n\n", vida, pontuacao);
        printf("1. Testar Vida (Aplicar Dano / Restaurar)\n");
        printf("2. Testar Pontuação (Bônus Duplo)\n");
        printf("3. Testar Mapa Linear (Criar e Explorar)\n");
        printf("4. Gerenciar Inventário (Consultar / Alterar)\n");
        printf("5. Cadastrar Jogador (Validação de Dados)\n");
        printf("6. Gerenciar Equipes e Lista (Cópia, Concatenação e Busca)\n");
        printf("7. Gerenciar Mapa Matricial da Equipe\n");
        printf("8. Cadastro Alocado em Tempo de Execução\n");
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
            case 7: {
                int op_matriz = -1;
                while(op_matriz != 0) {
                    printf("\n--- MENU MAPA MATRICIAL ---\n");
                    printf("1. Exibir Mapa\n");
                    printf("2. Posicionar Jogador\n");
                    printf("3. Reposicionar Jogador\n");
                    printf("4. Remover Jogador\n");
                    printf("0. Voltar ao Menu Principal\n");
                    printf("Escolha: ");
                    scanf("%d", &op_matriz);
                    
                    switch(op_matriz) {
                        case 1: exibir_mapa_matricial(matriz_equipe); break;
                        case 2: posicionar_jogador(matriz_equipe); break;
                        case 3: reposicionar_jogador(matriz_equipe); break;
                        case 4: remover_posicao(matriz_equipe); break;
                        case 0: break;
                        default: printf(">> Opção inválida.\n");
                    }
                }
                break;
            }
            case 8:
                gerenciar_cadastro_dinamico();
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