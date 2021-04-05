======================================
The Frequency Meter Synchronous driver
======================================

The Frequency Meter driver provides means to measure the frequency or the period
of input signal.

The driver uses direct method of frequency measurement, that means it counts the
amount of low-to-high transitions in the input signal during a period of time.
The lower the measured frequency, the longer the measurement period should be for
higher accuracy.

Features
--------
* Initialization and de-initialization
* Enabling and disabling
* Frequency or period measurement
* Different measurement periods

Applications
------------
* Measuring frequency/period, speed, rate or acceleration.

Dependencies
------------
* Frequency measure capable hardware

Concurrency
-----------
N/A

Limitations
-----------
* The driver is incapable to use reserve mode of frequency measurement.
* Only one input signal to measure

Knows issues and workarounds
----------------------------
N/A
