#include "mymemory.h"
#include <stdlib.h>
#include <stdio.h>

//inicializa o pool de memoria
mymemory_t* mymemory_init(size_t size) {
    mymemory_t *memory = (mymemory_t*) malloc(sizeof(mymemory_t));
    if (memory == NULL) {
        return NULL;
    }

    //malloc esta permitido para alocar o pool de memoria
    memory -> pool = malloc(size);
    if (memory -> pool == NULL) {
        return NULL;
    }

    memory -> total_size = size;
    memory -> head = NULL;
    return memory;
}



//alocador de memoria
void* mymemory_alloc(mymemory_t *memory, size_t size) {
    //verifica se o tamanho solicitado eh valido
    if(size <= 0 || memory == NULL) {
        return NULL;
    }

    //first fit foi a estrategia utilizada para alocar

    //variaveis de controle
    allocation_t *current = memory -> head; //ponteiro para o bloco atual
    allocation_t *previous = NULL; //ponteiro para o bloco anterior

    //calcular inicio da primeira alocacao
    char *free_start = (char*) memory -> pool; //aponta para o bloco livre mais proximo
    char *free_end;

    while (current != NULL) { //percorre a lista encadeada de alocacoes
        free_end =(char*) current -> start;

        //verifica se ha espaco suficiente antes do bloco alocado atual
        if((free_end - free_start) >= size + sizeof(allocation_t)) {

            //ao encontrar um espaco grande o suficiente, novo bloco eh alocado
            //o espaco eh usado pra armazenar estr. contrl. e dados requisitados
            allocation_t *new_allocation = (allocation_t*)free_start;
            new_allocation -> start = (void*)(free_start +sizeof(allocation_t));
            new_allocation -> size = size;

            //a nova alocacao eh inserida na lista encadeada, ajustando ponteiros
            if (previous == NULL) {
                new_allocation -> next = memory -> head;
                memory -> head = new_allocation;
            } else {
                new_allocation -> next = current;
                previous -> next = new_allocation;
            }
            return new_allocation -> start;
        }

        //atualiza ponteiro para o proximo bloco livre
        free_start = (char*) current -> start + current -> size;
        previous = current;
        current = current->next;
    }


    //verifica de ha espaco no final do pool
    if ((char*) memory -> pool + memory -> total_size - free_start >= size + sizeof(allocation_t)) {
        allocation_t *new_allocation = (allocation_t*)free_start;
        new_allocation -> start = (void*)(free_start + sizeof(allocation_t));
        new_allocation -> size = size;
        new_allocation -> next = NULL;

        if (previous == NULL) {
            memory -> head = new_allocation;
        } else {
            previous -> next = new_allocation;
        }
        return new_allocation -> start;
    }

    return NULL;
}



//liberador de memoria
void mymemory_free(mymemory_t *memory, void *ptr) {

    if (memory ==NULL || ptr == NULL) {
        return;
    }

    allocation_t *current = memory -> head;
    allocation_t *previous = NULL;

    //procurar a alocacao do ponteiro
    while (current != NULL) {
        if(current ->start == ptr) {
            //acha o bloco a ser liberado
            if (previous == NULL) {
                memory -> head = current -> next;
            } else {
                previous -> next = current -> next;
            }
            return;
        }
        previous = current;
        current = current -> next;
    }
    printf("ERRO, PONTEIRO NAO VALIDO \n");
}



//exibe alocacao
void mymemory_display(mymemory_t *memory) {
    allocation_t *current = memory -> head;
    while (current != NULL) {
        printf("ALLOCATION START: %p, SIZE: %zu\n", current->start, current->size);
        current = current->next;
    }
}



void mymemory_stats(mymemory_t *memory) {

    if (memory == NULL) {
        printf("ERRO, MEMORIA NAO INICIALIZADA \n");
        return;
    }

    size_t total_allocated = 0;
    size_t largest_free_block = 0;
    size_t free_fragments = 0;

    allocation_t *current = memory -> head;
    void *last_end = memory -> pool;
    size_t free_space =0;

    while (current != NULL) {
        total_allocated += current->size;

        // calcular o espaco livre
        free_space = (char*)current -> start - (char*)last_end;
        if (free_space > 0) {
            free_fragments++;
            if (free_space > largest_free_block) {
                largest_free_block = free_space;
            }
        }
        last_end = (char*)current -> start + current->size;
        current = current->next;
    }

    free_space = (char*)memory -> pool + memory -> total_size - (char*)last_end;
    if (free_space > 0) {
        free_fragments++;
        if (free_space > largest_free_block) {
            largest_free_block = free_space;
        }
    }
    printf("NUMERO TOTAL DE ALOCACOES: %zu\n", total_allocated);
    printf("MEMORIA TOTAL ALOCADA: %zu bytes\n", total_allocated);
    printf("MEMORIA TOTAL LIVRE: %zu bytes\n", memory -> total_size - total_allocated);
    printf("MAIOR BLOCO LIVRE: %zu bytes\n", largest_free_block);
    printf("NUMERO DE FRAGMENTOS LIVRES: %zu\n", free_fragments);


}


//free esta permitido para o pool de memoria
void mymemory_cleanup(mymemory_t *memory) {
    free(memory->pool);
    free(memory);
}