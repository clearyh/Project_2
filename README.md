# Project_2
Project 2 for ECE-218 W24 at Union College
Harmon Cleary and Papa Ndiaye

Code used to create an automatic headlight control system. Using a combination of external and internal components from the Nucleo-f429zi board, the electronics of a car were mimicked. The code operates these components to mimic a conditional car-ignition system, low beams, high beams with an independent switch, and a headlight setting dial. The headlights can be set to three settings: On, Off, or Auto. If the Auto setting is used a light sensor is used to determine the headlight behavior.

SUMMARY OF TEST RESULTS:

IGNITION SUBSYSTEM

Start the engine (i.e., light the blue LED) while the driver’s seat is occupied and when the ignition button is pushed and then released.
Pass


Keep the engine running even if the driver should exit the vehicle.
Pass


When the engine is running, stop the engine when the ignition button is pushed (i.e., before the button is released).
Pass



HEADLIGHT SUBSYSTEM
If the engine is running, and the user selects: ON, turn on both low beam lamps; OFF, turn off all lamps.
Pass


AUTO: When the light level is above a “daylight” level, switch headlights off after a delay of 2 seconds.
Pass


AUTO: When the light level is below a “dusk” level, switch headlights on after a delay of 1 second.
Pass


AUTO: When between these two levels, the headlights should hold their previous state. 
Pass


Turn off all headlights when the engine is off. 
Pass


High beam headlights controlled by switch (when engine running)
Pass




