#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"

// DEFINES 
#define LARGURA_TELA 800              
#define ALTURA_TELA 900               
#define FPS 60
#define GRAVIDADE 900.0f

#define MIN_FORCA_PULO 380.0f  

#define LARGURA_JOGADOR 50
#define ALTURA_JOGADOR 50

#define MAX_PLATAFORMAS 30
#define MAX_MOEDAS 30

#define DISTANCIA_MIN_PLATAFORMA 120
#define DISTANCIA_MAX_PLATAFORMA 180
#define ALTURA_PLATAFORMA 20

#define LARGURA_OBSTACULO 30
#define ALTURA_OBSTACULO 25

#define VALOR_MOEDA 1
#define TAM_NOME_SKIN 50
#define TEMPO_CONTAGEM_RETORNO 3

#define ARQUIVO_SAVE "save.dat"
#define NUM_SKINS_LOJA 4

// Taxas de progressão da dificuldade
#define TAXA_ACELERACAO 1.0f     
#define TAXA_REDUCAO_PULO 1.5f   

// Adicionado estado MANUAL
typedef enum { MENU_PRINCIPAL, MENU_DIFICULDADE, MANUAL, LOJA, JOGANDO, PAUSA, GAMEOVER } EstadoJogo;
typedef enum { FACIL, MEDIO, DIFICIL } Dificuldade;
typedef enum { PODER_NENHUM, PODER_VIDA_EXTRA, PODER_PULO_DUPLO } TipoPoder;

// ESTRUTURAS DO JOGO
typedef struct {
    float velocidadeScroll;
    int larguraPlataforma;
    int chanceObstaculo;
    float forcaPulo; 
} ConfigDificuldade;

typedef struct {
    float x;
    float y;
} Posicao;

typedef struct {
    Posicao pos;
    int largura;
    int altura;
    int temMoeda;
    int temObstaculo;
    TipoPoder poder;
} Plataforma;

typedef struct {
    Posicao pos;
    float velocidade_y;
    char nome[TAM_NOME_SKIN];
    int moedas;
    int pontuacao;
    int vidas;              
    int temPuloDuplo;       
    int pulosRestantes;     
    int skinAtual;          
} Personagem;

// ESTRUTURAS DA LOJA E SAVE
typedef struct {
    char nome[TAM_NOME_SKIN];
    Color cor;
    int preco;
} InfoSkin;

typedef struct {
    int moedasTotais;
    int skinSelecionada;
    int skinsDesbloqueadas[NUM_SKINS_LOJA];
} DadosSave;

// CATÁLOGO DE SKINS
InfoSkin catalogoSkins[NUM_SKINS_LOJA] = {
    {"Padrao (Branco)", RAYWHITE, 0},
    {"Azul (Veloz)", BLUE, 15},
    {"Vermelho (Ninja)", RED, 30},
    {"Dourado (Rei)", GOLD, 75}
};

// DECLARAÇÕES DE FUNÇÕES
void inicializarPersonagem(Personagem *p, const char *nome, int skinID);
void gerarPlataformas(Plataforma *plats, int numPlats, ConfigDificuldade cfg);
int atualizarFisica(Personagem *p, Plataforma *plats, int numPlats, float dt, ConfigDificuldade cfg);
void desenharFundoMatriz(int matrizFundo[5][5], int largura, int altura);
void desenharCenaJogo(Personagem *jogador, Plataforma *plataformas, int partidaIniciada);

void carregarDados(DadosSave *dados);
void salvarDados(DadosSave *dados);

