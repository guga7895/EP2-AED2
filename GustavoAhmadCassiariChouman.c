#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define t 3

typedef int TipoChave;

typedef struct str_no {
    TipoChave chave[2*t - 1];
    struct str_no* filhos[2*t];
    int numChaves;
    bool folha;
} NO;

typedef struct {
    NO* raiz;
} ArvB;

//procura se a chave desejado está no filho do NO (usada somente no procuraProxInt)
bool procuraNofilho(NO* no, int k, int i){
    NO* filho;
    filho = no->filhos[i];
    for(int j = 0; j < filho->numChaves; j++){
        if(filho->chave[j] == k && filho->folha) return true;
    }
    return false;
}

//procura a chave do pai do nó primo(usado no split)
int procuraProxInt(NO* raiz, int k){
    NO* aux;
    aux = raiz;
    int i;
    while(!raiz->folha){
        i = 0;
        while(raiz->chave[i] <= k && i < raiz->numChaves){
            i++;
        }
        if(procuraNofilho(raiz,k,i)){
            return raiz->chave[i];//i+1
        }
        aux = raiz;
        raiz = raiz->filhos[i];
    }
    return raiz->chave[i];
}

//pega o proximo nó para fazer os ponteiros das folhas primas(usada no split)
NO* procuraProxNO(NO* raiz, int k, int* indiceProxNO){
    NO* aux;
    aux = raiz;
    while(!raiz->folha){
        int i = 0;
        while(raiz->chave[i] <= k && i < raiz->numChaves){
            i++;
        }
        if(procuraNofilho(raiz,k,i)){
            *indiceProxNO = i;
            return raiz->filhos[i];//i+1
        }
        aux = raiz;
        raiz = raiz->filhos[i];
    }
    return NULL;
}

NO* ProcuraProxNOsemSPLIT(NO* raiz, int k, int *indiceProxNO){
    NO* aux;
    aux = raiz;
    while(!raiz->folha){
        int i = 0;
        while(raiz->chave[i] <= k && i < raiz->numChaves){
            i++;
        }
        if(procuraNofilho(raiz,k,i)){
            *indiceProxNO = i;
            return raiz->filhos[i]->filhos[2*t-1];//i+1
        }
        aux = raiz;
        raiz = raiz->filhos[i];
    }
    return NULL;
}

NO* procuraPaiNO(NO* raiz, int k){
    NO* aux;
    aux = raiz;
    while(!raiz->folha){
        int i = 0;
        while(raiz->chave[i] <= k && i < raiz->numChaves){
            i++;
        }
        if(procuraNofilho(raiz,k,i)){
            return raiz;//i+1
        }
        aux = raiz;
        raiz = raiz->filhos[i];
    }
    return NULL;
}

int procuraIndiceAntNO(NO* raiz, int k){
    NO* aux;
    aux = raiz;
    while(!raiz->folha){
        int i = 0;
        while(raiz->chave[i] <= k && i < raiz->numChaves){
            i++;
        }
        if(procuraNofilho(raiz,k,i)){
            return i-1;
        }
        aux = raiz;
        raiz = raiz->filhos[i];
    }
    return -1;
}

NO* procuraAntNO(NO* raiz, int k){
    while(!raiz->folha) raiz = raiz->filhos[0];
    NO* aux;
    int i = 0;
    while(raiz){
        for(i = 0; i < raiz->numChaves; i++){
            if(raiz->chave[i] == k){
                return aux;
            }
        }
        aux = raiz;
        raiz = raiz->filhos[2*t-1];
    }
    return NULL;
}

void criaArvoreB(ArvB* ArvoreB){
    NO* x = (NO*) malloc(sizeof(NO));
    x->folha = true;
    x->numChaves = 0;
    ArvoreB->raiz = x;
}

