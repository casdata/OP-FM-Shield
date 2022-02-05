# OP-FM-Shield
FM Synthesizer and VGM player for the Arduino Due

The OP-FM Shield is an FM synthesizer and VGM player for the Arduino Due development board, it’s powered by the YMF262 (OPL3) sound chip from Yamaha. Is compatible with any keyboard with a 5 pin MIDI OUT port.

In Synth mode, the YMF262 can work in OPL2 or OPL3 mode depending on the selected voice, a maximum of 12 keys can be pressed simultaneously in OPL2 and 6 keys for OPL3. New OPL2 and OPL3 voices can be created from scratch, also the Included voices in the firmware can be modified, the user has access to almost all the parameters of the YMF262, why almost all? The five native rhythms are not available. 

Synth mode has two options on how the keyboard can be used, in the first option (first screen) the synth uses all the keyboards for the selected voice, in the second option, the keyboard is divided into two, one-half keyboard for red voice and the other half for the green voice, an OPL3 voice can only be selected in the red voice half and the maximum number of simultaneous keys are downgraded to five. 

In VGM player mode the keyboard is disabled, all the VGM songs are readed from the SD card. This mode also has the option to extract the voices of the VGM, they’re stored in a temporary folder that can be accessed from synth mode, those extracted voices must be saved manually or they’re deleted when other voices are extracted from a VGM.

VGM files for the OPL, OPL2 and OPL3 are compatible with the OP-FM, songs can be found on the internet, but before the .vgm file is transferred to the “vgm” folder in the SD card, the file must be extracted using the 7-zip app (https://www.7-zip.org).

And the best part above all is that software and hardware parts of the OP-FM Shield are open source, enjoy!


### Recommended VGM websites: 
* https://opl.wafflenet.com
* https://vgmrips.net/packs/chip/ymf262


## Schematic Diagram
![Schematic](https://github.com/casdata/OP-FM-Shield/blob/main/Hardware/Schematic_DueFMSynth.png)


## IMPORTANT
The shield works either with the original or the Chinese clones Arduino Due boards, but it’s recommended to use the original due to some clone boards (almost all) comes with an issue in the power supply circuit that induces noise into the MIDI port when powering it over the power jack, also in the clone boards the shield doesn't boot until the reset button in the Arduino board is pressed after its connected to a power source.





## Contact

Sergio Daniel Castañeda N. - [@twitter_handle](https://twitter.com/CasdataSergio) - casdata@gmail.com

Website: http://casnisoft.weebly.com



