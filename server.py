import socket
import sys

class UDPServer: 
    def __init__(self, ip = "127.0.0.1", port = 20001):

        self.server_addr = ("0.0.0.0", port)
        print ("Connecting to {}:{}".format(ip, port))
        self.server = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        self.server.bind( self.server_addr )
        self.clients = []
        self.buffer_size = 1024

    def listen(self): 
        while True: 
            try: 

                client_msg_addr = self.server.recvfrom( self.buffer_size )
                client_msg, client_addr = client_msg_addr[0], client_msg_addr[1]

                if client_addr not in self.clients: 
                    print ("[Server][New Client] {}".format(client_addr))
                    self.clients.append( client_addr )

                #print("[Server] [Client {}:{}] [Msg Received]".format(client_addr[0], client_addr[1] ))
                

                # list of client addresses.
                for other_clients in self.clients: 
                    try:
                        self.server.sendto( client_msg, other_clients )
                    except ConnectionResetError as e: 
                        # self.clients.remove(other_clients)
                        break
 
            except KeyboardInterrupt: 
                break
            except ConnectionResetError as e: 
                print ("Client was droped: ", e.strerror) 
                break


if __name__ == "__main__" : 

    if len(sys.argv) != 3: 
        print ("Usage: python {} <server-ip> <server-port>".format(sys.argv[0]) )   
        sys.exit(0)

    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])

    server = UDPServer(ip = server_ip, port=server_port)
    server.listen()
