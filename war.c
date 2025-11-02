/*
 * PROJETO WAR ESTRUTURADO - NÍVEL MESTRE
 *
 * Este programa implementa um sistema simplificado do jogo War em C,
 * atendendo aos requisitos do desafio Nível Mestre.
 *
 * Funcionalidades:
 * 1. Cadastro de territórios (Nome, Cor, Tropas) em vetor de structs
 * alocado dinamicamente.
 * 2. Sistema de ataque (void atacar) entre territórios usando ponteiros
 * e lógica de dados (rand()).
 * 3. Sistema de missões estratégicas, com alocação dinâmica para a
 * missão do jogador e verificação de vitória.
 * 4. Modularização completa e gerenciamento de memória (malloc/calloc/free).
 * 
 * ALUNO: EDSON LUIZ BASSANI - 02-Nov-2025
 */

#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free, rand, srand
#include <string.h> // Para strcpy, strcmp
#include <time.h>   // Para time (seed do srand)

// --- ESTRUTURA DOS DADOS ---

/**
 * @brief Define a estrutura de um território no mapa.
 * (Requisito: Construir a base com structs)
 */
typedef struct {
    char nome[30];
    char cor[10]; // Cor do exército que domina o território
    int tropas;
} Territorio;


// --- VETOR GLOBAL DE MISSÕES ---

/**
 * @brief Vetor de strings contendo as descrições das missões.
 * (Requisito: Declaração de ao menos 5 missões)
 */
char* missoes[] = {
    "Conquistar 5 territorios",
    "Eliminar a cor Vermelha",
    "Eliminar a cor Verde",
    "Ter 15 tropas em um territorio",
    "Conquistar 3 territorios"
};
// Calcula o número total de missões no vetor
int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);


// --- PROTÓTIPOS DAS FUNÇÕES (MODULARIZAÇÃO) ---

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, char* missaoP1);


// --- FUNÇÃO PRINCIPAL (main) ---

