#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math, time
from array import array
import collections
import ROOT
from anaClass import *


def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('-i','--input', dest='INPUT_FILE', type='string', default='', help='Input file')
  parser.add_option('-o','--output', dest='OUTPUT_FILE', type='string', default='', help='Output file')
  parser.add_option('-t','--tree', dest='TREE_NAME', type='string', default='T', help='Tree name')

  global opt, args
  (opt, args) = parser.parse_args()
  
  if opt.INPUT_FILE == '':
    raise RuntimeError, 'Please pass an input file name (-i)!'

  if opt.OUTPUT_FILE == '':
    raise RuntimeError, 'Please pass an output file name (-o)!'

  if opt.TREE_NAME == '':
    raise RuntimeError, 'Please pass a tree name (-t)!'

  if not os.path.isfile(opt.INPUT_FILE):
    raise RuntimeError, 'Input file does not exist!'


def processCmd(cmd):
  status, output = commands.getstatusoutput(cmd)
  if status !=0:
    print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
    print output
    sys.exit()


if __name__ == "__main__":
  parseOptions()
  myClass = anaClass(opt.INPUT_FILE,opt.TREE_NAME,opt.OUTPUT_FILE)
  myClass.doAnalysis()
