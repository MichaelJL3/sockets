from __future__ import print_function

import random
import time
import grpc
import sys

import debate_pb2

#Answer passes message to RPC server and displays result
def answer(stub):
  question = sys.argv[2]
  size = len(sys.argv)

  #check for too many arguments (acts as a warning)
  if(size>3):
    print("Warning Too Many Args Passed For (answer)\n\t-ignoring extras")
  
  request = debate_pb2.AnswerRequest(question=question)
  
  #try to connect to the RPC and send the message
  try:
    response = stub.Answer(request).answer
    
    #check that the response is not empty
    if not response:
      print("Server returned nothing!")
      return

    print(response)

  #catch and display any connection errors
  except Exception as e:
    print('\nERR: error connecting to RPC server')
    print(e)

#Elabroate sends a message and array to the RPC server and display result
def elaborate(stub):
  topic = sys.argv[2]
  size = len(sys.argv)
  blah = []

  #loop through cmd args
  for x in range(3, size):
    #only add the argument if its a digit
    digit = sys.argv[x]
    if(digit.isdigit()):
      blah.append(int(digit))

    #else display warning anf ignore the value
    else:
      print("Warning: Input Included Non Digit\n\t-value was ignored")

  request = debate_pb2.ElaborateRequest(topic=topic, blah_run=blah)

  #try to connect to the RPC and send the message
  try:
    response = stub.Elaborate(request).answer
    
    #check that the response is not empty
    if not response:
      print("Server returned nothing!")
      return

    print(response)

  #catch and display any connection errors
  except Exception as e:
    print('\nERR: error connecting to RPC server')
    print(e)

#Setup the RPC client
def setupClient():
  channel = grpc.insecure_channel('localhost:50000')
  stub = debate_pb2.ControllerStub(channel)

  return stub

if __name__ == '__main__':
  args=sys.argv
  size=len(args)
  
  #check for proper number of arguments
  if size>2:
    stub = setupClient()
    command=args[1]

    #check for appropriate commands
    if(command=='Elaborate' or command=='elaborate'):
      elaborate(stub)
    elif(command=='Answer' or command=='answer'):
      answer(stub)
    else:
      print("ERR: Invalid Command\n====================\nuse (elaborate) or (answer)")

  else:
    print("ERR: Not Enough Args Passed")
