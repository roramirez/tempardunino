#!/usr/bin/env python

# Rodrigo Ramirez
# decipher.hk@gmail.com


import sys, os, time, signal, json, urllib2


KEYS = ["humidity", \
  "temperature", \
  ]

def usage():
  print("usage: ./get_temperature.py [<host_ip> <key>]  [options]")
  print("  hostname - ip of the server")
  print("  key - display value based on this key")
  print("    available keys:")
  for i in KEYS:
    print("      " + i)
  print("  options - extra options")
  print("    available options:")
  print("      -h,--help    - display this text")
  print("      -v,--verbose - verbose mode")
try:

  verbose = 0
  validargs = []
  for i in sys.argv:
    if i == "--verbose" or i == "-verbose" or i == "-v":
      verbose = 1
    elif i == "--help" or i == "-help" or i == "-h":
      usage()
      sys.exit()
    elif i == "--get" or i == "-get" or i == "-g":
      pass
    elif i == "--index" or i == "-index" or i == "-i":
      print("arduino")
      sys.exit()
    elif i == "--query" or i == "-query" or i == "-q":
      print("arduino:arduino") 
      sys.exit()
    else:
      validargs.append(i)
  if len(validargs) < 3:
    print len(validargs)
    usage()
    sys.exit()
  pid = os.fork()
  if pid: # parent

    result = 0
    # humidity
    if validargs[2] == "humidity":
      data = json.load(urllib2.urlopen("http://" + sys.argv[1]))
      result = data["humidity"]
    elif validargs[2] == "temperature":
      data = json.load(urllib2.urlopen("http://" + sys.argv[1]))
      result = data["temperature"]
    else:
      usage()
      sys.exit()
    print result
  else: # child
    time.sleep(60)
    os.kill(pid, signal.SIGTERM)
except Exception, e:
    print e
