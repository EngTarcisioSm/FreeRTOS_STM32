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
