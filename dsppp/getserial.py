import serial
import re
import io
from pyocd.core.target import Target

lines = []

def read_stdout(target):
    print("Waiting for serial")
    lines = []

    with serial.Serial('COM6', 115200, timeout=1,parity=serial.PARITY_NONE) as ser:
        sio = io.TextIOWrapper(ser)
        DONE = False
        target.reset()
        while not DONE:
            line = sio.readline()
            if len(line)==0:
                raise Exception('Timeout error')
            if re.match(r'Stats',line):
                DONE=True 
            else:
                #print(line)
                lines.append(line)
                        
           
    return(lines)
    