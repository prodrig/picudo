# picudo
Sw to collect audio samples of multiple sensor devices to a central workstation

# project setup
A workstation connects with several sensor devices (stm32f4 discovery + ethernet expansion boards) through a multicast channel and unicast channel

The multicast channel workstation->devices (all) serves these purposes:
<ul>
  <li>Discovery protocol (request)
  <li>Start/stop commands
  <li>Timestamp update
</ul>
and the required common setup is to be stored into the app_conf.h header file:
<ol>
  <li>IP group address (MULTICAST_ADDR)
  <li>Group port number (MULTICAST_PORT)
</ol>  

The unicast channel device (one)->workstation serves these purposes:
<ul>
  <li>Discovery protocol (answer)
  <li>Sampled audio data
</ul>
the common setup is sent by the workstation in the discovery request.

# Discovery protocol
The aim is to assist the workstation program to adapt to the actual number of devices in the network; it is also used to spread the setup of the unicast channel.

The discovery protocol is built from two messages:
<ol>
  <li>Request: The workstation sends a multicast discovery request to the agreed group address with the information of the unicast channel.
  <li>Answer: Each device in the network subscribes to the agreed group address and when receives a discovery request it identifies itself using the unicast channel information sent in the request.
</ol>

Discovery protocol request packet (binary):
00 AA BB CC DD EE FF
<ol>
  <li>00: protocol identifier (0x00)
  <li>AA BB CC DD: workstation unicast channel IP address AA.BB.CC.DD
  <li>EE FF: workstation unicast channel UDP port 0xEEFF
</ol>  

Discovery answer packet (unicast):
00 AA BB CC DD EE
<ol>
  <li>00: protocol identifier (0x00)
  <li>AA BB CC DD: device unicast channel IP address AA.BB.CC.DD / Device "serial number" 0xAABBCCDD
  <li>EE: number of analog channels in the device 0xEE
</ol>  

# Start/stop commands
Commands sent from the workstation to all devices in the network to start and stop recording audio samples. There is no specific answer from the devices

If the start command is sent the devices will start sending recorded data regularly through the unicast channel.
If the stop command is sent the devices will stop sending recorded data through the unicast channel.

Start command packet (binary):
01
<ol>
  <li>01: protocol identifier (0x01)
</ol>  
With the start command the devices initializes their local time to zero.

Stop command packet (binary):
02
<ol>
  <li>02: protocol identifier (0x02)
</ol>  

# Timestamp update
A multicast message sent by the workstation to synchronize the local time of all devices in the network and avoid local clock drift.
This could be replaced by any clock synchronization protocol available.

# Sampled audio data
A unicast message sent by each device in the network with audio recorded data.
Each audio sample is 16 bits, and the length of the recorded data should be adjusted to optimize traffic.

Sampled audio packet (binary):
FF AA BB CC DD EE
<ol>
  <li>FF: protocol identifier 0xFF
  <li>AA BB CC DD: device unicast channel IP address AA.BB.CC.DD / Device "serial number" 0xAABBCCDD
  <li>EE: recorded analog channel 0xEE
  <li>TT: timestamp (several bytes)
  <li>PP: Packet number 0xPP
  <li>FF GG: recorded data length (0xFFGG bytes)
  <li>HH II ... YY ZZ: recorded data (two bytes per sample: 0xHHII, ..., 0xYYZZ)
</ol>  
