#!/usr/bin/env python3
from pyocd.core.helpers import ConnectHelper
from pyocd.flash.file_programmer import FileProgrammer
from pyocd.debug.elf.symbols import ELFSymbolProvider
from pyocd.core.target import Target
from pyocd.debug.elf.elf  import ELFBinaryFile
from pyocd.flash.loader import MemoryLoader
import getserial

import time
import os.path

import serial
import re
import io

import logging
logging.basicConfig(level=logging.ERROR)



def run_out(exe_path,uuid):
    lines= ""
    
    with ConnectHelper.session_with_chosen_probe(unique_id = uuid) as session:
        print("Connecting")
        board = session.board
        target = board.target
        #flash = target.memory_map.get_boot_memory()
        
        # Load firmware into device.
        FileProgrammer(session).program(exe_path)
        
        #target.elf = elf_path
    
        
        #provider = ELFSymbolProvider(target.elf)
        #main_addr = provider.get_symbol_value("main")
        #print("main() address: 0x%X" % main_addr)
        
        ## Set breakpoint.
        #target.set_breakpoint(main_addr)
    
        #target.reset()
        lines = getserial.read_stdout(target)
        return("".join(lines))
        #target.resume()
        ##
        ##
        #target.reset()
        ##
        ### Wait until breakpoint is hit.
        #while target.get_state() != Target.State.HALTED:
        #    pass
        ##
        #pc = target.read_core_register("pc")
        #print("pc: 0x%X" % pc)
    #
        #target.remove_breakpoint()
    #
        #target.resume()

if __name__ == "__main__":
   path = "."
   out = "cprj/out/test/MPS3-Corstone-300"
   bin = "Release/test.axf"
   
   axf_path = os.path.join(path,out,bin)
   
   #axf=ELFBinaryFile(axf_path)
   #axf.close()

   lines = run_out(axf_path,"L85986697A")
   

   print(lines)

    