For instance, I have a firewall at home. I have two static IPv4 addresses allocated to me by the DSL company,
and yet I have seven computers on the network. How is this possible? Two computers can’t share the same
IP address, or else the data wouldn’t know which one to go to!
The answer is: they don’t share the same IP addresses. They are on a private network with 24 million IP
addresses allocated to it. They are all just for me. Well, all for me as far as anyone else is concerned. Here’s
what’s happening:
If I log into a remote computer, it tells me I’m logged in from 192.0.2.33 which is the public IP address
my ISP has provided to me. But if I ask my local computer what its IP address is, it says 10.0.0.5. Who is
translating the IP address from one to the other? That’s right, the firewall! It’s doing NAT!
10.x.x.x is one of a few reserved networks that are only to be used either on fully disconnected networks,
or on networks that are behind firewalls. The details of which private network numbers are available for
you to use are outlined in RFC 19186 , but some common ones you’ll see are 10.x.x.x and 192.168.x.x,
where x is 0-255, generally. Less common is 172.y.x.x, where y goes between 16 and 31.
Networks behind a NATing firewall don’t need to be on one of these reserved networks, but they commonly
are
