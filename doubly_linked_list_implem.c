#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 64

typedef struct dll_node_t
{
    void* data; 
    struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t
{
    dll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} doubly_linked_list_t;


doubly_linked_list_t*
dll_create(unsigned int data_size)
{
    doubly_linked_list_t *my_list = malloc(sizeof(*my_list));
    if (!my_list) {
        fprintf(stderr, "Malloc failed\n");
        return NULL;
    }

    my_list->head = NULL;
    my_list->data_size = data_size;
    my_list->size = 0;

    return my_list;
}

dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    if (!list) {
        return NULL;
    }
    
    if (!(list->head)) {
        return NULL;
    }

    n %= list->size;
    dll_node_t *node = list->head;
    for (int i = 0; i < n; i++) {
        node = node->next;
    }

    return node;
}


void
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
{
    if (!list) {
        return ;
    }

    dll_node_t *nod = malloc(sizeof(*nod));
    if (!nod) {
        fprintf(stderr, "Malloc failed\n");
        return;
    }

    nod->data = malloc(list->data_size);
    if (!nod->data) {
        fprintf(stderr, "Malloc failed\n");
        return;
    }

    nod->prev = NULL;
    nod->next = NULL;

    memset(nod->data, 0, list->data_size);
    memcpy(nod->data, data, list->data_size);

    //add the node if the list is empty,
    //it will be surrounded by itself
    //and the list size will be larger by 1
    if (list->size == 0) {
        list->head = nod;
        nod->prev = nod;
        nod->next = nod;
        list->size++;
        return;
    }

    if (n == 0) {
        dll_node_t *aux = list->head;
        list->head = nod;
        nod->next = aux;
        nod->prev = aux->prev;
        aux->prev = nod;
        aux = nod->prev;
        aux->next = nod;
        list->size++;
        return ;
    }

    if (n >= list->size)
        n =  list->size - 1;
    
    dll_node_t *aux = list->head;

    for (int i = 0; i < n; ++i)
        aux = aux->next;
    
    nod->next = aux->next;
    aux->next = nod;
    aux = nod->next;
    nod->prev = aux->prev;
    aux->prev = nod;

    list->size++;

    return;
    
}


dll_node_t*
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
    if (!list) {
        return NULL;
    }

    if (list->size == 0) {
        return NULL;
    }

    if (list->size == 1) {
        dll_node_t *nod = list->head;
        list->head = NULL;
        return nod;
    }

    if (n >= list->size)
        n = list->size - 1;
    
    dll_node_t *aux_node = list->head;
    
    for (int i = 0; i < n; ++i) {
        aux_node = aux_node->next;
    }

    dll_node_t *nod = aux_node;
    dll_node_t *nod1 = aux_node->prev;
    dll_node_t *nod2 = aux_node->next;
    nod1->next = nod2;
    nod2->prev = nod1;

    if (n == 0)
        list->head = nod->next;

    list->size--;
    return nod;
}

unsigned int
dll_get_size(doubly_linked_list_t* list)
{
    if (!list) {
        return 0;
    }

    return list->size;
}


void
dll_free(doubly_linked_list_t** pp_list)
{
    dll_node_t *aux_node = (*pp_list)->head;
    dll_node_t *aux_s;

    for (int i = 0; i < (*pp_list)->size; ++i) {
        free(aux_node->data);
        aux_s = aux_node;
        aux_node = aux_node->next;
        free(aux_s);
    }

    free(*pp_list);
}


void
dll_print_ints_right_circular(dll_node_t* start)
{
    dll_node_t *verify_node = start;
    int *p = NULL;

    do {
        p = (int *) start->data;
        printf("%d ", (*p));

        start = start->next;
    } while (start != verify_node);

    printf("\n");
}


void
dll_print_int_list(doubly_linked_list_t* list)
{
    dll_node_t *aux_node = list->head;
    int *p = NULL;

    for (int i = 0; i < (list->size); ++i) {
        p = (int *) aux_node->data;
        printf("%d ", (*p));

        aux_node = aux_node->next;
    }

    printf("\n");
}


void
dll_print_strings_left_circular(dll_node_t* start)
{
    dll_node_t *verify_node = start;
    char *ptr = NULL;

    do {

        ptr = (char *) start->data;
        printf("%s ", ptr);
        start = start->prev;

    } while (start != verify_node);

    printf("\n");
}

void
dll_print_string_list(doubly_linked_list_t* list)
{
    dll_node_t *start = list->head;
    // char *test = (char *) start->data;
    // printf("%s ", test);
    start = start->prev;
    // test = (char *) start->data;
    // printf("%s ", test);
    char *ptr = NULL;

    for (int i = 0; i < (list->size); ++i) {

        ptr = (char *) start->data;
        printf("%s ", ptr);
        start = start->prev;

    }

    printf("\n");
}

int main() {
    doubly_linked_list_t *doublyLinkedList;
    int is_int = 0;
    int is_string = 0;
    while(1) {
        char command[16], added_elem[MAX_STRING_SIZE], *end_ptr;
        long nr, pos;
        scanf("%s", command);
        if(strncmp(command, "create_str", 10) == 0){
            doublyLinkedList = dll_create(MAX_STRING_SIZE);
            is_string = 1;
        }
        if(strncmp(command, "create_int", 10) == 0){
            doublyLinkedList = dll_create(sizeof(int));
            is_int = 1;
        }
        if(strncmp(command, "add", 3) == 0){
            scanf("%ld", &pos);

            if(is_int) {
                scanf("%ld", &nr);
                dll_add_nth_node(doublyLinkedList, pos, &nr);
            } else if(is_string) {
                scanf("%s", added_elem);
                dll_add_nth_node(doublyLinkedList, pos, added_elem);
            } else {
                printf("Create a list before adding elements!\n");
                exit(0);
            }
        }
        if(strncmp(command, "remove", 6) == 0){
            if(!is_int && !is_string) {
                printf("Create a list before removing elements!\n");
                exit(0);
            }

            scanf("%ld", &pos);
            dll_node_t* removed = dll_remove_nth_node(doublyLinkedList, pos);
            free(removed->data);
            free(removed);
        }
        if(strncmp(command, "print", 5) == 0){
            if(!is_int && !is_string) {
                printf("Create a list before printing!\n");
                exit(0);
            }

            if(is_int == 1){
                dll_print_int_list(doublyLinkedList);
            }
            if(is_string == 1){
                dll_print_string_list(doublyLinkedList);
            }
        }
        if(strncmp(command, "free", 4) == 0){
            if(!is_int && !is_string) {
                printf("Create a list before freeing!\n");
                exit(0);
            }
            dll_free(&doublyLinkedList);
            break;
        }
    }
    return 0;
}

