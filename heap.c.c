#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HEAP 20

enum algo_heap{First_Fit = 1, Best_Fit, Worst_Fit};

enum tipo_instrucao{set_heap = 1, new, del, exibe, atribui};

enum status_var{ sofreuDel = -1 };

enum status_heap{ livre, alocado, lixo = -1 };

int MARCADOR_HEAP;
int linha = 0;

// Estrutura com informações de uma variável declarada no programa
typedef struct Variavel{
    char nome[20];          // nome da variável
    int qtdMem;             // tamanho da memória referenciada
    int blocoInicial;             // Posiçaõ do bloco inicial livre / posições da heap que ela  está referenciando (vetor dinÂmico para alocar a partir do tamanho a ser alocado)
}t_variavel;

// Estrutura do nó de uma lista de variáveis do programa
typedef struct ListaDeVariaveis{
    t_variavel info;                // Informações da variável
    struct ListaDeVariais *prox;    // Ponteiro para o próximo
}l_variavel;

// Estrutura de área livre do bloco
typedef struct AreaLivre{
    int inicio;                 // Posição do primeiro bloco livre no heap
    int qtdBlocosContiguos;     // Quantidade de blocos contíguos livres
}t_areaLivre;

// Estrutura de nó de lista contendo as áreas livres do heap
typedef struct ListaDeAreasLivres{
    t_areaLivre info;                   // Informações do nó
    struct ListaDeAreasLivres *prox;    // Ponteiro para o próximo
}l_areaLivre;

// Estrutura para guardar partes da instrução // Inicializadas em '*'
typedef struct Instrucao{
    char str[10];
}t_instrucao;

// Função para criar novo elemento para lista de áreas livres
// Recebe o início da área livre e a quantidade blocos livres que a área vai conter
l_areaLivre *NovoElementoAreaLivre(int inicioDeAreaLivre, int qtdBlocosContiguos);

// Função para criar novo elemento para lista de variáveis
// Recebe o nome da variável, área inicial e quantidade de blocos de memória
l_variavel *NovoElementoVariavel(char *nome, int blocoInicial, int qtdBlocosContiguos);

// Função para inicializar lista de área livre
l_areaLivre *InicializarListaAreaLivre();

// Função para inicializar lista de variáveis
l_variavel *InicializarListaVariavel();

// Função para inserir área livre na lista de áreas livres
void InserirAreaLivre(l_areaLivre **listaAre, int inicio, int qtdBlocos);

// Função para mostrar todas as áreas livres do heap
void MostrarAreasLivres(l_areaLivre *lista);

// Função para mostrar variáveis utilizadas no programa em execução
void MostrarVariaveis(l_variavel *lista);

// Função para mostrar variáveis que referenciam mesma área de variável liberada
void MostrarOutrasVariaveis(char *nome, int inicioBlocoLivre);

// Função para liberar lista de áreas livres
l_areaLivre *DestruirListaDeAreasLivres(l_areaLivre *lista);

// Função para liberar lista de variáveis
l_variavel *DestruirListaDeVariaveis(l_variavel *lista);

// Função para verificar se uma variável já foi declarada no programa
// Caso já exista, a área que ela referencia é atualizada
// Retorna : 1 - lista vazia/não existe, 0 - existe
int Existe(l_variavel **listaVar, char *nome, int n_inicioAlocado, int qtdBlocosContiguos);

// Função para inserir nova variável na lista de variáveis
void InserirListaDeVar(l_variavel **listaVar, int n_inicioAlocado, int qtdBlocosContiguos, char *nome);

// Função para alocar parte de uma área livre
void ParticionarNo(l_areaLivre **area, int qtdTotal);

// Função que executa o algoritmo First fit
void FirstFit(int *resp, l_areaLivre **listaAreaLivre, int *n_inicioAlocado, int mem);

// Função que executa o algoritmo Best fit
void BestFit(int *resp, l_areaLivre **lista, int *n_inicioAlocado, int qtd);

// Função que executa o algoritmo Worst fit
void WorstFit(int *resp, l_areaLivre **lista, int *n_inicioAlocado, int qtdTotal);

// Função para atualizar o estado do vetor heap
void AtualizarHeap(int *heap, int indiceInicial, int indiceFinal, int status);

