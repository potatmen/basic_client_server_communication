import time
from socketserver import *
from socketserver import BaseRequestHandler, TCPServer

SENDING_COOLDOWN = 0.3
BUFFER_SIZE = 4096


class EchoHandler(BaseRequestHandler):
    postSwitch = False
    content = ""
    error_str = 'ERROR - Command not understood'
    ok_str = 'OK'
    input_buffer = []

    def send_str(self, string):
        self.request.send(bytes('server: ' + string, encoding='utf-8'))
        time.sleep(SENDING_COOLDOWN)

    def recv_str(self):
        post_msg = self.request.recv(BUFFER_SIZE)
        return str(post_msg, encoding='utf-8')

    def handle(self):
        print('Got connection from', self.client_address)
        while True:
            # try:
            msg = self.request.recv(BUFFER_SIZE)
            if not msg:
                break
            msg_str = str(msg, encoding='utf-8')
            print('msg_str is :', msg_str)
            msg_pieces = msg_str.split()

            if len(msg_pieces) >= 2:
                self.send_str(self.error_str)
            elif len(msg_pieces) == 1:
                command = msg_pieces[0]
                print('command is :', command)
                if command in ['POST', 'READ', 'QUIT', '#']:
                    if command == 'POST':
                        in_post = True
                        while in_post:
                            post_msg_str = self.recv_str()
                            if post_msg_str[-1] == "#":
                                in_post = False
                                self.send_str(self.ok_str)
                            else:
                                print(f"added: {post_msg_str}")
                                self.content += "\n" + post_msg_str
                    elif command == 'READ':
                        # self.send_str('Welcome socket programming')
                        for line in self.content.strip().split("\n"):
                            self.send_str(f"{line}")
                        self.send_str('#')
                    elif command == 'QUIT':
                        self.send_str(self.ok_str)
                else:
                    self.send_str(self.error_str)
            elif len(msg_pieces) < 1:
                self.send_str(self.error_str)


serv = TCPServer(('', 16011), EchoHandler)
serv.serve_forever()