void splitChild(NO* pai, int i, NO* filhoCheio, ArvB* arvore){
    int puppet;
    NO* novoNO = (NO*) malloc(sizeof(NO));
    novoNO->folha = filhoCheio->folha;
    novoNO->numChaves = t-1;
    int j;

    //seta o novo no com a segunda metade do no cheio
    for(j = 0; j < t; j++){
        novoNO->chave[j] = filhoCheio->chave[j+t];
    }

    if(novoNO->folha){
            //não me orgulho dessa linha de codigo
            novoNO->filhos[2*t-1] = procuraProxNO(arvore->raiz,procuraProxInt(arvore->raiz,novoNO->chave[novoNO->numChaves-1]),&puppet);
    }

    //seta os ponteiros de filho da segunda metade do nó cheio pro nó novo
    if(!filhoCheio->folha){
        for(j = 0; j < t; j++){
            novoNO->filhos[j] = filhoCheio->filhos[j+t];//j+t-1
        }
    }
    if(filhoCheio->folha){
        if(novoNO->numChaves < 2*t-1){
            for(j = novoNO->numChaves; j > 0; j--){
                novoNO->chave[j] = novoNO->chave[j-1];
            }
            novoNO->numChaves++;
            novoNO->chave[0] = filhoCheio->chave[t-1];
        }
    }

    filhoCheio->numChaves = t-1;//ou t
    if(novoNO->folha){
        filhoCheio->filhos[2*t-1] = novoNO;
    }

    for(j = pai->numChaves+1; j > i+1; j--){
        pai->filhos[j] = pai->filhos[j-1];
    }
    pai->filhos[i+1] = novoNO;
    for(j = pai->numChaves; j > i; j--){
        pai->chave[j] = pai->chave[j-1];
    }
    pai->chave[i] = filhoCheio->chave[t-1];
    pai->numChaves++;
}

void inserirNaoCheioArvoreB(NO* no, int chave,ArvB* arvore){
    int i = no->numChaves;
    if(no->folha){
        while(i > 0 && chave < no->chave[i-1]){
            no->chave[i] = no->chave[i-1];
            i = i - 1;
        }
        no->chave[i] = chave;
        no->numChaves++;
    } else {
        while(i > 0  && chave < no->chave[i-1]){
            i = i-1;
        }
        if(no->filhos[i]->numChaves == 2*t-1){
            splitChild(no,i,no->filhos[i],arvore);
            if(chave > no->chave[i]){
                i++;
            }
        }
        inserirNaoCheioArvoreB(no->filhos[i],chave,arvore);
    }
}


void inserir(ArvB* arvore, int chave){
    NO* r = arvore->raiz;
    if(r->numChaves == 2*t - 1){
        NO* s = malloc(sizeof(NO));
        arvore->raiz = s;
        s->folha = false;
        s->numChaves = 0;
        s->filhos[0] = r;
        splitChild(s,0,r,arvore);
        inserirNaoCheioArvoreB(s,chave,arvore);
    } else {
        inserirNaoCheioArvoreB(r,chave,arvore);
    }
}

void printArvoreB(NO* no, ArvB* arvere, FILE* saida) {
    if(arvere->raiz->numChaves == 0){
        fprintf(saida,"Vazia");
        return;
    }
    fprintf(saida, "(");
    int i = 0;
    if(arvere->raiz == NULL){
        fprintf(saida, "Vazia");
    }
    //perdao pelo codigo abaixo
    for(i = 0; i < no->numChaves; i++){
        if(no->folha == false){
            printArvoreB(no->filhos[i],arvere, saida);
        }
        if(i == 0 && no->folha == false){
            fprintf(saida, " ");
        }
        if(i != 0 && no->folha == true){
            fprintf(saida, " ");
        }
        if(i != 0 && no->folha == false){
            fprintf(saida, " ");
        }
        fprintf(saida,"%d", no->chave[i]);
        if(i != 0 && no->folha == false){
            fprintf(saida, " ");
        }

        if(i == 0 && no->folha == false){
            fprintf(saida, " ");
        }
    }
    if(no->folha == false){
        printArvoreB(no->filhos[i],arvere, saida);
    }
    fprintf(saida, ")");
}


bool procuraNoNaFolha(int k, NO* raiz, NO** NOdaChave){
    while(!raiz->folha) raiz = raiz->filhos[0];
    while(raiz){
        for(int i = 0; i < raiz->numChaves; i++){
            if(raiz->chave[i] == k){
                *NOdaChave = raiz;//NOVO
                return true;
            }
        }
        raiz = raiz->filhos[2*t-1];
    }
    *NOdaChave = NULL;//NOVO
    return false;
}

