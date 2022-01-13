# Step 1

### Tux 42

```
ifconfig eth0 up
ifconfig eth0 172.16.41.1/24
ifconfig
```
Register Ip and MAC addresses.

### Tux 43
```
ifconfig eth0 up
ifconfig eth0 172.16.40.1/24
```

### Tux 44
```
ifconfig eth0 up
ifconfig eth0 172.16.40.254/24
```

# Step 2

### GTKTerm

Enter configuration commands, one per line. End with CNTL/Z.

```sh
enable
8nortel 

#Create VLAN
configure terminal
vlan 40
end

show vlan id 40

#Add Port 1
configure terminal
interface fastethernet 0/1
switchport mode access
switchport access vlan 40
end

show running config interface fastethernet 0/1
show interfaces fastethernet 0/1 switchport

#Add Port 2
configure terminal
interface fastethernet 0/2
switchport mode access
switchport access vlan 40
end

show running config interface fastethernet 0/2
show interfaces fastethernet 0/2 switchport
```

# Step 3

### GTKTerm

Enter configuration commands, one per line. End with CNTL/Z.

```sh
enable 
nortel

#Create VLAN
configure terminal
vlan 41
end

show vlan id 41

#Add Port 3
configure terminal
interface fastethernet 0/3
switchport mode access
switchport access vlan 41
end

show running config interface fastethernet 0/3
show interfaces fastethernet 0/3 switchport
```

# Step 4

### Tux 43

 - Start Wireshark capture at eth0

# Step 5

### Tux 43
```sh
ping 172.16.41.1 #tux42
ping 172.16.40.254 #tux44
```

# Step 6

### Tux 43

 - Stop capture and save log

# Step 7

### Tux 42/Tux 43/Tux 44
- Start Captures of eth0 in tux42,tux43,tux44


# Step 8

### Tux 42
```
ping -b 172.16.41.255
```
### Tux 43
```
ping -b 172.16.40.255
```

# Step 9 
- Stop captures, save logs.




