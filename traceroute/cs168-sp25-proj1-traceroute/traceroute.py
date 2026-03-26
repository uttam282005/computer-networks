from socket import AF_INET
import util

# Your program should send TTLs in the range [1, TRACEROUTE_MAX_TTL] inclusive.
# Technically IPv4 supports TTLs up to 255, but in practice this is excessive.
# Most traceroute implementations cap at approximately 30.  The unit tests
# assume you don't change this number.
TRACEROUTE_MAX_TTL = 30
TIME_EXCEEDED = 11
DESTINATION_UNREACHABLE = 3
PORT_UNREACHABLE = 3
TTL_EXPIRED = 0

# Cisco seems to have standardized on UDP ports [33434, 33464] for traceroute.
# While not a formal standard, it appears that some routers on the internet
# will only respond with time exceeeded ICMP messages to UDP packets send to
# those ports.  Ultimately, you can choose whatever port you like, but that
# range seems to give more interesting results.
TRACEROUTE_PORT_NUMBER = 33434  # Cisco traceroute port number.

# Sometimes packets on the internet get dropped.  PROBE_ATTEMPT_COUNT is the
# maximum number of times your traceroute function should attempt to probe a
# single router before giving up and moving on.
PROBE_ATTEMPT_COUNT = 3


class IPv4:
    # Each member below is a field from the IPv4 packet header.  They are
    # listed below in the order they appear in the packet.  All fields should
    # be stored in host byte order.
    #
    # You should only modify the __init__() method of this class.
    version: int
    header_len: int  # Note length in bytes, not the value in the packet.
    tos: int  # Also called DSCP and ECN bits (i.e. on wikipedia).
    length: int  # Total length of the packet.
    id: int
    flags: int
    frag_offset: int
    ttl: int
    proto: int
    cksum: int
    src: str
    dst: str

    def __init__(self, packet: bytes):
        version_ihl = packet[0]
        self.version = version_ihl >> 4
        ihl = version_ihl & 0x0F
        self.header_len = ihl * 4

        self.tos = packet[1]
        self.length = int.from_bytes(packet[2:4], 'big')
        self.id = int.from_bytes(packet[4:6], 'big')

        flags_fragment = int.from_bytes(packet[6:8], 'big')
        self.flags = (flags_fragment >> 13) & 0x7
        self.frag_offset = flags_fragment & 0x1FFF

        self.ttl = packet[8]
        self.proto = packet[9]
        self.cksum = int.from_bytes(packet[10:12], 'big')

        self.src = util.inet_ntop(AF_INET, packet[12:16])
        self.dst = util.inet_ntop(AF_INET, packet[16:20])

    def __str__(self) -> str:
        return (
            f"IPv{self.version} (tos 0x{self.tos:x}, ttl {self.ttl}, "
            + f"id {self.id}, flags 0x{self.flags:x}, "
            + f"ofsset {self.frag_offset}, "
            + f"proto {self.proto}, header_len {self.header_len}, "
            + f"len {self.length}, cksum 0x{self.cksum:x}) "
            + f"{self.src} > {self.dst}"
        )


class ICMP:
    # Each member below is a field from the ICMP header.  They are listed below
    # in the order they appear in the packet.  All fields should be stored in
    # host byte order.
    #
    # You should only modify the __init__() function of this class.
    type: int
    code: int
    cksum: int

    def __init__(self, packet: bytes):
        self.type = packet[0]
        self.code = packet[1]
        self.cksum = int.from_bytes(packet[2:4], 'big')


    def __str__(self) -> str:
        return (
            f"ICMP (type {self.type}, code {self.code}, " + f"cksum 0x{self.cksum:x})"
        )


class UDP:
    # Each member below is a field from the UDP header.  They are listed below
    # in the order they appear in the packet.  All fields should be stored in
    # host byte order.
    #
    # You should only modify the __init__() function of this class.
    src_port: int
    dst_port: int
    len: int
    cksum: int

    def __init__(self, packet: bytes):
        self.src_port = int.from_bytes(packet[0:2], 'big')
        self.dst_port = int.from_bytes(packet[2:4], 'big')
        self.len = int.from_bytes(packet[4:6], 'big')
        self.cksum = int.from_bytes(packet[6:8], 'big')

    def __str__(self) -> str:
        return (
            f"UDP (src_port {self.src_port}, dst_port {self.dst_port}, "
            + f"len {self.len}, cksum 0x{self.cksum:x})"
        )

def traceroute(
    sendsock: util.Socket, recvsock: util.Socket, ip: str
) -> list[list[str]]:
    """Run traceroute and returns the discovered path.

    Calls util.print_result() on the result of each TTL's probes to show
    progress.

    Arguments:
    sendsock -- This is a UDP socket you will use to send traceroute probes.
    recvsock -- This is the socket on which you will receive ICMP responses.
    ip -- This is the IP address of the end host you will be tracerouting.

    Returns:
    A list of lists representing the routers discovered for each ttl that was
    probed.  The ith list contains all of the routers found with TTL probe of
    i+1.   The routers discovered in the ith list can be in any order.  If no
    routers were found, the ith list can be empty.  If `ip` is discovered, it
    should be included as the final element in the list.
    """

    all_routers = []
    for ttl in range(1, TRACEROUTE_MAX_TTL + 1):
        routers = set()
        sendsock.set_ttl(ttl)
        for _ in range(PROBE_ATTEMPT_COUNT + 1):
            sendsock.sendto(b"hello", (ip, TRACEROUTE_PORT_NUMBER))
            can_receive = recvsock.recv_select()
            if can_receive:
                packet, _ = recvsock.recvfrom()

                ip_header = IPv4(packet)
                if ip_header.proto == 1:
                    icmp_header = ICMP(packet[ip_header.header_len: ip_header.header_len + 8])

                    if icmp_header.type == TIME_EXCEEDED and icmp_header.code == TTL_EXPIRED:
                        routers.add(ip_header.src)

                    if icmp_header.type == DESTINATION_UNREACHABLE and icmp_header.code == PORT_UNREACHABLE:
                        print("destination reached")
                        routers.add(ip_header.src)
                        all_routers.append(list(routers))
                        return all_routers

        all_routers.append(routers)
        util.print_result(list(routers), ttl)
    return all_routers

if __name__ == "__main__":
    args = util.parse_args()
    ip_addr = util.gethostbyname(args.host)
    print(f"traceroute to {args.host} ({ip_addr})")
    traceroute(util.Socket.make_udp(), util.Socket.make_icmp(), ip_addr)
