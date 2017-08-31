import sys

from twisted.web import server, resource
from twisted.web.server import Site
from twisted.web.static import File
from twisted.internet import reactor
from twisted.internet.protocol import Factory, Protocol
from twisted.internet.serialport import SerialPort

from autobahn.twisted.websocket import WebSocketServerFactory, WebSocketServerProtocol
from autobahn.twisted.resource import WebSocketResource

from serial.serialutil import SerialException

sockets = []

class USBclient(Protocol):
    def connectionMade(self):
        print 'Arduino device connected.'

    def dataReceived(self, data):
        for socket in sockets:
            socket.sendMessage(data);

class ArduinoSocketProtocol(WebSocketServerProtocol):
    def onOpen(self):
        sockets.append(self)

if __name__ == '__main__':
    factory = WebSocketServerFactory(u"ws://127.0.0.1:8080")
    factory.protocol = ArduinoSocketProtocol
    factory.startFactory()
    resource = WebSocketResource(factory)
    
    # we server static files under "/" ..
    root = File('public')
    root.putChild(b"ws", resource)
    site = Site(root)
    reactor.listenTCP(8080, site)
    
    try:
        SerialPort(USBclient(), '/dev/ttyACM0', reactor, baudrate='9600')
    except SerialException:
        print "Could not connect to serial device"
    reactor.run()