bool procuraNoInterno(int k, NO* raiz,NO** interno, int* indice){
    int i;
    while(!raiz->folha){
        i = 0;
        while(k >= raiz->chave[i] && i < raiz->numChaves){
            if(raiz->chave[i] == k){
                *interno = raiz;
                *indice = i;//
                return true;
            }
            i++;
        }
        raiz = raiz->filhos[i];
    }
    *interno = NULL;
    return false;
}

int IndiceApartirDaRaiz(NO* raiz, int k){
    int i = 0;
    while(raiz->chave[i] <= k && i < raiz->numChaves){
        i++;
    }
    return i;
}

void removeDaFolha(int k, NO* no){
    int i = no->numChaves-1;
    while(no->chave[i] > k && i > 0){
        i--;
    }

    for(int j = i; j < no->numChaves-1; j++){
        int aux = no->chave[j+1];
        no->chave[j+1] = no->chave[j];
        no->chave[j] = aux;
    }
    no->numChaves--;
}

void removeDaFolhaEdoNoInterno(int k, NO* noInterno,NO* NOdaChave,int indice){
    NO* noChave = NOdaChave;
    //posso excluir do nó interno direto
    if(noInterno->numChaves > t-1){
        removeDaFolha(k,NOdaChave);
        noInterno->chave[indice] = NOdaChave->chave[0];
    } else {
        removeDaFolha(k,NOdaChave);
        noInterno->chave[indice] = NOdaChave->chave[0];
    }
}

void removeComMinimoElementosDoAntComInterno(int k, NO* ant, int indiceDoAnt, NO* paiDoAnt,NO* NOdaChave, NO* raiz,ArvB* arv){
    NO* NOdoAntecessor;
    int menor = ant->chave[ant->numChaves-1];
    int sucessorDoMenor = ant->chave[1];
    NO* paiDoAtual = procuraPaiNO(raiz,k);
    int indiceDaRaiz = IndiceApartirDaRaiz(raiz,menor);
    bool check = procuraNoNaFolha(menor,raiz,&NOdoAntecessor);
    if(paiDoAnt != paiDoAtual){
        indiceDoAnt = paiDoAnt->numChaves;
        removeDaFolhaEdoNoInterno(k,paiDoAnt,NOdoAntecessor,indiceDoAnt);
        raiz->chave[indiceDaRaiz] = sucessorDoMenor;
        removeDaFolha(k,NOdaChave);
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
    } else {
        removeDaFolhaEdoNoInterno(menor,paiDoAnt,NOdoAntecessor,indiceDoAnt);
        paiDoAnt->chave[indiceDoAnt] = menor;
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
        removeDaFolha(k, NOdaChave);
    }
}

void removeComMinimoElementosDoAnt(int k, NO* ant, int indiceDoAnt, NO* paiDoAnt,NO* NOdaChave, NO* raiz,ArvB* arv){
    NO* NOdoAntecessor;
    int menor = ant->chave[ant->numChaves-1];
    int sucessorDoMenor = ant->chave[1];
    NO* paiDoAtual = procuraPaiNO(raiz,k);
    int indiceDaRaiz = IndiceApartirDaRaiz(raiz,menor);
    bool check = procuraNoNaFolha(menor,raiz,&NOdoAntecessor);
    if(paiDoAnt != paiDoAtual){
        indiceDoAnt = paiDoAnt->numChaves;
        removeDaFolhaEdoNoInterno(menor,paiDoAnt,NOdoAntecessor,indiceDoAnt);
        raiz->chave[indiceDaRaiz] = sucessorDoMenor;
        removeDaFolha(k,NOdaChave);
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
    } else {
        removeDaFolhaEdoNoInterno(menor,paiDoAnt,NOdoAntecessor,indiceDoAnt);
        paiDoAnt->chave[indiceDoAnt] = menor;
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
        NOdaChave->numChaves--;
    }
}

