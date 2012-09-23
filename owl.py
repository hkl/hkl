import argparse           #parse cmd line arg flags
from os import walk       #will allow for traversing files in the test directory
from os import getcwd     #get the working directory
from os.path import join  #create directories on all systems
from os.path import isdir #check for directories existing
from os import makedirs   #allow creating folders if needed
import errno              #for checking errors
from json import loads, dumps    #for parsing our test files and creating our out files
import subprocess 
from re import match      #finding if a file is a valid .test file
from re import sub        #convert .test file into a .result file
from difflib import Differ #allows us to diff our inputs and outputs
import datetime

##################################################
#                  GLOBALS                       #
##################################################
testPath = join(getcwd(), "owl", "tests")
expectedPath = join(getcwd(), "owl", "expected")
resultsPath = join(getcwd(), "owl", "results")
currentResults = join(resultsPath, datetime.datetime.now().strftime("%m-%d-%y(%Hh%Mm%Ss)"))
##################################################


###############################################################################################
# This is where the tests are actually run.
# The caller needs to provide a function that will take the following
# * A Test OBJ as defined by the JSON files
# * An output tuple [0] = stdout, [1] = stderr
# * An output file that will contain the json for the expected output
###############################################################################################
def Execute(ResultMethod, testsToRun):
  for filename, testInfo in testsToRun:
    result = join(expectedPath, sub(r"(.*\.)test", r"\1result", filename))#create result file name from test file name

    #since subprocess wants one list, we prepend the process to the list of args
    runData = list(testInfo['args'])
    runData.insert(0, testInfo['binary'])

    try:
      testProcess = subprocess.Popen(runData, stdout =subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
    except Exception as err:
      yield({'StderrMatch': False, 'StdoutMatch': False}, testInfo)
      continue

    outTuple = testProcess.communicate(bytes(testInfo['input'], "UTF-8"))#need bytes here because of diff between strings in python 3.x and 2.7

    yield (ResultMethod(testInfo, outTuple, result), testInfo)
###############################################################################################


###############################################################################################
# Takes a list of test files and reads the json
# objects from it and returns the test file and the json
# from that file
###############################################################################################
def ParseTests(filenames):
  tests = []
  for filename in filenames:
    test = open(join(testPath, filename))#open the test file (should be there or we wouldn't be here from walk)
    testContent = test.read()
    testInfo = loads(testContent)
    test.close()
    tests.append((filename, testInfo))
  return tests
###############################################################################################


###############################################################################################
# This function will find all the test files in the owl test directory.
# It creates the list and return it, the default if this program is run as main
###############################################################################################
def GetAllFiles():
  files = []
  for dirpath, dirnames, filenames in walk(testPath):
    for filename in filenames:
      if match(r".*\.test", filename):#skip non test files
        files.append(filename)
  return files
###############################################################################################


###############################################################################################
# Will take the xpected output and determine if they match
# This is the default function of the program
###############################################################################################
def DisplayResult(testObj, outTuple, outFile):
  try:#attempt to open a result file
    outFile_Read = open(outFile, 'r')
  except Exception as err: #some error happens while attempting to get a result, inform user
    return {"StdoutMatch": False, "StderrMatch": False}
  
  stdoutMatch = True
  stderrMatch = True

  #initialize some values
  diffObj = Differ()#apparently we can use a single differ object
  outFile_Obj = loads(outFile_Read.read())#parse output file
  outFile_Read.close()
  realStdOut = outTuple[0].decode("UTF-8").splitlines(1)#outTuple has old strings, we need to decode
  expectedStdOut = outFile_Obj['stdout'].splitlines(1)#the JSON parser returns 3.x strs, no decode
  realStdErr = outTuple[1].decode("UTF-8").splitlines(1)#outTuple has old strings, we need to decode
  expectedStdErr = outFile_Obj['stderr'].splitlines(1)#the JSON parser returns 3.x strs, no decode
  stdOutDiffs = diffObj.compare(realStdOut, expectedStdOut)
  stdErrDiffs = diffObj.compare(realStdErr, expectedStdErr)

  #differs output sucks, use DetectDiffs to clean it up and make it readable
  stdOutResults = DetectDiffs(stdOutDiffs)
  stdErrResults = DetectDiffs(stdErrDiffs)

  if len(stdOutResults) > 0 or len(stdErrResults) > 0:#if we have some type of failed test
    currentResultDir = join(currentResults, testObj['name'])
    SafeMakeDir(currentResultDir)

    stdOutRaw = open(join(currentResultDir, "stdout.txt"), 'w')
    stdOutRaw.write(outTuple[0].decode("UTF-8"))
    stdOutRaw.close()

    stdErrRaw = open(join(currentResultDir, "stderr.txt"), 'w')
    stdErrRaw.write(outTuple[1].decode("UTF-8"))
    stdErrRaw.close()

    diffsOut = open(join(currentResultDir, "diff.txt"), 'w')

    if len(stdOutResults) > 0:#output the diff for stdout to the file stdoutMatch = False
      diffsOut.write("STDOUT RESULTS\n")
      diffsOut.write("------------------------------------\n")
      for line in stdOutResults:
        diffsOut.write(line)

    if len(stdErrResults) > 0:#output the diff for the stderr to the file
      stderrMatch = False
      diffsOut.write("STDERR RESULTS\n")
      diffsOut.write("------------------------------------\n")
      for line in stdErrResults:
        diffsOut.write(line)

    diffsOut.close()
    passed = False
  
  return {"StdoutMatch": stdoutMatch, "StderrMatch": stderrMatch}
###############################################################################################


###############################################################################################
#Differ has shitty output, instead of using it, we can clean it up
#This function will provide context for each diff
#This includes the output line number
###############################################################################################
def DetectDiffs(diffs):
  lineno = 1
  diffstrings = []                              
  for l in diffs:
    if l[0] != ' ': #space as first char indicates common line
      diffGroup = True
      if l[0] == '-':#- indicates output file line
        diffstrings.append('output\n')
      elif l[0] == '+': #+ indicates result file line
        diffstrings.append('expected\n')

      if l[0] != '?': # a non ? char indicates an actual line, print line no
        diffstrings.append(str(lineno) + ':' + l[2:])
      else: #other lines are generated, just print them (don't print their marker or line no)
        diffstrings.append(" " + l[1:])
    if l[0] == ' ' or l[0] == '+':#if lines matched or we just printed the expected line, inc lin no
      lineno += 1 
  return diffstrings
###############################################################################################


###############################################################################################
# This is an alternative functiont o be run by execute, it generates output files
# instead of diffing them
###############################################################################################
def GenerateResultFiles(testObj, outTuple, outFile):
  outFile_Write = open(outFile, 'w')
  #we need to decode because the subprocess tuple of outTuple is binary encoded
  outFile_Write.write(dumps({"stdout": outTuple[0].decode("UTF-8"), "stderr":outTuple[1].decode("UTF-8")}, indent=4))
  return True
###############################################################################################

###############################################################################################
# This function ensures that we have some legal directories for
# The entire framework to work correctly
###############################################################################################
def SafeMakeDir(path):
  try:
    makedirs(path)
  except OSError as err:
    if err.errno == errno.EEXIST:
      pass#we are ok, the file exists, keep calm and carry on
    else:
      raise#lets have someone handle this issue if needed?
###############################################################################################


###############################################################################################
# This function will ensure that the owl environment is set up at the minimum. 
# It creates the base directories that are needed by Owl
###############################################################################################
def Init():
  #lets try to create our files (should be there but if not just create them, only one fs call)
  SafeMakeDir(testPath)
  SafeMakeDir(expectedPath)
  SafeMakeDir(resultsPath)
  SafeMakeDir(currentResults)
###############################################################################################
  

if __name__=="__main__":
  Init()
  parser = argparse.ArgumentParser(description="A utility for testing executables")
  parser.add_argument("--generate", dest="method", default=DisplayResult, help="Triggers generation of new result files", action='store_const', const=GenerateResultFiles)
  args = parser.parse_args()
  for result in Execute(args.method, ParseTests(GetAllFiles())):
    if result[0]['StdoutMatch'] and result[0]['StderrMatch']:  
      print("[O]" + result[1]['name'] + " passed")
    else:
      errorStr = " "
      if not result[0]['StdoutMatch']:
        errorStr += "stdout error "
      if not result[0]['StderrMatch']:
        errorStr += "stderr error"
      print("[X]" + result[1]['name'] + errorStr)
