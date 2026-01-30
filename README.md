# Computer Networks Mastery Roadmap - Complete Edition

## Phase 1: Socket Programming Fundamentals (Weeks 1-2)

### Week 1: Raw Sockets & Basic Communication

**Resources:**
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) - Read sections 1-7
- Man pages: `man socket`, `man tcp`, `man ip`

**Projects:**

**Project 1.1: Echo Server & Client**
- Build TCP echo server that sends back whatever it receives
- Build client that connects and sends messages
- Requirements:
  - Handle multiple clients (fork/thread per client)
  - Graceful shutdown on Ctrl+C
  - Error handling for all syscalls
- Test: Connect 10+ clients simultaneously

**Project 1.2: Simple Chat Application**
- Multi-client chat room
- Server broadcasts messages to all connected clients
- Requirements:
  - Username support
  - Join/leave notifications
  - Private messaging (bonus)
- Test: Chat between 3+ terminals

**Project 1.3: File Transfer (FTP Clone)**
- Client uploads/downloads files to/from server
- Requirements:
  - Binary file support
  - Progress indicator
  - Resume capability (bonus)
- Test: Transfer 100MB+ files successfully

**Deliverables:**
- 3 working programs in C
- You understand: sockets, bind, listen, accept, connect, send, recv
- You can explain what file descriptors are

---

### Week 2: Packet Analysis & Network Tools

