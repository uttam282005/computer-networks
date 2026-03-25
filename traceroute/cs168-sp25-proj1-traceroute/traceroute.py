from socket import AF_INET
import util

# Your program should send TTLs in the range [1, TRACEROUTE_MAX_TTL] inclusive.
# Technically IPv4 supports TTLs up to 255, but in practice this is excessive.
# Most traceroute implementations cap at approximately 30.  The unit tests
# assume you don't change this number.
TRACEROUTE_MAX_TTL = 30
TIME_EXCEEDED = 11
MAX_ROUTERS_COUNT = 4
DESTINATION_UNREACHABLE = 3

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

    def __init__(self, buffer: bytes):
        pass  # TODO

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

    def __init__(self, buffer: bytes):
        pass  # TODO

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

    def __init__(self, buffer: bytes):
        pass  # TODO

    def __str__(self) -> str:
        return (
            f"UDP (src_port {self.src_port}, dst_port {self.dst_port}, "
            + f"len {self.len}, cksum 0x{self.cksum:x})"
        )


def parse_ip_header(packet) -> IPv4:
    ip = IPv4(b'')

    version_ihl = packet[0]
    ip.version = version_ihl >> 4
    ihl = version_ihl & 0x0F
    ip.header_len = ihl * 4

    ip.tos = packet[1]
    ip.length = int.from_bytes(packet[2:4], 'big')
    ip.id = int.from_bytes(packet[4:6], 'big')

    flags_fragment = int.from_bytes(packet[6:8], 'big')
    ip.flags = (flags_fragment >> 13) & 0x7
    ip.frag_offset = flags_fragment & 0x1FFF

    ip.ttl = packet[8]
    ip.proto = packet[9]
    ip.cksum = int.from_bytes(packet[10:12], 'big')

    ip.src = util.inet_ntop(AF_INET, packet[12:16])
    ip.dst = util.inet_ntop(AF_INET, packet[16:20])

    return ip


def parse_icmp_header(packet) -> ICMP:
    icmp = ICMP(b'')

    icmp.type = packet[0]
    icmp.code = packet[1]
    icmp.cksum = int.from_bytes(packet[2:4], 'big')

    return icmp

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
        routers = []
        sendsock.set_ttl(ttl)
        for _ in range(PROBE_ATTEMPT_COUNT + 1):
            sendsock.sendto(b"hello", (ip, TRACEROUTE_PORT_NUMBER))
            can_receive = recvsock.recv_select()
            if can_receive:
                packet, _ = recvsock.recvfrom()
                version_ihl = packet[0]
                ihl = version_ihl & 0xf
                ihl = ihl * 4

                ip_header = parse_ip_header(packet)

                if ip_header.proto == 1:
                    icmp_header = parse_icmp_header(packet[ihl: ihl + 8])

                    if icmp_header.type == TIME_EXCEEDED:
                        routers.append(ip_header.src)

                    if icmp_header.type == DESTINATION_UNREACHABLE:
                        print("destination reached")
                        routers.append(ip_header.src)
                        all_routers.append(routers)
                        return all_routers
                break

        all_routers.append(routers)
        util.print_result(routers, ttl)
    return all_routers

if __name__ == "__main__":
    args = util.parse_args()
    ip_addr = util.gethostbyname(args.host)
    print(f"traceroute to {args.host} ({ip_addr})")
    traceroute(util.Socket.make_udp(), util.Socket.make_icmp(), ip_addr)