int main(void) {
    SetTraceLogLevel(LOG_NONE); 
    
    InitWindow(LARGURA_TELA, ALTURA_TELA, "Jump Hop - Game");
    SetTargetFPS(FPS);
    SetExitKey(KEY_NULL); 

    // Estado inicial e Variáveis de Menu
    EstadoJogo estadoAtual = MENU_PRINCIPAL;
    int opcaoMenuPrincipal = 0; 
    int opcaoDificuldade = 1;   
    
    float temporizador = 0.0f;
    int partidaIniciada = 0;
    int opcaoPausa = 0;     
    int opcaoLoja = 0;      
    int deveSairDoJogo = 0;

    // Carregar save
    DadosSave dadosSave;
    carregarDados(&dadosSave);

    ConfigDificuldade niveisDificuldade[3] = {
        { 50.0f,  180, 15, 550.0f }, // FACIL
        { 90.0f,  140, 35, 550.0f }, // MEDIO
        { 130.0f, 110, 55, 550.0f }  // DIFICIL
    };
    
    ConfigDificuldade cfgAtual = niveisDificuldade[MEDIO]; 

    int fundoEstrelas[5][5] = {
        {1, 0, 0, 1, 0},
        {0, 1, 0, 0, 1},
        {0, 0, 1, 0, 0},
        {1, 0, 0, 1, 0},
        {0, 1, 0, 0, 1}
    };

    Plataforma *plataformas = (Plataforma *)malloc(MAX_PLATAFORMAS * sizeof(Plataforma));
    Personagem jogador;

    // Loop do Jogo
    while (!WindowShouldClose() && !deveSairDoJogo) {
        float dt = GetFrameTime();

        // ==========================================
        // ATUALIZAÇÃO DE ESTADOS E LÓGICA
        // ==========================================
        
        if (estadoAtual == MENU_PRINCIPAL) {
            if (IsKeyPressed(KEY_UP)) {
                opcaoMenuPrincipal--;
                if (opcaoMenuPrincipal < 0) opcaoMenuPrincipal = 2; 
            }
            if (IsKeyPressed(KEY_DOWN)) {
                opcaoMenuPrincipal++;
                if (opcaoMenuPrincipal > 2) opcaoMenuPrincipal = 0; 
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoMenuPrincipal == 0) {
                    estadoAtual = MENU_DIFICULDADE;
                } else if (opcaoMenuPrincipal == 1) {
                    estadoAtual = LOJA;
                    opcaoLoja = dadosSave.skinSelecionada; 
                } else if (opcaoMenuPrincipal == 2) {
                    deveSairDoJogo = 1; 
                }
            }
        }
        else if (estadoAtual == MENU_DIFICULDADE) {
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = MENU_PRINCIPAL; 
            }
            if (IsKeyPressed(KEY_UP)) {
                opcaoDificuldade--;
                if (opcaoDificuldade < 0) opcaoDificuldade = 2; 
            }
            if (IsKeyPressed(KEY_DOWN)) {
                opcaoDificuldade++;
                if (opcaoDificuldade > 2) opcaoDificuldade = 0; 
            }

            if (IsKeyPressed(KEY_ENTER)) {
                cfgAtual = niveisDificuldade[opcaoDificuldade]; 
                inicializarPersonagem(&jogador, "Atleta", dadosSave.skinSelecionada);
                gerarPlataformas(plataformas, MAX_PLATAFORMAS, cfgAtual);
                
                // Em vez de ir direto para o jogo, vai para o MANUAL
                estadoAtual = MANUAL; 
                partidaIniciada = 0; 
            }
        }
        else if (estadoAtual == MANUAL) {
            // Se apertar ENTER ou ESPAÇO no manual, o jogo começa
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                estadoAtual = JOGANDO;
            }
        }
        else if (estadoAtual == LOJA) {
            if (IsKeyPressed(KEY_ESCAPE) || (IsKeyPressed(KEY_ENTER) && dadosSave.skinsDesbloqueadas[opcaoLoja] && dadosSave.skinSelecionada == opcaoLoja)) {
                estadoAtual = MENU_PRINCIPAL;
            }
            
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_RIGHT)) {
                opcaoLoja = (opcaoLoja + 1) % NUM_SKINS_LOJA;
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_LEFT)) {
                opcaoLoja = (opcaoLoja - 1 + NUM_SKINS_LOJA) % NUM_SKINS_LOJA;
            }
            
            if (IsKeyPressed(KEY_ENTER)) {
                if (dadosSave.skinsDesbloqueadas[opcaoLoja]) {
                    dadosSave.skinSelecionada = opcaoLoja;
                    salvarDados(&dadosSave);
                    estadoAtual = MENU_PRINCIPAL; 
                } else if (dadosSave.moedasTotais >= catalogoSkins[opcaoLoja].preco) {
                    dadosSave.moedasTotais -= catalogoSkins[opcaoLoja].preco;
                    dadosSave.skinsDesbloqueadas[opcaoLoja] = 1;
                    dadosSave.skinSelecionada = opcaoLoja;
                    salvarDados(&dadosSave);
                    estadoAtual = MENU_PRINCIPAL; 
                }
            }
        }
        else if (estadoAtual == JOGANDO) {
            if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = PAUSA;
                opcaoPausa = 0;
            }
            else if (!partidaIniciada) {
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
                    partidaIniciada = 1;
                    jogador.velocidade_y = -cfgAtual.forcaPulo; 
                }
            } 
            else {
                cfgAtual.velocidadeScroll += TAXA_ACELERACAO * dt;
                cfgAtual.forcaPulo -= TAXA_REDUCAO_PULO * dt;
                if (cfgAtual.forcaPulo < MIN_FORCA_PULO) cfgAtual.forcaPulo = MIN_FORCA_PULO;

                int morreuParaObstaculo = atualizarFisica(&jogador, plataformas, MAX_PLATAFORMAS, dt, cfgAtual);
                
                if (jogador.pos.y > ALTURA_TELA || morreuParaObstaculo) {
                    if (jogador.vidas > 0) {
                        jogador.vidas--;
                        jogador.pos.y = ALTURA_TELA / 2.0f;
                        jogador.pos.x = LARGURA_TELA / 2.0f - LARGURA_JOGADOR / 2.0f;
                        jogador.velocidade_y = -cfgAtual.forcaPulo; 
                    } else {
                        dadosSave.moedasTotais += jogador.moedas;
                        salvarDados(&dadosSave);
                        estadoAtual = GAMEOVER;
                        temporizador = TEMPO_CONTAGEM_RETORNO;
                    }
                }
            }
        } 
        else if (estadoAtual == PAUSA) {
            if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = JOGANDO;
            }

            if (IsKeyPressed(KEY_UP)) {
                opcaoPausa--;
                if (opcaoPausa < 0) opcaoPausa = 2;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                opcaoPausa++;
                if (opcaoPausa > 2) opcaoPausa = 0;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoPausa == 0) {
                    estadoAtual = JOGANDO;
                } 
                else if (opcaoPausa == 1) { 
                    dadosSave.moedasTotais += jogador.moedas; 
                    salvarDados(&dadosSave);
                    
                    cfgAtual = niveisDificuldade[opcaoDificuldade]; 
                    inicializarPersonagem(&jogador, "Atleta", dadosSave.skinSelecionada);
                    gerarPlataformas(plataformas, MAX_PLATAFORMAS, cfgAtual);
                    estadoAtual = JOGANDO;
                    partidaIniciada = 0;
                } 
                else if (opcaoPausa == 2) { 
                    dadosSave.moedasTotais += jogador.moedas; 
                    salvarDados(&dadosSave);
                    estadoAtual = MENU_PRINCIPAL;
                }
            }
        }
        else if (estadoAtual == GAMEOVER) {
            temporizador -= dt;
            if (temporizador <= 0.0f || IsKeyPressed(KEY_ENTER)) {
                estadoAtual = MENU_PRINCIPAL;
            }
        }

        // ==========================================
        // RENDERIZAÇÃO
        // ==========================================
        BeginDrawing();
        ClearBackground(DARKBLUE);
        desenharFundoMatriz(fundoEstrelas, LARGURA_TELA, ALTURA_TELA);

        if (estadoAtual == MENU_PRINCIPAL) {
            DrawText("JUMP HOP", LARGURA_TELA / 2 - 120, ALTURA_TELA / 4, 50, WHITE);
            
            char strMoedas[50];
            sprintf(strMoedas, "Banco: %d Moedas", dadosSave.moedasTotais);
            DrawText(strMoedas, 20, 20, 24, GOLD);

            Color corJogar = (opcaoMenuPrincipal == 0) ? YELLOW : DARKGRAY;
            Color corLoja = (opcaoMenuPrincipal == 1) ? YELLOW : DARKGRAY;
            Color corSair = (opcaoMenuPrincipal == 2) ? YELLOW : DARKGRAY;

            DrawText((opcaoMenuPrincipal == 0) ? "> JOGAR <" : "  JOGAR  ", LARGURA_TELA / 2 - 85, ALTURA_TELA / 2 - 20, 40, corJogar);
            DrawText((opcaoMenuPrincipal == 1) ? "> LOJA DE SKINS <" : "  LOJA DE SKINS  ", LARGURA_TELA / 2 - 150, ALTURA_TELA / 2 + 50, 40, corLoja);
            DrawText((opcaoMenuPrincipal == 2) ? "> SAIR <" : "  SAIR  ", LARGURA_TELA / 2 - 75, ALTURA_TELA / 2 + 120, 40, corSair);

            DrawText("Setas para escolher e ENTER para confirmar", LARGURA_TELA / 2 - 220, ALTURA_TELA - 80, 20, LIGHTGRAY);
        }
        else if (estadoAtual == MENU_DIFICULDADE) {
            DrawText("DIFICULDADE", LARGURA_TELA / 2 - 140, ALTURA_TELA / 4, 40, WHITE);

            Color corFacil = (opcaoDificuldade == 0) ? YELLOW : DARKGRAY;
            Color corMedio = (opcaoDificuldade == 1) ? YELLOW : DARKGRAY;
            Color corDificil = (opcaoDificuldade == 2) ? YELLOW : DARKGRAY;

            DrawText((opcaoDificuldade == 0) ? "> FACIL <" : "  FACIL  ", LARGURA_TELA / 2 - 70, ALTURA_TELA / 2 - 20, 30, corFacil);
            DrawText((opcaoDificuldade == 1) ? "> MEDIO <" : "  MEDIO  ", LARGURA_TELA / 2 - 70, ALTURA_TELA / 2 + 30, 30, corMedio);
            DrawText((opcaoDificuldade == 2) ? "> DIFICIL <" : "  DIFICIL  ", LARGURA_TELA / 2 - 80, ALTURA_TELA / 2 + 80, 30, corDificil);

            DrawText("ESC para voltar ao Menu Principal", LARGURA_TELA / 2 - 170, ALTURA_TELA - 80, 20, LIGHTGRAY);
        }
        else if (estadoAtual == MANUAL) {
            // Desenho da tela de instruções (Manual)
            DrawText("COMO JOGAR", LARGURA_TELA / 2 - MeasureText("COMO JOGAR", 40)/2, 80, 40, YELLOW);

            int startY = 180;
            int espacamento = 45;

            // Seção de Controles
            DrawText("CONTROLES:", 50, startY, 24, WHITE);
            DrawText("- Mover: Setas Esquerda/Direita ou A/D", 80, startY + espacamento, 20, LIGHTGRAY);
            DrawText("- Pular: Espaco ou W", 80, startY + espacamento * 2, 20, LIGHTGRAY);
            DrawText("- Pausar: Tecla P ou ESC", 80, startY + espacamento * 3, 20, LIGHTGRAY);

            startY += espacamento * 5;

            // Seção de Itens e Objetos
            DrawText("O QUE ENCONTRAR NO CAMINHO:", 50, startY, 24, WHITE);

            // Moeda
            DrawCircle(80, startY + espacamento + 10, 8, YELLOW);
            DrawText("- Moeda (Pegue para comprar SKINS na loja!)", 110, startY + espacamento, 20, LIGHTGRAY);

            // Obstáculo
            DrawRectangle(70, startY + espacamento*2, LARGURA_OBSTACULO, ALTURA_OBSTACULO, RED);
            DrawText("- Obstaculo (Cuidado! Encostar = Perca 1 Vida)", 110, startY + espacamento*2 + 2, 20, LIGHTGRAY);

            // Vida Extra
            DrawCircle(80, startY + espacamento*3 + 10, 10, PINK);
            DrawText("+1V - Vida Extra (Sobreviva a um erro a mais)", 110, startY + espacamento*3, 20, LIGHTGRAY);

            // Pulo Duplo
            DrawCircle(80, startY + espacamento*4 + 10, 10, SKYBLUE);
            DrawText("2X  - Pulo Duplo (Pule novamente enquanto esta no ar!)", 110, startY + espacamento*4, 20, LIGHTGRAY);

            // Animação simples de "piscar" para o botão de começar
            if ((int)(GetTime() * 2) % 2 == 0) {
                DrawText("Pressione ENTER para Comecar!", LARGURA_TELA / 2 - MeasureText("Pressione ENTER para Comecar!", 24)/2, ALTURA_TELA - 100, 24, YELLOW);
            }
        }
        else if (estadoAtual == LOJA) {
            DrawText("LOJINHA DE SKINS", LARGURA_TELA / 2 - 160, 50, 40, YELLOW);
            
            char strMoedas[50];
            sprintf(strMoedas, "Saldo: %d Moedas", dadosSave.moedasTotais);
            DrawText(strMoedas, LARGURA_TELA / 2 - MeasureText(strMoedas, 24) / 2, 110, 24, GOLD);

            // Desenhar as opções de Skins
            for (int i = 0; i < NUM_SKINS_LOJA; i++) {
                int caixaX = LARGURA_TELA / 2 - 170;
                int caixaY = 180 + i * 90;
                
                Color corBorda = (opcaoLoja == i) ? YELLOW : DARKGRAY;
                DrawRectangleLines(caixaX, caixaY, 340, 70, corBorda);
                
                DrawRectangle(caixaX + 15, caixaY + 10, 50, 50, catalogoSkins[i].cor);
                DrawText(catalogoSkins[i].nome, caixaX + 80, caixaY + 15, 20, WHITE);
                
                if (dadosSave.skinsDesbloqueadas[i]) {
                    if (dadosSave.skinSelecionada == i) {
                        DrawText(">> EQUIPADO <<", caixaX + 80, caixaY + 40, 18, GREEN);
                    } else {
                        DrawText("Aperte ENTER para Equipar", caixaX + 80, caixaY + 40, 14, LIGHTGRAY);
                    }
                } else {
                    char strPreco[30];
                    sprintf(strPreco, "Preco: %d (ENTER P/ COMPRAR)", catalogoSkins[i].preco);
                    Color corPreco = (dadosSave.moedasTotais >= catalogoSkins[i].preco) ? GREEN : RED;
                    DrawText(strPreco, caixaX + 80, caixaY + 40, 16, corPreco);
                }
            }

            DrawText("Setas: Mover | ESC: Voltar ao Menu", LARGURA_TELA / 2 - 200, ALTURA_TELA - 80, 20, LIGHTGRAY);
        }
        else if (estadoAtual == JOGANDO) {
            desenharCenaJogo(&jogador, plataformas, partidaIniciada);
            DrawText("ESC: Pausar", LARGURA_TELA - 140, 20, 18, LIGHTGRAY);
        } 
        else if (estadoAtual == PAUSA) {
            desenharCenaJogo(&jogador, plataformas, partidaIniciada);
            DrawRectangle(0, 0, LARGURA_TELA, ALTURA_TELA, ColorAlpha(BLACK, 0.7f));

            DrawText("JOGO PAUSADO", LARGURA_TELA / 2 - 140, ALTURA_TELA / 3 - 40, 40, WHITE);

            Color corVoltar = (opcaoPausa == 0) ? YELLOW : RAYWHITE;
            Color corReiniciar = (opcaoPausa == 1) ? YELLOW : RAYWHITE;
            Color corMenu = (opcaoPausa == 2) ? YELLOW : RAYWHITE;

            DrawText((opcaoPausa == 0) ? "> Continuar <" : "  Continuar  ", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 - 10, 24, corVoltar);
            DrawText((opcaoPausa == 1) ? "> Reiniciar <" : "  Reiniciar  ", LARGURA_TELA / 2 - 100, ALTURA_TELA / 2 + 40, 24, corReiniciar);
            DrawText((opcaoPausa == 2) ? "> Voltar ao Menu <" : "  Voltar ao Menu  ", LARGURA_TELA / 2 - 120, ALTURA_TELA / 2 + 90, 24, corMenu);
        }
        else if (estadoAtual == GAMEOVER) {
            DrawText("GAME OVER", LARGURA_TELA / 2 - 120, ALTURA_TELA / 3, 40, RED);
            char textoRetorno[50];
            sprintf(textoRetorno, "Retornando ao Menu em: %d s", (int)temporizador + 1);
            DrawText(textoRetorno, LARGURA_TELA / 2 - MeasureText(textoRetorno, 22)/2, ALTURA_TELA / 2, 22, LIGHTGRAY);
            
            char moedas[50];
            sprintf(moedas, "Voce coletou %d moedas nesta partida!", jogador.moedas);
            DrawText(moedas, LARGURA_TELA / 2 - MeasureText(moedas, 20)/2, ALTURA_TELA / 2 + 40, 20, GOLD);
        }

        EndDrawing();
    }

    free(plataformas);
    CloseWindow();
    return 0;
}

