from concurrent import futures
from random import randint

import sys
import time
import math
import grpc

import consultation_pb2
import debate_pb2

_ONE_DAY_IN_SECONDS = 60 * 60 * 24

#RPC to consultation and get response
def rpcCampaign(message):
  channel = grpc.insecure_channel('23.236.49.28:50051')
  stub = consultation_pb2.CampaignManagerStub(channel)

  request = consultation_pb2.RetortRequest(original_question=message)

  #try to connect to the RPC and send the message
  try:
    retort = stub.Retort(request).retort
    
    #check that the response is not empty
    if not retort:
      print("Consultation server returned nothing!")
      return

    return retort

  #catch and display any connection errors
  except Exception as e:
    print('\nERR: error connecting to RPC server')
    print(e)

  return

#replace a word throughout a string with a new word
def replaceWord(line, word, newWord):
  return line.replace(word, newWord)

#return int between 0 and 1
def getRandBinary():
  return randint(0, 1)

#randomly choose between 2 sentences
def randomSub():
  head=getRandBinary()

  if(head):
    return "your 3 cent titanium tax goes too far"
  else:
    return "your 3 cent titanium tax doesn't go too far enough"

class ControllerServicer(debate_pb2.ControllerServicer):
  #Answer Method
  def Answer(self, request, context):
    #grab the question and the first word in the question
    question_sub = request.question
    first = question_sub.split(' ')[0].strip()

    #check for keywords
    if('why'==first or 'what'==first or 'how'==first or 'when'==first or 'who'==first):
      question_sub=replaceWord(question_sub, 'your', 'my')
      question_sub=replaceWord(question_sub, 'you', 'I')

    #else pick between random strings
    else: 
      return debate_pb2.AnswerReply(answer=randomSub())

    #answer response line
    answer="You asked me "+question_sub+" but I want to say that "+rpcCampaign(question_sub)
    return debate_pb2.AnswerReply(answer=answer)

  #Elaborate Method
  def Elaborate(self, request, context):
    #grab the args passed through the RPC
    topic=request.topic
    blah=request.blah_run
    blahTimes=len(blah)
    answer=""

    #for each element in the blah array
    for i in range(blahTimes):
      #write blah the number of times specified in the array
      for j in range(blah[i]):
        answer+=" blah "
      answer+=topic

    #if the array is empty only write the topic once
    if not blahTimes:
      answer=topic

    return debate_pb2.ElaborateReply(answer=answer.strip())

#Start server
def serve():
  #default port
  port = '50000'
  size = len(sys.argv)

  if(size>1 and sys.argv[1].isdigit()):
  	port = sys.argv[1]

  server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
  debate_pb2.add_ControllerServicer_to_server(ControllerServicer(), server)
  server.add_insecure_port('[::]:'+port)
  server.start()
  try:
    while True:
      time.sleep(_ONE_DAY_IN_SECONDS)
  except KeyboardInterrupt:
    server.stop(0)

if __name__ == '__main__':
  serve()
