#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   

//Constantes Globais
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100
#define TOTAL_MISSOES 5

//Definição de struct territorio
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// Vetor de missões pré-definidas
char* missoesPredefinidas[TOTAL_MISSOES] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor vermelha",
    "Controlar pelo menos 5 territorios",
    "Destruir todas as tropas inimigas",
    "Manter 10 tropas em seu territorio principal"
};

//prototipo de funções
void limparBuffer();
Territorio* cadastrarTerritorios(int quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
int rolarDado();
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void exibirMissao(char* missao);

//Funçao principal
int main() {
    srand(time(NULL)); 
    
    int quantidade;
    printf("=== SISTEMA DE GUERRA - WAR ===\n\n");
    printf("Quantos territorios deseja cadastrar? ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 1) {
        printf("Quantidade invalida! O minimo e 2 territorios.\n");
        return 1;
    }
    limparBuffer();
    
    //alocação dinamica de memoria para o mapa
    Territorio* mapa = cadastrarTerritorios(quantidade);
    if (mapa == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }

    // Alocação dinâmica para a missão do jogador
    char* missaoJogador = (char*)malloc(TAM_MISSAO * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro na alocacao de memoria para missao!\n");
        liberarMemoria(mapa);
        return 1;
    }
    
    // Atribuir e exibir missão
    atribuirMissao(missaoJogador, missoesPredefinidas, TOTAL_MISSOES);
    printf("\n=== MISSAO DO JOGADOR ===\n");
    exibirMissao(missaoJogador); 
    printf("===========================\n\n");
    
    exibirMapa(mapa, quantidade);
    
    //sistema de ataques
    int continuar = 1;
    while (continuar) {
        printf("\n=== FASE DE ATAQUE ===\n");
        
        //EXIBIR TERRITORIOS DISPONIVEIS
        printf("\nTerritorios disponiveis (ID):\n");
        for (int i = 0; i < quantidade; i++) {
            printf("%d - %s (%s) - %d tropas\n", 
                        i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
        }
        
        //SELECIONAR ATACANTE
        int idAtacante, idDefensor;
        printf("\nSelecione o territorio ATACANTE (numero): ");
        if (scanf("%d", &idAtacante) != 1) { limparBuffer(); continue; }
        idAtacante--;
        
        if (idAtacante < 0 || idAtacante >= quantidade) {
            printf("Territorio invalido!\n");
            limparBuffer();
            continue;
        }
        
        //SELECIONAR DEFENSOR
        printf("Selecione o territorio DEFENSOR (numero): ");
        if (scanf("%d", &idDefensor) != 1) { limparBuffer(); continue; }
        idDefensor--;
        
        if (idDefensor < 0 || idDefensor >= quantidade) {
            printf("Territorio invalido!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        //VALIDAR ATAQUE
        if (idAtacante == idDefensor) {
            printf("Nao pode atacar o proprio territorio!\n");
            continue;
        }
        
        if (strcmp(mapa[idAtacante].cor, mapa[idDefensor].cor) == 0) {
            printf("Nao pode ataque territorio da mesma cor!\n");
            continue;
        }
        
        if (mapa[idAtacante].tropas <= 1) {
            printf("Atacante precisa de pelo menos 2 tropas para atacar!\n");
            continue;
        }
        
        //REALIZAR ATAQUE
        printf("\n=== BATALHA ===\n");
        printf("%s (%s) ataca %s (%s)\n", 
                mapa[idAtacante].nome, mapa[idAtacante].cor,
                mapa[idDefensor].nome, mapa[idDefensor].cor);
        
        atacar(&mapa[idAtacante], &mapa[idDefensor]);
        
        //EXIBIR RESULTADO
        exibirMapa(mapa, quantidade);

        // VERIFICAR MISSÃO AO FINAL DO TURNO
        if (verificarMissao(missaoJogador, mapa, quantidade)) {
            printf("\n🎉 PARABENS! Voce cumpriu sua missao: %s\n", missaoJogador);
            printf("🏆 VITORIA DO JOGADOR!\n");
            break;
        }
        
        //VERIFICAR SE DESEJA CONTINUAR
        printf("\nDeseja realizar outro ataque? (1-Sim, 0-Nao): ");
        if (scanf("%d", &continuar) != 1) {
             continuar = 0;
        }
        limparBuffer();
    }
    
    //LIBERAR MEMORIA (mapa e missaoJogador)
    free(missaoJogador);
    liberarMemoria(mapa);
    printf("\nJogo finalizado. Memoria liberada.\n");
    
    return 0;
}

// ====================================================================
// FUNÇÕES DE UTILIDADE E JOGO BASE
// ====================================================================

int rolarDado() {
    return (rand() % 6) + 1;
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rolarDado();
    int dadoDefensor = rolarDado();
    
    printf("\nDado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA do atacante! Defensor perde 1 tropa.\n");
        defensor->tropas--;
        
        if (defensor->tropas <= 0) {
            printf("CONQUISTA! %s tomou %s.\n", atacante->nome, defensor->nome);
            
            strcpy(defensor->cor, atacante->cor);
            
            int tropasTransferidas = atacante->tropas / 2;
            if (tropasTransferidas == 0) tropasTransferidas = 1; // Garante pelo menos 1 tropa

            atacante->tropas -= tropasTransferidas;
            defensor->tropas = tropasTransferidas;
            
            printf("%d tropas foram movidas de %s para %s.\n", tropasTransferidas, atacante->nome, defensor->nome);
        }
        
    } else {
        printf("VITÓRIA do defensor (ou Empate). Atacante perde 1 tropa.\n");
        atacante->tropas--;
    }

    if (atacante->tropas < 0) atacante->tropas = 0;
    if (defensor->tropas < 0) defensor->tropas = 0;
}

Territorio* cadastrarTerritorios(int quantidade) {
    Territorio* mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) return NULL;
    
    printf("\n=== CADASTRO DE TERRITORIOS ===\n\n");
    
    for (int i = 0; i < quantidade; i++) {
        printf("Territorio %d:\n", i + 1);
        
        printf("  Nome: ");
        fgets(mapa[i].nome, TAM_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        
        printf("  Cor do exercito: ");
        fgets(mapa[i].cor, TAM_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        
        printf("  Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
        
        printf("\n");
    }
    
    return mapa;
}

void exibirMapa(Territorio* mapa, int quantidade) {
    printf("\n=== MAPA ATUAL ===\n");
    printf("==========================================\n");
    printf("%-20s %-10s %-10s\n", "TERRITORIO", "COR", "TROPAS");
    printf("==========================================\n");
    
    for (int i = 0; i < quantidade; i++) {
        printf("%-20s %-10s %-10d\n", 
                mapa[i].nome, 
                mapa[i].cor, 
                mapa[i].tropas);
    }
    
    printf("==========================================\n");
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ====================================================================
// FUNÇÕES DE MISSÃO E ESTRATÉGIA
// ====================================================================

void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indiceAleatorio = rand() % totalMissoes;
    strcpy(destino, missoes[indiceAleatorio]);
}

void exibirMissao(char* missao) {
    printf("Sua missao: %s\n", missao);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // A cor do jogador principal é definida pelo primeiro território (mapa[0])
    char corJogador[TAM_COR];
    if (tamanho > 0) {
        strcpy(corJogador, mapa[0].cor);
    } else {
        return 0; 
    }

    int countTerritoriosJogador = 0;
    int countTropasInimigas = 0;
    
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            countTerritoriosJogador++;
        } else {
            countTropasInimigas += mapa[i].tropas;
        }
    }
    
    // 1. "Conquistar 3 territorios seguidos" (Simplificado para 3 territórios do jogador)
    if (strstr(missao, "Conquistar 3 territorios seguidos")) {
        return countTerritoriosJogador >= 3;
    }
    
    // 2. "Eliminar todas as tropas da cor vermelha"
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0) {
                return 0;
            }
        }
        return 1;
    }
    
    // 3. "Controlar pelo menos 5 territorios"
    if (strstr(missao, "Controlar pelo menos 5 territorios")) {
        return countTerritoriosJogador >= 5;
    }
    
    // 4. "Destruir todas as tropas inimigas"
    if (strstr(missao, "Destruir todas as tropas inimigas")) {
        return countTropasInimigas == 0;
    }
    
    // 5. "Manter 10 tropas em seu territorio principal"
    if (strstr(missao, "Manter 10 tropas em seu territorio principal")) {
        return (mapa[0].tropas >= 10 && strcmp(mapa[0].cor, corJogador) == 0);
    }
    
    return 0;
}
