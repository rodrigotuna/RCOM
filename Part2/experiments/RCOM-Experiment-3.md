# Step 2
- a. gnu-rtr1
- b. 2 (0,1) ports, fastethernet.
- c. 0 - 172.16.30.1 255.255.255.0 // 1 - 172.16.254.45 255.255.255.0
- d. ip route 0.0.0.0 0.0.0.0 172.16.254.1 // ip route 172.16.40.0 255.255.255.0 172.16.30.2

# Step 3
- A interface 1
- 14?
- Yes

# step 2
No

# step 3
Destination 127.0.0.53

# step 4
Destination 9.9.9.9

# step 1
VM:
default via 10.0.2.1 dev enp0s3 proto dhcp metric 100 

# step 2
sudo route del default

# step 3
traceroute -n 104.17.113.188 

traceroute to 104.17.113.188 (104.17.113.188), 30 hops max, 60 byte packets
connect: Network is unreachable

# step 4
sudo route add 104.17.113.188 gw 10.0.2.1

# step 5
log wireshark

# step 6
traceroute -n who.int

traceroute to who.int (104.17.113.188), 30 hops max, 60 byte packets
 1  10.0.2.1  0.612 ms  0.474 ms  0.400 ms
 2  * * *
 (...)
 30 * * *

sudo route add 127.0.0.53 gw 10.0.2.1

traceroute -n who.int está preso

# step 7

sudo route del 127.0.0.53
sudo route add default gw 10.0.2.1
