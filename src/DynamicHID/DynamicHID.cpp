/*
  Modified by Jean-Marc Lalouette to try something
  Modified by Matthew Heironimus to support HID Report Descriptors to be in 
  standard RAM in addition to program memory (PROGMEM).

   Copyright (c) 2015, Arduino LLC
   Original code (pre-library): Copyright (c) 2011, Peter Barrett

   Permission to use, copy, modify, and/or distribute this software for
   any purpose with or without fee is hereby granted, provided that the
   above copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
   BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
   OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
   WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
   ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
   SOFTWARE.
 */

#include "DynamicHID.h"

#if defined(USBCON)

#ifdef _VARIANT_ARDUINO_DUE_X_
#define USB_SendControl USBD_SendControl
#define USB_Send USBD_Send
#endif

DynamicHID_& DynamicHID()
{
	static DynamicHID_ obj;
	return obj;
}

int DynamicHID_::getInterface(uint8_t* interfaceCount)
{
	*interfaceCount += 1; // uses 1
	DYNAMIC_HIDDescriptor hidInterface = {
		D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, DYNAMIC_HID_SUBCLASS_NONE, DYNAMIC_HID_PROTOCOL_NONE),
		D_HIDREPORT(descriptorSize),
		D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
	};
	return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

bool DynamicHID_::SendStringDescriptor(u8* stringP, u8 stringLen, uint8_t flags)
{
	USB_SendControl(2 + stringLen * 2);
	USB_SendControl(3);
	bool pgm = flags & TRANSFER_PGM;
	for (u8 i = 0; i < stringLen; i++) 
	{
		bool r = USB_SendControl(pgm ? pgm_read_byte(&stringP[i]) : stringP[i]);
		r &= USB_SendControl(0); // high byte
		if (!r) 
		{
			return false;
		}
	}
	return true;
}

int DynamicHID_::getDescriptor(USBSetup& setup)
{
	if (setup.wValueH == USB_STRING_DESCRIPTOR_TYPE)
	{
		switch (setup.wValueL)
		{
			case 2:
				//Setup.wValueL == 2 does in fact == 2
				u8 productStringArray[] = { 'F','l','i','g','h','t',' ','C','o','n','t','r','o','l','l','e','r' };
				bool prResult = SendStringDescriptor(productStringArray, 17, 0);
				return prResult ? 1 : 0;
			case 1:
				//For some UNKNOWN reason setup.wValueL == 1 does NOT == 1
				u8 * manufacturerString = (u8*)&"Raptyr Electronics";
				bool mnResult = SendStringDescriptor(manufacturerString, strlen(manufacturerString), 0);
				return mnResult ? 1 : 0;
			case 3:
				//For some UNKNOWN reason setup.wValueL == 3 does NOT == 3
				u8 * serialString = (u8*)&"HID100";//MAX LEN 20
				bool srResult = SendStringDescriptor(serialString, strlen(serialString), 0);
				return srResult ? 1 : 0;
			case 4:
				u8 *mainCtrl = (u8*)&"Flight Controller Main";
				return SendStringDescriptor(mainCtrl, strlen(mainCtrl), 0) ? 1 : 0;
			case 5:
				u8 *aux1Ctrl = (u8*)&"Flight Controller Aux1";
				return SendStringDescriptor(aux1Ctrl, strlen(aux1Ctrl), 0) ? 1 : 0;
			case 6:
				u8 *aux2Ctrl = (u8*)&"Flight Controller Aux2";
				return SendStringDescriptor(aux2Ctrl, strlen(aux2Ctrl), 0) ? 1 : 0;
			case 7:
				u8 * aileron = (u8*)&"Aileron";
				return SendStringDescriptor(aileron, strlen(aileron), 0) ? 1 : 0;
			case 8:
				u8 * elevator= (u8*)&"Elevator";
				return SendStringDescriptor(elevator, strlen(elevator), 0) ? 1 : 0;
			case 9:
				u8 *leftBrakeCtrl = (u8*)&"Left Toe Brake";
				return SendStringDescriptor(leftBrakeCtrl, strlen(leftBrakeCtrl), 0) ? 1 : 0;
			case 10:
				u8 *rightBrakeCtrl = (u8*)&"Right Toe Brake";
				return SendStringDescriptor(rightBrakeCtrl, strlen(rightBrakeCtrl), 0) ? 1 : 0;
		}
	}
	if (setup.bmRequestType == REQUEST_DEVICETOHOST_STANDARD_INTERFACE)
	{
		//HID Class Descriptor Request
		//case REQUEST_DEVICETOHOST_STANDARD_INTERFACE:
		if (setup.wValueH == DYNAMIC_HID_REPORT_DESCRIPTOR_TYPE)
		{
			//Check for the current interface number
			//wIndex=interface number for HID Class Descriptor
			if (setup.wIndex == pluggedInterface)
			{
				int total = 0;
				DynamicHIDSubDescriptor* node;
				for (node = rootNode; node; node = node->next)
				{
					int result = USB_SendControl((node->inProgMem ? TRANSFER_PGM : 0), node->data, node->length);
					if (result == -1)
					{
						return -1;
					}
					total += result;
				}
				// Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
				// due to the USB specs, but Windows and Linux just assumes its in report mode.
				protocol = DYNAMIC_HID_REPORT_PROTOCOL;
				return total;
			}
		}
	}
	return 0;
}

