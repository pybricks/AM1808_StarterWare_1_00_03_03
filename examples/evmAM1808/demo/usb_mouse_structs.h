//*****************************************************************************
//
// usb_mouse_structs.h - Data structures defining the mouse USB device.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of AM1808 StarterWare firmware package, modified and reused
// from revision 6288 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#ifndef _USB_MOUSE_STRUCTS_H_
#define _USB_MOUSE_STRUCTS_H_

extern unsigned int MouseHandler(void *pvCBData,
                                  unsigned int ulEvent,
                                  unsigned int ulMsgData,
                                  void *pvMsgData);

extern const tUSBDHIDMouseDevice g_sMouseDevice;

#endif
