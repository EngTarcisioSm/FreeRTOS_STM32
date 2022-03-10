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

## (003_Queue_String)
    - Passando Strings por fila 
    - Não esta passando o valor mais sim um ponteiro 
    - Quando em uma leitura de Queue, se usado "portMAX_DELAY" não há necessidade de utilizar vTaskDelay pois a tarefa ficará em stand-by até ocorrer uma nova recarga na Queue
    - pvPortMalloc e pvPortFree são ports especificos do freeRTOS para alocação e desalocação de memoria de maneira deterministica e sem ocorrer problemas com tempo fixo
    - Sempre ao alocar memoria deve-se desalocar ao fim do seu uso 


    PAREI VIDEO 2: 1:50:02