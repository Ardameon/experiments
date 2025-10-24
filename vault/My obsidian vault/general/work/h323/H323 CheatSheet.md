### H.323 - стек протоколов IP-телефонии

1. [[#Протоколы в стеке H.323]]
2. [[#Дополнительные протоколы]]
3. [[#Элементы системы]]
4. [[#Порты]]
5. [[#Gatekeeper]]
6. [[#H.323 Protocol Syntax]]
7. [[#RAS Protocol Messages]]

---
#### Протоколы в стеке H.323

* H.225.0 RAS (UDP) - Registration Admission and Status (используется между endpoint/gateway/gatekeeper и gatekeeper)
* H.225.0 Call Signaling (Q.931) (TCP/UDP) - используется для установления и разрыва вызовов
* H.245 Media control (TCP) - управление медиа-потоками (аналог SDP)
* RTP - это RTP

---
#### Дополнительные протоколы

* H.235 - Security for signaling and media
* H.239 - Dual stream use for videoconferencing
* H.450 - Various supplementary services
* H.460 - NAT/Firewall
---
#### Элементы системы

* Terminal
* MCU (Multipoint Control Unit)
* Gateway
* Gatekeeper
* Border element
---
#### Порты 

Default H.323 port - TCP 1720
Default RAS port - UDP 1719

---
#### Gatekeeper

Взаимодействуют с терминалами и другими гейткиперами по протоколу RAS

Функции

* Enpoint registration
* Address resolution
* Admission control
* User athentication

Режимы работы

* Direct-routed
* Gatekeeper-routed

Zone - набор терминалов зарегистрированных на одном гейткипере
Гейткиперы могут проводить вызовы между зонами обращаясь на другие гейткиперы

Administrative domain - набор зон, обслуживаемых одним оператором.
Связь между доменами осуществляется через border element

---
#### H.323 Protocol Syntax

* ASN.1

---
#### RAS Protocol Messages

* Gatekeeper request, reject and confirm messages (GRx)
* Registration request, reject and confirm messages (RRx)
* Unregister request, reject and confirm messages (URx)
* Admission request, reject and confirm messages (ARx)
* Bandwidth request, reject and confirm message (BRx)
* Disengage request, reject and confirm (DRx)
* Location request, reject and confirm messages (LRx)
* Info request, ack, nack and response (IRx)
* Nonstandard message
* Unknown message response
* Request in progress (RIP)
* Resource availability indication and confirm (RAx)
* Service control indication and response (SCx)

---
#voip/h323 #work/voip