//  Função para procurar variável e retornar os valores do bloco inicial do heap para o qual ela aponta e quantos blocos contíguos a partir desse bloco inicial ocupa
//  Retorna 0 se variável já foi declarada - 1 caso contrário
int ProcuraVariavel(l_variavel **lista, char *var, int *inicioDeBloco, int *blocosContiguos);

//  Função auxiliar da atribuição de ponteiros, que faz a variável da esquerda apontar para a mesma área que a área da direita aponta
void Atribuir_aux(l_variavel **lista, char *e_var, int inicioDeBloco, int blocosContiguos);

// Função para realizar a atribuição
void Atribuir(l_variavel **listaVar, l_areaLivre **listaAreaLivre, t_instrucao *instrucao, int *heap);

// Função de configuração do algoritmo para evitar segmentação externa do heap
void ConfigurarHeap(t_instrucao *instrucao);

// Função para realizar a instrução de declaração
void Declarar(int *heap, l_variavel **listaVar, l_areaLivre **listaAreaLivre, t_instrucao *intrucao);

//  Função para atualizar lista de variáveis livres após deletar uma variável, liberando espaço no heap nas áreas desalocadas inserindo-as novamente na lista de variáveis livres
void LiberarListaDeAreasLivres(l_areaLivre **listaArea, int n_blocoLivre, int n_blocosContiguosLivres);

//  Função para verificar se existem outras variáveis que apontam para a mesma área de memória que a variável deletada  
void VerificarOutrasVariaveis(l_variavel **lista, int inicioBlocoLivre);

// Função para realizar a instrução de deletar
void Deletar(int *heap, l_variavel **listaVar, l_areaLivre **listaArea, char *variavel);

// Função para mostrar campos do heap e seus status 
void MostrarHeap(int *heap);

// Função para exibir a estrutura do heap, variáveis do programa e seu comportamento no momento e as áreas livres do heap no momento
void Exibe(l_variavel *listaVar, l_areaLivre *listaArea, int *heap);

// Função para simular execução programa
void ExecutarPrograma(char *nomeDoPrograma);

// Função para verificar o tipo de instrução
// Retorna um inteiro correspondente ao tipo
int VerificarInstrucao(t_instrucao *vet_instrucao);

int main()
{
    int continuar = 1;
    char nomeDoArquivo[20];

    while(continuar == 1){
        printf("Nome do programa: ");
        fflush(stdin);
        gets(nomeDoArquivo);

        ExecutarPrograma(nomeDoArquivo);

        printf("Executar novo programa?\n<0> N\n<1> S\n>>>");
        scanf("%d", &continuar);

        if(!continuar)
            exit(0);
        
        linha = 0;
    }
    return (0);
}

l_areaLivre *DestruirListaDeAreasLivres(l_areaLivre *lista){
    if(lista!=NULL)
    {
        l_areaLivre *aux = lista;
        l_areaLivre *prox = NULL;
        
        while(aux!=NULL)
        {
            prox = aux->prox;
         
            free(aux);

            aux = prox;
        }
    }
    return NULL;
}

l_variavel *DestruirListaDeVariaveis(l_variavel *lista)
{
    if(lista!=NULL)
    {
        l_variavel *aux = lista;
        l_variavel *prox = NULL;
        
        while(aux!=NULL)
        {
            prox = aux->prox;
         
            free(aux);

            aux = prox;
        }
    }
    return NULL;    
}

void MostrarAreasLivres(l_areaLivre *lista)
{
    l_areaLivre *aux = lista;

    while (aux != NULL)
    {
        printf("Inicio: %d   --  Blocos livres contiguos: %d\n", aux->info.inicio, aux->info.qtdBlocosContiguos);
        printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ \n");

        aux = aux->prox;
    }
}

l_areaLivre *NovoElementoAreaLivre(int inicioDeAreaLivre, int qtdBlocosContiguos)
{
    l_areaLivre *novoElemento = (l_areaLivre *)malloc(sizeof(l_areaLivre));

    novoElemento->info.inicio = inicioDeAreaLivre;

    novoElemento->info.qtdBlocosContiguos = qtdBlocosContiguos;

    novoElemento->prox = NULL;

    return novoElemento;
}

