#ifndef DEFINE_HID_h
#define DEFINE_HID_h

//GLOBAL ITEMS
#define _USAGE_PAGE(size) 	(0x04 | size)
#define _LOGICAL_MIN(size) 	(0x14 | size)
#define _LOGICAL_MAX(size) 	(0x24 | size)
#define _PHYSICAL_MIN(size) (0x34 | size)
#define _PHYSICAL_MAX(size) (0x44 | size)
#define _UNIT_EXP(size) 	(0x54 | size)
#define _UNIT(size) 		(0x64 | size)
#define _REPORT_SIZE(size) 	(0x74 | size)
#define _REPORT_ID(size)	(0x84 | size)
#define _REPORT_COUNT(size)	(0x94 | size)
#define _PUSH(size)			(0xA4 | size)
#define _POP(size)			(0xB4 | size)

#define USAGE_PAGE 		_USAGE_PAGE(1) 	
#define LOGICAL_MIN 	_LOGICAL_MIN(1) 	
#define LOGICAL_MAX 	_LOGICAL_MAX(1) 	
#define PHYSICAL_MIN 	_PHYSICAL_MIN(1) 
#define PHYSICAL_MAX 	_PHYSICAL_MAX(1) 
#define UNIT_EXP 		_UNIT_EXP(1) 	
#define UNIT 			_UNIT(1) 		
#define REPORT_SIZE 	_REPORT_SIZE(1) 	
#define REPORT_ID		_REPORT_ID(1)	
#define REPORT_COUNT	_REPORT_COUNT(1)	
#define PUSH			_PUSH(1)			
#define POP				_POP(1)			

//LOCAL ITEMS
#define _USAGE(size) 			(0x04 | size)
#define _USAGE_MIN(size) 		(0x14 | size)
#define _USAGE_MAX(size) 		(0x24 | size)
#define _DESIGNATOR_INDEX(size) (0x34 | size)
#define _DESIGNATOR_MIN(size) 	(0x44 | size)
#define _DESIGNATOR_MAX(size) 	(0x54 | size)
#define _STRING_INDEX(size) 	(0x74 | size)
#define _STRING_MIN(size) 		(0x84 | size)
#define _STRING_MAX(size) 		(0x94 | size)
#define _DELIMITER(size) 		(0xA4 | size)

#define USAGE 				_USAGE(1) 		
#define USAGE_MIN 			_USAGE_MIN(1) 	
#define USAGE_MAX 			_USAGE_MAX(1) 	
#define DESIGNATOR_INDEX 	_DESIGNATOR_INDEX(1)
#define DESIGNATOR_MIN 		_DESIGNATOR_MIN(1) 
#define DESIGNATOR_MAX 		_DESIGNATOR_MAX(1) 
#define STRING_INDEX 		_STRING_INDEX(1) 
#define STRING_MIN 			_STRING_MIN(1) 	
#define STRING_MAX 			_STRING_MAX(1) 	
#define DELIMITER 			_DELIMITER(1) 	

//INPUT / OUTPUT / FEATURE / COLLECTION
#define _INPUT(size) 		(0x80 | size)
#define _OUTPUT(size) 		(0x90 | size)
#define _COLLECTION(size) 	(0xA0 | size)
#define _FEATURE(size) 		(0xB0 | size)
#define _END_COLLECTION		(0xC0 | size)

#define INPUT 			_INPUT(1)
#define OUTPUT 			_OUTPUT(1)
#define COLLECTION 		_COLLECTION(1)
#define FEATURE 		_FEATURE(1)
#define END_COLLECTION 	_END_COLLECTION(0)

//INPUT / OUTPUT types
#define IO_DATA 			0x00
#define IO_CONSTANT 		0x01
#define IO_ARRAY 			0x00
#define IO_VARIABLE 		0x02
#define IO_ABSOLUTE 		0x00
#define IO_RELATIVE 		0x04
#define IO_NOWRAP 			0x00
#define IO_WRAP 			0x08
#define IO_LINEAR 			0x00
#define IO_NONLINEAR 		0x10
#define IO_PREFERRED 		0x00
#define IO_NOPREFERRED 		0x20
#define IO_NONULL 			0x00
#define IO_NULLSTATE 		0x40
#define IO_BUFFEREDBYTES 	0x0100

//COLLECTION TYPES
#define COLLECTION_PHYSICAL 		0x00
#define COLLECTION_APPLICATION 		0x01
#define COLLECTION_LOGICAL 			0x02
#define COLLECTION_REPORT 			0x03
#define COLLECTION_NAMEDARRAY 		0x04
#define COLLECTION_USAGESWITCH 		0x05
#define COLLECTION_USAGEMODIFIER 	0x06

//USAGE PAGES
#define PAGE_GENERIC_DESKTOP 		0x01
#define PAGE_SIMULATION_CONTROLS 	0x02
#define PAGE_VR_CONTROLS 			0x03
#define PAGE_SPORTS_CONTROLS		0x04
#define PAGE_GAME_CONTROLS			0x05
#define PAGE_GENERIC_DEVICE_CONTROLS 0x06
#define PAGE_KEYBOARD				0x07
#define PAGE_LED					0x08
#define PAGE_BUTTON					0x09
#define PAGE_ORDINAL				0x0A
#define PAGE_TELEPHONY				0x0B
#define PAGE_CONSUMER				0x0C
#define PAGE_DIGITIZER				0x0D
#define PAGE_HAPTICS				0x0E
#define PAGE_PID					0x0F
#define PAGE_UNICODE				0x10
#define PAGE_TRACKER				0x12
#define PAGE_AUX_DISPLAY			0x14
#define PAGE_SENSOR					0x20
#define PAGE_MEDICAL				0x40
#define PAGE_BRAILLE				0x41
#define PAGE_LIGHTING				0x59