// ==========================================
// SISTEMA DE SAVE (SALVAR/CARREGAR)
// ==========================================
void carregarDados(DadosSave *dados) {
    FILE *f = fopen(ARQUIVO_SAVE, "rb");
    if (f != NULL) {
        fread(dados, sizeof(DadosSave), 1, f);
        fclose(f);
    } else {
        dados->moedasTotais = 0;
        dados->skinSelecionada = 0; 
        dados->skinsDesbloqueadas[0] = 1; 
        for (int i = 1; i < NUM_SKINS_LOJA; i++) {
            dados->skinsDesbloqueadas[i] = 0;
        }
    }
}

void salvarDados(DadosSave *dados) {
    FILE *f = fopen(ARQUIVO_SAVE, "wb");
    if (f != NULL) {
        fwrite(dados, sizeof(DadosSave), 1, f);
        fclose(f);
    }
}

// ==========================================
// FUNÇÕES AUXILIARES, RENDERIZAÇÃO E FÍSICA
// ==========================================
void inicializarPersonagem(Personagem *p, const char *nome, int skinID) {
    p->pos.x = LARGURA_TELA / 2.0f - LARGURA_JOGADOR / 2.0f;
    p->pos.y = (ALTURA_TELA - 100.0f) - ALTURA_JOGADOR; 
    p->velocidade_y = 0.0f;
    p->moedas = 0;         
    p->pontuacao = 0;
    p->vidas = 0;
    p->temPuloDuplo = 0;
    p->pulosRestantes = 0;
    p->skinAtual = skinID; 
    strncpy(p->nome, nome, TAM_NOME_SKIN - 1);
    p->nome[TAM_NOME_SKIN - 1] = '\0';
}