uint8_t DynamicHID_::getShortName(char *name)
{
	name[0] = 'H';
	name[1] = 'I';
	name[2] = 'D';
	name[3] = 'A' + (descriptorSize & 0x0F);
	name[4] = 'A' + ((descriptorSize >> 4) & 0x0F);

	return 5;
}

void DynamicHID_::AppendDescriptor(DynamicHIDSubDescriptor *node)
{
	if (!rootNode) 
	{
		rootNode = node;
	} 
	else 
	{
		DynamicHIDSubDescriptor *current = rootNode;
		while (current->next) 
		{
			current = current->next;
		}
		current->next = node;
	}
	descriptorSize += node->length;
}

int DynamicHID_::SendReport(uint8_t id, const void* data, int len)
{
	uint8_t p[len + 1];
	p[0] = id;
	memcpy(&p[1], data, len);
	return USB_Send(pluggedEndpoint | TRANSFER_RELEASE, p, len + 1);
}

bool DynamicHID_::setup(USBSetup& setup)
{
	if (pluggedInterface != setup.wIndex) 
	{
		return false;
	}

	uint8_t request = setup.bRequest;
	uint8_t requestType = setup.bmRequestType;

	if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
	{
		if (request == DYNAMIC_HID_GET_REPORT) 
		{
			// TODO: DYNAMIC_HID_GetReport();
			return true;
		}
		if (request == DYNAMIC_HID_GET_PROTOCOL) 
		{
			// TODO: Send8(protocol);
			return true;
		}
		if (request == DYNAMIC_HID_GET_IDLE) 
		{
			// TODO: Send8(idle);
		}
	}

	if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
	{
		if (request == DYNAMIC_HID_SET_PROTOCOL) 
		{
			// The USB Host tells us if we are in boot or report mode.
			// This only works with a real boot compatible device.
			protocol = setup.wValueL;
			return true;
		}
		if (request == DYNAMIC_HID_SET_IDLE) 
		{
			idle = setup.wValueL;
			return true;
		}
		if (request == DYNAMIC_HID_SET_REPORT)
		{
			//uint8_t reportID = setup.wValueL;
			//uint16_t length = setup.wLength;
			//uint8_t data[length];
			// Make sure to not read more data than USB_EP_SIZE.
			// You can read multiple times through a loop.
			// The first byte (may!) contain the reportID on a multreport.
			//USB_RecvControl(data, length);
		}
	}

	return false;
}

DynamicHID_::DynamicHID_(void) : PluggableUSBModule(1, 1, epType),
                   rootNode(NULL), descriptorSize(0),
                   protocol(DYNAMIC_HID_REPORT_PROTOCOL), idle(1)
{
	epType[0] = EP_TYPE_INTERRUPT_IN;
	PluggableUSB().plug(this);
}

int DynamicHID_::begin(void)
{
	return 0;
}

#endif /* if defined(USBCON) */