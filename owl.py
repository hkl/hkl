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

def Execute(ResultMethod):
  passed = 0
  failed = 0
  
  for dirpath, dirnames, filenames in walk(join(testPath)):
    for filename in filenames:

      if not match(r".*\.test", filename):#skip non test files
        continue

      test = open(join(testPath, filename))#open the test file (should be there or we wouldn't be here from walk)
      result = join(expectedPath, sub(r"(.*\.)test", r"\1result", filename))#create result file name from test file name
      testContent = test.read()
      testInfo = loads(testContent)
      test.close()
      testInfo["args"].insert(0, testInfo['binary'])#since subprocess wants one list, we prepend the process to the list of args

      try:
        testProcess = subprocess.Popen(testInfo['args'], stdout =subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
      except Exception as err:
        print("[X]" + testInfo['test'] + " failed due to bad binary provided error:" + str(err))
        failed += 1
        continue

      outTuple = testProcess.communicate(bytes(testInfo['input'], "UTF-8"))#need bytes here because of diff between strings in python 3.x and 2.7

      if ResultMethod(testInfo, outTuple, result) == True:
        passed += 1
      else:
        failed += 1

  print("passed:" + str(passed))
  print("failed:" + str(failed))

# Will take the xpected output and determine if they match
# This is the default function of the program
def DisplayResult(testObj, outTuple, outFile):
  try:#attempt to open a result file
    outFile_Read = open(outFile, 'r')
  except Exception as err: #some error happens while attempting to get a result, inform user
    print(testObj['test'] + " failed due to error:" + str(err))
    return True#NothingToDoHere.jpg
  
  passed = True #lets assume we passed the test

  outFile_Obj = loads(outFile_Read.read())#parse output file
  outFile_Read.close()

  realStdOut = outTuple[0].decode("UTF-8").splitlines(1)#outTuple has old strings, we need to decode
  expectedStdOut = outFile_Obj['stdout'].splitlines(1)#the JSON parser returns 3.x strs, no decode
  realStdErr = outTuple[1].decode("UTF-8").splitlines(1)#outTuple has old strings, we need to decode
  expectedStdErr = outFile_Obj['stderr'].splitlines(1)#the JSON parser returns 3.x strs, no decode

  diffObj = Differ()#apparently we can use a single differ object

  stdOutDiffs = diffObj.compare(realStdOut, expectedStdOut)
  stdErrDiffs = diffObj.compare(realStdErr, expectedStdErr)

  #differs output sucks, use DetectDiffs to clean it up and make it readable
  stdOutResults = DetectDiffs(stdOutDiffs)
  stdErrResults = DetectDiffs(stdErrDiffs)

  if len(stdOutResults) > 0 or len(stdErrResults) > 0:
    currentResultDir = join(currentResults, testObj['test'])
    SafeMakeDir(currentResultDir)

    stdOutRaw = open(join(currentResultDir, "stdout.txt"), 'w')
    stdOutRaw.write(outTuple[0].decode("UTF-8"))
    stdOutRaw.close()

    stdErrRaw = open(join(currentResultDir, "stderr.txt"), 'w')
    stdErrRaw.write(outTuple[1].decode("UTF-8"))
    stdErrRaw.close()

    diffsOut = open(join(currentResultDir, "diff.txt"), 'w')

    displayString = "[X]" + testObj["test"] + " failed:"
    if len(stdOutResults) > 0:
      displayString += " bad stdout "
      diffsOut.write("STDOUT RESULTS\n")
      diffsOut.write("------------------------------------\n")
      for line in stdOutResults:
        diffsOut.write(line)

    if len(stdErrResults) > 0:
      displayString += "bad stderr"
      diffsOut.write("STDERR RESULTS\n")
      diffsOut.write("------------------------------------\n")
      for line in stdErrResults:
        diffsOut.write(line)

    diffsOut.close()
    passed = False
  else:
    displayString = "[O]" + testObj['test'] + " passed"

  
  print(displayString)
  return passed

#Differ has shitty output, instead of using it, we can clean it up
#This function will provide context for each diff
#This includes the output line number
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

def GenerateResultFiles(testObj, outTuple, outFile):
  outFile_Write = open(outFile, 'w')
  #we need to decode because the subprocess tuple of outTuple is binary encoded
  outFile_Write.write(dumps({"stdout": outTuple[0].decode("UTF-8"), "stderr":outTuple[1].decode("UTF-8")}, sort_keys=False, indent=4))
  return True

def SafeMakeDir(path):
  try:
    makedirs(path)
  except OSError as err:
    if err.errno == errno.EEXIST:
      pass#we are ok, the file exists, keep calm and carry on
    else:
      raise#lets have someone handle this issue if needed?

if __name__=="__main__":
  #lets try to create our files (should be there but if not just create them, only one fs call)
  SafeMakeDir(testPath)
  SafeMakeDir(expectedPath)
  SafeMakeDir(resultsPath)
  SafeMakeDir(currentResults)

  parser = argparse.ArgumentParser(description="A utility for testing executables")
  parser.add_argument("--generate", dest="method", default=DisplayResult, help="Triggers generation of new result files", action='store_const', const=GenerateResultFiles)
  args = parser.parse_args()
  Execute(args.method)