int main() {
    int numTerritorios;
    Territorio* mapa = NULL;
    char* missaoJogador1 = NULL;
    int game_over = 0;
    int opcao;
    
    // Inicializa o gerador de números aleatórios
    // (Requisito: Utilizar srand(time(NULL)))
    srand(time(NULL));

    printf("--- BEM-VINDO AO WAR ESTRUTURADO (NIVEL MESTRE) ---\n");
    printf("Quantos territorios existem no mapa? ");
    scanf("%d", &numTerritorios);

    if (numTerritorios <= 1) {
        printf("Erro: O mapa precisa de pelo menos 2 territorios.\n");
        return 1;
    }

    // 1. ALOCAÇÃO DINÂMICA DOS TERRITÓRIOS
    // (Requisito: Utilizar calloc ou malloc para o vetor de Territorio)
    mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para o mapa.\n");
        return 1;
    }

    // 2. CADASTRO DOS TERRITÓRIOS
    cadastrarTerritorios(mapa, numTerritorios);

    // 3. ALOCAÇÃO E ATRIBUIÇÃO DA MISSÃO
    // (Requisito: Missão armazenada dinamicamente com malloc)
    missaoJogador1 = (char*)malloc(100 * sizeof(char)); // 100 char deve ser suficiente
    if (missaoJogador1 == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para a missao.\n");
        liberarMemoria(mapa, missaoJogador1); // Libera o que já foi alocado
        return 1;
    }

    // (Requisito: Chamar a função de atribuição)
    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    
    // (Requisito: Exibir a missão ao jogador apenas uma vez no início)
    printf("\n--- SUA MISSAO (Jogador 'Azul') ---\n");
    printf("Objetivo: %s\n", missaoJogador1);
    printf("--------------------------------------\n");
    printf("Pressione Enter para continuar...");
    // Limpa o buffer de entrada antes de esperar o Enter
    while (getchar() != '\n'); // Limpa buffer
    getchar(); // Espera pelo Enter

    // --- LOOP PRINCIPAL DO JOGO (Turno do Jogador 'Azul') ---
    while (!game_over) {
        exibirMapa(mapa, numTerritorios);

        printf("\nTurno do Jogador 'Azul'. O que deseja fazer?\n");
        printf("1. Atacar\n");
        printf("0. Encerrar turno (e verificar vitoria)\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int idxAtacante, idxDefensor;
            printf("Digite o ID do territorio ATACANTE (deve ser 'Azul'): ");
            scanf("%d", &idxAtacante);
            printf("Digite o ID do territorio DEFENSOR (nao pode ser 'Azul'): ");
            scanf("%d", &idxDefensor);

            // Validações
            if (idxAtacante < 0 || idxAtacante >= numTerritorios ||
                idxDefensor < 0 || idxDefensor >= numTerritorios) {
                printf("Erro: ID de territorio invalido.\n");
                continue;
            }

            // (Requisito: Validar para que o jogador não ataque a própria cor)
            // (Usando ponteiros para acessar os dados)
            Territorio* pAtacante = &mapa[idxAtacante];
            Territorio* pDefensor = &mapa[idxDefensor];

            if (strcmp(pAtacante->cor, "Azul") != 0) {
                printf("Erro: Voce so pode atacar com territorios da cor 'Azul'.\n");
                continue;
            }
            if (strcmp(pAtacante->cor, pDefensor->cor) == 0) {
                printf("Erro: Voce nao pode atacar um territorio que ja e seu.\n");
                continue;
            }
            if (pAtacante->tropas <= 1) {
                printf("Erro: O territorio atacante precisa ter mais de 1 tropa.\n");
                continue;
            }

            // 4. CHAMADA DA FUNÇÃO DE ATAQUE
            // (Requisito: Acessos e modificações por ponteiros)
            atacar(pAtacante, pDefensor);

        } else if (opcao == 0) {
            printf("Encerrando turno...\n");
            game_over = 1; // Simplificando: o jogo acaba ao encerrar o turno
        } else {
            printf("Opcao invalida.\n");
        }

        // 5. VERIFICAÇÃO DA MISSÃO
        // (Requisito: Verificar ao final do turno)
        if (verificarMissao(missaoJogador1, mapa, numTerritorios, "Azul")) {
            printf("\n=============================================\n");
            printf("!!! VITORIA !!!\n");
            printf("Jogador 'Azul' cumpriu a missao: %s\n", missaoJogador1);
            printf("=============================================\n");
            game_over = 1;
        } else if (game_over) {
            // Se o jogo terminou (opcao 0) e não houve vitória
            printf("Jogo encerrado. Missao nao cumprida.\n");
        }
    }

    // 6. LIBERAÇÃO DE MEMÓRIA
    // (Requisito: Liberar toda memória alocada dinamicamente)
    liberarMemoria(mapa, missaoJogador1);
    printf("Jogo finalizado. Memoria liberada.\n");

    return 0;
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

