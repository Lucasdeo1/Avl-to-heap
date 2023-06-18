#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Lucas Deodato

//----------------------------arquivo para AVL e insere as palavras que mais se repete na heap

typedef struct arvore {
    char palavra[100];
    int altura;
    int repete;
    struct arvore* esq;
    struct arvore* dir;
} arvore;

typedef struct {
    char palavra[100];
    int frequencia;
} filap;

void liberaArvore(arvore* a); // --------------------------------libera memoria da AVL
arvore* criaArvore(char* palavra);//-----------------------------Cria aVL
arvore* encontrarMin(arvore* a);//------------------------------encontra menor nó chave da avl
int altura(arvore* a);//----------------------------------------contabilizia a altura da avl
int fb(arvore* a);//--------------------------------------------retorna o fato balnceamento
arvore* rotadir(arvore* a);//-----------------------------------realiza a rotação a direita
arvore* rotaesq(arvore* a);//------------------------------------realiza a rotação a esquerda
arvore* insereNaArvore(arvore* a, char* palavra);//---------------insere dados na avl
void emordem(arvore* a);//-----------------------------------------imprime dados da AVL em ordem
void tiraospontos(char* palavra);//--------------------------------tira os caracteres especiais das palavras e passa para minusculas todas elas
arvore* lerEinsereNaAVL(arvore* a, int* cont);//------------------abre arquivo, os lê e em seguida os insere a cada palavra na avl, usando a função acima para tirar os pontos


filap* criafila(int tam); //----------------------------------------cria fila com a quantidade de palavras com tam alocacado
void liberaFila(filap*f);//-----------------------------------------libera memoria da fila
int raiz(int i);//--------------------------------------------------retorna a posição raiz na fila de prioridade
int filhosesq(int i);//---------------------------------------------retorna a posição do filho esquerdo na fila de prioridade
int filhosdir(int i);//---------------------------------------------retorna a posição do filho direito na fila de prioridade
void troca(filap* a, filap* b);//-----------------------------------troca os elementos de posição na fila de prioridade.
void sobe(filap* f, int index);//-----------------------------------corrige o heap máximo subindo um elemento na fila de prioridade.
void desce(filap* f, int tam, int index);//-------------------------corrige o heap máximo descendo um elemento na fila de prioridade.
void enfila(filap* f, int* tam, int frequencia, char* palavra);//---insere um elemento na fila de prioridade.
void preencher(arvore* a, filap* f, int* tam);//---------------------preenche a fila de prioridade com as palavras e suas frequências da árvore AVL.
void desenfila(filap* f, int* tam, int index);//---------------------emove o elemento de maior raiz da fila 
void imprimirfila(filap*f,int tam);
int buscarPalavraNaFila(filap* f, int tam, char* palavra);

int main() {
    char palavra[100];
    arvore* a = criaArvore(palavra);
    int index,cont = 0;
    a = lerEinsereNaAVL(a, &cont); //------------------------------------------avl recebe a função que é para ler e receber dados do arquivo
    //printf("Palavras inseridas na arvore AVL com sucesso!\n");
    //emordem(a);
    printf("quantidade de palavras na AVL: %d\n", cont);
    printf("\n");//------------------------------------------------------------só para dar uma distancia na tela do usuario
    filap* f = criafila(cont);
    int tam = 0;
    preencher(a, f, &tam);//---------------------------------------------------preencher a fila de prioridade com as palavras e suas frequências da árvore AVL.
    printf("quantidade de palavras na fila: %d\n",tam);
	//imprimirfila(f,tam);
	printf("raiz: '%s' - repete = %d \n", f[0].palavra, f[0].frequencia);//-------sabendo que o maior elemento será a posição 0 basta imprimir fila da posição 0
    desenfila(f, &tam, index);
    printf("Nova raiz: '%s' - repete = %d \n", f[0].palavra,f[0].frequencia);
	char palavraDesejada[100];
	printf("Digite a palavra que deseja buscar na fila: ");
	scanf("%s", palavraDesejada);
	
	int pos = buscarPalavraNaFila(f, tam, palavraDesejada);
	if (pos != -1) {
	    printf("A palavra '%s' foi encontrada na posição %d da fila.\n", palavraDesejada, pos);
	    // Faça o que desejar com o elemento encontrado
	} else {
	    printf("A palavra '%s' não foi encontrada na fila.\n", palavraDesejada);
	}
    liberaFila(f);
    liberaArvore(a);

    return 0;
}

void liberaArvore(arvore* a) {
    if (a != NULL) {
        liberaArvore(a->esq);
        liberaArvore(a->dir);
        free(a);
    }
}

arvore* criaArvore(char* palavra) {
    arvore* a = (arvore*)malloc(sizeof(arvore));
    strcpy(a->palavra, palavra);
    a->altura = 1;
    a->repete = 1;
    a->esq = NULL;
    a->dir = NULL;
    return a;
}

arvore* encontrarMin(arvore* a) {
    if (a == NULL) {
        return NULL;
    } else if (a->esq == NULL) {
        return a;
    } else {
        return encontrarMin(a->esq);
    }
}

int altura(arvore* a) {
    if (a == NULL) {
        return 0;
    }
    return a->altura;
}

int fb(arvore* a) {
    if (a == NULL) {
        return 0;
    }
    return altura(a->esq) - altura(a->dir);
}

