from sys import argv
from os import walk, path, getcwd
from re import sub, match, search

#find only c files
HEADER = r"(.*\.)h$"
CFILE = r"(.*\.)c$"
CREPLACE = r"\1c"

#find include
INCLUDE = r"(#include .*/.*)"#allows to check for lines that are includes with a comment

def main(arg):
  for dirpath, dirnames, filenames in walk(getcwd()):
    for filename in filenames:
      if match(HEADER, filename):#header file detected time to check

        cFilePath = path.join(dirpath, sub(HEADER, CREPLACE, filename))
        hFilePath = path.join(dirpath, filename)

        r_HFile = open(hFilePath, 'r')
        hFileLines = r_HFile.readlines()
        includes = GetIncludes(hFileLines)
        rawFile = str.join('',hFileLines)

        try:
          r_CFile = open(cFilePath)
          cFileLines = r_CFile.read()
          rawFile += cFileLines
        except IOError:
          pass#just ignore this, we couldn't open a c but that is ok

        DetectIncludes(includes, filename, rawFile)

      elif match(CFILE, filename):
        cFilePath = path.join(dirpath, filename)
        r_CFile = open(cFilePath,'r', encoding='utf-8')
        cFileLines = r_CFile.readlines()
        includes = GetIncludes(cFileLines)
        rawFile = str.join('',cFileLines)
        DetectIncludes(includes, filename, rawFile)
        

def DetectIncludes(includes, filePath, fileStr):
  #strip the includes from the fileStr
  fileStr = sub(INCLUDE+'\n', ' ', fileStr)
  for include in includes:
    used = False
    #get the list of uses, and split that into individual uses
    uses = str.split(include[1], "//")
    uses = str.split(uses[1], ", ")
    for use in uses:
      use = use.rstrip('\n')
      if search(use, fileStr):
        used = True
        break
    if not used:
      print(include[1] + " in file:" + filePath + "  at line " + str(include[0]) + " unused")
      print()

#returns a tuple of all includes for the given file
#and the line which they are on
def GetIncludes(lines):
  result = []
  i = 0#keep track of the line number
  for line in lines:
    if(match(INCLUDE, line)):
      result.append((i, line))
    i += 1
  return result

#entry point
if __name__ == "__main__":
  main(argv)
