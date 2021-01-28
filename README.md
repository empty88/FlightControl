# FlightControl
HID Flight Simulator controller library for Arduinio. Heavily inspired by Matthew Hieronimus' Joystick Library

For this library to work. You need to make the following changes to your arduino core USB code to add some required functionality

1. Arduino\IDE\Arduino\hardware\arduino\avr\cores\arduino\USBAPI.h
   Find the line that reads "int USB_SendControl(u8 flags, const void* d, int len);" #197 for me
   ADD the line "int USB_SendControl(u8 d);" above it

2. Arduino\IDE\Arduino\hardware\arduino\avr\cores\arduino\USBCore.cpp
   Find the line that reads "int USB_SendControl(u8 flags, const void* d, int len)" #411 for me
   ADD the function:
   int USB_SendControl(u8 d)
   {
     return SendControl(d) ? 1 : 0;
	 }