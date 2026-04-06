"""
Your awesome Distance Vector router for CS 168

Based on skeleton code by:
  MurphyMc, zhangwen0411, lab352
"""

from collections import defaultdict
from os import eventfd_read
import sim.api as api
from cs168.dv import (
    RoutePacket,
    Table,
    TableEntry,
    DVRouterBase,
    Ports,
    FOREVER,
    INFINITY,
)


class DVRouter(DVRouterBase):
    # A route should time out after this interval
    ROUTE_TTL = 15

    # -----------------------------------------------
    # At most one of these should ever be on at once
    SPLIT_HORIZON = False
    POISON_REVERSE = True
    # -----------------------------------------------

    # Determines if you send poison for expired routes
    POISON_EXPIRED = True

    # Determines if you send updates when a link comes up
    SEND_ON_LINK_UP = False

    # Determines if you send poison when a link goes down
    POISON_ON_LINK_DOWN = False

    def __init__(self):
        """
        Called when the instance is initialized.
        DO NOT remove any existing code from this method.
        However, feel free to add to it for memory purposes in the final stage!
        """
        assert not (self.SPLIT_HORIZON and self.POISON_REVERSE), (
            "Split horizon and poison reverse can't both be on"
        )

        self.start_timer()  # Starts signaling the timer at correct rate.

        # Contains all current ports and their latencies.
        # See the write-up for documentation.
        self.ports = Ports()

        # This is the table that contains all current routes
        self.table = Table()
        self.table.owner = self
        self.history = defaultdict(dict) 

        ##### Begin Stage 10A #####

        ##### End Stage 10A #####

    def add_static_route(self, host, port):
        """
        Adds a static route to this router's table.

        Called automatically by the framework whenever a host is connected
        to this router.

        :param host: the host.
        :param port: the port that the host is attached to.
        :returns: nothing.
        """
        # `port` should have been added to `peer_tables` by `handle_link_up`
        # when the link came up.
        assert port in self.ports.get_all_ports(), "Link should be up, but is not."

        ##### Begin Stage 1 #####
        self.table[host] = TableEntry(
            dst=host,
            port=port,
            latency=self.ports.get_latency(port),
            expire_time=FOREVER,
        )

        ##### End Stage 1 #####

    def handle_data_packet(self, packet, in_port):
        """
        Called when a data packet arrives at this router.

        You may want to forward the packet, drop the packet, etc. here.

        :param packet: the packet that arrived.
        :param in_port: the port from which the packet arrived.
        :return: nothing.
        """

        ##### Begin Stage 2 #####
        next_hop = self.table.get(packet.dst, None)
        if next_hop and next_hop.latency < INFINITY:
            self.send(packet=packet, port=next_hop.port)
        ##### End Stage 2 #####

    def get_advertized_latency(self, port, entry):
        if port == entry.port:
            if self.POISON_REVERSE:
                return INFINITY

            if self.SPLIT_HORIZON:
                return None

        return entry.latency

    def send_routes(self, force=False, single_port=None):
        """
        Send route advertisements for all routes in the table.

        :param force: if True, advertises ALL routes in the table;
                      otherwise, advertises only those routes that have
                      changed since the last advertisement.
               single_port: if not None, sends updates only to that port; to
                            be used in conjunction with handle_link_up.
        :return: nothing.
        """

        ##### Begin Stages 3, 6, 7, 8, 10 #####
        for p in self.ports.get_all_ports():
            for dst, entry in self.table.items():

                adv_latency = self.get_advertized_latency(p, entry)
                if adv_latency is None:
                    continue

                if force:
                    self.send_route(dst=dst, port=p, latency=adv_latency)

                elif self.should_advertize(p, dst, adv_latency):
                    self.history[p][dst] = adv_latency
                    self.send_route(dst=dst, port=p, latency=adv_latency)

        ##### End Stages 3, 6, 7, 8, 10 #####

    def expire_routes(self):
        """
        Clears out expired routes from table.
        accordingly.
        """

        ##### Begin Stages 5, 9 #####
        expired_routes = []
        changed = False

        for h, entry in self.table.items():
            expire_time = entry.expire_time

            if expire_time == FOREVER:
                continue

            if expire_time <= api.current_time():
                expired_routes.append(h)

        for h in expired_routes:
            if self.POISON_EXPIRED:
                self.table[h] = TableEntry(
                    dst=self.table[h].dst,
                    latency=INFINITY,
                    expire_time=api.current_time() + self.ROUTE_TTL,
                    port=self.table[h].port
                )

            else:
                self.table.pop(h)
        ##### End Stages 5, 9 #####

    def handle_route_advertisement(self, route_dst, route_latency, port):
        """
        Called when the router receives a route advertisement from a neighbor.

        :param route_dst: the destination of the advertised route.
        :param route_latency: latency from the neighbor to the destination.
        :param port: the port that the advertisement arrived on.
        :return: nothing.
        """

        ##### Begin Stages 4, 10 #####
        neighbor_latency = self.ports.get_latency(port)

        advertised_route = TableEntry(
                dst=route_dst,
                port=port,
                latency=min(neighbor_latency+route_latency, INFINITY),
                expire_time= api.current_time() + self.ROUTE_TTL,
            )
        cur_route = self.table.get(route_dst, None)

        if cur_route is None or cur_route.port == port:
            self.table[route_dst] =  advertised_route
            self.send_routes(force=False)

        elif cur_route.latency > advertised_route.latency:
            self.table[route_dst] =  advertised_route
            self.send_routes(force=False)
        ##### End Stages 4, 10 #####

    def handle_link_up(self, port, latency):
        """
        Called by the framework when a link attached to this router goes up.

        :param port: the port that the link is attached to.
        :param latency: the link latency.
        :returns: nothing.
        """
        self.ports.add_port(port, latency)

        ##### Begin Stage 10B #####

        ##### End Stage 10B #####

    def handle_link_down(self, port):
        """
        Called by the framework when a link attached to this router goes down.

        :param port: the port number used by the link.
        :returns: nothing.
        """
        self.ports.remove_port(port)

        ##### Begin Stage 10B #####

        ##### End Stage 10B #####

    # Feel free to add any helper methods!
    def should_advertize(self, port, dst, adv_latency):
        prev_latency = self.history[port].get(dst)
        if prev_latency is None:
            return True

        if prev_latency != adv_latency:
            return True

        return False
