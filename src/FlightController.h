#ifndef FLIGHTCONTROL_h
#define FLIGHTCONTROL_h

//#define _USING_DYNAMIC_HID//Comment this OUT before checking in

#include "DynamicHID/DynamicHID.h"
#include "DynamicHID/DefineHID.h"

#if ARDUINO < 10606
#error The Flight Control library requires Arduino IDE 1.6.6 or greater. Please update your IDE.
#endif // ARDUINO < 10606

#if ARDUINO > 10606
#if !defined(USBCON)
#error The Flight Control library can only be used with a USB MCU (e.g. Arduino Leonardo, Arduino Micro, etc.).
#endif // !defined(USBCON)
#endif // ARDUINO > 10606

#if !defined(_USING_DYNAMIC_HID)

#warning "Using legacy HID core (non pluggable)"

#else // !defined(_USING_DYNAMIC_HID)

//================================================================================

#define FLIGHTCONTROL_DEFAULT_REPORT_ID         0x03
#define FLIGHTCONTROL_AUX_REPORT_ID							(FLIGHTCONTROL_DEFAULT_REPORT_ID+1)
#define FLIGHTCONTROL_AUX2_REPORT_ID						(FLIGHTCONTROL_DEFAULT_REPORT_ID+1)
#define FLIGHTCONTROL_DEFAULT_BUTTON_COUNT        32
#define FLIGHTCONTROL_DEFAULT_AXIS_MINIMUM         0
#define FLIGHTCONTROL_DEFAULT_AXIS_MAXIMUM      1023
#define FLIGHTCONTROL_DEFAULT_SIMULATOR_MINIMUM    0
#define FLIGHTCONTROL_DEFAULT_SIMULATOR_MAXIMUM 1023
#define FLIGHTCONTROL_DEFAULT_HATSWITCH_COUNT      2
#define FLIGHTCONTROL_HATSWITCH_COUNT_MAXIMUM      2
#define FLIGHTCONTROL_HATSWITCH_RELEASE           -1
#define FLIGHTCONTROL_TYPE_JOYSTICK             0x04
#define FLIGHTCONTROL_TYPE_GAMEPAD              0x05
#define FLIGHTCONTROL_TYPE_MULTI_AXIS           0x08
#define FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM		-32767
#define FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM		32767

class FlightControl_
{
private:
	// FlightController State
	int16_t	        _aileronAxis;
	int16_t	        _elevatorAxis;
	int16_t         _rudderAxis;
	int16_t					_brakeLeftAxis;
	int16_t					_brakeRightAxis;
	//int16_t	      _hatSwitchValues[FLIGHTCONTROL_HATSWITCH_COUNT_MAXIMUM];
	uint8_t*				_buttonValues = NULL;
	// Flight Controller Settings
	bool            _autoSendState;
	bool						_useRudder;
	bool						_useBrakes;
	int							_throttleCount;
	int16_t*				_throttleValues = NULL;

	uint8_t         _buttonCount;
	uint8_t         _buttonValueArraySize = 0;
	uint8_t					_hatSwitchCount;
	//uint8_t				_includeAxisFlags;
	//uint8_t				_includeSimulatorFlags;

	int16_t         _aileronPhysicalMinimum = FLIGHTCONTROL_DEFAULT_AXIS_MINIMUM;
	int16_t         _aileronPhysicalMaximum = FLIGHTCONTROL_DEFAULT_AXIS_MAXIMUM;
	int16_t					_aileronAxisLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t					_aileronAxisLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	int16_t         _elevatorPhysicalMinimum = FLIGHTCONTROL_DEFAULT_AXIS_MINIMUM;
	int16_t         _elevatorPhysicalMaximum = FLIGHTCONTROL_DEFAULT_AXIS_MAXIMUM;
	int16_t         _elevatorAxisLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t         _elevatorAxisLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	int16_t         _rudderPhysicalMinimum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MINIMUM;
	int16_t         _rudderPhysicalMaximum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t         _rudderLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t         _rudderLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	int16_t         _throttlePhysicalMinimum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MINIMUM;
	int16_t         _throttlePhysicalMaximum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t         _throttleLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t         _throttleLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	int16_t         _brakeLeftPhysicalMinimum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MINIMUM;
	int16_t         _brakeLeftPhysicalMaximum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t         _brakeLeftLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t         _brakeLeftLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	int16_t         _brakeRightPhysicalMinimum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MINIMUM;
	int16_t         _brakeRightPhysicalMaximum = FLIGHTCONTROL_DEFAULT_SIMULATOR_MAXIMUM;
	int16_t         _brakeRightLogicalMinimum = FLIGHTCONTROL_LOGICAL_AXIS_MINIMUM;
	int16_t         _brakeRightLogicalMaximum = FLIGHTCONTROL_LOGICAL_AXIS_MAXIMUM;

	uint8_t         _hidReportID;
	uint8_t         _hidReportSize;
	uint8_t					_controller;
protected:

	int buildAndSet16BitValue(int16_t value, int16_t valueMinimum, int16_t valueMaximum, int16_t actualMinimum, int16_t actualMaximum, uint8_t dataLocation[]);

public:
	FlightControl_(uint8_t hidReportID = FLIGHTCONTROL_DEFAULT_REPORT_ID,uint8_t controller = 0);
	void Initialise();
	void Begin(bool startAutoSend = true);
	void End();
	// Set Range Functions
	inline void setAileronRange(int16_t minimum, int16_t maximum)
	{
		_aileronPhysicalMinimum = minimum;
		_aileronPhysicalMaximum = maximum;
	}
	inline void setElevatorRange(int16_t minimum, int16_t maximum)
	{
		_elevatorPhysicalMinimum = minimum;
		_elevatorPhysicalMaximum = maximum;
	}
	inline void setRudderRange(int16_t minimum, int16_t maximum)
	{
		_rudderPhysicalMinimum = minimum;
		_rudderPhysicalMaximum = maximum;
	}
	inline void setThrottleRange(int16_t minimum, int16_t maximum)
	{
		_throttlePhysicalMinimum = minimum;
		_throttlePhysicalMaximum = maximum;
	}
	inline void setBrakeLeftRange(int16_t minimum, int16_t maximum)
	{
		_brakeLeftPhysicalMinimum = minimum;
		_brakeLeftPhysicalMaximum = maximum;
	}
	inline void setBrakeRightRange(int16_t minimum, int16_t maximum)
	{
		_brakeRightPhysicalMinimum = minimum;
		_brakeRightPhysicalMaximum = maximum;
	}


	// Set Axis Values
	void setAileronAxis(int16_t value);
	void setElevatorAxis(int16_t value);

	// Set Simuation Values
	void setRudder(int16_t value);
	void setThrottle(uint8_t throttle,int16_t value);
	void setBrakeLeft(int16_t value);
	void setBrakeRight(int16_t value);
	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	//void setHatSwitch(int8_t hatSwitch, int16_t value);

	void SendState();
};

#endif // !defined(_USING_DYNAMIC_HID)
#endif // FLIGHTCONTROL_h