arvore* rotadir(arvore* a) {
    arvore* aux = a->esq;
    a->esq = aux->dir;
    aux->dir = a;
    a->altura = 1 + (altura(a->esq) > altura(a->dir) ? altura(a->esq) : altura(a->dir));
    aux->altura = 1 + (altura(aux->esq) > altura(aux->dir) ? altura(aux->esq) : altura(aux->dir));
    return aux;
}

arvore* rotaesq(arvore* a) {
    arvore* aux = a->dir;
    a->dir = aux->esq;
    aux->esq = a;
    a->altura = 1 + (altura(a->esq) > altura(a->dir) ? altura(a->esq) : altura(a->dir));
    aux->altura = 1 + (altura(aux->esq) > altura(aux->dir) ? altura(aux->esq) : altura(aux->dir));
    return aux;
}

arvore* insereNaArvore(arvore* a, char* palavra) {
    if (a == NULL) {
        return criaArvore(palavra);
    }

    int cmp = strcmp(palavra, a->palavra);
    if (cmp < 0) {
        a->esq = insereNaArvore(a->esq, palavra);
    } else if (cmp > 0) {
        a->dir = insereNaArvore(a->dir, palavra);
    } else {
        a->repete++;
        return a;
    }

    a->altura = 1 + (altura(a->esq) > altura(a->dir) ? altura(a->esq) : altura(a->dir));

    int balance = fb(a);

    if (balance > 1 && strcmp(palavra, a->esq->palavra) < 0) {
        return rotadir(a);
    }

    if (balance < -1 && strcmp(palavra, a->dir->palavra) > 0) {
        return rotaesq(a);
    }

    if (balance > 1 && strcmp(palavra, a->esq->palavra) > 0) {
        a->esq = rotaesq(a->esq);
        return rotadir(a);
    }

    if (balance < -1 && strcmp(palavra, a->dir->palavra) < 0) {
        a->dir = rotadir(a->dir);
        return rotaesq(a);
    }

    return a;
}

void emordem(arvore* a) {
    if (a != NULL) {
        emordem(a->esq);
        printf("%s (%d) ", a->palavra, a->repete);
        emordem(a->dir);
    }
}

void tiraospontos(char* palavra) {
    int i, j;
    for (i = 0, j = 0; palavra[i] != '\0'; i++) {
        if (isalpha(palavra[i])) { 
            palavra[j] = palavra[i];
            j++;
        }
    }
    palavra[j] = '\0';
}

arvore* lerEinsereNaAVL(arvore* a, int* cont) {
    FILE*file = fopen("arquivo_texto_tp1.txt","r");
    if (file == NULL) {
        printf("Nao foi possivel abrir o arquivo.\n");
        return NULL;
    }

    char palavra[100];
    
    while (fscanf(file, "%s", palavra) != EOF) {
        tiraospontos(palavra);
        a = insereNaArvore(a, palavra);
        (*cont)++;
    }
    fclose(file);
    return a;
}

filap* criafila(int tam) {
    return (filap*)malloc(sizeof(filap) * tam);
}

void liberaFila(filap*f){
	free(f);
}

int raiz(int i) {
    return (i - 1) / 2;
}

int filhosesq(int i) {
    return 2 * i + 1;
}

int filhosdir(int i) {
    return 2 * i + 2;
}

void troca(filap* a, filap* b) {
    filap temp = *a;
    *a = *b;
    *b = temp;
}

void sobe(filap* f, int index) {
    int pai = raiz(index);
    if (pai >= 0 && f[pai].frequencia < f[index].frequencia) {
        troca(&f[pai], &f[index]);
        sobe(f, pai);
    }
}

void desce(filap* f, int tam, int index) {
    int maior = index;
    int fesq = filhosesq(index);
    int fdir = filhosdir(index);

    if (fesq < tam && f[fesq].frequencia > f[maior].frequencia) {
        maior = fesq;
    }

    if (fdir < tam && f[fdir].frequencia > f[maior].frequencia) {
        maior = fdir;
    }

    if (maior != index) {
        troca(&f[index], &f[maior]);
        desce(f, tam, maior);
    }
}

void enfila(filap* f, int* tam, int frequencia, char* palavra) {
    f[*tam].frequencia = frequencia;
    strcpy(f[*tam].palavra, palavra);
    (*tam)++;
    sobe(f, *tam - 1);
}

void preencher(arvore* a, filap* f, int* tam) {
    if (a == NULL) {
        return;
    }
    preencher(a->esq, f, tam);
    enfila(f, tam, a->repete, a->palavra);
    preencher(a->dir, f, tam);
}

void desenfila(filap* f, int* tam, int index) {
    if (*tam == 0) {
        return;
    }
    f[index] = f[*tam - 1];
    (*tam)--;
    desce(f, *tam, index);
}

void imprimirfila(filap*f,int tam){
	int i;
	for (i=0; i< tam; i++){
		printf("'%s' repete = %d\n", f[i].palavra, f[i].frequencia);
	}
}

int buscarPalavraNaFila(filap* f, int tam, char* palavra) {
	int i;
    for (i = 0; i < tam; i++) {
        if (strcmp(f[i].palavra, palavra) == 0) {
            return i; // Retorna o índice da palavra encontrada na fila
        }
    }
    return -1; // Retorna -1 se a palavra não for encontrada na fila
}





