# requirements
- connectivity 
- unicast 
- multicast
- broadcast 
- scalability 

## different network perspectives-
- consumer of network -> needs fast and consistent experience
- application developer ->  a guarantee that each message the application sends will be delivered without error within a certain amount of time or the ability to switch gracefully among different connections to the network as the user moves around.
- network manager -> easy to maintain, expand (modular)

## 1. scalable connectivity
- A system that is designed to support growth to an arbitrarily large size is said to scale

- at the lowest level, connectivity can be physical 
where two or more computers (nodes) are connected to each other by a physical medium (called link)

## switched network

![switched network](https://book.systemsapproach.org/_images/f01-03-9780123850591.png) 

switched networks solves the problem of unmanagable wires coming out from a node 
by providing indirect connection between two nodes via switches.

### two types of switches
- circuit switches 
- packet switches

#### circuit switches
first a dedicated circuit is established between the host and destination node.
and then a stream of bits is sent over the network. 

#### packet switches 
store and forward strategy - 
each node receives the complete packet and store it in it's memory and then forwards 
the complete packet to the next node.

*packet switches are prefered in computer networks due to performance and efficiency reason.*

- every node has a unique address in a network.
> the process of finding a path to forward data from src to destination node is called routing.

> unicast: a node wants to send data to a single node.

> broadcast: a node wants to send data to all the nodes in the network.

> multicast: sending data to a subset of nodes.

## 2. cost effective resource sharing

### problems
- how can all the hosts communicate with each other at the same time?
- how can two or more hosts share the same physical link, if they want to use it at the same time?

## multiplexing
- sharing of system resources among multiple users.
- multiplexing multiple logical flows over a single link
![multiplexing and demultiplexing](https://book.systemsapproach.org/_images/f01-05-9780123850591.png)
### types of multiplexing
- synchronous time division multiplexing (stdm)
- frequency division multiplexing (fdm)



#### stdm
- time is divided into fixed size quantums
- each flow runs sequentially in a round robin fashion for a fixed quanta 

#### ftm 
- data in each flow is sent on a different frequency
- like radio or tv broadcasts

**problems with both**
what if a s1 doesn't have data to send.
others still have to wait for their turn.
network becomes idle.

#### solution 

#### statistical multiplexing
- similar to stdm but a flow is allowed to send data only if requested and it has data to send.
- to make sure that all the flows get their turn to send data, data is sent in the form of fixed size packets.
- a flow can send only a single packet of data a given time.
- whose packet will be sent next is decided by the switch (can be round robin or fifo).

> a network that attempts to allocate bandwidth to particular flows is said to support *quality of service* (QoS) .

- what if switch is receiving more data than the link can handle?
*switch will buffer the packet in it's own memory*
- what if buffer space is full?
*packet loss* (congestion)

## 3. support for common services
- extract out some common services needed by the applications
- implement them via logical channels

**how to choose channels?**
ask
- do the packets needs to be protected over the connection?
- should packets arrive in a certain order?
- is packet loss acceptable?

### identify common communication patterns

example-
a digital library might want to support a **request/reply channel**
- each message which is sent, must be received.
- exactly one copy of the messages should be delivered.
- security and privacy

a videoconferencing tool might want a **message channel**-
- packets needs to be in a certain order.
- packet loss is acceptable.
- privacy and security.
- multicasting.

## 4. reliable message delivery
- hardware and software problems can introduce errors in the data.
- detect and correct these errors.
- how to distinguish if a packet is indeed lost or just slow to arrive because it was pushed into an incorrect route?
- how to distinguish between a slow node or crashed node?

## 5. manageability
- easy to expand
- easy to identify problems
- easy to rectify
