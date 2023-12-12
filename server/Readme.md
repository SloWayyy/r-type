# Network Protocol

Communication protocols are essential in the field of computer networks to enable the exchange of information between different devices. Two widely used protocols in this context are TCP (Transmission Control Protocol) and UDP (User Datagram Protocol). You will see below the differences between these two protocols and how they work.


## TCP (Transmission Control Protocol)

<details>
  <summary><code>TCP</code></summary>
  <p>TCP is a connection-oriented and reliable protocol. It is often used in situations where data loss is not acceptable, such as file transfer or web browsing.</p>
  <p>In the R-Type project, we use TCP to keep a connection between the client and the server. With that, if the client is disconnected, the server will be notified and will be able to remove the client from the game.
</p>
</details>

## UDP (User Datagram Protocol)

<details>
  <summary><code>UDP</code></summary>
  <p>UDP is a connectionless and unreliable protocol. It is often used in situations where data loss is acceptable, such as video streaming or online gaming. (like our project)</p>
  <p>UDP is a connectionless and unreliable protocol. It is often used in situations where data loss is acceptable, such as video streaming or online gaming.</p>
</details>

## Communication


All the game information is sent through the network using a protocol that we created. This protocol is composed of 3 parts:

- <summary><code>Entity ID</code></summary>
  <p>The Entity ID is a unique identifier for each entity in the game. It is used to identify the entity that will receive the component.</p>

- <summary><code>type_index</code></summary>
  <p>The type_index is an index that represents the type of the component. It is used to know which component will be sent.</p>
- <summary><code>Component</code></summary>
  <p>The component is the data that will be sent. It is used to update the entity on the client side.</p>

```cpp
struct Packet {
    uint32_t entity_id;
    uint8_t[8] type_index;
    sizeof(struct component) component;
};
```