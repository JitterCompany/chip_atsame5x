====================================
Position Decoder Asynchronous driver
====================================

The Position Decoder (PDEC) can be used to detect movement of an motor axis.
This module can be used for a wide array of applications.
The driver supports motor axis position, and support position window monitor.
The window monitor can be used to monitor axis position and compare to predefined threshold.
Callback function can be registered to overflow interrupt, direction interrupt,
position changed interrupt, and error interrupt. Once the condition happens, the callback
function is invoked to notify the application.

Features
--------

* Initialization and de-initialization
* Staring and Stopping
* Reading position
* Setting threshold for position
* Monitoring direction

Applications
------------
* Monitoring the movement of motor axis

Dependencies
------------
* Position Decoder capable hardware

Concurrency
-----------
N/A

Limitations
-----------
* Axis speed measurement must work with timer

Known issues and workarounds
----------------------------
N/A