void sortearConteudoPlataforma(Plataforma *p, ConfigDificuldade cfg, int permitirObstaculo) {
    int chanceRealObstaculo = permitirObstaculo ? cfg.chanceObstaculo : 0;
    
    if (GetRandomValue(1, 100) <= chanceRealObstaculo) {
        p->temObstaculo = 1;
        p->temMoeda = 0;
        p->poder = PODER_NENHUM;
    } else {
        p->temObstaculo = 0;
        int sorteio = GetRandomValue(1, 100);
        if (sorteio <= 35) {
            p->temMoeda = 1;
            p->poder = PODER_NENHUM;
        } else if (sorteio <= 55) { 
            p->temMoeda = 0;
            p->poder = (GetRandomValue(0, 1) == 0) ? PODER_VIDA_EXTRA : PODER_PULO_DUPLO;
        } else {
            p->temMoeda = 0;
            p->poder = PODER_NENHUM;
        }
    }
}

void gerarPlataformas(Plataforma *plats, int numPlats, ConfigDificuldade cfg) {
    plats[0].largura = cfg.larguraPlataforma; 
    plats[0].pos.x = LARGURA_TELA / 2.0f - plats[0].largura / 2.0f;
    plats[0].pos.y = ALTURA_TELA - 100.0f;
    plats[0].altura = ALTURA_PLATAFORMA;
    plats[0].temMoeda = 0;
    plats[0].temObstaculo = 0;
    plats[0].poder = PODER_NENHUM;

    float yBase = plats[0].pos.y - GetRandomValue(DISTANCIA_MIN_PLATAFORMA, DISTANCIA_MAX_PLATAFORMA);
    
    for (int i = 1; i < numPlats; i++) {
        plats[i].largura = cfg.larguraPlataforma;
        plats[i].pos.x = GetRandomValue(20, LARGURA_TELA - plats[i].largura - 20);
        plats[i].pos.y = yBase;
        plats[i].altura = ALTURA_PLATAFORMA;
        
        sortearConteudoPlataforma(&plats[i], cfg, i >= 10);
        
        yBase -= GetRandomValue(DISTANCIA_MIN_PLATAFORMA, DISTANCIA_MAX_PLATAFORMA);
    }
}