void removeComMinimoElementosDoSuc(int k, NO* suc, int indiceDoSuc, NO* paiDoSuc,NO* NOdaChave, NO* raiz, ArvB* arv){
    NO* NOdoSucessor;
    int menor = suc->chave[0];
    int sucessorDoMenor = suc->chave[1];
    NO* paiDoAtual = procuraPaiNO(raiz,k);
    int indiceDaRaiz = IndiceApartirDaRaiz(raiz,menor)-1;
    bool check = procuraNoNaFolha(menor,raiz,&NOdoSucessor);
    if(paiDoSuc != paiDoAtual){
        indiceDoSuc = 0;
        removeDaFolhaEdoNoInterno(menor,paiDoSuc,NOdoSucessor,indiceDoSuc);
        raiz->chave[indiceDaRaiz] = sucessorDoMenor;
        removeDaFolha(k,NOdaChave);
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
    } else {
        removeDaFolhaEdoNoInterno(menor,paiDoSuc,NOdoSucessor,indiceDoSuc);
        paiDoSuc->chave[indiceDoSuc] = sucessorDoMenor;
        NOdaChave->chave[NOdaChave->numChaves-1] = menor;
    }
}

void removeComMinimoElementosDoSucComInterno(int k, NO* suc, int indiceDoSuc, NO* paiDoSuc,NO* NOdaChave, NO* raiz, ArvB* arv){
    NO* NOdoSucessor;
    int menor = suc->chave[0];
    int sucessorDoMenor = suc->chave[1];
    NO* paiDoAtual = procuraPaiNO(raiz,k);
    int indiceDaRaiz = IndiceApartirDaRaiz(raiz,menor)-1;
    bool check = procuraNoNaFolha(menor,raiz,&NOdoSucessor);
    if(paiDoSuc != paiDoAtual){
        indiceDoSuc = 0;
        removeDaFolhaEdoNoInterno(menor,paiDoSuc,NOdoSucessor,indiceDoSuc);
        raiz->chave[indiceDaRaiz] = sucessorDoMenor;
        removeDaFolha(k,NOdaChave);
        inserirNaoCheioArvoreB(NOdaChave,menor,arv);
    } else {
        removeDaFolhaEdoNoInterno(menor,paiDoSuc,NOdoSucessor,indiceDoSuc);
        paiDoSuc->chave[indiceDoSuc] = sucessorDoMenor;
        NOdaChave->chave[NOdaChave->numChaves-1] = menor;
    }
}

void RemoveEdesfazSplitSemNOinterno(int k, NO* antecessor, int indiceDoAnt, NO* paiDoAnt, NO* NOdaChave, NO* raiz, ArvB* arv){
    NO* novoNO = (NO*) malloc(sizeof(NO));
    novoNO->folha = NOdaChave->folha;
    NO* antecessorDoAntecessor = procuraAntNO(raiz,antecessor->chave[0]);
    int puppet;
    NO* proxNOdoAtual = ProcuraProxNOsemSPLIT(raiz,NOdaChave->chave[0],&puppet);
    novoNO->numChaves = antecessor->numChaves + NOdaChave->numChaves;
    int i = 0;
    int j = 0;
    novoNO->numChaves = 2*t-2;
    for(i = 0; i < antecessor->numChaves; i++){
        novoNO->chave[i] = antecessor->chave[i];
    }
    for(j = 0; j < NOdaChave->numChaves; j++){
        novoNO->chave[i+j] = NOdaChave->chave[j];
    }
    removeDaFolha(k, novoNO);
    for(int z = 0; z < 2*t; z++){
        novoNO->filhos[z] = NOdaChave->filhos[z];//se for a raiz talvez de pau
    }
    int chaveDoNOabolido = NOdaChave->chave[0];
    int indiceDoNOabolido = 0;
    for(int x = 0; x < paiDoAnt->numChaves; x++){
        if(paiDoAnt->chave[x] == chaveDoNOabolido) indiceDoNOabolido = x;
    }
    for(int i = indiceDoNOabolido; i < paiDoAnt->numChaves-1; i++){
        int aux = paiDoAnt->chave[i+1];
        paiDoAnt->chave[i+1] = paiDoAnt->chave[i];
        paiDoAnt->chave[i] = aux;
    }
    for(int i = indiceDoAnt; i < 2*t-1; i++){
        NO* aux = paiDoAnt->filhos[i+1];
        paiDoAnt->filhos[i+1] = paiDoAnt->filhos[i];
        paiDoAnt->filhos[i] = aux;
    }
    antecessorDoAntecessor->filhos[2*t-1] = novoNO;
    novoNO->filhos[2*t-1] = proxNOdoAtual;
    removeDaFolha(k, paiDoAnt);
    paiDoAnt->filhos[indiceDoAnt] = novoNO;
    for(int i = paiDoAnt->numChaves+1; i < 2*t; i++){
        paiDoAnt->filhos[i] = NULL;
    }
}

