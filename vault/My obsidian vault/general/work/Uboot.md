> [!INFO]- Настройка свитча 1016
>```uboot
>> mii si x
>```

> [!INFO]- Настройка переменных окружения
>```uboot
>> print
>> setenv *var_name* *val*
>> save
>```

> [!INFO]- Настройка сети 2016
>```uboot
>SMG2016>> setenv serverip 10.25.72.107
>SMG2016>> saveenv
>
>SMG2016>> setenv ipaddr 192.168.23.98
>SMG2016>> saveenv
>
>SMG2016>> setenv gatewayip 192.162.23.1
>SMG2016>> saveenv
>
>SMG2016>> run ramboot
>```

>[!INFO]- Загрузка ядра на устройство через Uboot
>```uboot
>SMG2016>> setenv serverip 10.25.72.107
>SMG2016>> saveenv
>
>SMG2016>> setenv ipaddr 192.168.23.98
>SMG2016>> saveenv
>
>SMG2016>> run flash_kern
>```


#uboot/work #smg2016 #smg1016m

