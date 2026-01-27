# Architecture
**why define a network architecture?**
- to satisfy the requirements defined in previous chapter.
- a blueprint to implement new protocols (channels/ abstractions).

## 1.3.1 Layering and protocols
layered netowrk system
![layered network system](https://book.systemsapproach.org/_images/f01-08-9780123850591.png)

- a network architecture consistes of many layers (abstractions)
- a single solves one part of the problem
- modular design 
- easy to add or delete layers
- process to process channels does not need to bother about host to host connectivity layer.
- there can be many abstractions in a single layer.
for example.
![layered system with alternative abstractions at a given layer](https://book.systemsapproach.org/_images/f01-09-9780123850591.png)

## protocols
- is an abstraction
- a protocal specification defines rules for communication
- exposes two interfaces
  - service interface (provides local services) 
  - peer to peer interface (for connections)
  ![service interfaces and peer to peer iterfaces](https://book.systemsapproach.org/_images/f01-10-9780123850591.png)
  
  **protocal graph**
  - defines interdependence
  ![an example of a protocal graph](https://book.systemsapproach.org/_images/f01-11-9780123850591.png)

## encapsulation
- each layer encapsulates the message from the upper layer
- each layer adds it's own headers to the information coming from above
- so that the services running on the destination machines software can take appropriate action

![encapsulation](https://book.systemsapproach.org/_images/f01-12-9780123850591.png)

## multiplexing and demultiplexing
- fundamental idea of packet switching
- each protocol implements a demux key 
- which is present in headers and helps in identifying to which applicatoin does this infomation belongs to

## OSI model

