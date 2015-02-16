#-------------------------------------------------------------------------------
# Name:        midi2ino
# Purpose:     Import midi files into Arduino sketches to play the music
#
# Author:      Alex Porto
#
# Created:     16/02/2015
# Copyright:   (c) aporto 2015
# Licence:     GPlv3

# Note: This code requires the following lib installed on Python 2.7
# https://github.com/vishnubob/python-midi
#-------------------------------------------------------------------------------

"""
Print a description of a MIDI file.
"""
import midi
import sys

#if len(sys.argv) != 2:
#    print "Usage: {0} <midifile>".format(sys.argv[0])
#    sys.exit(2)

#midifile = sys.argv[1]
midifile = "C:\\dados\\audio\\midi\\rush - tom sawyer - synth.mid"
pattern = midi.read_midifile(midifile)
print repr(pattern)