**Resources:**
- [Wireshark Tutorial](https://www.wireshark.org/docs/wsug_html_chunked/)
- [Packet Life - Cheat Sheets](http://packetlife.net/library/cheat-sheets/)

**Projects:**

**Project 2.1: Packet Sniffer**
- Capture raw packets on interface
- Parse and display: Ethernet, IP, TCP/UDP headers
- Requirements:
  - Use `AF_PACKET` sockets (Linux) or `pcap` library
  - Display source/dest IPs, ports, protocols
  - Filter by protocol type
- Test: Capture traffic while browsing websites

**Project 2.2: Network Analysis Exercises**
- Capture traffic for:
  - Your chat app from Week 1
  - HTTP request to example.com
  - DNS query
  - SSH connection
- For each: Document every packet exchanged with screenshots

**Project 2.3: Basic Port Scanner**
- Scan ports on localhost
- Three methods: TCP connect, SYN scan (raw sockets), UDP scan
- Requirements:
  - Scan range of ports (e.g., 1-1024)
  - Multi-threaded for speed
  - Output: open/closed/filtered status
- Test: Scan your own services

**Deliverables:**
- Working packet sniffer that displays all headers
- Wireshark captures with annotations explaining each packet
- Port scanner that finds your running services

---

## Phase 2: Build Your Own TCP/IP Stack (Weeks 3-6)

### Week 3: Data Link Layer (Ethernet & ARP)

**Resources:**
- [Let's Code TCP/IP Stack - Part 1 (Ethernet & ARP)](https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/)
- RFC 826 - ARP
- [Linux TUN/TAP Documentation](https://www.kernel.org/doc/Documentation/networking/tuntap.txt)

**Projects:**

**Project 3.1: Setup TUN Device**
- Create virtual network interface using TUN/TAP
- Requirements:
  - Write code to read/write packets from TUN device
  - Configure IP address on the interface
  - Verify with `ip addr` and `ping`

**Project 3.2: Ethernet Frame Parser**
- Read frames from TUN device
- Parse: destination MAC, source MAC, EtherType
- Display in human-readable format
- Test: Send packets to your TUN interface

**Project 3.3: ARP Implementation**
- Implement ARP request/reply
- Requirements:
  - Build and send ARP requests
  - Parse ARP replies
  - Maintain ARP cache
  - Respond to ARP requests for your IP
- Test: Ping your TUN interface from another terminal

**Deliverables:**
- TUN device that responds to ARP requests
- Working ARP cache
- You can explain Ethernet frames byte-by-byte

---

### Week 4: Network Layer (IP & ICMP)

**Resources:**
- [Let's Code TCP/IP Stack - Part 2 (IPv4 & ICMPv4)](https://www.saminiir.com/lets-code-tcp-ip-stack-2-ipv4-icmpv4/)
- RFC 791 - IP
- RFC 792 - ICMP

**Projects:**

**Project 4.1: IP Packet Parser**
- Parse IP headers from your TUN device
- Extract: version, header length, TTL, protocol, source/dest IP, checksum
- Validate checksum
- Test: Send IP packets to your interface

**Project 4.2: IP Packet Builder**
- Create IP packets from scratch
- Requirements:
  - Calculate and set checksum
  - Handle options field
  - Set appropriate TTL
- Test: Build and send valid IP packets

**Project 4.3: ICMP Echo (Your Own Ping)**
- Implement ping using ICMP
- Requirements:
  - Send ICMP echo request
  - Wait for echo reply
  - Calculate round-trip time
  - Handle timeouts
  - Display statistics (min/max/avg RTT, packet loss)
- Test: Ping real hosts like 8.8.8.8

**Project 4.4: IP Fragmentation**
- Implement IP fragmentation and reassembly
- Requirements:
  - Fragment large packets into MTU-sized pieces
  - Reassemble fragments at receiver
  - Handle out-of-order fragments
- Test: Send 5000-byte packets

**Deliverables:**
- Working ping implementation
- IP stack that handles fragmentation
- Can explain IP header fields from memory

---

### Week 5: Transport Layer Part 1 (UDP)

**Resources:**
- [Let's Code TCP/IP Stack - Part 3 (UDP)](https://www.saminiir.com/lets-code-tcp-ip-stack-3-udp/)
- RFC 768 - UDP

**Projects:**

**Project 5.1: UDP Implementation**
- Add UDP layer to your stack
- Requirements:
  - Parse UDP headers
  - Build UDP packets
  - Calculate checksum (pseudo-header + UDP)
  - Handle port demultiplexing
- Test: Send UDP packets between two instances

**Project 5.2: DNS Client**
- Build DNS resolver using your UDP stack
- Requirements:
  - Construct DNS queries (A records)
  - Parse DNS responses
  - Handle multiple queries
  - Support recursive resolution (bonus)
- Test: Resolve google.com, github.com

**Project 5.3: TFTP Client**
- Implement TFTP (Trivial File Transfer Protocol)
- Requirements:
  - Upload files
  - Download files
  - Handle packet loss (retransmissions)
- Test: Transfer files to/from real TFTP server

**Deliverables:**
- Working UDP implementation
- DNS resolver that works
- Understanding of why UDP is simpler than TCP

---

### Week 6: Transport Layer Part 2 (TCP Basics)

**Resources:**
- [Let's Code TCP/IP Stack - Part 4 (TCP Basics)](https://www.saminiir.com/lets-code-tcp-ip-stack-4-tcp-basics/)
- RFC 793 - TCP (sections 1-3)
- TCP/IP Illustrated Vol 1 - Chapter 17

**Projects:**

**Project 6.1: TCP Parser**
- Parse TCP segments
- Extract: ports, seq num, ack num, flags, window, checksum
- Validate checksum
- Test: Capture real TCP traffic and parse it

**Project 6.2: Three-Way Handshake**
- Implement TCP connection establishment
- Requirements:
  - Client: send SYN, wait for SYN-ACK, send ACK
  - Server: listen, respond to SYN with SYN-ACK, wait for ACK
  - Handle timeouts and retransmissions
  - Track connection state (CLOSED, SYN_SENT, ESTABLISHED, etc.)
- Test: Establish connection between client and server

**Project 6.3: Basic Data Transfer**
- Send data over established TCP connection
- Requirements:
  - Send data with correct sequence numbers
  - Acknowledge received data
  - Handle cumulative ACKs
  - No flow control or congestion control yet
- Test: Transfer small text messages

**Project 6.4: Connection Teardown**
- Implement four-way handshake (FIN-ACK)
- Requirements:
  - Handle active and passive close
  - Implement TIME_WAIT state
  - Clean up connection state
- Test: Gracefully close connections

**Deliverables:**
- TCP stack that can establish, transfer data, and close connections
- State machine diagram of your TCP implementation
- Working simple data transfer (even if unreliable)

---

## Phase 3: Advanced TCP & Application Protocols (Weeks 7-10)

### Week 7: TCP Reliability & Flow Control

**Resources:**
- RFC 793 - TCP (sections 3.7, 3.8)
- TCP/IP Illustrated Vol 1 - Chapter 20
- [TCP Congestion Control](https://ee.lbl.gov/papers/congavoid.pdf) - Original paper

**Projects:**

**Project 7.1: Sliding Window Protocol**
- Implement sender and receiver windows
- Requirements:
  - Send multiple segments without waiting for ACKs
  - Track unacknowledged data
  - Handle cumulative acknowledgments
  - Implement receive buffer
- Test: Transfer data with 10% packet loss

**Project 7.2: Retransmission Logic**
- Implement RTO (Retransmission Timeout)
- Requirements:
  - Exponential backoff
  - Karn's algorithm for RTT estimation
  - Fast retransmit (3 duplicate ACKs)
- Test: Transfer data with simulated packet loss

**Project 7.3: Congestion Control**
- Implement basic congestion control
- Requirements:
  - Slow start
  - Congestion avoidance
  - Fast recovery (bonus)
  - Track cwnd (congestion window)
- Test: Measure throughput over lossy links

**Project 7.4: Reliable File Transfer**
- Build FTP-like application using your TCP stack
- Requirements:
  - Transfer 100MB+ files reliably
  - No corruption (verify with checksums)
  - Reasonable performance
- Test: Transfer files over simulated network with 5% packet loss, 100ms latency

**Deliverables:**
- TCP stack with full reliability guarantees
- Performance benchmarks comparing your stack to Linux TCP
- Can explain slow start vs congestion avoidance

---

### Week 8: HTTP/1.1 Server

**Resources:**
- RFC 2616 - HTTP/1.1 (or RFC 7230-7235 for updated spec)
- [MDN HTTP Documentation](https://developer.mozilla.org/en-US/docs/Web/HTTP)

**Projects:**

**Project 8.1: Basic HTTP Server**
- Build HTTP/1.1 server from scratch (use OS sockets, not your stack)
- Requirements:
  - Parse HTTP requests (method, URI, headers, body)
  - Serve static files
  - Return proper status codes (200, 404, 500, etc.)
  - Handle GET and POST methods
- Test: Access from browser, curl, wget

**Project 8.2: HTTP Features**
- Add more HTTP/1.1 features
- Requirements:
  - Persistent connections (Connection: keep-alive)
  - Chunked transfer encoding
  - Range requests (for video streaming)
  - Multiple MIME types
  - Query parameters and URL parsing
- Test: Stream video file, download large files

**Project 8.3: Dynamic Content**
- Serve dynamic content
- Requirements:
  - CGI-style execution (run scripts, return output)
  - JSON API endpoints
  - Form handling (POST data)
  - Cookies (set and parse)
- Test: Build simple web app (todo list, counter, etc.)

**Project 8.4: Performance & Concurrency**
- Handle many concurrent connections
- Requirements:
  - Thread pool (pre-spawned threads)
  - epoll/kqueue for I/O multiplexing
  - Non-blocking I/O
  - Load testing with 1000+ concurrent connections
- Test: Use `wrk` or `ab` to benchmark

**Deliverables:**
- HTTP server that works with real browsers
- Serves your personal website or simple web app
- Can handle 1000+ req/sec on your machine

---

### Week 9: HTTP/2

**Resources:**
- RFC 7540 - HTTP/2
- [HTTP/2 Explained](https://http2-explained.haxx.se/)
- [HTTP/2 Demo](https://http2.akamai.com/demo)

**Projects:**

**Project 9.1: HTTP/2 Basics**
- Add HTTP/2 support to your server
- Requirements:
  - Binary framing layer
  - HPACK header compression
  - Stream prioritization
  - Flow control at stream level
- This is HARD - consider using a library for framing and focus on understanding

**Project 9.2: Server Push**
- Implement server push
- Requirements:
  - Push CSS/JS when HTML is requested
  - Detect what to push from HTML parsing
- Test: Compare page load with/without push

**Project 9.3: Multiplexing Demo**
- Build demo showing HTTP/2 vs HTTP/1.1
- Requirements:
  - Page with 100 small resources
  - Measure load time for both protocols
  - Visualize waterfall
- Test: Demonstrate multiplexing benefits

**Deliverables:**
- HTTP/2 server (even if basic)
- Performance comparison: HTTP/1.1 vs HTTP/2
- Deep understanding of why HTTP/2 is faster

---

### Week 10: TLS/HTTPS

**Resources:**
- [TLS 1.3 RFC 8446](https://tools.ietf.org/html/rfc8446)
- [High Performance Browser Networking - TLS](https://hpbn.co/transport-layer-security-tls/)
- OpenSSL documentation

**Projects:**

**Project 10.1: TLS Wrapper**
- Add HTTPS support using OpenSSL
- Requirements:
  - Generate self-signed certificates
  - TLS handshake
  - Encrypted communication
  - Certificate validation (client-side)
- Test: Access https://localhost from browser

**Project 10.2: Certificate Management**
- Proper certificate handling
- Requirements:
  - Use Let's Encrypt certificates
  - Implement certificate chain validation
  - Handle certificate expiry
  - SNI (Server Name Indication) support
- Test: Deploy on VPS with real domain

**Project 10.3: TLS Analysis**
- Deep dive into TLS
- Requirements:
  - Capture TLS handshake in Wireshark
  - Identify each message (ClientHello, ServerHello, etc.)
  - Understand cipher suites
  - Document session resumption
- Deliverable: Annotated packet capture

**Deliverables:**
- HTTPS server with valid certificates
- Can explain TLS handshake from memory
- Understand performance implications of TLS

---

## Phase 4: Advanced Topics (Weeks 11-14)

### Week 11: Concurrency Patterns

**Resources:**
- [The C10K Problem](http://www.kegel.com/c10k.html)
- Linux man pages: epoll, select, poll
- [libuv Design Overview](http://docs.libuv.org/en/v1.x/design.html)

**Projects:**

**Project 11.1: Thread-Per-Connection Server**
- HTTP server using thread per connection
- Requirements:
  - Thread pool with queue
  - Worker threads
  - Load balancing across threads
- Benchmark: Find maximum concurrent connections

**Project 11.2: I/O Multiplexing Server**
- Rewrite server using epoll (Linux) or kqueue (BSD/Mac)
- Requirements:
  - Single-threaded event loop
  - Non-blocking sockets
  - Handle thousands of connections
- Benchmark: Compare to threaded version

**Project 11.3: Hybrid Model**
- Combine threads + epoll
- Requirements:
  - Multiple event loops (one per thread)
  - Load balancing (SO_REUSEPORT)
- Benchmark: Find optimal thread count for your CPU

**Project 11.4: Performance Analysis**
- Profile all three implementations
- Requirements:
  - CPU usage
  - Memory usage
  - Latency (p50, p95, p99)
  - Throughput
  - Context switches
- Tools: `perf`, `strace`, `htop`

**Deliverables:**
- Three server implementations
- Performance report with graphs
- Recommendation for when to use each pattern

---

### Week 12: Application Protocols

**Resources:**
- RFC 5321 - SMTP
- RFC 959 - FTP
- RFC 6455 - WebSocket
- RFC 4253 - SSH (for understanding, don't implement)

**Projects:**

**Project 12.1: SMTP Client**
- Send emails programmatically
- Requirements:
  - Connect to SMTP server
  - AUTH LOGIN
  - Send with attachments
  - HTML emails
- Test: Send email to yourself

**Project 12.2: WebSocket Server**
- Add WebSocket support to HTTP server
- Requirements:
  - Handshake upgrade from HTTP
  - Frame parsing/building
  - Handle ping/pong
  - Broadcast to multiple clients
- Test: Build real-time chat or game

**Project 12.3: FTP Server**
- Build FTP server from spec
- Requirements:
  - USER, PASS, LIST, RETR, STOR commands
  - Active and passive mode
  - Directory navigation
- Test: Connect with FileZilla

**Pick one more:**
- IRC server
- Redis protocol (RESP)
- MQTT broker
- Custom protocol of your choice

**Deliverables:**
- At least 3 working protocol implementations
- Can read RFC and implement any protocol
- Understanding of protocol design principles

---

### Week 13: Network Tools & Diagnostics

**Resources:**
- Man pages: tcpdump, traceroute, ss, iptables
- [Linux Network Administrator's Guide](https://tldp.org/LDP/nag2/index.html)

**Projects:**

**Project 13.1: Traceroute Clone**
- Implement traceroute
- Requirements:
  - Send UDP packets with increasing TTL
  - Parse ICMP Time Exceeded messages
  - Display hop-by-hop path
  - DNS reverse lookup for IPs
- Test: Trace route to google.com

**Project 13.2: Advanced Port Scanner**
- Professional-grade port scanner
- Requirements:
  - SYN scan (stealth)
  - Service version detection
  - OS fingerprinting (basic)
  - Scan timing options
  - Output in multiple formats
- Test: Scan your own network

**Project 13.3: Packet Analyzer Tool**
- Build comprehensive analyzer
- Requirements:
  - Live capture + PCAP file reading
  - Protocol hierarchy statistics
  - Conversation tracking (TCP streams)
  - Export capabilities
  - Display filters
- Test: Analyze captured traffic from various apps

**Project 13.4: Network Monitor**
- Real-time network monitoring
- Requirements:
  - Bandwidth usage per process
  - Connection tracking
  - Alert on suspicious activity
  - Historical data + graphs
- Test: Monitor your own system

**Deliverables:**
- Suite of network tools
- Can debug any network issue
- Comfortable reading packet captures

---

### Week 14: Load Balancing & Proxying

**Resources:**
- [HAProxy Documentation](http://www.haproxy.org/)
- [Nginx Architecture](https://www.aosabook.org/en/nginx.html)

**Projects:**

**Project 14.1: Simple Load Balancer**
- Layer 7 (HTTP) load balancer
- Requirements:
  - Round-robin distribution
  - Health checks
  - Connection pooling to backends
  - Support for 100+ concurrent connections
- Test: Load balance between 3 backend servers

**Project 14.2: HTTP Proxy**
- Forward and reverse proxy
- Requirements:
  - Forward: client -> proxy -> internet
  - Reverse: client -> proxy -> backend servers
  - Caching (bonus)
  - Request/response modification
- Test: Use as system proxy

**Project 14.3: SOCKS Proxy**
- SOCKS5 proxy server
- Requirements:
  - Support TCP and UDP
  - Authentication
  - IPv4 and IPv6
- Test: Configure browser to use SOCKS proxy

**Project 14.4: Load Testing**
- Test your load balancer under stress
- Requirements:
  - Use `wrk` to generate 10k+ req/sec
  - Measure latency distribution
  - Test failover scenarios
  - Document bottlenecks
- Optimize and re-test

**Deliverables:**
- Working load balancer handling real traffic
- Performance benchmarks
- Understanding of production load balancing

---

## Phase 5: Real-World Systems (Weeks 15-16)

### Week 15: VPN Implementation

**Resources:**
- [WireGuard Paper](https://www.wireguard.com/papers/wireguard.pdf)
- [OpenVPN Protocol](https://openvpn.net/community-resources/reference-manual-for-openvpn-2-4/)
- TUN/TAP documentation

**Projects:**

**Project 15.1: Simple VPN**
- Build basic VPN tunnel
- Requirements:
  - TUN device on both sides
  - TCP or UDP tunnel between them
  - Encrypt traffic (use existing crypto library)
  - Route all traffic through tunnel
- Test: Browse internet through your VPN

**Project 15.2: VPN Features**
- Add production features
- Requirements:
  - Authentication
  - IP address assignment (DHCP-like)
  - DNS configuration
  - Reconnection handling
  - Kill switch
- Test: Use for actual browsing

**Project 15.3: Performance Optimization**
- Make it fast
- Requirements:
  - Minimize overhead
  - UDP vs TCP comparison
  - Benchmark throughput and latency
  - Compare to WireGuard/OpenVPN
- Document: Performance characteristics

**Deliverables:**
- Working VPN you can actually use
- Performance analysis vs commercial VPNs
- Deep understanding of tunneling

---

### Week 16: Capstone Project

**Pick ONE large project:**

**Option A: Distributed Chat System**
- Requirements:
  - Multiple servers with message routing
  - Persistent message storage
  - Presence detection
  - File transfers
  - E2E encryption
  - Mobile-friendly web interface

**Option B: Content Delivery Network (CDN) Simulator**
- Requirements:
  - Multiple cache servers
  - Origin server
  - Consistent hashing for cache distribution
  - Cache invalidation
  - Bandwidth optimization
  - Load testing with 10k+ req/sec

**Option C: Custom Protocol & Application**
- Design your own protocol for a specific use case
- Requirements:
  - Full RFC-style specification
  - Client and server implementation
  - Performance benchmarks
  - Security analysis
  - Compare to alternatives

**Option D: Network Packet Analysis Platform**
- Requirements:
  - Capture engine
  - Real-time analysis
  - Protocol dissectors for 10+ protocols
  - Anomaly detection
  - Web dashboard for visualization
  - Export and reporting

**Deliverables:**
- Production-quality implementation
- Documentation (architecture, API, deployment)
- Performance testing results
- GitHub repo with clean code
- Blog post explaining what you built

---

## Evaluation Checklist

You've mastered computer networks when you can:

### Technical Skills
- [ ] Build a functional HTTP server from scratch in a weekend
- [ ] Implement any RFC-specified protocol
- [ ] Debug network issues using packet captures
- [ ] Explain every byte in a TCP/IP packet
- [ ] Optimize network application performance
- [ ] Design custom protocols for specific needs

### Conceptual Understanding
- [ ] Explain why TCP is slower than UDP (with nuance)
- [ ] Describe how congestion control prevents network collapse
- [ ] Understand security implications of network design
- [ ] Know when to use UDP vs TCP vs QUIC
- [ ] Explain HTTP/2 multiplexing vs HTTP/1.1 pipelining
- [ ] Understand NAT, firewalls, and middleboxes

### Practical Abilities
- [ ] Can read Wireshark captures like reading English
- [ ] Debug production network issues
- [ ] Build secure network applications
- [ ] Optimize for latency vs throughput
- [ ] Handle errors and edge cases properly
- [ ] Write production-quality network code

---

## Daily Workflow

### Every Day (1-2 hours minimum)
1. **Code first** (60-90 mins): Work on current project
2. **Read when stuck** (15-30 mins): Reference RFCs, book chapters
3. **Capture & analyze** (10 mins): Wireshark analysis of something you use

### Every Week
- Complete all projects for that week
- Code review: Read your code from 2 weeks ago, refactor
- Read one RFC (even if just skimming)

### Every Project
- Write README explaining what you built
- Add to GitHub portfolio
- Test it breaks in 5 different ways, fix them

---

## Tools You'll Need

**Required:**
- C compiler (gcc/clang)
- Wireshark
- Text editor / IDE
- Linux (native or VM - WSL2 works)

**Recommended:**
- `tcpdump`
- `nmap`
- `wrk` or `ab` (HTTP benchmarking)
- `iperf3` (network performance)
- `strace` / `ltrace`
- `perf` (profiling)
- `valgrind` (memory debugging)

**Optional:**
- Python (for quick prototypes)
- Go (for modern network tools)
- Rust (for learning systems programming)

---

## Time Commitment

**Minimum effective dose:** 2 hours/day, 6 days/week
- **Total time:** 4 months (16 weeks × 12 hours = ~192 hours)

**Accelerated:** 4 hours/day, 6 days/week
- **Total time:** 2 months

**Hardcore:** 6-8 hours/day
- **Total time:** 1-1.5 months

**Reality check:** This is 150-200 hours of actual building. Budget accordingly.
