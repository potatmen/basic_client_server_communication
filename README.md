Python server is running on the __port 16011__ and first of all you need to start this python server (it can be changed by setting other value in __.py__ file in __line 64__).

After starting in you can connect to it by running __client.cpp__ like this: 
```
g++ client.cpp -o run

./run IP_ADDRESS PORT
```

__IP_ADDRESS__ replaced with IP of server, __PORT__ with port.

For example: 

```
./run 127.0.0.1 16011
```

In this case server is hosted __locally__.
