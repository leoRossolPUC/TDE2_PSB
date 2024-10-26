#include <stdio.h>
#include "mymemory.h"

int main() {

    //inicializa memoria com 1024 bytes
    printf("iniciando pool de memoria \n");
    mymemory_t *memory = mymemory_init(1024);

    if (memory == NULL) {
        printf("FALHA AO INICIALIZAR MEMORIA\n");
        return 1;
    }

    //exibir stats iniciais
    printf("exibindo stats iniciais\n");
    mymemory_stats(memory);


    //alocar blocos de diferentes tamanhos
    printf("alocando blocos de 100, 200, 300 bytes \n");
    void *ptr1 = mymemory_alloc(memory, 100);
    void *ptr2 = mymemory_alloc(memory, 200);
    void *ptr3 = mymemory_alloc(memory, 300);
    //verifica se alocacoes foram bem sucedidas
    if (ptr1) printf("bloco de 100 bytes alocado em: %p\n", ptr1);
    if (ptr2) printf("bloco de 200 bytes alocado em: %p\n", ptr2);
    if (ptr3) printf("bloco de 300 bytes alocado em: %p\n", ptr3);


    //exibir alocacoes atuais
    printf("alocacoes atuais\n");
    mymemory_display(memory);

    //mostrar pos locacao
    printf("exibindo stats pos locacao \n");
    mymemory_stats(memory);

    //liberar bloco de memoria
    printf("liberando o bloco de 100 bytes \n");
    mymemory_free(memory, ptr1);

    //exibir locacoes e stats pos liberar
    printf("exibindo alocacoes apos liberar bloco \n");
    mymemory_display(memory);
    printf("exibindo estatisticas apos liberar bloco \n");
    mymemory_stats(memory);

    //limpar todos os recursos
    printf("limpando memoria \n");
    mymemory_cleanup(memory);

    return 0;
}
