#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Estrutura que representa o Evento
typedef struct {
    int id_chamada;
    int gravidade; // 1 a 5
} ChamadaEmergencia;

// Função que a Thread vai executar (o "Tratador de Evento")
void* processar_chamada(void *sla) {
    ChamadaEmergencia *evento = (ChamadaEmergencia*)sla;
    
    printf("[Thread %ld] Atendendo chamada #%d (Gravidade: %d)...\n", 
            pthread_self(), evento->id_chamada, evento->gravidade);
    
    // Simula o tempo de atendimento (evento demorado)
    sleep(evento->gravidade); 
    
    printf("[Thread %ld] Chamada #%d FINALIZADA.\n", pthread_self(), evento->id_chamada);
    
    free(evento); // Libera a memória alocada para o evento
    pthread_exit(NULL); // Encerra a thread
}

int main() {
    int contador = 0;

    printf("=== Central de Emergências Rodando (Thread-per-Event) ===\n");

    while (contador < 10) { // Simula 10 eventos chegando
        printf("\nAguardando nova chamada...\n");
        sleep(1); // Espera 1 segundo entre chamadas

        // 1. Criar os dados do Evento
        ChamadaEmergencia *nova_chamada = malloc(sizeof(ChamadaEmergencia));
        nova_chamada->id_chamada = ++contador;
        nova_chamada->gravidade = (rand() % 5) + 1;

        // 2. Implementar Thread por Evento
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, processar_chamada, (void*)nova_chamada) != 0) {
            perror("Falha ao criar thread");
            return 1;
        }
        
        //for(int i =0; i<=10; i++){
            //printf("%d\n", i);
        //}
        

        // 3. Detach: Informa que a thread limpa seus recursos sozinha ao terminar
        pthread_detach(thread_id);
    }

    printf("\n[Main] Todas as chamadas foram encaminhadas. Saindo...\n");
    pthread_exit(NULL); 
}