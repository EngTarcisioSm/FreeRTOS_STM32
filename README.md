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


## (012_EVENT SYNC)
 - Semelhante ao event group na criação, entretanto trata de mais de um bit 
 - Funções:
    - xEventGroupSync(xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait)
        - 1: Handle do eventgroup
        - 2: bit a ser setado 
        - 3: resposta que se deseja dos bits setados 
        - 4: tempo de espera 

        - possui retorno informando os bits setados, podendo esse retorno ser ou não utilizado


## (013_Eventgroups)
- Manipular várias tarefas com eventgroups 
- No exemplo anterior 3 tarefas eram utilizadas para manipular 1, neste exemplo 1 tarefa manipula 3
- A tarefa vTask_check_event não testa um evento, ela gera os eventos;
- Uma tarefa pode ser desbloqueada interminadamente quando o terceiro parametro de "xEventGroupWaitBits" é colocado em pdFALSE, e pode ser bloqueado novamente com a função "xEventGroupClearBits"

## (014_Software_Timer)
- Configuração FreeRTOSConfig.h
    ~~~c
        #define configUSE_TIMERS						1   //habilitar o timer 
        #define configTIMER_TASK_PRIORITY				5   //configurar nivel de prioridade
        #define configTIMER_QUEUE_LENGTH				10  //tamanho do buffer indicando quantidade que os timersiop
        #define configTIMER_TASK_STACK_DEPTH			256 //tamanho da memoria de guardar contextos em words
    ~~~
