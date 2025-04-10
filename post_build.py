Import("env")
import shutil
import os
import glob
import datetime

def postBuild(source, target, env):
    print("Running postBuild script:")
    binFile = target[0].get_abspath()
    print("Bin file Path:", binFile)
    projectDir = os.getcwd()
    outDir = os.path.join(projectDir, "out")
    print("Project directory: " + projectDir)
    print("Out directory: " + outDir)
    
    # Get current date and time and format to 202311140442 (YYYYDDMMHHMM)
    now = datetime.datetime.now()
    timeStr = now.strftime("%Y%m%d%H%M")
    
    outFile = os.path.join(outDir,"bfk6-"+timeStr+ ".bin")

    # create out directory if it doesn't exist
    if not os.path.exists(outDir):
        os.makedirs(outDir)

    # Remove all files in out directory
    files = glob.glob(outDir + "/bfk6-*.bin")
    for f in files:
        os.remove(f)

    # Copy bin file to out directory
    shutil.copy(binFile, outFile) 


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", postBuild)