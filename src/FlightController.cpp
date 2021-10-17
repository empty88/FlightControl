#include <FlightController.h>
#include <DynamicHID.h>
#if defined(_USING_DYNAMIC_HID)

FlightControl_::FlightControl_(uint8_t hidReportID,uint8_t joystickType)
{
	_hidReportID = hidReportID;
	_joystickType = joystickType;
}
void FlightControl_::Initialise()
{
	//BUILD HID Report Descriptor
	uint8_t descriptor[150];
	int descriptorIndex = 0;
	DESCRIPTOR(descriptor, descriptorIndex, USAGE_PAGE, PAGE_GENERIC_DESKTOP)
	DESCRIPTOR(descriptor, descriptorIndex, USAGE, _joystickType)
	DESCRIPTOR(descriptor, descriptorIndex, STRING_INDEX,_hidReportID+1)
		DESCRIPTOR(descriptor, descriptorIndex, COLLECTION, COLLECTION_APPLICATION)
		DESCRIPTOR(descriptor, descriptorIndex, REPORT_ID, _hidReportID)
	if (_buttonCount > 0)
	{
		uint8_t buttonsInLastByte = _buttonCount % 8;
		uint8_t paddingBits = buttonsInLastByte > 0 ? 8 - buttonsInLastByte : 0;
		DESCRIPTOR(descriptor,descriptorIndex,USAGE_PAGE,PAGE_BUTTON)
		DESCRIPTOR(descriptor,descriptorIndex,USAGE_MIN,01)
		DESCRIPTOR(descriptor,descriptorIndex,USAGE_MAX,_buttonCount)
		DESCRIPTOR(descriptor,descriptorIndex,LOGICAL_MIN,0)
		DESCRIPTOR(descriptor,descriptorIndex,LOGICAL_MAX,1)
		DESCRIPTOR(descriptor,descriptorIndex,REPORT_SIZE,1)
		DESCRIPTOR(descriptor,descriptorIndex,REPORT_COUNT,_buttonCount)
		DESCRIPTOR(descriptor,descriptorIndex,UNIT_EXP,0)
		DESCRIPTOR(descriptor,descriptorIndex,UNIT,0)
		DESCRIPTOR(descriptor,descriptorIndex,HIDINPUT,IO_DATA | IO_VARIABLE | IO_ABSOLUTE)
		if (paddingBits > 0)
		{
			DESCRIPTOR(descriptor,descriptorIndex,REPORT_SIZE,1)
			DESCRIPTOR(descriptor,descriptorIndex,REPORT_COUNT,paddingBits)
			DESCRIPTOR(descriptor,descriptorIndex,HIDINPUT,IO_CONSTANT | IO_VARIABLE | IO_ABSOLUTE)
		}
	}
		DESCRIPTOR(descriptor, descriptorIndex, USAGE_PAGE, PAGE_GENERIC_DESKTOP)
		DESCRIPTOR(descriptor, descriptorIndex, USAGE, GENERIC_POINTER)
		DESCRIPTOR3(descriptor, descriptorIndex, LOGICAL_MIN16, 0x01, 0x80)
		DESCRIPTOR3(descriptor, descriptorIndex, LOGICAL_MAX16, 0xFF, 0x7F)
		DESCRIPTOR(descriptor, descriptorIndex, REPORT_SIZE, 0x10)
		DESCRIPTOR(descriptor, descriptorIndex, REPORT_COUNT, _useBrakes ? 0x04 : 0x02)
		if (_useBrakes)
		{
			DESCRIPTOR(descriptor, descriptorIndex, STRING_MIN, 7)
			DESCRIPTOR(descriptor, descriptorIndex, STRING_MAX, 10)
		}
		else
		{
			DESCRIPTOR(descriptor, descriptorIndex, STRING_MIN, 7)
			DESCRIPTOR(descriptor, descriptorIndex, STRING_MAX, 8)
		}
		DESCRIPTOR(descriptor, descriptorIndex, COLLECTION, COLLECTION_PHYSICAL)
		DESCRIPTOR(descriptor, descriptorIndex, USAGE, GENERIC_X)
		DESCRIPTOR(descriptor, descriptorIndex, USAGE, GENERIC_Y)
		if (_useBrakes)
		{
			DESCRIPTOR(descriptor, descriptorIndex,USAGE,GENERIC_RX)
			DESCRIPTOR(descriptor, descriptorIndex,USAGE,GENERIC_RY)
		}
			DESCRIPTOR(descriptor, descriptorIndex, HIDINPUT, IO_DATA | IO_VARIABLE | IO_ABSOLUTE)
		DESCRIPTOR1(descriptor, descriptorIndex, END_COLLECTION)
		if (_useRudder || _throttleCount > 0)
		{
			uint8_t simulationCount = (_useRudder ? 1 : 0) + _throttleCount;
			DESCRIPTOR(descriptor, descriptorIndex, USAGE_PAGE, PAGE_SIMULATION_CONTROLS)
				DESCRIPTOR3(descriptor, descriptorIndex, LOGICAL_MIN16, 0x01, 0x80)
				DESCRIPTOR3(descriptor, descriptorIndex, LOGICAL_MAX16, 0xFF, 0x7F)
				DESCRIPTOR(descriptor, descriptorIndex, REPORT_SIZE, 0x10)
				DESCRIPTOR(descriptor, descriptorIndex, REPORT_COUNT, simulationCount)
				DESCRIPTOR(descriptor, descriptorIndex, COLLECTION, COLLECTION_PHYSICAL)
			if (_useRudder)
			{
					DESCRIPTOR(descriptor, descriptorIndex, USAGE, SIMULATION_RUDDER)
			}
			if (_throttleCount > 0)
			{
				for (uint8_t i = 0; i < _throttleCount; i++)
				{
					DESCRIPTOR(descriptor, descriptorIndex, USAGE, SIMULATION_THROTTLE)
				}
			}
					DESCRIPTOR(descriptor, descriptorIndex, HIDINPUT, IO_DATA | IO_VARIABLE | IO_ABSOLUTE)
				DESCRIPTOR1(descriptor, descriptorIndex, END_COLLECTION)
		}
		DESCRIPTOR1(descriptor, descriptorIndex, END_COLLECTION)
	//END BUILD HID Report Descriptor

	// Create a copy of the HID Report Descriptor template that is just the right size
	uint8_t* hidReportDescriptor = new uint8_t[descriptorIndex];
	memcpy(hidReportDescriptor, descriptor, descriptorIndex);

	// Register HID Report Description
	DynamicHIDSubDescriptor* node = new DynamicHIDSubDescriptor(hidReportDescriptor, descriptorIndex, false);
	DynamicHID().AppendDescriptor(node);

	if (_buttonCount > 0)
	{
		_buttonValueArraySize = _buttonCount / 8;
		if ((_buttonCount % 8) > 0)
		{
			_buttonValueArraySize++;
		}
		_buttonValues = new uint8_t[_buttonValueArraySize];
	}
	if (_throttleCount > 0)
	{
		_throttleValues = new int16_t[_throttleCount];
	}
	_hidReportSize = _buttonValueArraySize;
	_hidReportSize += 4;//2 Bytes per Axis
	if (_useRudder)_hidReportSize += 2;//2 bytes for the rudder
	if (_throttleCount > 0)_hidReportSize += 2 * _throttleCount;//2 bytes per throttle
	if (_useBrakes) _hidReportSize += 4;//2 bytes per brake

	//Initialise
	_aileronAxis = 0;
	_elevatorAxis = 0;
	if (_useRudder) _rudderAxis = 0;
	if (_throttleCount)
	{
		for (int index = 0; index < _throttleCount; index++)
		{
			_throttleValues[index] = 0;
		}
	}
	if (_useBrakes)
	{
		_brakeLeftAxis = 0;
		_brakeRightAxis = 0;
	}
	if (_buttonCount > 0)
	{
		for (int index = 0; index < _buttonValueArraySize; index++)
		{
			_buttonValues[index] = 0;
		}
	}
}
void FlightControl_::Begin(bool startAutoSend)
{
	_autoSendState = startAutoSend;
	SendState();
}

