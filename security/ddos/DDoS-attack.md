

# DDoS attack

A hacker uses DDoS attacks in order to prevent legitimate users from accessing the service of a provider. The attacker does this through the use of an attack that streams multiple illegitimate requests to the victim, i.e. a High-Rate Flooding (HRF) attack. There have been various classifications of DDoS attacks.

## TCP three-way handshake

A TCP SYN flooding attack is an example of a network layer flooding attack, and it is one of the most common and powerful flooding methods. It exploits the vulnerabilities of the TCP three-way handshake.

In a normal TCP connection, the client initiates the connection by sending a SYN packet to the server, as a way of requesting a connection. Upon receiving the connection request, the server will open a connection session and respond with a SYN_ACK packet; by doing this the server stores details of the requested TCP connection in the memory stack and allocates resources to this open session.  The connection remains in a half-open state, i.e. the SYN_RECVD state. To complete the three-way handskake with the server, the client will need to confirm the connection and respond with an ACK packet. The server will then check the memory stack for an existing connection request, and the TCP connection will be moved from the SYN_RECVD state to ESTABLISHED state. If there is no ACK packet sent within a specific period of time, the connection will timeout and therefore releasing the allocated resources.

## TCP SYN flooding attack

In a TCP SYN flooding attack, the attacker streams large volumes of SYN packets towards the victim server. These packets normally contain spoofed IP addresses, i.e. IP addresses that are non-existent or are not utilized. TCP SYN floods can also be launched using compromised machines with legitimate IP addresses, however the machines need to be configured in such a way that it does not respond or acknowledge a SYN_ACK packet from the victim server.

In this way the server will not receive any ACK packet from the clients for the "half-open" connection request. During the high rate flooding attack, and for a period of time, the server will maintain a large volume of incomplete three-way handshake and allocates resource towards the fictitious connection requests. The server will gather more fictitious requests and eventually exhaust its resources. The will prevent new requests, including legitimate client requests, from being further processed by the server.

## Mitigation

In order to remove the Mirai malware from an infected IoT device, users and administrators should take the following actions:
  
  - Disconnect device from the network.
  - While disconnected from the network and Internet, perform a reboot. Because Mirai malware exists in dynamic memory, rebooting the device clears the malware [8].
  - Ensure that the password for accessing the device has been changed from the default password to a strong password. See US-CERT Tip Choosing and Protecting Passwords for more information.
  - You should reconnect to the network only after rebooting and changing the password. If you reconnect before changing the password, the device could be quickly reinfected with the Mirai malware.

## Preventive Steps

In order to prevent a malware infection on an IoT device, users and administrators should take following precautions:

  - Ensure all default passwords are changed to strong passwords. Default usernames and passwords for most devices can easily be found on the Internet, making devices with default passwords extremely vulnerable.
  - Update IoT devices with security patches as soon as patches become available.
  - Disable Universal Plug and Play (UPnP) on routers unless absolutely necessary.
  - Purchase IoT devices from companies with a reputation for providing secure devices.
  - Consumers should be aware of the capabilities of the devices and appliances installed in their homes and businesses. If a device comes with a default password or an open Wi-Fi connection, consumers should change the password and only allow it to operate on a home network with a secured Wi-Fi router.
  - Understand the capabilities of any medical devices intended for at-home use. If the device transmits data or can be operated remotely, it has the potential to be infected.

## Reference

* Machaka, Pheeha, Antoine Bagula, and Fulufhelo Nelwamondo. "Using exponentially weighted moving average algorithm to defend against DDoS attacks." Pattern Recognition Association of South Africa and Robotics and Mechatronics International Conference (PRASA-RobMech), 2016. IEEE, 2016.