l_areaLivre *InicializarListaAreaLivre()
{
    l_areaLivre *areaLivreInicial = NovoElementoAreaLivre(0, TAM_HEAP);

    return areaLivreInicial;
}

void MostrarVariaveis(l_variavel *lista)
{
    if (lista == NULL)
    {
        printf("O programa ainda nao possui variaveis.\n");
    }
    else
    {
        l_variavel *aux = lista;

        while (aux != NULL)
        {
            if (aux->info.blocoInicial == -1)
                printf("%s -  nao referencia a'rea no heap\n", aux->info.nome);
            else
                printf("%s -  Blocos alocados: %d   --  Inicio: %d\n", aux->info.nome, aux->info.qtdMem, aux->info.blocoInicial);
            printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  _ _ \n");

            aux = aux->prox;
        }
    }
}

l_variavel *InicializarListaVariavel()
{
    return NULL;
}

l_variavel *NovoElementoVariavel(char *nome, int blocoInicial, int qtdBlocosContiguos)
{
    l_variavel *novaVar = (l_variavel *)malloc(sizeof(l_variavel));

    strcpy(novaVar->info.nome, nome);

    novaVar->info.blocoInicial = blocoInicial;
    novaVar->info.qtdMem = qtdBlocosContiguos;
    novaVar->prox = NULL;

    return novaVar;
}

int Existe(l_variavel **listaVar, char *nome, int n_inicioAlocado, int qtdBlocosContiguos)
{
    l_variavel *aux = (*listaVar);

    if ((*listaVar) == NULL)
        return 1;
    else
    {
        while (aux != NULL)
        {
            if (!strcmp(aux->info.nome, nome))
            {
                aux->info.blocoInicial = n_inicioAlocado;
                aux->info.qtdMem = qtdBlocosContiguos;
                return 0;
            }
            aux = aux->prox;
        }
        return 1;
    }
}

void InserirListaDeVar(l_variavel **listaVar, int n_inicioAlocado, int qtdBlocosContiguos, char *nome)
{
    int resp = Existe(listaVar, nome, n_inicioAlocado, qtdBlocosContiguos);

    if (resp == 1)
    {
        l_variavel *novaVar = NovoElementoVariavel(nome, n_inicioAlocado, qtdBlocosContiguos);
        l_variavel *aux = (*listaVar);
        l_variavel *ant = NULL;

        if ((*listaVar) == NULL)
            (*listaVar) = novaVar;
        else
        {
            while (aux != NULL)
            {
                ant = aux;
                aux = aux->prox;
            }
            ant->prox = novaVar;
        }
    }
}

void AtualizarHeap(int *heap, int indiceInicial, int indiceFinal, int status)
{
    int max = indiceInicial + indiceFinal;

    for (int i = indiceInicial; i < max; i++)
    {
        heap[i] = status;
    }
}

int ProcuraVariavel(l_variavel **lista, char *var, int *inicioDeBloco, int *blocosContiguos)
{
    l_variavel *aux = (*lista);

    while (aux != NULL)
    {
        if (!strcmp(aux->info.nome, var))
        {
            (*inicioDeBloco) = aux->info.blocoInicial;
            (*blocosContiguos) = aux->info.qtdMem;
            return 0;
        }
        aux = aux->prox;
    }
    return 1;
}

void Atribuir_aux(l_variavel **lista, char *e_var, int inicioDeBloco, int blocosContiguos)
{
    l_variavel *aux = (*lista);

    while (aux != NULL)
    {
        if (!strcmp(aux->info.nome, e_var))
        {
            aux->info.blocoInicial = inicioDeBloco;
            aux->info.qtdMem = blocosContiguos;
        }

        aux = aux->prox;
    }
}

