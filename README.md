# Volume and Balance Plug-in
Audio plug-ins are applications that are used in digital audio workstations (DAW), with the most popular ones being Ableton, Logic, and Studio One. Plug-ins modify incoming signals and output the result, depending on the type of effect. Here, I demonstrate an extremely simple volume and balance plug-in that can edit the gain and pan of a sound. 

This was written using JUCE library found on their [website](https://docs.juce.com/master/index.html). They provide the framework necessary to easily import and export the program into a .vst3

### Function
Volume is the loudness of a sound, and has units in dB. Since dB is measured in a logarithmic scale, the program processes incoming audio discretely and applies a formula for the appropriate output. The range of the volume slider is from -60 dB to +10 dB. The slider has also been scaled logarithmically so that the user has more control at higher volume levels. 

Balance is the position of the sound. Usually, a stereo signal is input, meaning the left and right channels both have an incoming signal. The program applies a function to these signals depending on the knob location to pan the volume to either the left, the right, or a mix of both. This is calculated on scale from -50 to 50, with -50 being the far left and 50 being the far right.

### Usage
Typically audio plug-ins are used in DAWs, and require the file format .vst3, which I've included. I also attached the standalone plug-in for testing purposes. The .cpp files to create the plug-in is included in the source folder.

## GUI

![GUI](https://github.com/clifflinrichie/VolumePlugIn/blob/master/GUI.PNG)
Volume and Balance Plug-in

![Test Image](https://github.com/clifflinrichie/VolumePlugIn/blob/master/GUI.PNG)
