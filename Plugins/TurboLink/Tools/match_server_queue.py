import grpc
from concurrent import futures
import time
import match_pb2_grpc as pb2_grpc
import match_pb2 as pb2
from collections import deque

USE_SSL = True
SSL_PRIVATE_KEY_PATH = 'auth/uengine.kozow.com.key'
SSL_CERTIFICATE_PATH = 'auth/uengine.kozow.com.crt'

def print_status(deq) :

    print('----------------------')
    for e in deq :
        print('[', e, ']')
    print('----------------------')
    print('')

class MatchService(pb2_grpc.MatchServicer):

    deq = deque()

    def __init__(self, *args, **kwargs) :
        pass

    def GetMatchServer(self, request, context) :

        if self.deq :
            result = {'success': True, 'matched_server': self.deq[0]}
            print('GetMatchServer():', self.deq[0])
            return pb2.MatchResponse(**result)
        else :
            result = {'success': False, 'matched_server': ''}
            print('GetMatchServer(): False')
            return pb2.MatchResponse(**result)

    def AddToMatchQueueFront(self, request, context) :

        self.deq.appendleft(request.server_address)

        if self.deq and self.deq[0] == request.server_address :
            result = {'success': True}
            print('AddToMatchQueueFront():', self.deq[0])
            print_status(self.deq)
            return pb2.AddResponse(**result)
        else :
            result = {'success': False}
            print('AddToMatchQueueFront(): False')
            print_status(self.deq)
            return pb2.AddResponse(**result)

    def AddToMatchQueueRear(self, request, context) :

        self.deq.append(request.server_address)

        if self.deq and self.deq[-1] == request.server_address :
            result = {'success': True}
            print('AddToMatchQueueRear():', self.deq[-1])
            print_status(self.deq)
            return pb2.AddResponse(**result)
        else :
            result = {'success': False}
            print('AddToMatchQueueRear(): False')
            print_status(self.deq)
            return pb2.AddResponse(**result)

    def RemoveFromMatchQueue(self, request, context) :

        if self.deq and self.deq[0] == request.server_address :
            self.deq.popleft()
            result = {'success': True}
            print('RemoveFromMatchQueue():', request.server_address)
            print_status(self.deq)
            return pb2.RemoveResponse(**result)
        else :
            result = {'success': False}
            print('RemoveFromMatchQueue(): False')
            print_status(self.deq)
            return pb2.RemoveResponse(**result)

def serve():
    
    server = grpc.server(futures.ThreadPoolExecutor(max_workers = 4))
    pb2_grpc.add_MatchServicer_to_server(MatchService(), server)

    if USE_SSL :
        with open(SSL_PRIVATE_KEY_PATH, 'rb') as f:
            private_key = f.read()
        with open(SSL_CERTIFICATE_PATH, 'rb') as f:
            certificate_chain = f.read()
        server_credentials = grpc.ssl_server_credentials(((private_key, certificate_chain,),),)
        server.add_secure_port('[::]:57318', server_credentials)
    else :
        server.add_insecure_port('[::]:57318')

    server.start()
    server.wait_for_termination()

if __name__ == '__main__' :
    serve()