- Inclusão em main.c
    ~~~c
        #include "timers.h"			//incluido
    ~~~

    - Definir a quantidade de ticks para configurar o timer 
        ~~~c
            #define mainONE_SHOT_TIMER_PERIOD 		pdMS_TO_TICKS( 3333 )
            #define mainAUTO_RELOAD_TIMER_PERIOD	pdMS_TO_TICKS( 500 )
        ~~~
        - A macro converte o valor passado em ticks para as funções que necessitam desse valor e armazena dentro dos respectivos defines 
    - Criado os prototipos das funções de callback 
        ~~~c
            static void prvOneShotTimerCallback( TimerHandle_t xTimer );
            static void prvAutoReloadTimerCallback( TimerHandle_t xTimer );
        ~~~
        - Os parametros são os Handle dos timers
    - Criação dos handle dos timers 
        ~~~c
            TimerHandle_t xAutoReloadTimer, xOneShotTimer;
        ~~~
    - Criação de variáveis para checar se os timers foram inicalizados com sucesso 
        ~~~c
            BaseType_t xTimer1Started, xTimer2Started;
        ~~~
    - Criação do timer com a função xTimerCreate, recebe 5 parametros 
        - 1: nome do timer (string - de preferencia um nome pequeno)
        - 2: periodo em ticks que se deseja configurar o timer 
        - 3: 
            - pdFALSE -> configurado como estouro unico 
            - pdTRUE -> configurado como reinicialização automatico do timer 
        - 4: possibilidade de criação de um ID (torna-se desnecessário pois o contextos dos timers esta sendo salvo dentro das variaveis do timo TimerHandle_t)
        - 5: Função de callback a qual deve ser executada ao ocorrer o estouro do timer 
        ~~~c
            /*Cria timer com a configuração de disparo único*/
            xOneShotTimer = xTimerCreate(
                    "OneShot", 					/* Nome do texto para o cronômetro do software - não usado pelo FreeRTOS */
                    mainONE_SHOT_TIMER_PERIOD, 	/* Periodo de ISR do timer em ticks */
                    pdFALSE, 					   /* Se pdTRUE for passado como parametro ele configura reinicialização automática
                                                    * Se pdFALSE for passado como parametro ele configura como disparo único
                                                    */
                    0, 							/* Não utilizamos ID de timer neste exemplo */
                    prvOneShotTimerCallback 		/* Função de callback deve ser passado nesse parametro para seja processado a ISR */
                    );

            /* Cria um software timer com a configuração de reinicialização automática */
            xAutoReloadTimer = xTimerCreate(
                    "AutoReload",					/*Nome do texto para o cronômetro do software - não usado pelo FreeRTOS...*/
                    mainAUTO_RELOAD_TIMER_PERIOD,	/*Periodo de ISR do timer em ticks*/
                    pdTRUE,						/* Se pdTRUE for passado como parametro ele configura reinicialização automática
                                                    * Se pdFALSE for passado como parametro ele configura como disparo único
                                                    */
                    0,							/*Não utilizado ID de timer neste exemplo*/
                    prvAutoReloadTimerCallback	/*Função de callback deve ser passado nesse parametro para seja processado a ISR*/
                    );
        ~~~ 
    - Como os timers são objetos do freeRTOS pois são criados dinamicamente, é necessário vertificar se de fato ocorreu a criação dos timers inseridos 
        ~~~c
            if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL)) {
        ~~~
    - Após a verificação dos objetos criados é necessário iniciar os timers com a função xTimerStart, passando o handle do timer e em seguida um possivel atraso para inicialização do mesmo 
        ~~~c
            xTimer1Started = xTimerStart( xOneShotTimer, 0);
	        xTimer2Started = xTimerStart( xAutoReloadTimer, 0);
        ~~~
    - É possivel verificar se os timers foram criados com sucesso, é praticamente impossivel depois desses passos os timers não ser inicializados (se torna não necessário essa parte)
        ~~~c
            if((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS)) {
        ~~~

    - Retorna a quantidade de ticks
        ~~~c
            xTimeNow = xTaskGetTickCount();
        ~~~
    
    - A task "Tmr Svc" é a task daemon 
        ~~~c
            Task-------------State-----Prio------Stack---Num<CR><LF>
            Task Blink     <HT>	X<HT>	1<HT>	82<HT>	1<CR><LF>
            IDLE           <HT>	R<HT>	0<HT>	108<HT>	2<CR><LF>
            Tmr Svc        <HT>	B<HT>	5<HT>	114<HT>	3<CR><LF>
        ~~~

## (014_2_Software_Timer)

- Reativação  do oneTime a cada 10 estouros do timer AutoReload 
- OneShot alterado para 100ms
- Os Handle devem realocados globalmente para ser visualizados em outras funções de callback
- Sempre verificar o consumo de memoria por "Tmr Svc"

## (015_Software_Timer_ID)

- Manipulando Software timers através do ID
- a função de callback dos dois timers é a mesma função 
    - se descobre qual a tratativa a partir do handle de estouro do timer 
- Explicação do código abaixo 
    ~~~c
        ulExecutionCount = (uint32_t) pvTimerGetTimerID( xTimer );

        ulExecutionCount++;

        vTimerSetTimerID( xTimer, (void *)ulExecutionCount);
    ~~~
    - É pego o ID do timer o valor é incrementado e depois alterado o valor do ID, no fim 
    o termo ID funciona para armazenar o incremento ocorrido na variavel "ulExecutionCount" 

    - vTaskList(0) presente na tarefa de debug não é deterministica
    - Valores expressos no debug na coluna de Stack é dados em words, para converter para bytes na arquitetura de 32bits basta multiplicar por 4


## (016_Software_Timer_AltTimer)
- Modificação do tempo de estouro do software timer baseado em condicionais inseridas 

## (017_SemaforosISR)
- Necessário utilizar a biblioteca de semáforos 
    ~~~c
        #include "semphr.h" 
    ~~~
- Gerar um Handle para o semáforo
    ~~~c
        SemaphoreHandle_t xBinarySemaphore = NULL;
    ~~~
- Criar o semáforo a partir da função de criação do semáforo 
    ~~~c
        xBinarySemaphore = xSemaphoreCreateBinary();
    ~~~
    - É testado se o semáforo foi criado com sucesso 
    ~~~c
        if(xBinarySemaphore != NULL) vPrintString("Semaforo criado com sucesso\n");
    ~~~
- Nos arquivos do FreeRTOS o Semáforo nada mais é do que uma fila, uma QUEUE, entretanto os semáforos são mais rápidos que as filas por não tratar nenhum tipo de dado 
- É possivel trabalhar com xSemaphoreTake() por timeout, o mesmo tem como retorno uma variavel do tipo BaseType_t que indica com base no segundo parametro se dentro daquele intervalo a condição foi atendida, retornando pdTRUE, caso o estouro tenha ocorrido sem ser possivel efetuar o take é retornado pdFALSE
    ~~~c
        BaseType_t Timeout_Semaphore = pdFALSE;
        Timeout_Semaphore = xSemaphoreTake( xSemaphoreBinary, tempo / portTICK_PERIOD_MS );
        if (Timeout_Semaphore == pdTRUE) {
            //take foi possivel
        } else {
            //ocorreu o timeout
        }
    ~~~

## (018_SemaforosContador)
- Descrições dentro do código 
- Habilitação no arquivo FreeRTOSConfig da funcionalidade de Semaforos contadores 
    ~~~c
        #define configUSE_COUNTING_SEMAPHORES			1
    ~~~
- É necessário ainda utilizar a lib de semáforos 
    ~~~c
        #include "semphr.h"  
    ~~~
- O Handle de semáforos binarios e contadores são de mesmo tipo "SemaphoreHandle_t"
- A função de criação do semáforo é diferente pois a a necessidade de especificar a quantidade de espaços disponiveis para tokens e a partir de qual bloco deve-se iniciar a inclusão de tokens, caso seja inserido valor diferente de 0 no segundo parametro os demais ficaram inutilizados 
    ~~~c
        //exemplo
        xCountingSemaphore = xSemaphoreCreateCounting(10,0);
    ~~~

## (019_Semaforos_Binarios_E_Contador)
- 3 Semáforos 
    - 2 Contadores 
    - 1 Binário

## (020_Mutex)
- Para a utilização de Mutex é necessário efetuar sua habilitação no arquivo de configuração do FreeRTOS
    ~~~c
        #define configUSE_MUTEXES						1
    ~~~
- Para criação de uma mutex é necessario criar seu handle que é o mesmo de um semáforo comum e em seguida utilizar a função xSemaphoreCreateMutex();

    ~~~c
        SemaphoreHandle_t xMutex = NULL;
        xMutex = xSemaphoreCreateMutex();
    ~~~
- A mutex neste exemplo é utilizada para gerenciar o acesso ao barramento UART na função vPrintString(char *pc_uartSend_f)
- Para utilização é necessário incluir as bibliotecas de semaforos e filas 

## (021_Mutex_Recursiva)
- Para a utilização de Mutex recursivas é necessárui efetuar a configuração do recurso no arquivo de configuração 
    ~~~c
        #define configUSE_RECURSIVE_MUTEXES				1
    ~~~
- Para criar a Mutex Recursiva utiliza a função 
    ~~~c
        xMutexRecursive = xSemaphoreCreateRecursiveMutex();
    ~~~
- As mutex recursivas ocorre o incremento de uma mutex unica em um processo, e ao sair desse processo a mutex é decrementada
- Para todo take dado é necessário um give;
- Takes internos n podem ser bloqueantes, devem ter um valor de timeout
- Quando existe algum processo que usa funções e essas funções utilizam outras funções, sendo que alguma dessas funções internas pode ser utilizada em algum outro processo ou tarefa é recomendado a utilização de mutex recursivas 
    - Exemplo: SPI desenvolvidas na mão, existem várias funções, cada uma delas podendo ser usada por um dispositivo específico 

## (022_TaskNotify_Como_Semaforo)
- É necessário de configurar as TaskNotify no arquivo de configuração 
    ~~~c
        #define configUSE_TASK_NOTIFICATIONS			1
    ~~~
- TaskNotify é implementado sobre a biblioteca "task.h"
- Para sua utilização é necessário a utilização de TaskHandle
    ~~~c
        static TaskHandle_t xTaskHandle_ISR;
    ~~~
- Não é necessário criar nenhum objeto do FreeRTOS
    - A unica memoria utilizada é quando se declara o Handle 
- Observação:
    - Não substitui 100% o uso de filas (simula bem o uso de mailbox) apenas o uso de semáforos 
- Para utilizar o TaskNotify, ele é passado como ultimo parametro na criação da tarefa que o utilizará 
    - Exemplo:
    ~~~c
        if ((xTaskCreate(vTask_Handle, "Task handle", configMINIMAL_STACK_SIZE * 2,
	        NULL, 1, &xTaskHandle_ISR)) != pdTRUE) {
		    vPrintString(
				"não foi possivel alocar tarefa vTaskHandle no escalonador\n");
        } else {
            vPrintString("Tarefa vTaskHandle criada com sucesso!\n");
        }
    ~~~
- Topologia deste exemplo baseado a timeout, pois utiliza um periodo de tempo pré definifo 
    ~~~c
        ulEventsToProcess = ulTaskNotifyTake(pdTRUE, xMaxExpectedBlockTime);
        //xMaxExpectedBlockTime marcado como 5000ms
    ~~~
## (022_TaskNotify_2_EVENTO)
- Topologia baseada a evento 
    ~~~c
        ulEventsToProcess = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        //processos
    ~~~
- Nos dois exemplos 022, é necessário a utilização do loop while quando ocorre o evento para decrementar a variavel  ulEventsToProcess

## (022_TaskNotify_3_SEMAFORO_BINARIO)
- Não é pego o retorno ulTaskNotifyTake()

## (023_TaskNotify_SEMAFORO_CONTADOR)
- Observações 
    - obs1
    ~~~c
        //o valor de xHigherPriorityTaskWoken é pdFALSE, desta forma ao sair da interrupção não é dado preferencia para a tarefa que usa a notificação 
        //chamar esta função em mais de uma vez incrementa do valor de xTaskHandle_ISR em 1 unidade 
        vTaskNotifyGiveFromISR(xTaskHandle_ISR, &xHigherPriorityTaskWoken);
    ~~~
    - obs2
    ~~~c
        //passado com o valor de pdFALSE a função decrementa a TaskNotify existente na tarefa em 1 unidade
        ulTaskNotifyTake(pdFALSE, xMaxExpectedBlockTime)
    ~~~
    - obs3: com o segundo parametro de ulTaskNotifyTake definido em um periodo, essa logica esta associada a timeout, pois caso não ocorrendo o processo, alguma outra atitude pode ser tomada 

## (023_TaskNotify_SEMAFORO_CONTADOR_2)
- Lógica baseada a evento com:
~~~c
    ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
~~~
    - Valor do segundo parametro em "portMAX_DELAY" 
- Nos dois exemplos 023 é possivel pegar o retorno da função "ulTaskNotifyTake()" sendo o numero que consta na TaskNotify, ela ainda será decrementada

## (023_TaskNotify_Varios)
- Várias TaskNotify
- Mostra ser possivel além de controlar tarefas a partir de interrupções, controlar tarefas a partir de outras tarefas também 


## (025_TaskNotify_)
- Operando como eventgroup;

- São definidos os bits 
    ~~~c
        #define ULONG_MAX	0xFFFFFFFF

        #define TNBIT_1 0x01
        #define TNBIT_2 0x02
        #define TNBIT_3 0x03
        #define TNBIT_4 0x04
    ~~~

- Criação dos TaskNotify
    ~~~c
        static TaskHandle_t xTaskHandle_ISR;
        static TaskHandle_t xTaskHandle_Evt_Ext;
        static TaskHandle_t xTaskHandle_3;
    ~~~

~~~c
    xTaskNotifyFromISR(xTaskHandle_Evt_Ext, // Handle da task a ser notificada
                    TNBIT_1,// BIT a ser verificado
                    eSetBits,// Setando bit nivel alto
                    NULL);//Parametro NULLpois não estou usando Token para acordar esta task, e sim outra.
~~~
- Observações da função acima
    - A função escreve no tasknotify
    - parametro 01: a TaskNotify a ser escrita
    - parametro 02: o bit da tasknotify a ser escrito 
    - parametro 03: o valor a ser escrito no bit 
    - parametro 04: existe duas situações possiveis neste caso não ira efetuar o wakeup automático da tarefa que aguarda 

~~~c
    xTaskNotifyWait( pdFALSE, //pdFALSE = não limpa os bits na entrada da função / pdTRUE = limpa os bits na entrada da função
                    ULONG_MAX, //Limpa todos os BITS na saída, caso queira limpar separadamente (TNBIT_3 | TNBIT_2) como exemplo
                    &ulNotifiedValue, //Variavel para armazenar o valor do bit notificado
                    portMAX_DELAY);
~~~
- Observações da função acima
    - parametro 01: Não limpa os dados na entrada justamente para os valores poderem ser copiados  em ulNotifiedValue
    - parametro 02: Ao sair sinaliza quais bits serão zerados, "ULONG_MAX" neste caso, todos os bits serão resetados 
    - parametro 03: Local onde o valor será armazenado 
    - parametro 04: Tempo de espera "portMAX_DELAY" neste caso será associado a evento 

    - A função quanto possui valor retorna pdPASS
    - Mesmo que o segundo parametro seja definido como 0, isso não permite que a tarefa fica acordada eternamente 

~~~c
    xTaskNotify(xTaskHandle_3, TNBIT_3,  eSetBits);
~~~
- Observações da função acima
    - parametro 01: a tasknotify a ser manipulada em escrita
    - parametro 02: bit a ser escrito 
    - parametro 03: estado do bit (se high ou low)

## (026_ADC_Polling_TaskNotify)
- Apresenta duas tarefas extras, sendo a primeira de leitura do adc e a segunda de escrita na serial do valor adc 
- Na função de leitura adc existe uma lógica que o valor lido no canal ADC é escrito na tasknotify da função de leitura. 
- Existe uma lógica em que a função de leitura só consegue escrever na tasknotify da de leitura se ela estiver vazia:
    ~~~c
        do {
			//enquanto xResult não retornar pdPASS que é informado caso a outra tarefa ja tenha lido o valor, ele irá permancer neste loop tentando executar o envio
			//é importante observar que o parametro eSetValueWithoutOverwrite significa que:
			//so irá modificar o valor passado no parametro value utilizado na variavel analog_pot
			//caso a tarefa notificada tenha lido este valor.
			xResult = xTaskNotify(xTaskHandle_Recv_ADC, analog_pot,
					eSetValueWithoutOverwrite);
		} while (xResult != pdPASS);
    ~~~
    - caso a escrita não seja possivel o valor de xResult será diferente de pdPASS 
        - isso é possivel devido ao terceiro parametro da função "eSetValueWithoutOverwrite", para que seja possivel sobrescrever utilizaria "eSetValueWithOverwrite"
- Na tarefa de recebimento existe a função "xTaskNotifyWait()" tendo seus dois primeiros parametros iguais a zero não limpando os dados na entrada e também não limpando o buffer representado por &ulADCValue
- Lógica efetuada com o stm32f411ceu

## (027_ADC_DMA)
- Lógica efetuada com o stm32f411ceu
- Conversão ADC utilizando DMA com o buffer circular
    - Efetua a conversão sem utilizar processamento 
- A função Task_SEND_ADC inicializa o hardware DMA, como o canal ADC é de 12bits, é passado um array de 10 posições de 16bits que funcionará como o buffer 
~~~c
HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADCSamples, 10); //o ultimo parametro é o tamanho do buffer 
~~~
- A função entra em estado suspenso indefinidamente até que algum processo a acorde (nesse caso o preenchimento do buffer, na função de callback do dma) 
~~~c
vTaskSuspend(NULL); //como foi passado NULLesta suspendendo a propria tarefa, em caso de suspensão de outra tarefa teria de passar seu handle 
~~~
- Quando a função é acordada, ela desliga o hardware DMA 
~~~c
HAL_ADC_Stop_DMA(&hadc1);
~~~
- É preparado um segundo buffer de 10 posições e tamanho de 32bits cada e é enviado esse buffer através de uma fila, essa fila acorda a função de recebimento "vTask_Recv_ADC"
- A função "vTask_Recv_ADC" printa para o usuário as 10 amostras obtidas 