void FlightControl_::End()
{
}

void FlightControl_::AddButton(int count)
{
	_buttonCount += count;
}

void FlightControl_::AddRudder()
{
	_useRudder = true;
}
void FlightControl_::AddBrakes()
{
	_useBrakes = true;
}
void FlightControl_::AddThrottle(bool reverseThrustButton)
{
	_throttleCount++;
	if (reverseThrustButton)
	{
		_buttonCount++;
	}
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
	if (_autoSendState) SendState();
}
void FlightControl_::releaseButton(uint8_t button)
{
	if (button >= _buttonCount) return;

	int index = button / 8;
	int bit = button % 8;

	bitClear(_buttonValues[index], bit);
	if (_autoSendState) SendState();
}
void FlightControl_::setAileronAxis(int16_t value)
{
	_aileronAxis = value;
	if (_autoSendState) SendState();
}
void FlightControl_::setElevatorAxis(int16_t value)
{
	_elevatorAxis = value;
	if (_autoSendState) SendState();
}
void FlightControl_::setRudder(int16_t value)
{
	_rudderAxis = value;
	if (_autoSendState) SendState();
}
void FlightControl_::setThrottle(uint8_t throttle,int16_t value)
{
	_throttleValues[throttle] = value;
	if (_autoSendState) SendState();
}
void FlightControl_::setBrakeLeft(int16_t value)
{
	_brakeLeftAxis = value;
	if (_autoSendState) SendState();
}
void FlightControl_::setBrakeRight(int16_t value)
{
	_brakeRightAxis = value;
	if (_autoSendState) SendState();
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
	if (_buttonCount > 0)
	{
		for (; index < _buttonValueArraySize; index++)
		{
			data[index] = _buttonValues[index];
		}
	}
	index += buildAndSet16BitValue(_aileronAxis, _aileronPhysicalMinimum, _aileronPhysicalMaximum, _aileronAxisLogicalMinimum, _aileronAxisLogicalMaximum, &(data[index]));
	index += buildAndSet16BitValue(_elevatorAxis, _elevatorPhysicalMinimum, _elevatorPhysicalMaximum, _elevatorAxisLogicalMinimum, _elevatorAxisLogicalMaximum, &(data[index]));
	if (_useBrakes)
	{
		index += buildAndSet16BitValue(_brakeLeftAxis,_brakeLeftPhysicalMinimum,_brakeLeftPhysicalMaximum,_brakeLeftLogicalMinimum,_brakeLeftLogicalMaximum,&(data[index]));
		index += buildAndSet16BitValue(_brakeRightAxis,_brakeRightPhysicalMinimum,_brakeRightPhysicalMaximum,_brakeRightLogicalMinimum,_brakeRightLogicalMaximum,&(data[index]));
	}
	if (_useRudder) { index += buildAndSet16BitValue(_rudderAxis, _rudderPhysicalMinimum, _rudderPhysicalMaximum, _rudderLogicalMinimum, _rudderLogicalMaximum, &(data[index])); }
	if (_throttleCount > 0)
	{
		for (uint8_t i = 0; i < _throttleCount; i++)
		{
			index += buildAndSet16BitValue(_throttleValues[i], _throttlePhysicalMinimum, _throttlePhysicalMaximum, _throttleLogicalMinimum, _throttleLogicalMaximum, &(data[index]));
		}
	}
	//END Build Data Packet
	DynamicHID().SendReport(_hidReportID, data, _hidReportSize);
}
#endif