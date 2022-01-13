## TUX44

-- ifconfig eth0 up

-- ifconfig eth0 172.16.40.254/24

> MAC ADDRESS: 00:21:5a:5a:7b:ea 

> IP ADDRESS: 172.16.40.254

----------------------------------------------------------
## TUX43
-- ifconfig eth0 up

-- ifconfig eth0 172.16.40.1/24

> MAC ADDRESS: 00:21:5a:61:2f:d4

> IP ADDRESS: 172.16.40.1

-------------------------------------------------

## TUX43

> ping 172.16.40.254 -> ALL OK, CONNECTED 

## TUX44

> ping 172.16.40.1 -> ALL OK, CONNECTED

-------------------------------------
route -n -> ver foto para cada TUX

arp -a -> ver foto para cada TUX

----------------------------------------------

arp -d 172.16.40.254 -> delete arp table entry

-----------------------------------------------

abrir wireshark começou a capturar no eth0
ping 172.16.40.254
paramos e recebeu 15 pacotes
paramos de capturar no wireshark e guardamos o log