/**
 * @brief Preenche o vetor de territórios com dados do usuário.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param tamanho Número total de territórios.
 */
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    printf("\n--- CADASTRO DE TERRITORIOS ---\n");
    printf("Cores sugeridas: 'Azul', 'Vermelha', 'Verde'\n");
    for (int i = 0; i < tamanho; i++) {
        // (Uso de ponteiros: mapa[i] é equivalente a *(mapa + i))
        printf("--- Territorio %d ---\n", i);
        printf("Nome: ");
        scanf("%29s", mapa[i].nome); // %29s para evitar buffer overflow
        printf("Cor: ");
        scanf("%9s", mapa[i].cor);   // %9s
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Número total de territórios.
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- ESTADO ATUAL DO MAPA ---\n");
    for (int i = 0; i < tamanho; i++) {
        // (Uso de ponteiros: mapa[i].nome)
        printf("ID: %d | Nome: %-15s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------\n");
}

/**
 * @brief Sorteia uma missão do vetor global e a copia para o destino.
 * (Requisito: Implementar void atribuirMissao)
 * @param destino Ponteiro para a string onde a missão será armazenada (passagem por referência).
 * @param missoes Vetor de missões disponíveis.
 * @param totalMissoes Tamanho do vetor de missões.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    // Sorteia um índice aleatório
    int indiceSorteado = rand() % totalMissoes;
    
    // (Requisito: Copiar para a variável de missão usando strcpy)
    strcpy(destino, missoes[indiceSorteado]);
}

/**
 * @brief Simula um ataque entre dois territórios.
 * (Requisito: Implementar void atacar(Territorio* atacante, Territorio* defensor))
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // (Requisito: Simular rolagem de dados de 1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("  > Ataque de %s (%d tropas) contra %s (%d tropas)\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    printf("  > Dado Atacante: %d\n", dadoAtacante);
    printf("  > Dado Defensor: %d\n", dadoDefensor);

    // (Requisito: Atualização dos campos se o atacante vencer)
    if (dadoAtacante > dadoDefensor) {
        printf("  > VITORIA DO ATACANTE! %s foi conquistado.\n", defensor->nome);
        
        // (Requisito: Transfere a cor)
        strcpy(defensor->cor, atacante->cor);

        // (Requisito: Transfere metade das tropas do atacante)
        // (Garante que pelo menos 1 tropa seja movida se o atacante tiver > 1)
        int tropasMovidas = atacante->tropas / 2;
        if (tropasMovidas == 0 && atacante->tropas > 1) {
            tropasMovidas = 1;
        }
        
        defensor->tropas = tropasMovidas; // Território conquistado recebe as tropas
        atacante->tropas -= tropasMovidas; // Atacante perde as tropas movidas

    } else {
        // (Requisito: Se o atacante perder, perde uma tropa)
        printf("  > VITORIA DO DEFENSOR! %s perdeu 1 tropa.\n", atacante->nome);
        atacante->tropas--;
        // Garante que o território não fique com menos de 1 tropa após ataque
        if (atacante->tropas < 1) {
            atacante->tropas = 1;
        }
    }
}

/**
 * @brief Verifica se a missão do jogador foi cumprida.
 * (Requisito: Implementar int verificarMissao)
 * @param missao A string da missão do jogador (passagem por valor/ponteiro).
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Número total de territórios.
 * @param corJogador String da cor do jogador (ex: "Azul").
 * @return 1 (true) se a missão foi cumprida, 0 (false) caso contrário.
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    // (Requisito: Lógica simples inicial para verificação)
    
    // Missão: "Conquistar 5 territorios"
    if (strcmp(missao, "Conquistar 5 territorios") == 0) {
        int contagem = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contagem++;
            }
        }
        return (contagem >= 5);
    }
    
    // Missão: "Conquistar 3 territorios"
    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        int contagem = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contagem++;
            }
        }
        return (contagem >= 3);
    }

    // Missão: "Eliminar a cor Vermelha"
    if (strcmp(missao, "Eliminar a cor Vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0) {
                return 0; // Ainda existe, missão incompleta
            }
        }
        return 1; // Nenhuma "Vermelha" encontrada, missão completa
    }
    
    // Missão: "Eliminar a cor Verde"
    if (strcmp(missao, "Eliminar a cor Verde") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Ainda existe, missão incompleta
            }
        }
        return 1; // Nenhuma "Verde" encontrada, missão completa
    }

    // Missão: "Ter 15 tropas em um territorio"
    if (strcmp(missao, "Ter 15 tropas em um territorio") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 15) {
                return 1; // Encontrou um território, missão completa
            }
        }
        return 0; // Não encontrou, missão incompleta
    }

    return 0; // Missão desconhecida ou não implementada
}


/**
 * @brief Libera toda a memória alocada dinamicamente.
 * (Requisito: Implementar void liberarMemoria)
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missaoP1 Ponteiro para a string da missão do jogador 1.
 */
void liberarMemoria(Territorio* mapa, char* missaoP1) {
    // (Requisito: Usar free corretamente)
    if (mapa != NULL) {
        free(mapa);
    }
    if (missaoP1 != NULL) {
        free(missaoP1);
    }
}