void Atribuir(l_variavel **listaVar, l_areaLivre **listaAreaLivre, t_instrucao *instrucao, int *heap)
{
    int e_varInicioDeBloco, d_varInicioDeBloco, e_varBlocosContiguos, d_varBlocosContiguos;
    int resp;

    // Procurar quais áreas a variável da esquerda referencia
    resp = ProcuraVariavel(listaVar, instrucao[0].str, &e_varInicioDeBloco, &e_varBlocosContiguos);

    // Procurar quais áreas a variável da direita referencia
    ProcuraVariavel(listaVar, instrucao[2].str, &d_varInicioDeBloco, &d_varBlocosContiguos);

    if (!resp)
    {
        if (e_varInicioDeBloco != d_varInicioDeBloco)
        {
             // Fazer variável da esquerda apontar para a mesma área da variável da direita
            Atribuir_aux(listaVar, instrucao[0].str, d_varInicioDeBloco, d_varBlocosContiguos);

            // Se variável da esquerda não foi desalocada antes da atribuição, a área para a qual ela apontava vira lixo
            if (e_varInicioDeBloco != sofreuDel)
                AtualizarHeap(heap, e_varInicioDeBloco, e_varBlocosContiguos, lixo);
        }
    }
    else
        InserirListaDeVar(listaVar, d_varInicioDeBloco, d_varBlocosContiguos, instrucao[0].str);
}

void ConfigurarHeap(t_instrucao *instrucao)
{
    if (!strcmp(instrucao[1].str, "first"))
        MARCADOR_HEAP = First_Fit;

    else if (!strcmp(instrucao[1].str, "best"))
        MARCADOR_HEAP = Best_Fit;

    else if (!strcmp(instrucao[1].str, "worst"))
        MARCADOR_HEAP = Worst_Fit;
}

void MostrarHeap(int *heap)
{
    for (int i = 0; i < TAM_HEAP; i++)
    {
        switch (heap[i])
        {
            case livre:
                if (i == 0)
                    printf("|   |");
                else
                    printf("   |");
                break;

            case alocado:
                if (i == 0)
                    printf("| * |");
                else
                    printf(" * |");
                break;

            case lixo:
                if (i == 0)
                    printf("|lixo|");
                else
                    printf("lixo|");
                break;

            default:
                break;
        }
    }
    printf("\n");
}

void InserirAreaLivre(l_areaLivre **listaAre, int inicio, int qtdBlocos)
{
    l_areaLivre *nova = NovoElementoAreaLivre(inicio, qtdBlocos);
    l_areaLivre *aux = (*listaAre);
    l_areaLivre *ant = NULL;

    if ((*listaAre) != NULL)
    {
        while (aux != NULL)
        {
            if (aux->info.inicio > inicio)
                break;
            ant = aux;
            aux = aux->prox;
        }
        if (ant == NULL)
        {
            nova->prox = (*listaAre);

            (*listaAre) = nova;
        }
        else
        {
            ant->prox = nova;
            nova->prox = aux;
        }
    }

    (*listaAre) = nova;
}

void LiberarListaDeAreasLivres(l_areaLivre **listaArea, int n_blocoLivre, int n_blocosContiguosLivres)
{
    l_areaLivre *aux = (*listaArea);

    int indiceAnteriorAoPrimeiroBloco = n_blocoLivre - 1; // guarda valor do índice anterior e contíguo ao início da área a ser desalocada
    int indicePosteriorAoUltimoBloco = (n_blocoLivre + (n_blocosContiguosLivres - 1)) + 1; // guarda valor do índice do início da área livre contígua ao final da área a ser desalocada
    int tmp_inicio, tmp_final; // guardam  valor de onde começa/termina provável a área posterior/anterior ao conjunto de blocos liberados

    while (aux != NULL)
    {
        tmp_final = aux->info.inicio + (aux->info.qtdBlocosContiguos - 1);
        tmp_inicio = aux->info.inicio;

        if (tmp_final == indiceAnteriorAoPrimeiroBloco)
        {
            aux->info.qtdBlocosContiguos = aux->info.qtdBlocosContiguos + n_blocosContiguosLivres;

            if (aux->prox->info.inicio == indicePosteriorAoUltimoBloco) // Verifica se o bloco posterior a ele está livre
            {
                l_areaLivre *tmp = aux->prox;   // Referência a próxima área livre da lista que se tornou contígua á área em análise

                aux->info.qtdBlocosContiguos = aux->info.qtdBlocosContiguos + tmp->info.qtdBlocosContiguos;

                aux->prox = tmp->prox;

                free(tmp);

                break;
            }
        }
        else if (tmp_inicio == indicePosteriorAoUltimoBloco)
        {
            aux->info.inicio = n_blocoLivre;
            aux->info.qtdBlocosContiguos = aux->info.qtdBlocosContiguos + n_blocosContiguosLivres;
            break;
        }

        aux = aux->prox;
    }
    if (aux == NULL)
        InserirAreaLivre(listaArea, n_blocoLivre, n_blocosContiguosLivres);
}

