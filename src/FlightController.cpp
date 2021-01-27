#include "FlightController.h"
#if defined(_USING_DYNAMIC_HID)

FlightControl_::FlightControl_(uint8_t hidReportID)
{
	_hidReportID = hidReportID;
}
void FlightControl_::Initialise()
{
	//BUILD HID Report Descriptor
	uint8_t descriptor[150];
    int descriptorIndex = 0;
	
	DESCRIPTOR(descriptor,descriptorIndex,USAGE_PAGE,PAGE_GENERIC_DESKTOP)
		DESCRIPTOR(descriptor,descriptorIndex,USAGE,GENERIC_MULTI_AXIS)
		DESCRIPTOR(descriptor,descriptorIndex,COLLECTION,COLLECTION_APPLICATION)
			DESCRIPTOR(descriptor,descriptorIndex,REPORT_ID,_hidReportID)
			DESCRIPTOR(descriptor,descriptorIndex,USAGE_PAGE,PAGE_GENERIC_DESKTOP)
				DESCRIPTOR(descriptor,descriptorIndex,USAGE,GENERIC_POINTER)
				DESCRIPTOR3(descriptor,descriptorIndex,LOGICAL_MINIMUM,0x01,0x80)
				DESCRIPTOR3(descriptor,descriptorIndex,LOGICAL_MAXIMUM,0xFF,0x7F)
				DESCRIPTOR(descriptor,descriptorIndex,REPORT_SIZE,0x10)
				DESCRIPTOR(descriptor,descriptorIndex,REPORT_COUNT,0x02)
				DESCRIPTOR(descriptor,descriptorIndex,COLLECTION,COLLECTION_PHYSICAL)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,GENERIC_X)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,GENERIC_Y)
					DESCRIPTOR(descriptor,descriptorIndex,INPUT,IO_DATA | IO_VARIABLE | IO_ABSOLUTE)
				DESCRIPTOR1(descriptor,descriptorIndex,END_COLLECTION)
				DESCRIPTOR(descriptor,descriptorIndex,USAGE_PAGE,PAGE_SIMULATION_CONTROLS)
				DESCRIPTOR3(descriptor,descriptorIndex,LOGICAL_MINIMUM,0x01,0x80)
				DESCRIPTOR3(descriptor,descriptorIndex,LOGICAL_MAXIMUM,0xFF,0x7F)
				DESCRIPTOR(descriptor,descriptorIndex,REPORT_SIZE,0x10)
				DESCRIPTOR(descriptor,descriptorIndex,REPORT_COUNT(4)
				DESCRIPTOR(descriptor,descriptorIndex,COLLECTION,COLLECTION_PHYSICAL)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,SIMULATION_RUDDER)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,SIMULATION_THROTTLE)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,SIMULATION_TOEBRAKE)
					DESCRIPTOR(descriptor,descriptorIndex,USAGE,SIMULATION_TOEBRAKE)
					DESCRIPTOR(descriptor,descriptorIndex,INPUT,IO_DATA | IO_VARIABLE | IO_ABSOLUTE)
				DESCRIPTOR1(descriptor,descriptorIndex,END_COLLECTION)
		DESCRIPTOR1(descriptor,descriptorIndex,END_COLLECTION)
	//END BUILD HID Report Descriptor
	
	// Create a copy of the HID Report Descriptor template that is just the right size
	uint8_t *hidReportDescriptor = new uint8_t[descriptorIndex];
	memcpy(hidReportDescriptor, descriptor, descriptorIndex);
	
	// Register HID Report Description
	DynamicHIDSubDescriptor *node = new DynamicHIDSubDescriptor(hidReportDescriptor, descriptorIndex, false);
	DynamicHID().AppendDescriptor(node);
}
void FlightControl_::Begin(bool startAutoSend = true)
{
	_autoSendState = startAutoSend;
	SendState();
}

void FlightControl_:End()
{
}

