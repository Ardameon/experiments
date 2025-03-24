> [!FAQ] Как вытащить сообщения syslog из pcap?
> ```bash
> $ tshark -r syslog.pcap -T fields -e syslog udp | awk NF > syslog.log
>```

#syslog/pcap #tshark