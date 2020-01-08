import socket, ssl, pprint, time

# Require a certificate from the server. We used a self-signed certificate
# so here ca_certs must be the server certificate itself.

print "================================================================================"
print "TCP Secure Client"
print "================================================================================"


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ssl_sock = ssl.wrap_socket(s, ca_certs='server.crt', cert_reqs=ssl.CERT_REQUIRED)
ssl_sock.connect(('172.20.10.3', 50007))
print "Connected to: ", repr(ssl_sock.getpeername())
print "================================================================================"
print "Server Certificate Details:"
print ssl_sock.cipher()
print pprint.pformat(ssl_sock.getpeercert())
ssl_sock.write("Hello ModusToolbox!")
print "================================================================================"
print "Message from Server:"
data = ssl_sock.read()
print repr(data)
print "================================================================================"

ssl_sock.close()

# note that closing the SSLSocket will also close the underlying socket

