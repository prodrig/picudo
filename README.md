# picudo
Sw to collect audio samples of multiple sensor devices to a central workstation

## Project setup
A workstation connects with several sensor devices (stm32f4 discovery + ethernet expansion boards) through a multicast channel and unicast channel

The multicast channel is workstation->devices (all) and serves the following purposes:
* Discovery protocol (request)
* Start/stop commands
* Timestamp update

and the required common setup is to be stored into the app_conf.h header file:
1. IP group address (MULTICAST_ADDR)
2. Group port number (MULTICAST_PORT)

The unicast channel is device (one)->workstation and serves these purposes:
* Discovery protocol (answer)
* Sampled audio data

the common setup for this unicast cahnnel is sent by the workstation in the discovery request.

## Discovery protocol
The aim is to assist the workstation program to adapt to the actual number of devices in the network; it is also used to spread the setup of the unicast channel.

The discovery protocol is built from two messages:
1. Request: The workstation sends a multicast discovery request to the agreed group address with the information of the unicast channel.
2. Answer: Each device in the network subscribes to the agreed group address and when receives a discovery request it identifies itself using the unicast channel information sent in the request.

### Discovery protocol request packet (binary):
Protocol identifier | worskation unicast channel's IP address | worskation unicast channel's UDP port
--------------------|-----------------------------------------|--------------------------------------
00 | AA BB CC DD | EE FF

Packet fields:
1. 00: protocol identifier (0x00)
2. AA BB CC DD (placeholders): workstation unicast channel IP address AA.BB.CC.DD
3. EE FF (placeholders): workstation unicast channel UDP port 0xEEFF

### Discovery answer packet (unicast):
Protocol identifier | device unicast channel's IP address / serial number | analog channels
--------------------|-----------------------------------------------------|----------------
00 | AA BB CC DD | EE

Packet fields:
1. 00: protocol identifier (0x00)
2. AA BB CC DD (placeholders): device unicast channel IP address AA.BB.CC.DD / Device "serial number" 0xAABBCCDD
3. EE (placeholder): number of analog channels in the device 0xEE

## Start/stop commands
Commands sent from the workstation to all devices in the network to start and stop recording audio samples. There is no specific answer from the devices

If the start command is sent the devices will start sending recorded data regularly through the unicast channel.
If the stop command is sent the devices will stop sending recorded data through the unicast channel.

### Start command packet (binary):
Protocol identifier 
--------------------|
01        

Packet fields:
1. 01: protocol identifier (0x01)
With the start command the devices initializes their local time to zero.

### Stop command packet (binary):
Protocol identifier 
--------------------|
02        

Packet fields:
1. 02: protocol identifier (0x02)

## Timestamp update
A multicast message sent by the workstation to synchronize the local time of all devices in the network and avoid local clock drift.
This could be *replaced* by any clock synchronization protocol available.

## Sampled audio data
A unicast message sent by each device in the network with audio recorded data.
Each audio sample is 16 bits, and the length of the recorded data should be adjusted to optimize traffic.

### Sampled audio packet (binary):
Protocol identifier | device unicast channel's IP address / serial number | analog channel | timestamp | packet number | recorded data length | recorded data
--------------------|-----------------------------------------------------|----------------|-----------|---------------|---------------------|---------------|
FF | AA BB CC DD | EE | TT | PP | GG HH | II JJ ... YY ZZ

Packet fields:
1. FF: protocol identifier (0xFF)
2. AA BB CC DD (placeholders): device unicast channel IP address AA.BB.CC.DD / Device "serial number" 0xAABBCCDD
3. EE (placeholder): recorded analog channel 0xEE
4. TT (placeholder): timestamp (several bytes)
5. PP (placeholder): Packet number 0xPP
6. GG HH (placeholders): recorded data length (0xGGHH bytes)
7. II JJ ... YY ZZ (placeholders): recorded data (two bytes per sample: 0xIIJJ, ..., 0xYYZZ)

