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

> packet switches are prefered in computer networks due to performance and efficiency reason.

- every node has a unique address in a network.
> the process of finding a path to forward data from src to destination node is called routing.

> unicast: a node wants to send data to a single node.
> broadcast: a node wants to send data to all the nodes in the network.
> multicast: sending data to a subset of nodes.


