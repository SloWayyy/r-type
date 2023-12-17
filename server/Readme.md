# Network Protocol

Communication protocols are essential in the field of computer networks to enable the exchange of information between different devices. Two widely used protocols in this context are TCP (Transmission Control Protocol) and UDP (User Datagram Protocol). You will see below the differences between these two protocols and how they work.


## TCP (Transmission Control Protocol)

<details>
  <summary><code>TCP</code></summary>
  <p>TCP is a connection-oriented and reliable protocol. It is often used in situations where data loss is not acceptable, such as file transfer or web browsing.</p>
  <p>In the R-Type project, we use TCP to keep a connection between the client and the server. With that, if the client is disconnected, the server will be notified and will be able to remove the client from the game.
</p>
<img src="docs/TCP.png" alt="TCP Handshake">
</details>

## UDP (User Datagram Protocol)

<details>
  <summary><code>UDP</code></summary>
  <p>UDP is a connectionless and unreliable protocol. It is often used in situations where data loss is acceptable, such as video streaming or online gaming. (like our project)</p>
  <p>UDP is a connectionless and unreliable protocol. It is often used in situations where data loss is acceptable, such as video streaming or online gaming.</p>
  <img src="docs/UDP.png" alt="UDP Handshake">
</details>

## Communication


All the game information is sent through the network using a protocol that we created. This protocol is composed of 2 parts the packet and the component :

```cpp
struct Packet {
    uint32_t magic_number;
    PacketType packet_type;
    long timestamp;
    uint32_t entity_id;
    uint32_t type_index;
    std::array<char, 37> uuid;
};
```

<table style="width:100%">
  <tr>
    <th>Field</th>
    <th>Size</th>
    <th>Type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>magic_number</td>
    <td>uint32_t</td>
    <td>4 bytes</td>
    <td>It is used to check if the packet is valid.</td>
  </tr>
  <tr>
    <td>packet_type</td>
    <td>PacketType</td>
    <td>1 byte</td>
    <td>It is used to know the type of the packet.</td>
  </tr>
  <tr>
    <td>timestamp</td>
    <td>long</td>
    <td>8 bytes</td>
    <td>It is used to know when the packet was sent.</td>
  </tr>
  <tr>
    <td>entity_id</td>
    <td>uint32_t</td>
    <td>4 bytes</td>
    <td>It is used to know which entity will receive the component.</td>
  </tr>
  <tr>
    <td>type_index</td>
    <td>uint32_t</td>
    <td>4 bytes</td>
    <td>It is used to know which component will be sent.</td>
  </tr>
  <tr>
    <td>uuid</td>
    <td>std::array<char, 37></td>
    <td>37 bytes</td>
    <td>It is used to know which client will receive the component.</td>
  </tr>
</table>

### Packet Types

The following enum is used to know the type of the packet.
```cpp
enum PacketType {
    DATA_PACKET = '0',
    REPEAT_PACKET = '1',
    RESPONSE_PACKET = '2',
    NEW_CONNECTION = '3',
};
```
We have 4 types of packets:

<table style="width:100%">
  <tr>
    <th>Packet Type</th>
    <th>Value</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>DATA_PACKET</td>
    <td>'0'</td>
    <td>It is used to send a component to a client.</td>
  </tr>
  <tr>
    <td>REPEAT_PACKET</td>
    <td>'1'</td>
    <td>It is used to send the query again when the client does not receive the query.</td>
  </tr>
  <tr>
    <td>RESPONSE_PACKET</td>
    <td>'2'</td>
    <td>The client sends this packet to the server to notify that it has received the query.</td>
  </tr>
  <tr>
    <td>NEW_CONNECTION</td>
    <td>'3'</td>
    <td>It is used to notify the server that a new client has connected.</td>
  </tr>
</table>

### Routine

The server sends a packet to the client every 0.2 second.<br>
The client must respond to the server within 100ms.
If the client does not respond within 100ms, the server will send the packet again.<br>
But if the client receive a packet with a timestamp older than the last one, the client must ignore the packet because it is an old packet but the client must respond to the server with a RESPONSE_PACKET to notify that it has received the packet.