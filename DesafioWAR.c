#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   

//Constantes Globais
#define TAM_NOME 30
#define TAM_COR 10

//Definição de struct territorio
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

//prototipo de funções
void limparBuffer();
Territorio* cadastrarTerritorios(int quantidade);
void exibirMapa(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);
int rolarDado();

//Funçao principal
int main() {
    srand(time(NULL)); //gera numeros aleatorios
    
    int quantidade;
    printf("=== SISTEMA DE GUERRA - WAR ===\n\n");
    printf("Quantos territorios deseja cadastrar? ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 1) {
        printf("Quantidade invalida! O minimo e 2 territorios.\n");
        return 1;
    }
    limparBuffer();
    
    //alocação dinamica de memoria
    Territorio* mapa = cadastrarTerritorios(quantidade);
    if (mapa == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1;
    }
    
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
        int idxAtacante, idxDefensor;
        printf("\nSelecione o territorio ATACANTE (numero): ");
        if (scanf("%d", &idxAtacante) != 1) continue;
        idxAtacante--;
        
        if (idxAtacante < 0 || idxAtacante >= quantidade) {
            printf("Territorio invalido!\n");
            limparBuffer();
            continue;
        }
        
        //SELECIONAR DEFENSOR
        printf("Selecione o territorio DEFENSOR (numero): ");
        if (scanf("%d", &idxDefensor) != 1) continue;
        idxDefensor--;
        
        if (idxDefensor < 0 || idxDefensor >= quantidade) {
            printf("Territorio invalido!\n");
            limparBuffer();
            continue;
        }
        limparBuffer();
        
        //VALIDAR ATAQUE
        if (idxAtacante == idxDefensor) {
            printf("Nao pode atacar o proprio territorio!\n");
            continue;
        }
        
        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Nao pode ataque territorio da mesma cor!\n");
            continue;
        }
        
        if (mapa[idxAtacante].tropas <= 1) {
            printf("Atacante precisa de pelo menos 2 tropas para atacar!\n");
            continue;
        }
        
        //REALIZAR ATAQUE
        printf("\n=== BATALHA ===\n");
        printf("%s (%s) ataca %s (%s)\n", 
                mapa[idxAtacante].nome, mapa[idxAtacante].cor,
                mapa[idxDefensor].nome, mapa[idxDefensor].cor);
        
        //ACESSO PONTEIROS NA CHAMADA DA FUNÇÃO
        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        
        //EXIBIR RESULTADO
        exibirMapa(mapa, quantidade);
        
        //VERIFICAR SE DESEJA CONTINUAR
        printf("\nDeseja realizar outro ataque? (1-Sim, 0-Nao): ");
        if (scanf("%d", &continuar) != 1) {
             continuar = 0;
        }
        limparBuffer();
    }
    
    //LIBERAR MEMORIA
    liberarMemoria(mapa);
    printf("\nJogo finalizado. Memoria liberada.\n");
    
    return 0;
}

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