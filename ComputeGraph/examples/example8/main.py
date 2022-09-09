import sched as s 
from custom import *

# Only ONE FileSink can be used since the data will be dumped
# into this global buffer for display with Matplotlib
# It will have to be cleaned and reworked in future to use better
# mechanism of communication with the main code

print("Start")
nb,error = s.scheduler("test")
print("Nb sched = %d" % nb)

