/**
 * \file ParamDevice.c
 * \brief Contain the paramater of the devices
 * \author Gautier.B
 * \version 0.1
 * \date 3 August 2007
 *
 *
 */
#ifndef PARAMDEVICE_H_
#define PARAMDEVICE_H_

/**
 * \struct ParamDevice
 * \brief Object   parameter characterizing the device
 *
 * ParamDevice is a device character management object.
 * It contains the ID of the node, its broadcast IP address,
 * unicast channel, timestamp, data length and data. !
 */
struct ParamDevice{
	char *nodeId;
	char addUnicast[35];
	int analogChannels;
	char *timestamp;
	int lengthData;
	char data[8];
};


typedef struct ParamDevice device_prototype;

#endif /* PARAMDEVICE_H_ */
