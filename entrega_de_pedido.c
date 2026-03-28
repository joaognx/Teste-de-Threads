#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct{
    int id_pedido;
    int distancia;
} dadosPedido;

void* entrega_produto(void* arg){

    dadosPedido* entrega = (dadosPedido*)arg;

    printf("[THREAD %ld] = Pedido %d (Distancia: %d)\n", pthread_self(), entrega->id_pedido, entrega->distancia);

    sleep(entrega->distancia); 
    
    printf("[Thread %ld] Pedido #%d ENTREGUE.\n", pthread_self(), entrega->id_pedido);

    free(entrega);
    pthread_exit(NULL);
}

int main(){

    int contador =0;

    while(contador < 5){
        sleep(1);
        
        dadosPedido* pedido = malloc(sizeof(dadosPedido));
        pedido->id_pedido = ++contador;
        pedido->distancia = (rand() %50 + 1);

        pthread_t id_thread;
        int ok = pthread_create(&id_thread, NULL, entrega_produto, (void*)pedido);
        if( ok !=0){
            printf("erro");
            return 1;
        }

        pthread_detach(id_thread);
    }

    printf("\n[Main] Todas os pedidos foram encaminhados.\n");
    pthread_exit(NULL); 
}