Работа либы проходит в бесконечном цикле внутри следующей функции:

```c
ooMonitorChannels()
```

>[!note]- Список сокетов, которые мониторятся
>```c
>/* RAS socket of GK client (UDP)*/
>gH323ep.gkClient->rasSocket
>/* endpoint Q.931 signaling channel listener (TCP) */
>gH323ep.listener
>/* endpoint command channel listener (TCP) (only for WIN) */
>gH323ep.cmdListener
>/* endpoint command channel accepted socket (PIPE in linux) */
>gH323ep.cmdSock
>/* call signaling channel accepted from endpoint listener or 
> * created by connecting to remote side */
>call->pH225Channel->sock
>/* call media control channel accepted from h245listener or 
> * created by connecting to remote side h245listener */
>call->pH245Channel->sock
>/* call media control channel listener (TCP) */
>call->h245listener
>```

>[!note]- Функции манипуляции с основными сокетами
>```c
>ooGkClientCreateChannel()
>ooCreateH323Listener()
>ooWriteStackCommand()
>ooProcessStackCommand()
>ooCreateH225Connection()
>ooAcceptH225Connection()
>ooH2250Receive()
>ooCreateH245Connection()
>ooAcceptH245Connection()
>ooH245Receive()
>```

>[!note]- Функции декодирования и обработки Q.931
>```c
>ooQ931Decode()
>ooHandleH2250Message()
>```