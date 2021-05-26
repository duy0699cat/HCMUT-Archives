import simpy
import numpy.random as random

MAXSIMTIME = 100 # max idle time
VERBOSE = False

class Server:
    def __init__(self, env, BUFFER_SIZE, servant, maxJobs, strat = 'FIFO'):

        self.env = env
        self.strat = strat
        self.Jobs = list(())
        self.serversleeping = None
        self.servant = servant
        ''' statistics '''
        self.waitingTime = 0
        self.totalServeTime = 0
        self.idleTime = 0
        self.jobsDone = 0
        self.stopCondition = 0
        self.maxJobs = maxJobs
        self.CPUtime = -1
        self.maxBuffer = BUFFER_SIZE

        ''''for plot'''
        # watch the distribution of arrival_rate and service_rate
        self.listArrivalRate = []
        self.listServiceRate = []

        self.processInQueue = [] # number of process in queue
        self.timeprocessInQueue = [] # specific time for the number of process in queue

        self.jobsWaiting = []
        self.jobsWaitingTime = []

        self.jobsRejected = []
        self.jobsRejectedTime = []


        ''' register a new server process '''
        self.servproc = env.process(self.serve(env, servant))


    def serve(self, env, servant):
        while True:
            ''' do nothing, just change server to idle
              and then yield a wait event which takes infinite time
            '''
            if len( self.Jobs ) == 0 :
                if (self.maxJobs == self.stopCondition) :
                        break
                self.serversleeping = env.process( self.waiting( env ))
                t1 = self.env.now
                yield self.serversleeping

                ''' accumulate the server idle time'''
                self.idleTime += self.env.now - t1

            else:
                ''' get the first job to be served'''
                # FIFO
                with servant.request() as req:
                    yield req
                    j = self.Jobs.pop(0)
                    j.beginService = env.now
                    print('%.2f Serving %d ' %(j.beginService, j.name ))


                    ''' sum up the waiting time'''
                    self.jobsWaitingTime.append(j.beginService - j.arrtime)
                    self.waitingTime += j.beginService - j.arrtime
                    #######
                    ''' sum up the duration time'''
                    self.totalServeTime += j.duration

                    ''' yield an event for the job finish'''
                    yield self.env.timeout( j.duration )

                    print('%.2f  Finished serving %d ' %(env.now, j.name ))
                    self.CPUtime = env.now


                    ''' sum up the jobs done '''
                    self.jobsDone += 1
        env.exit(23)

    def waiting(self, env):
        try:
            if VERBOSE:
                print( '%.2f  idle' % self.env.now )
            yield self.env.timeout( MAXSIMTIME )
            self.idleTime =  self.idleTime - MAXSIMTIME
        except simpy.Interrupt as i:
            if VERBOSE:
                 print('Server waken up and works at %.2f' % self.env.now )


    def done(self):
        self.servproc.interrupt()
