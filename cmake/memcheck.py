#!/usr/bin/python

import sys
import subprocess
import xml.etree.ElementTree


try:
    from subprocess import DEVNULL
except ImportError:
    import os
    DEVNULL = open(os.devnull, 'wb')
        

def printDebug(text):
    print "   "+text
    
def printInfo(text):
    print text

def analyze_DefinitelyLost(errorTag):
    xwhatTag = errorTag.find("xwhat")
    if xwhatTag is None:
        return
    leakedbytesTag = xwhatTag.find("leakedbytes")
    if leakedbytesTag is None:
        return
    
    printDebug("Lost "+leakedbytesTag.text+" bytes")
    stackTag = errorTag.find("stack")
    
    return int(leakedbytesTag.text)
    
def analyze_InvalidFree(errorTag):
    printDebug("Invalid Free")
    return 1
    
def analyze_InvalidRead(errorTag):
    printDebug("InvalidRead")
    return 1
    
def analyze_InvalidWrite(errorTag):
    printDebug("InvalidWrite")
    return 1


if __name__ == '__main__':

    
    if (sys.argv) <= 1:
        sys.ext(1)
    
    print "memtest: ",sys.argv[1]
    
    totalLostBytes = 0
    totalInvalidFrees = 0
    totalInvalidReads = 0
    totalInvalidWrites = 0
    
    p = subprocess.Popen(
        [
            "valgrind",
            "--tool=memcheck",
            "--leak-check=summary",
            "--xml=yes",
            "--xml-file=memcheck.xml",
            sys.argv[1],
            
        ],
        stdout=DEVNULL, 
        stderr=DEVNULL
    )
    p.wait()
    
    if p.returncode!=0:
        print "Valgrind exited with status "+str(p.returncode)+". Memleak analysis not run."
        sys.exit(1)
    
    e = xml.etree.ElementTree.parse('memcheck.xml').getroot()
    for errorTag in e.findall('error'):
        kindTag = errorTag.find("kind")
        if kindTag is None:
            continue
        if kindTag.text=="Leak_DefinitelyLost":
            totalLostBytes+=analyze_DefinitelyLost(errorTag)
        if kindTag.text=="InvalidFree":
            totalInvalidFrees+=analyze_InvalidFree(errorTag)
        if kindTag.text=="InvalidRead":
            totalInvalidReads+=analyze_InvalidRead(errorTag)
        if kindTag.text=="InvalidWrite":
            totalInvalidWrites+=analyze_InvalidWrite(errorTag)
        
    printInfo("Total lost: "+str(totalLostBytes)+" bytes")
    printInfo("Total invalid frees: "+str(totalInvalidFrees))
    printInfo("Total invalid reads: "+str(totalInvalidReads))
    printInfo("Total invalid writes: "+str(totalInvalidWrites))
    
    if (totalLostBytes+totalInvalidFrees+totalInvalidReads+totalInvalidWrites)>0:
        sys.exit(1)
    sys.exit(0)
        