void ParticionarNo(l_areaLivre **area, int qtdTotal)
{
    int inicioAntigo = (*area)->info.inicio;
    int qtdTotalAntiga = (*area)->info.qtdBlocosContiguos;

    (*area)->info.inicio = inicioAntigo + qtdTotal;
    (*area)->info.qtdBlocosContiguos = qtdTotalAntiga - qtdTotal;
}

void FirstFit(int *resp, l_areaLivre **lista, int *n_inicioAlocado, int qtdTotal)
{
    l_areaLivre *aux = (*lista);
    l_areaLivre *ant = NULL;

    int sobra;
    int cpy_inicio;

    while (aux != NULL)
    {
        sobra = aux->info.qtdBlocosContiguos - qtdTotal;

        if (sobra == 0)
        {
            (*n_inicioAlocado) = aux->info.inicio;

            (*resp) = 0;

            if ((*lista)->prox == NULL) // Caso só tenha um elemento na lista
            {
                free(*lista);
                (*lista) = NULL;
            }
            else
            {
                if (ant != NULL)
                    ant->prox = aux->prox;
                else
                    (*lista) = aux->prox;
                free(aux);
            }
            break;
        }
        else if (sobra > 0) // Se encontrar e se não for a quantidade exata
        {
            (*n_inicioAlocado) = aux->info.inicio;
            (*resp) = 0;

            ParticionarNo(&aux, qtdTotal);

            break;
        }
        ant = aux;
        aux = aux->prox;
    }
    if (aux == NULL)
    {
        printf("\n<AVISO> Sem espaco para alocar essa quantidade de memo'ria\n");
        (*resp) = 1;
    }
}

void WorstFit(int *resp, l_areaLivre **lista, int *n_inicioAlocado, int qtdTotal)
{
    int tmp_sobra;
    l_areaLivre *aux = (*lista);
    l_areaLivre *ant = NULL;         // guarda referência para área livre anterior á área analizada
    l_areaLivre *p_maiorSobra = NULL; // guarda referência para área de maior sobra de memória
    int maiorSobra = 0;

    while (aux != NULL)
    {
        tmp_sobra = aux->info.qtdBlocosContiguos - qtdTotal;

        // Verificar se área livre analizada no momento tem exatamente o mesmo tamanho da quantidade de área que se deseja alocar
        if (tmp_sobra == 0)
        {
            p_maiorSobra = aux;

            (*n_inicioAlocado) = aux->info.inicio;

            if ((*lista)->prox == NULL)
            {
                free(*lista);
                (*lista) = NULL;
            }
            else
            {
                if (ant != NULL)
                    ant->prox = aux->prox;
                else
                    (*lista) = aux->prox;
            }
            break;
        }
        //Verificar se ao preencher a atual área livre a quantidade restante de blocos contíguos livres analizados seria a maior de todas
        else if (tmp_sobra > 0 && tmp_sobra > maiorSobra)
        {
            p_maiorSobra = aux;

            if ((*lista)->prox == NULL) // Se a lista só possuir um elemento
            {
                (*n_inicioAlocado) = (*lista)->info.inicio;
                (*resp) = 0;

                break;
            }

            maiorSobra = tmp_sobra;
        }
        ant = aux;
        aux = aux->prox;
    }
    // Se p_maiorSobra for nulo significa que não encontrou espaço ou também se não for nulo e a sobra for negativa significando que não teria sobra suficiente
    if (p_maiorSobra == NULL || tmp_sobra < 0)
    {
        printf("\n<AVISO> Sem espaco para alocar essa quantidade de memo'ria\n");
        (*resp) = 1;
    }
    else if (tmp_sobra != 0) // Caso contrário e se já não foi encontrado um bloco com tamanho exato
    {
        (*n_inicioAlocado) = p_maiorSobra->info.inicio;

        ParticionarNo(&p_maiorSobra, qtdTotal);

        (*resp) = 0;
    }
}