#define GENERIC_POINTER		0x01
#define GENERIC_MOUSE		0x02
#define GENERIC_JOYSTICK	0x04
#define GENERIC_GAMEPAD		0x05
#define GENERIC_KEYBOARD	0x06
#define GENERIC_KEYPAD		0x07
#define GENERIC_MULTI_AXIS	0x08

#define GENERIC_X 			0x30
#define GENERIC_Y 			0x31
#define GENERIC_Z 			0x32
#define GENERIC_RX 			0x33
#define GENERIC_RY			0x34
#define GENERIC_RZ			0x35
#define GENERIC_SLIDER		0x36
#define GENERIC_DIAL		0x37
#define GENERIC_WHEEL		0x38
#define GENERIC_HAT			0x39
#define GENERIC_COUNTED_BUFFER	0x3A
#define GENERIC_BYTE_COUNT	0x3B
#define GENERIC_MOTION_WAKE	0x3C
#define GENERIC_START 		0x3D
#define GENERIC_SELECT 		0x3E
#define GENERIC_VX			0x40
#define GENERIC_VY			0x41
#define GENERIC_VZ			0x42
#define GENERIC_VBRX		0x43
#define GENERIC_VBRY		0x44
#define GENERIC_VBRZ		0x45
#define GENERIC_VNO			0x46
#define GENERIC_FEATURE_NOTE 0x47
#define GENERIC_RESOLUTION_MULT 0x48
#define GENERIC_QX			0x49
#define GENERIC_QY			0x4A
#define GENERIC_QZ			0x4B
#define GENERIC_QW			0x4C
#define GENERIC_THUMBSTICK	0x96

#define SIMULATION_FLIGHT 		0x01
#define SIMULATION_AUTOMOBILE 	0x02
#define SIMULATION_TANK 		0x03
#define SIMULATION_SPACESHIP	0x04
#define SIMULATION_SUBMARINE	0x05
#define SIMULATION_SAILING		0x06
#define SIMULATION_MOTORCYCLE	0x07
#define SIMULATION_SPORTS		0x08
#define SIMULATION_AIRPLANE		0x09
#define SIMULATION_HELICOPTER	0x0A
#define SIMULATION_MAGICCARPET	0x0B
#define SIMULATION_BICYCLE		0x0C
#define SIMULATION_FLIGHTCONTROLSTICK	0x20
#define SIMULATION_FLIGHTSTICK	0x21
#define SIMULATION_CYCLIC		0x22
#define SIMULATION_CYCLICTRIM	0x23
#define SIMULATION_FLIGHTYOKE	0x24
#define SIMULATION_TRACKCONTROL	0x25
#define SIMULATION_AILERON		0xB0
#define SIMULATION_AILERONTRIM	0xB1
#define SIMULATION_ANTITORQUE	0xB2
#define SIMULATION_AUTOPILOTENABLE	0xB3
#define SIMULATION_CHAFFRELEASE	0xB4
#define SIMULATION_COLLECTIVE	0xB5
#define SIMULATION_DIVEBRAKE	0xB6
#define SIMULATION_ELECTRONIC_COUNTERMEASURES 0xB7
#define SIMULATION_ELEVATOR		0xB8
#define SIMULATION_ELEVATORTRIM	0xB9
#define SIMULATION_RUDDER		0xBA
#define SIMULATION_THROTTLE		0xBB
#define SIMULATION_FLIGHTCOMS	0xBC
#define SIMULATION_FLARERELEASE	0xBD
#define SIMULATION_LANDINGGEAR	0xBE
#define SIMULATION_TOEBRAKE		0xBF
#define SIMULATION_TRIGGER		0xC0
#define SIMULATION_WEAPONS_ARM	0xC1
#define SIMULATION_WEAPONS_SELECT	0xC2
#define SIMULATION_WINGFLAPS	0xC3
#define SIMULATION_ACCELERATOR	0xC4
#define SIMULATION_BRAKE		0xC5
#define SIMULATION_CLUTCH		0xC6
#define SIMULATION_SHIFTER		0xC7
#define SIMULATION_STEERING		0xC8
#define SIMULATION_TURRET_DIRECTION	0xC9
#define SIMULATION_BARREL_ELEVATION	0xCA
#define SIMULATION_DIVE_PLANE	0xCB
#define SIMULATION_BALLAST	0xCC
#define SIMULATION_BICYCLE_CRANK	0xCD
#define SIMULATION_HANDLEBARS	0xCE
#define SIMULATION_FRONTBRAKE	0xCF
#define SIMULATION_REARBRAKE	0xD0



//Add a value pair to a descriptor
#define DESCRIPTOR(_array,_index,label,value) _array[_index++] = label;_array[index++] = value;
#define DESCRIPTOR1(_array,_index,label) _array[_index++] = label;
#define DESCRIPTOR3(_array,_index,label,value1,value2) _array[_index++] = label; _array[index++] = value1;_array[index++] = value2;

#endif