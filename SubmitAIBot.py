from datetime import datetime
import json
import sys
import os, errno
from subprocess import call
import shutil



def DelTempFile():

    try:
        os.remove(tempjobfilename)
    except OSError:
        pass
    return

def GenerateJobFilename():
    jobfilename = datetime.now().strftime('%Y-%m-%d-%H-%M-%S') + "_" + target + "_" + teamname + ".job"
    return jobfilename

tempjobfilename = "jobfiles7z.txt";

teamname = sys.argv[1]
target = sys.argv[2]
jobsDir = sys.argv[3] + "\jobs"
sevenZipDir = sys.argv[4]

#print("teamname = " + teamname)
#print("target = " + target)
#print("jobsDir = " + jobsDir)
#print("sevenZipDir = " + sevenZipDir)

jobfilename = GenerateJobFilename()

DelTempFile()

with open('jobfiles.txt','r') as f:
    filelist = f.read().split(';')

    with open('jobfiles7z.txt','w') as fzip:
        for filename in filelist:
            fzip.write(filename + '\n');
    fzip.closed
f.closed

if os.path.isfile(tempjobfilename):
    call([sevenZipDir + "/7z", "a", "-tzip", jobfilename, "@" + tempjobfilename])    

DelTempFile()

if os.access(jobsDir, os.W_OK) and os.path.isfile(jobfilename):
    shutil.copy2(jobfilename, jobsDir)

print()
print(jobfilename)