void BestFit(int *resp, l_areaLivre **lista, int *n_inicioAlocado, int qtdTotal)
{
    int tmp_sobra;
    l_areaLivre *aux = (*lista);
    l_areaLivre *ant = NULL;          // guarda referência para área livre anterior á área analizada
    l_areaLivre *p_menorSobra = NULL; // guarda referência para área de menor sobra de memória
    int menorSobra = TAM_HEAP;

    while (aux != NULL)
    {
        tmp_sobra = aux->info.qtdBlocosContiguos - qtdTotal;

        // Verificar se área livre analizada no momento tem exatamente o mesmo tamanho da quantidade de áreaque se deseja alocar
        if (tmp_sobra == 0)
        {
            p_menorSobra = aux;

            (*n_inicioAlocado) = aux->info.inicio;

            if ((*lista)->prox == NULL) // Caso só tenha um elemento na lista
            {
                free(*lista);
                (*lista) = NULL;
            }
            else
            {
                if (ant != NULL)
                    ant->prox = aux->prox;
                else                         // remover do início
                    (*lista) = aux->prox;
            }
            break;
        }
        //Verificar se ao preencher a atual área livre a quantidade restante de blocos contíguos livres analizados seria a menor de todas
        else if (tmp_sobra > 0 && tmp_sobra < menorSobra)
        {
            p_menorSobra = aux;

            if ((*lista)->prox == NULL) // Se a lista só possuir um elemento
            {
                (*n_inicioAlocado) = (*lista)->info.inicio;
                (*resp) = 0;
                break;
            }

            menorSobra = tmp_sobra;
        }
        ant = aux;
        aux = aux->prox;
    }
    // Se p_menorSobra for nulo significa que não encontrou espaço ou também se não for nulo e a sobra for negativa significando que não teria sobra suficiente
    if (p_menorSobra == NULL || tmp_sobra < 0)
    {
        printf("\n<AVISO> Sem espaco para alocar essa quantidade de memo'ria\n");
        (*resp) = 1;
    }
    else if (tmp_sobra != 0) // Caso contrário e se já não foi encontrado um bloco com tamanho exato
    {
        (*n_inicioAlocado) = p_menorSobra->info.inicio;

        ParticionarNo(&p_menorSobra, qtdTotal);

        (*resp) = 0;
    }
}

void Declarar(int *heap, l_variavel **listaVar, l_areaLivre **listaAreaLivre, t_instrucao *instrucao)
{
    int n_inicioAlocado;
    int resp = 0 ; // Resposta da operação
    int mem = atoi(instrucao[2].str);

    if ((*listaAreaLivre) == NULL) // Se lista de áreas livres estiver vazia
        printf("\n<AVISO> Heap cheia\n");
    else                           // se não
    {
        // Verificar marcado de implementação do heap do momento
        switch (MARCADOR_HEAP)
        {
            case First_Fit:
                FirstFit(&resp, listaAreaLivre, &n_inicioAlocado, mem);
                break;

            case Best_Fit:
                BestFit(&resp, listaAreaLivre, &n_inicioAlocado, mem);
                break;

            case Worst_Fit:
                WorstFit(&resp, listaAreaLivre, &n_inicioAlocado, mem);
                break;
        }

        if (!resp) // Se tinha memória suficiente para a alocação
        {
            // Criar novo elemento para a lista de variáveis
            InserirListaDeVar(listaVar, n_inicioAlocado, mem, instrucao[1].str);

            // Atualizar heap
            AtualizarHeap(heap, n_inicioAlocado, mem, alocado);
        }
    }
}

void MostrarOutrasVariaveis(char *nome, int inicioBlocoLivre)
{
    printf("\n<AVISO> %s tambe'm referenciava o bloco %d\n", nome, inicioBlocoLivre);
}

void VerificarOutrasVariaveis(l_variavel **lista, int inicioBlocoLivre)
{
    l_variavel *auxVar = (*lista);

    while (auxVar != NULL)
    {
        if (auxVar->info.blocoInicial == inicioBlocoLivre)
        {
            auxVar->info.blocoInicial = sofreuDel;
            MostrarOutrasVariaveis(auxVar->info.nome, inicioBlocoLivre);
        }
        auxVar = auxVar->prox;
    }
}