int atualizarFisica(Personagem *p, Plataforma *plats, int numPlats, float dt, ConfigDificuldade cfg) {
    float velHorizontal = 380.0f;
    
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  p->pos.x -= velHorizontal * dt;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) p->pos.x += velHorizontal * dt;

    if (p->pos.x < 0) p->pos.x = 0;
    if (p->pos.x > LARGURA_TELA - LARGURA_JOGADOR) p->pos.x = LARGURA_TELA - LARGURA_JOGADOR;

    p->velocidade_y += GRAVIDADE * dt;
    p->pos.y += p->velocidade_y * dt;

    Rectangle recJogador = { p->pos.x, p->pos.y, LARGURA_JOGADOR, ALTURA_JOGADOR };
    
    for (int i = 0; i < numPlats; i++) {
        if (plats[i].temObstaculo) {
            float obsX = plats[i].pos.x + plats[i].largura / 2.0f - LARGURA_OBSTACULO / 2.0f;
            float obsY = plats[i].pos.y - ALTURA_OBSTACULO;
            Rectangle recObs = { obsX, obsY, LARGURA_OBSTACULO, ALTURA_OBSTACULO };
            
            if (CheckCollisionRecs(recJogador, recObs)) {
                return 1; 
            }
        }
    }

    int noChao = 0;

    if (p->velocidade_y >= 0) { 
        for (int i = 0; i < numPlats; i++) {
            if (p->pos.x < plats[i].pos.x + plats[i].largura &&
                p->pos.x + LARGURA_JOGADOR > plats[i].pos.x &&
                p->pos.y + ALTURA_JOGADOR >= plats[i].pos.y &&
                p->pos.y + ALTURA_JOGADOR <= plats[i].pos.y + plats[i].altura + 12) {
                    
                p->pos.y = plats[i].pos.y - ALTURA_JOGADOR;
                p->velocidade_y = 0;
                noChao = 1;

                if (plats[i].temMoeda) {
                    p->moedas += VALOR_MOEDA;
                    plats[i].temMoeda = 0;
                }

                if (plats[i].poder != PODER_NENHUM) {
                    if (plats[i].poder == PODER_VIDA_EXTRA) p->vidas++;
                    else if (plats[i].poder == PODER_PULO_DUPLO) p->temPuloDuplo = 1;
                    plats[i].poder = PODER_NENHUM;
                }
            }
        }
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) {
        if (noChao) {
            p->velocidade_y = -cfg.forcaPulo;
            if (p->temPuloDuplo) p->pulosRestantes = 1; 
        } else if (p->temPuloDuplo && p->pulosRestantes > 0) {
            p->velocidade_y = -cfg.forcaPulo * 1.25f; 
            p->pulosRestantes--;
            p->temPuloDuplo = 0; 
        }
    }

    float minY = plats[0].pos.y;
    for (int j = 1; j < numPlats; j++) {
        if (plats[j].pos.y < minY) minY = plats[j].pos.y;
    }

    for (int i = 0; i < numPlats; i++) {
        plats[i].pos.y += cfg.velocidadeScroll * dt; 
        
        if (plats[i].pos.y > ALTURA_TELA) {
            plats[i].pos.y = minY - GetRandomValue(DISTANCIA_MIN_PLATAFORMA, DISTANCIA_MAX_PLATAFORMA);
            minY = plats[i].pos.y; 
            plats[i].pos.x = GetRandomValue(20, LARGURA_TELA - plats[i].largura - 20);
            sortearConteudoPlataforma(&plats[i], cfg, 1);
        }
    }

    if (noChao && p->velocidade_y >= 0) {
        p->pos.y += cfg.velocidadeScroll * dt; 
    }
    
    return 0;
}

