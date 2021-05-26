import simpy
import numpy.random as random
import matplotlib.pyplot as plt
import pdb
MAXSIMTIME = 1000 # max idle time
BUFFER_SIZE = 10
Jobs = 5000
lam = 0.1 #arrival time
mu = 0.5 #service time

VERBOSE = False
 
class Job:
    def __init__(self, name, arrtime, duration):
 
        self.name = name
        self.arrtime = arrtime
        self.duration = duration
        self.responseTime = -1
        self.beginService = -1
 
    def __str__(self):
        return '%s at %.2f, duration %.2f' %(self.name, self.arrtime, self.duration)
 
class Server:
    def __init__(self, env, BUFFER_SIZE, servant, maxJobs, strat = 'FIFO'):
 
        self.env = env
        self.strat = strat
        self.Jobs = list(())
        self.serversleeping = None
        self.servant = servant
        ''' statistics '''
        self.waitingTime = 0
        self.idleTime = 0
        self.jobsDone = 0
        self.stopCondition = 0
        self.maxJobs = maxJobs
        self.CPUtime = -1
        self.maxBuffer = BUFFER_SIZE
 
        ''''for plot'''
        self.processInQueue = [] # number of process in queue
        self.timeprocessInQueue = [] # specific time for the number of process in queue
 
        self.jobsWaiting = []
        self.jobsWaitingTime = []
 
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
                    #print('waiting time %.2f'%(self.env.now - j[1]))
                    self.jobsWaitingTime.append(j.beginService - j.arrtime)
                    self.waitingTime += j.beginService - j.arrtime
                    #######
 
                    ''' yield an event for the job finish'''
                    yield self.env.timeout( j.duration )
 
                    print('%.2f  Finished serving %d ' %(env.now, j.name ))
                    self.CPUtime = env.now
 
 
                    ''' sum up the jobs done '''
                    self.jobsDone += 1          
        return
 
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
 
 
class JobGenerator:
    def __init__(self, env, server, nrjobs, lam, mu):
        self.server = server
        self.nrjobs = nrjobs
        self.interarrivaltime = lam; # for random gen
        self.servicetime = mu; # for random gen
        self.servproc = env.process(self.generatejobs(env, nrjobs))
 
 
    def generatejobs(self, env, nrjobs ):
 
        self.server.processInQueue.append(len( self.server.Jobs)) # number of process in queue
        self.server.timeprocessInQueue.append(env.now)
 
        for i in range(1, nrjobs + 1) :
            """ yield an event for new job arrival """
 
            job_interarrival = random.poisson( self.interarrivaltime )
            yield env.timeout( job_interarrival )  # event: job's arrival
            """ generate service time and add job to the list """
            job_duration = random.exponential( self.servicetime )
 
            job = Job(i, env.now, job_duration)
            #print(job.__str__())
 
            if ( len( self.server.Jobs) <  self.server.maxBuffer) :
                self.server.Jobs.append(job)
 
                self.server.jobsWaiting.append(job.name)
                self.server.processInQueue.append(len( self.server.Jobs)) # number of process in queue
                self.server.timeprocessInQueue.append(env.now)
 
                print('%.2f Job %d : duration %.2f len %d'%(job.arrtime , job.name , job.duration, len( self.server.Jobs)))
 
            else:
                print('%.2f Job %d is rejected'%(env.now, i ))
 
 
            ''' if server is idle, wake it up'''
            if not self.server.serversleeping.triggered:
                self.server.serversleeping.interrupt( 'Wake up, please.' )
 
            #self.servproc = env.process( self.waiting(env) )
            #yield self.servproc
 
            yield env.timeout(job_interarrival)
 
            self.server.stopCondition = self.server.stopCondition + 1
 
    def run(self, env):
        self.servproc = env.process( self.waiting(env) )
        yield self.servproc
 
    def done(self):
        self.servproc.interrupt()
 
    def waiting(self, env):
        try:
            yield( env.timeout(0) )
        except simpy.Interrupt:
            pass
def survey(njob, l, m, BUFFER_SIZE):
    POPULATION = njob
    LAMBDA = l
    MU = m 
 
    env = simpy.Environment()
    servant = simpy.Resource(env, capacity=1)
 
    MyServer = Server( env,  BUFFER_SIZE, servant, POPULATION, 'FIFO')
 
    MyJobGenerator = JobGenerator(env, MyServer, POPULATION, LAMBDA, MU)
 
    env.run()
 
    MyServer.timeprocessInQueue.append( MyServer.CPUtime + 100)
    #print( MyServer.CPUtime + 1000) # trick
    MyServer.processInQueue.append(0)
 
    print('FINAL RESULT:')    
    print('Jobs processed : %d / %d'% (MyServer.jobsDone, POPULATION))
    print('Waiting time : %.2f'% MyServer.waitingTime)
 
    print('Idle time : %.2f'% MyServer.idleTime)
    print('CPU time = idle + runTime: %.2f'%  MyServer.CPUtime)
 
    print('Average waiting time : %.2f'% ( MyServer.waitingTime / MyServer.jobsDone))
 
    Untilization =  1 - MyServer.idleTime / MyServer.CPUtime
    print('Untilization: %.2f'% Untilization)
 
 
    a = MyServer.jobsWaiting
    b = MyServer.jobsWaitingTime
 
    plt.plot(a, b, label ='id over waiting time')
    plt.xlabel ('job id')
    plt.ylabel ('waiting time')
    plt.title ('waiting time of each job')
    plt.legend()
    plt.show()
 
    c = MyServer.jobsWaiting
    d = MyServer.jobsWaitingTime
 
    plt.plot(c, d, label ='reponse time / id')
    plt.xlabel ('job id')
    plt.ylabel ('reponse time')
    plt.title ('reponse time of each job')
    plt.legend()
    plt.show()
 
    x = MyServer.timeprocessInQueue
    y = MyServer.processInQueue
    plt.plot(x, y, label ='size buffer over time ')
    plt.xlabel ('time')
    plt.ylabel ('number of jobs in queue')
    plt.title ('processes in queue over time ')
    plt.legend()
    plt.show()
 
 

 
survey(Jobs, lam, mu, BUFFER_SIZE)
