import numpy as np
import pyglobus

####################
# Read SHT example #
####################

# 0. Create reader

sht_reader = pyglobus.ShtReader("../../data/test/sht-reader/sht38515.sht")

# 1. Read signal

signal75 = sht_reader.get_signal(75)

# 2. Construct a view witn int32 type (without copying)

signal75_view = signal75.data.view("int32")

# 3. Output some data
# Should be: [8700729 8700344 8702642 8707400 8709699 8708960 8707735 8706932 8706026 8706328]

print(signal75_view[0:10])