void Deletar(int *heap, l_variavel **listaVar, l_areaLivre **listaArea, char *variavel)
{
    int n_blocoLivre, n_blocosContiguosLivres; // guarda informação da área que for liberada (bloco inicial e áreas contiguas á ele)

    l_variavel *auxVar = (*listaVar);

    // Procura na lista de variáveis a variável
    while (auxVar != NULL)
    {
        if (!strcmp(auxVar->info.nome, variavel))
            break;

        auxVar = auxVar->prox;
    }
    if(auxVar == NULL)      //Se não achar : variável não está lá 
        printf("Varia'vel inexistente\n");
    else     //Se achar
    {
        // copiar informações da variável que vai ser deletada
        n_blocoLivre = auxVar->info.blocoInicial;
        n_blocosContiguosLivres = auxVar->info.qtdMem;

        // Atualizar status de variáveis 
        auxVar->info.blocoInicial = sofreuDel;

        //Verificar se outra variável está referenciando essa mesma área
        VerificarOutrasVariaveis(listaVar, n_blocoLivre);

        // Atualiza a lista de áreas livres
        LiberarListaDeAreasLivres(listaArea, n_blocoLivre, n_blocosContiguosLivres);

        // Atualiza heap nos campos que não estão sendo referenciados para 0
        AtualizarHeap(heap, n_blocoLivre, n_blocosContiguosLivres, livre);
    }
}

void Exibe(l_variavel *listaVar, l_areaLivre *listaArea, int *heap)
{
    printf("\n\n> HEAP\n\n");
    MostrarHeap(heap);
    
    printf("\n\n> VARIA'VEIS \n\n");
    MostrarVariaveis(listaVar);

    printf("\n\n> A'REAS LIVRES \n\n");
    MostrarAreasLivres(listaArea);
}

int VerificarInstrucao(t_instrucao *vet_instrucao)
{
    // Verificar se é atribuição:
    if (strcmp(vet_instrucao[0].str, "exibe") && !strcmp(vet_instrucao[1].str, "="))
        return atribui;
    else  // Se não for, verifica se é:
    {
        //declaração
        if (!strcmp(vet_instrucao[0].str, "new"))
            return new;

        //configuração do heap
        else if (!strcmp(vet_instrucao[0].str, "heap"))
            return set_heap;

        //deletar
        else if (!strcmp(vet_instrucao[0].str, "del"))
            return del;

        //exibir heap
        else if (!strcmp(vet_instrucao[0].str, "exibe"))
            return exibe;
        else
            return -1; // erro
    }
}

void ExecutarPrograma(char *nomeDoPrograma)
{
    FILE *arq = fopen(nomeDoPrograma, "r");


    if (arq != NULL)
    {
        int heap[TAM_HEAP] = {livre}; // Estrutura do heap inicializada em 0's
        l_variavel *listaVar = InicializarListaVariavel();
        l_areaLivre *listaAre = InicializarListaAreaLivre();

        char *palavra;
        char instrucao[30];
        int tipoDeInstrucao;
        int indice;

        t_instrucao vet_instrucao[3];

        // enquanto não está no final do arquivo de instruções
        while (fscanf(arq, " %[^\n]s ", instrucao) != EOF)           // lê cada linha de instrução
        {
            linha++;
            indice = 0;

            printf("\n%d| %s\n", linha, instrucao);

            for (palavra = strtok(instrucao, " "); palavra != NULL; palavra = strtok(NULL, " "))
            {
                strcpy(vet_instrucao[indice].str, palavra);         // Copiando para o vetor de cada palavra da instrução
                indice++;
            }

            // Verificar o tipo de operação (<set_heap> | <new> | <del> | <exibe> | <atribui>)
            tipoDeInstrucao = VerificarInstrucao(vet_instrucao);

            switch (tipoDeInstrucao)
            {
            case set_heap:
                ConfigurarHeap(vet_instrucao);

                break;
            case new:
                Declarar(heap, &listaVar, &listaAre, vet_instrucao);

                break;
            case del:
                Deletar(heap, &listaVar, &listaAre, vet_instrucao[1].str);

                break;
            case exibe:
                Exibe(listaVar, listaAre, heap);
                break;
            case atribui:
                Atribuir(&listaVar, &listaAre, vet_instrucao, heap);
                break;
            default:
                break;
            }
            system("pause");
            system("cls");
        }
        listaAre = DestruirListaDeAreasLivres(listaAre);
        listaVar = DestruirListaDeVariaveis(listaVar);
    }
    fclose(arq);
}

