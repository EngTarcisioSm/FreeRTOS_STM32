# ESTUDOS FREERTOS 
- Observações:
    - Microcontrolador utilizado  STM32F401RBT6;
    - Cada branch representa um tópico.
___

## BRANCH

### (001_Inicio)
- INICIALIZAÇÃO DE UM PROJETO COM FREERTOS 
- DUAS TAREFAS QUE IMPRIMEM NA SERIAM MENSAGEM 

## (002_Queue)
    - Criação de filas
    - Passando valores pela fila

## (003_Queue_String_Referencia)
- Passando Strings por fila 
- Não esta passando o valor mais sim um ponteiro 
- Quando em uma leitura de Queue, se usado "portMAX_DELAY" não há necessidade de utilizar vTaskDelay pois a tarefa ficará em stand-by até ocorrer uma nova recarga na Queue
- pvPortMalloc e pvPortFree são ports especificos do freeRTOS para alocação e desalocação de memoria de maneira deterministica e sem ocorrer problemas com tempo fixo
- Sempre ao alocar memoria deve-se desalocar ao fim do seu uso 

## (004_Queue_Constantes_Por_Copia)
- Passando constantes por fila através de cópia 
- Efeito Bounce tratado neste exemplo

## (005_Queue_Struct_Por_Referencia)
- Passando estruturas por fila 

## (006_Queue_Struct_Copia)
- Mais de uma fila em execução
- Passagem de parametros para a fila por cópia e por referencia (um para cada fila);
- Filas são utilizadas para passar informações para outras tasks;
- Quando xQueueSend, utiliza portMAX_DELAY não há necessidade de checar se foi enviado para queue pois se a queue estiver cheia a tarefa ficará travada ali até que seja enviado 
- Fim aula2

## (007_TaskDebug_FreeRTOS)
- Começar novamente 

- Inserir no arquivo de configuração do FreeRTOS (FreeRTOSConfig.h) os dois defines abaixo
    ~~~c
        /*DEFINES PARA HABILITAR O USO DAS FUNÇÕES VTASKLIST*/
        #define configUSE_TRACE_FACILITY				1
        #define configUSE_STATS_FORMATTING_FUNCTIONS	1
    ~~~

- Função de debug:
    ~~~c
        void vTask_blink(void *pvParameters) {
            
            char  c_buff[200];
            
            vPrintString("Entrando da Task de debug");
            
            for (;;) {
                //Altera o estado do led
                HAL_GPIO_TogglePin(DOUT_LED1_GPIO_Port, DOUT_LED1_Pin);
                
                vTaskList(c_buff);
                
                vPrintString("\n\r\nTask-------------State-----Prio------Stack---Num\r\n");
                vPrintString(c_buff);
                vPrintString("\n\n\n");
                
                memset(c_buff, 0, sizeof(c_buff));
                
                sprintf(c_buff, "Free Heap: %d bytes \n\n", xPortGetMinimumEverFreeHeapSize());
                vPrintString(c_buff);
                
                memset(c_buff, 0, sizeof(c_buff));
                
                vTaskDelay(5000 / portTICK_PERIOD_MS);
            }
            vTaskDelete( NULL);
        }
    ~~~

- Saída
    - Stack refere-se a quantidade de memória restante daquela atribuida a tarefa, lembrando que para calcular é o valor restante da Stack x 4, resultando no numero de bytes
    ~~~
    <LF>
    Task-------------State-----Prio------Stack---Num<CR><LF>
    Task Blink     <HT>	X<HT>	1<HT>	88<HT>	2<CR><LF>
    IDLE           <HT>	R<HT>	0<HT>	104<HT>	7<CR><LF>
    task_1         <HT>	B<HT>	1<HT>	10<HT>	4<CR><LF>
    task_2         <HT>	B<HT>	1<HT>	10<HT>	5<CR><LF>
    Task Print Q   <HT>	B<HT>	2<HT>	316<HT>	1<CR><LF>
    Task Print     <HT>	B<HT>	1<HT>	318<HT>	3<CR><LF>
    vTask_print_cou<HT>	B<HT>	1<HT>	272<HT>	6<CR><LF>
    <LF>
    Free Heap: 47720 bytes <LF>

    ~~~
- Buffer da task blink que funciona como debug do sistema teve sua memoria de tarefa aumentada 

- OBSERVAÇÃO: 
    - Em #define configTOTAL_HEAP_SIZE, configurar para que utilize toda memoria ram, pois a lib HAL necessita de uma quantidade de memoria ram para efetuar seus procedimentos 