void FlightControl_::setButton(uint8_t button, uint8_t value)
{
	if (value == 0)
	{
		releaseButton(button);
	}
	else
	{
		pressButton(button);
	}
}
void FlightControl_::pressButton(uint8_t button)
{
    if (button >= _buttonCount) return;

    int index = button / 8;
    int bit = button % 8;

	bitSet(_buttonValues[index], bit);
	if (_autoSendState) sendState();
}
void FlightControl_::releaseButton(uint8_t button)
{
    if (button >= _buttonCount) return;

    int index = button / 8;
    int bit = button % 8;

    bitClear(_buttonValues[index], bit);
	if (_autoSendState) sendState();
}
void FlightControl_::setAileronAxis(int16_t value)
{
	_aileronAxis = value;
	if (_autoSendState) sendState();
}
void FlightControl_::setElevatorAxis(int16_t value)
{
	_elevatorAxis = value;
	if (_autoSendState) sendState();
}
void FlightControl_::setRudder(int16_t value)
{
	_rudder = value;
	if (_autoSendState) sendState();
}
void FlightControl_::setThrottle(int16_t value)
{
	_throttle = value;
	if (_autoSendState) sendState();
}
void FlightControl_::setBrakeLeft(int16_t value)
{
	_brakeLeft = value;
	if (_autoSendState) sendState();
}
void FlightControl_::setBrakeRight(int16_t value)
{
	_brakeRight = value;
	if (_autoSendState) sendState();
}
int FlightControl_::buildAndSet16BitValue(int16_t value, int16_t valueMinimum, int16_t valueMaximum, int16_t actualMinimum, int16_t actualMaximum, uint8_t dataLocation[]) 
{
	int16_t convertedValue;
	uint8_t highByte;
	uint8_t lowByte;
	int16_t realMinimum = min(valueMinimum, valueMaximum);
	int16_t realMaximum = max(valueMinimum, valueMaximum);

	if (value < realMinimum) 
	{
		value = realMinimum;
	}
	if (value > realMaximum) 
	{
		value = realMaximum;
	}

	if (valueMinimum > valueMaximum) 
	{
		// Values go from a larger number to a smaller number (e.g. 1024 to 0)
		value = realMaximum - value + realMinimum;
	}

	convertedValue = map(value, realMinimum, realMaximum, actualMinimum, actualMaximum);

	highByte = (uint8_t)(convertedValue >> 8);
	lowByte = (uint8_t)(convertedValue & 0x00FF);
	
	dataLocation[0] = lowByte;
	dataLocation[1] = highByte;
	
	return 2;
}

void FlightControl_::SendState()
{
	uint8_t data[_hidReportSize];
	int index = 0;
	// Build Data Packet
	index += buildAndSet16BitValue(_aileronAxis,_aileronPhysicalMinimum,_aileronPhysicalMaximum,_aileronAxisLogicalMinimum,_aileronAxisLogicalMaximum,&(data[index]));
	index += buildAndSet16BitValue(_elevatorAxis,_elevatorPhysicalMinimum,_elevatorPhysicalMaximum,_elevatorAxisLogicalMinimum,_elevatorAxisLogicalMaximum,&(data[index]));
	index += buildAndSet16BitValue(_rudderAxis,_rudderPhysicalMinimum,_rudderPhysicalMaximum,_rudderAxisLogicalMinimum,_rudderAxisLogicalMaximum,&(data[index]));
	index += buildAndSet16BitValue(_throttleAxis,_throttlePhysicalMinimum,_throttlePhysicalMaximum,_throttleAxisLogicalMinimum,_throttleAxisLogicalMaximum,&(data[index]));
	index += buildAndSet16BitValue(_brakeLeftAxis,_brakeLeftPhysicalMinimum,_brakeLeftPhysicalMaximum,_brakeLeftAxisLogicalMinimum,_brakeLeftAxisLogicalMaximum,&(data[index]));
	index += buildAndSet16BitValue(_brakeRightAxis,_brakeRightPhysicalMinimum,_brakeRightPhysicalMaximum,_brakeRightAxisLogicalMinimum,_brakeRightAxisLogicalMaximum,&(data[index]));
	//END Build Data Packet
	DynamicHID().SendReport(_hidReportID, data, _hidReportSize);
}
#endif