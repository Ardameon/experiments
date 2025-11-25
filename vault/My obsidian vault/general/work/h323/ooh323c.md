Работа либы проходит в бесконечном цикле внутри следующей функции:

```c
ooMonitorChannels()
```

>[!example]- Список сокетов, которые мониторятся
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

>[!example]- Функции манипуляции с основными сокетами
>```c
>ooGkClientCreateChannel()
>ooCreateH323Listener()
>ooWriteStackCommand()
>ooProcessStackCommand()
>ooCreateH225Connection()
>ooAcceptH225Connection()
>ooH2250Receive()
>ooSendH225Msg()
>ooCreateH245Connection()
>ooAcceptH245Connection()
>ooH245Receive()
>```

>[!example]- Функции декодирования и обработки Q.931
>```c
>ooQ931Decode()
>ooHandleH2250Message()
>```

> [!note] Обработка входящего вызова
```c
/* Accepting new TCP-connection and call cration */
ooAcceptH225Connection()
 ooGenerateCallToken()
 ooCreateCall()
 ooSocketGetIpAndPort()
```
```c
/* Handle incoming H.225.0 SETUP message */
ooH2250Receive()
 ooSocketRecv()
 initializePrintHandler()
 setEventHandler()
 ooQ931Decode()
 finishPrint()
 removeEventHandler()
 ooHandleH2250Message()
  ooSendCallProceeding()
  ooH323CallAdmitted()
   /* Notify application layer with callback */
   gH323ep.h323Callbacks.onIncomingCall()
  
```
> [!note] Отправка сообщений
```c
/* Preparing new Q.931 message and store it to out queue */
ooSendCallProceeding()
 new_Q931_UUIE()
 ooSendH225Msg()
  ooEncodeH225Message()
  dListAppend(call->pH225Channel->outQueue)
```
```c
/* Get message from out queue and send it */
ooMonitorChannels()
 ooProcessFDSETsAndTimers()
  if(call->pH225Channel->outQueue.count>0)
   ooSendMsg(call, OOQ931MSG)
    dListRemove(&(call->pH225Channel->outQueue), p_msgNode)
	ooSocketSend()
```

