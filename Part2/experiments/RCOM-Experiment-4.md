# Step 2

### GTKTerm

```sh
enable
8nortel

show vlan brief
```

# Step 3

### Tux 44

```
ifconfig eth1 up
ifconfig eth1 172.16.41.253/24
ifconfig
```

### GTKTerm 
```
configure terminal
interface fastethernet 0/4
switchport mode access
switchport access vlan 41
end

show running config interface fastethernet 0/4
show interfaces fastethernet 0/4 switchport
```

# Step 4

### Tux 44

```sh
echo 1 > /proc/sys/net/ipv4/ip_forward
echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_broadcasts
```

# Step 5

### Tux 44
```
ifconfig
```
 Take ip and mac addresses of eth0 eth1

# Step 6

### Tux 42

```
route add -net 172.16.40.0/24 gw 172.16.41.253
```

### Tux 43
```
route add -net 172.16.41.0/24 gw 172.16.40.254 
```

# Step 7

### All Tuxes
```
route -n
```

# Step 8

### Tux 43
Start Capture

# Step 9

### Tux 43
```
ping 172.16.40.254
ping 172.16.41.253
ping 172.16.41.1
```

# Step 10 

### Tux 43
Stop capture save logs
 
# Step 11

### Tux 44
Start capture on eth0 eth1

# Step 12

### All tuxes
```
arp -d <ip>

```

# Step 13

### Tux 43
```
ping 172.16.41.1
```

# Step 14

### Tux 44
Stop capture and save logs

# Step 15

### Tux 44
Filter log with 
```
frame.interface_id == <id>
```

---------------------------

# CISCO ROUTER

## Step 1

GE0 <-> Lab Router
## Step 2. 
GE1 <-> Switch
  
##  Step 3 
### GTKterm
```sh
enable
8nortel

configure terminal
interface fastethernet 0/5
switchport mode access
switchport access vlan 41
end

show running config interface fastethernet 0/5
show interfaces fastethernet 0/5 switchport
```

## Step 4
### GTKTerm
```sh
enable
8nortel

show vlan brief
```

## Step 1,2,3,4,1,2,3
```
router >enable
configure terminal

interface gigabitethernet 0/0 
ip address 172.16.41.254 255.255.255.0
no shutdown
ip nat inside
exit

interface gigabitethernet 0/1
ip address 172.16.1.49 255.255.255.0
no shutdown
ip nat outside
exit

ip nat pool ovrld 172.16.1.49 172.16.1.49 prefix 24
ip nat inside source list 1 pool ovrld overload

access-list 1 permit 172.16.40.0 0.0.0.7
access-list 1 permit 172.16.41.0 0.0.0.7

ip route 0.0.0.0 0.0.0.0 172.16.1.254
ip route 172.16.40.0 255.255.255.0 172.16.41.253
end
```
## Step 4
```
show running-config
```

## Step 5
```
router > enable
configure terminal
copy running-config startup-config
```

## Step 1
```
router > enable
ping 172.16.40.1
ping 172.16.40.254
ping 172.16.41.253
ping 172.16.41.1
```

## Step 2
```
router > enable
ping 172.16.1.254
```

## Step 3
```
router > enable
ping 104.17.113.188
```

# Configure tux42, tux43 and tux44

## Step 1

### Tux42
```
route add default gw 172.16.41.254
```

### Tux43
```
route add default gw 172.16.40.254
```

### Tux44
```
route add default gw 172.16.41.254
```

## Step 2

### Tux 43
```
ping 172.16.1.254
```

## Step 3

### Tux 43
```
ping 104.17.113.188
```

# Copiar as configurações 

### Switch e Router
```
configure terminal
copy running-config flash:6-5
``` 