void desenharCenaJogo(Personagem *jogador, Plataforma *plataformas, int partidaIniciada) {
    for (int i = 0; i < MAX_PLATAFORMAS; i++) {
        DrawRectangle(plataformas[i].pos.x, plataformas[i].pos.y, 
                      plataformas[i].largura, plataformas[i].altura, GREEN);
        
        if (plataformas[i].temMoeda) {
            DrawCircle(plataformas[i].pos.x + plataformas[i].largura / 2, 
                       plataformas[i].pos.y - 12, 8, YELLOW);
        }
        
        if (plataformas[i].temObstaculo) {
            float obsX = plataformas[i].pos.x + plataformas[i].largura / 2.0f - LARGURA_OBSTACULO / 2.0f;
            float obsY = plataformas[i].pos.y - ALTURA_OBSTACULO;
            DrawRectangle(obsX, obsY, LARGURA_OBSTACULO, ALTURA_OBSTACULO, RED);
        }

        if (plataformas[i].poder == PODER_VIDA_EXTRA) {
            float posX = plataformas[i].pos.x + plataformas[i].largura / 2.0f;
            DrawCircle(posX, plataformas[i].pos.y - 12, 10, PINK);
            DrawText("+1V", posX - 10, plataformas[i].pos.y - 18, 12, WHITE);
        } 
        else if (plataformas[i].poder == PODER_PULO_DUPLO) {
            float posX = plataformas[i].pos.x + plataformas[i].largura / 2.0f;
            DrawCircle(posX, plataformas[i].pos.y - 12, 10, SKYBLUE);
            DrawText("2X", posX - 8, plataformas[i].pos.y - 18, 12, DARKBLUE);
        }
    }

    Color corSkin = catalogoSkins[jogador->skinAtual].cor;
    DrawRectangle(jogador->pos.x, jogador->pos.y, LARGURA_JOGADOR, ALTURA_JOGADOR, corSkin);

    char textoPlacar[50];
    sprintf(textoPlacar, "Moedas: %d | Vidas: %d", jogador->moedas, jogador->vidas);
    DrawText(textoPlacar, 20, 20, 22, RAYWHITE);

    if (jogador->temPuloDuplo) {
        DrawText("Pulo Duplo: PRONTO!", 20, 48, 18, SKYBLUE);
    }

    if (!partidaIniciada) {
        DrawText("Pressione ESPACO ou W para pular e iniciar!", LARGURA_TELA / 2 - 250, ALTURA_TELA / 2, 22, YELLOW);
    }
}

void desenharFundoMatriz(int matriz[5][5], int largura, int altura) {
    int celulaW = largura / 5;
    int celulaH = altura / 5;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matriz[i][j] == 1) {
                DrawCircle(j * celulaW + celulaW / 2, i * celulaH + celulaH / 2, 3, DARKGRAY);
            }
        }
    }
}