void removeDaArvore(int k, NO* raiz, ArvB* arv){
    NO* NOdaChave;
    NO* NOinterno;
    NO* PaiDoAnt;
    int indice = 0;
    int indiceDoAnt = 0;
    int indiceDoSuc = 0;
    NO* ant = procuraAntNO(raiz,k);
    indiceDoAnt = procuraIndiceAntNO(raiz,k);
    if(ant) PaiDoAnt = procuraPaiNO(raiz,ant->chave[0]);
    NO* sucessor = ProcuraProxNOsemSPLIT(raiz, k, &indiceDoSuc);
    NO* PaiDoSuc;
    if(sucessor) PaiDoSuc = procuraPaiNO(raiz,sucessor->chave[0]);

    if(!procuraNoNaFolha(k,raiz, &NOdaChave)) return;
//caso 1:a chave está nas folhas, mas não está nos internos
    if(procuraNoNaFolha(k, raiz, &NOdaChave) && !procuraNoInterno(k,raiz, &NOinterno, &indice)){

        //caso 1a: posso excluir da folha direto
        if(NOdaChave->numChaves > t-1 && !procuraNoInterno(k,raiz,&NOinterno,&indice)){
            removeDaFolha(k,NOdaChave);
            return;
        }
        //caso 1b: folha com elemento possui t-1 elementos, nao ta no no interno e o antecessor tem mais de t-1
        if(NOdaChave->numChaves == t-1 && (ant->numChaves > t-1)){
            removeComMinimoElementosDoAnt(k ,ant , indiceDoAnt, PaiDoAnt,NOdaChave,raiz, arv);
            return;
        }
        if(NOdaChave->numChaves == t-1 && (sucessor->numChaves > t-1)){
            removeComMinimoElementosDoSuc(k , sucessor , indiceDoSuc, PaiDoSuc,NOdaChave,raiz, arv);//indicedosuc
            return;
        }
        if(NOdaChave->numChaves == t-1 && sucessor->numChaves == t-1 && ant->numChaves == t-1){
            RemoveEdesfazSplitSemNOinterno(k, ant, indiceDoAnt, PaiDoAnt, NOdaChave, raiz, arv);
            return;
        }
    }
//caso 2:a chave está na folha e está no nó interno
    if(procuraNoNaFolha(k, raiz, &NOdaChave) && procuraNoInterno(k,raiz, &NOinterno, &indice)){

        if(NOdaChave->numChaves > t-1 && procuraNoInterno(k,raiz,&NOinterno,&indice)){
            removeDaFolhaEdoNoInterno(k,NOinterno,NOdaChave,indice);
            return;
        };
        if(NOdaChave->numChaves == t-1 && (ant->numChaves > t-1)){
            removeComMinimoElementosDoAntComInterno(k ,ant , indiceDoAnt, PaiDoAnt,NOdaChave,raiz, arv);
            return;
        }
        if(NOdaChave->numChaves == t-1 && (sucessor->numChaves > t-1)){
            removeComMinimoElementosDoSucComInterno(k , sucessor , indiceDoSuc, PaiDoSuc,NOdaChave,raiz, arv);//indicedosuc
            return;
        }
        if(NOdaChave->numChaves == t-1 && sucessor->numChaves == t-1 && ant->numChaves == t-1){
            RemoveEdesfazSplitSemNOinterno(k, ant, indiceDoAnt, PaiDoAnt, NOdaChave, raiz, arv);
            return;
        }
    }
}

int main(int argc, char** argv) {
    ArvB* arvere = malloc(sizeof(ArvB));
    criaArvoreB(arvere);
    FILE* entrada = fopen(argv[1],"r");
    FILE* saida = fopen(argv[2], "w");
    char operacao;
    int key;
    while(operacao != 'f'){
        fscanf(entrada, "%c %d\n", &operacao, &key);
        if(operacao == 'i') inserir(arvere,key);
        else if(operacao == 'r') removeDaArvore(key,arvere->raiz,arvere);
        else if(operacao == 'p'){
            printArvoreB(arvere->raiz,arvere, saida);
            fprintf(saida,"\n");
        }
    }
    return 0;
}
