import numpy as np
import pyglobus

####################
# Read SHT example #
####################

# 0. Create reader

sht_reader = pyglobus.util.ShtReader("../../data/test/sht-reader/sht38515.sht")

# 1. Read signal

signal = sht_reader.get_signal(26)

# 2. Construct a view for OY data (without copying)

signal_data_y = signal.get_data_y()

# 3. Output some data

print(signal_data_y[0:10])

# 4. Operate with signal names instead of numbers

# This will be a bytes object
signal_name = sht_reader.get_signal_name(26)

# Signal name is encoded in cp1251
print(signal_name.decode("cp1251"))

# Signal name can be used to get the signal itself
# Note: since signal names are noy unique, this method
# returns a list of signals which have the given name.
signal = sht_reader.get_signals(signal_name)

signal_data_y = signal[0].get_data_y()

print(signal_data_y[0:10])
