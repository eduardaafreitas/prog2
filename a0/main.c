#include <stdlib.h>
#include <stdio.h>

/* Structs */

typedef struct teddy_machine teddy_machine;

struct teddy_machine{
    unsigned int id;
    unsigned int probability;

    struct teddy_machine *next;
    struct teddy_machine *previous;
};

/* List functions */

teddy_machine* create_list (unsigned int machines){ /* INSTRU��O: Implemente a fun��o */ 

    teddy_machine *head = (teddy_machine*) malloc(sizeof(teddy_machine));
    teddy_machine *aux = head;
    teddy_machine *aux2 = head;

    head->id = 1;
    head->probability = 5;
    head->next = NULL;
    head->previous = NULL;

    for (int i = 1; i < machines; i++) {
        aux2 = (teddy_machine*) malloc(sizeof(teddy_machine));
        aux2->id = i + 1;
        aux2->probability = 5;
        aux2->next = NULL;
        aux2->previous = aux;
        aux->next = (struct teddy_machine*) aux2;
        aux = aux2;
    }

    aux->next = (struct teddy_machine*) head;
    head->previous = (struct teddy_machine*) aux;

    return head;
 
}

teddy_machine* select_machine (teddy_machine *list, unsigned int place){ /* INSTRU��O: Implemente a fun��o */ 

    int i;
    teddy_machine *selected = list;

    for (i = 0; i < place; i++) {
        selected = (teddy_machine*) selected->next;
    }
    return selected;    

}

teddy_machine* remove_machine (teddy_machine *list, teddy_machine *remove) { /* INSTRU��O: Implemente a fun��o */ 

    if (!list || !remove)
        return list;

    // Se o nó a ser removido é o primeiro da lista
    if (list == remove) {
        teddy_machine *next = remove->next;
        if (next) {
            next->previous = remove->previous;
            list = next;
        }
        if (remove->previous)
            remove->previous->next = next;
        free(remove);
        return list;
    }

    if(list->next == NULL && list->previous == NULL){
        list = NULL;
        return list;
    }

    // Se o nó a ser removido não for o primeiro da lista
    teddy_machine *prev = remove->previous;
    teddy_machine *next = remove->next;
    if (prev)
        prev->next = next;
    if (next)
        next->previous = prev;
    free(remove);
    return list;

}
    


void destroy_list (teddy_machine *list){/* INSTRU��O: Implemente a fun��o */ 

    if (!list) // Verifica se a lista é nula
            return;

        teddy_machine *current = list;
        teddy_machine *next;

        do {
            next = current->next; // Salva o próximo nó
            free(current); // Libera a memória do nó atual
            current = next; // Avança para o próximo nó
        } while (current != list); // Repete até voltar ao início da lista
}

/* Randomization functions */

unsigned int get_place(unsigned int machines){
    return rand() % machines;
}

unsigned int get_attempt(){
    return rand() % 100 + 1;
}

/* Printing functions */

void print_attempt(teddy_machine *machine, unsigned int attempt){

    if (attempt <= machine->probability) printf("O URSO DA MAQUINA %u [FOI] OBTIDO!\n", machine->id);
    else printf("O URSO DA MAQUINA %u [NAO FOI] OBTIDO!\n", machine->id);
}
void print_available_machines(teddy_machine *list){

    if (!list) printf("NAO HA MAQUINAS DISPONIVEIS!\n");
    else{
        teddy_machine *i = list;
        do {
            printf("PROBABILIDADE DA MAQUINA %u: %u\n", i->id, i->probability);
            i = (teddy_machine*) i->next;
        } while((i) && (i != list));
    }

}

/* Main function */

int main(int argc, char *argv[]){
    unsigned int machines, rounds, seed;
    int rv = 0;

    if (argc != 4) rv = -1;
    else {
        machines = atoi(argv[1]);
        if (!machines) rv = -2;
        else {
            rounds = atoi(argv[2]);
            if (!rounds) rv = -3;
            else {
                seed = atoi(argv[3]);
                if (!seed) rv = -4;
            }
        }
    }

    if (rv){
        printf("USO: main [NR. DE MAQUINAS] [NR. DE RODADAS] [SEMENTE DE RAND.]\n");
        return rv;
    }


    teddy_machine *list = create_list(machines); //ok
    if (!list) return -5;
    srand(seed);

    unsigned int machine_place, machine_attempt;
    for (unsigned int r = 0; r < rounds; r++){
        printf("\n============================ ROUND %u ============================\n", r+1);
        machine_place = get_place(machines); /* Define a localiza��o da m�quina da rodada, n�o considera m�quinas sem urso */
        machine_attempt = get_attempt(); /* Define a tentativa da rodada; se for menor ou igual � probabilidade da m�quina selecionada, o urso foi pego */
    
        teddy_machine *selected = select_machine(list, machine_place);      
        
        print_attempt(selected, machine_attempt); 

        if(selected->probability > 0)
            selected->probability = selected->probability + 2;


        if ( machine_attempt <= list->probability){
            list = remove_machine(list, selected);
            machines--;            
        }
         
        /* INSTRU��O: Implemente a l�gica do seu programa aqui */
        /* INSTRU��O: Utilize a fun��o "print_attempt" antes do "print_available_machines"! */

        print_available_machines(list);
        printf("==================================================================\n");
        
    }
    destroy_list(list);
    return 0;
}