## (008_ISR_QUEUE_DETERMINISMO)
- Envio de QUEUE por acionamento de interrupção de botão
- Observação: xQueueSendFromISR não possui temporização pois ela necessita de ser o mais breve possivel para não acarretar em problemas na temporização no sistema 
    - Quando xQueueSendFromISR gera um valor verdadeiro em seu terceiro parametro portYIELD_FROM_ISR devolve o contexto para a tarefa de maior prioridade. Quando xQueueSendFromISR retorna falso no terceiro parametro o contexto é devolvido para a tarefa que anteriormente estava sendo executada, independente da prioridade 
- Toda vez que que o token "BaseType_t xHigherPriorityTaskWoken" for inicializado, é indicado pela documentação do FreeRTOS que seja inicializado com o valor pdFALSE 

- Observação
    - Dentro de ISR é somente permitidos funçõe que mecham com o FreeRTOS que tenham final FROM_ISR, caso desrespeito dessa regra ocorre erro no FreeRTOS
        ~~~c
            //em todos os projetos
            #define configSUPPORT_STATIC_ALLOCATION          0
        ~~~
    - void vTask_print(void *pvParameters), trabalha como uma task de controle de outras duas outras tasks que enviam uma struct por referencia 

## (009_MAILBOX)
    - 

## (010_QUEUE_SET)
- Não recomendado a utilização, a não ser que o uso de Queue normal seja impossibilitado
- É necessário o define descrito abaixo
    ~~~c
        #define configUSE_QUEUE_SETS					1
    ~~~
- São criadas 3 queues
- É criado um QueueSet 
    - QueuesSets possui um consumo de memória grande 
    - Ao criar a QueueSet é utilizado como parametro (1 * 3) o 1 indica o tamanho que cada queue ocupa e o 3 indica a quantidade de queues 
- A vantagem de se utilizar QueueSet é gerenciar váras Queues com uma unica função

## (011_EVENT GROUPS)
- Para utilização de eventgroups utilizar o include 
    ~~~c
        #include "event_groups.h" 	//incluido
    ~~~
- Para auxiliar na checagem dos eventgroups utiliza-se os defines:
    ~~~c
        #define BIT_0 (1 << 0)
        #define BIT_1 (1 << 1)
        #define BIT_2 (1 << 2)
        #define BIT_3 (1 << 3)
        #define BIT_4 (1 << 4)
        #define BIT_5 (1 << 5)
        #define BIT_6 (1 << 6)
        #define BIT_7 (1 << 7)
        #define BIT_8 (1 << 8)
        #define BIT_9 (1 << 9)
        #define BIT_10 (1 << 10)
        #define BIT_11 (1 << 11)
        #define BIT_12 (1 << 12)
        #define BIT_13 (1 << 13)
        #define BIT_14 (1 << 14)
        #define BIT_15 (1 << 15)
        #define BIT_16 (1 << 16)
        #define BIT_17 (1 << 17)
        #define BIT_18 (1 << 18)
        #define BIT_19 (1 << 19)
        #define BIT_20 (1 << 20)
        #define BIT_21 (1 << 21)
        #define BIT_22 (1 << 22)
        #define BIT_23 (1 << 23)
    ~~~
- É criado o Handle para utilização da função 
    ~~~c 
        static EventGroupHandle_t xHandle_Event_Group;
    ~~~
- É necessário criar o eventgroup com a função descrita abaixo:
    ~~~c
        xHandle_Event_Group = xEventGroupCreate();
    ~~~
- EventGroups podem ser utilizadas dentro de ISR
- Funções importantes:
    - xEventGroupSetBits()
        - Escreve bits dentro do eventgroup
        - 2 parametros 
            - 1 parametro: handle de eventgroup
            - 2 parametro: bit a ser escrito com base nos defines acima mensionados 
    - xEventGroupWaitBits()
        - retorna os bits que estão escritos no event group, o tipo desse retorno é "EventBits_t"
        - possui 5 atributos
            - parametro 1 : handle do eventgroup
            - parametro 2 : bits a serem escritos
            - parametro 3 : valor booleano, pdTRUE, assim que os bits forem lidos eles serão apagados dentro do eventgroup se pdFALSE, os bits não são resetados
            - parametro 4 : em pdTRUE todos os bits escolhidos na leitura deve estar em alto, pdFASE não há necessidade de que todos bits avaliados estejam em alto. Para a leitura de apenas 1 bit, esse parametro tanto fazer ser true ou false 
            - tempo de espera para que todas os bits esejam disponiveis 


## (011_EVENT SYNC)





~~~c

~~~

1:31:15 aula 3 - PASSANDO O CÓDIGO PARA A CUBEIDE