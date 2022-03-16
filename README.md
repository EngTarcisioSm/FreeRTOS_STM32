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
    

1:01:29 aula 3 - PASSANDO O CÓDIGO PARA A CUBEIDE