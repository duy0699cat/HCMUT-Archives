import numpy.random as random
import JobAttribute as J

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
            self.server.listArrivalRate.append(job_interarrival)
            yield env.timeout( job_interarrival )  # event: job's arrival

            """ generate service time and add job to the list """
            job_duration = random.exponential( self.servicetime )
            self.server.listServiceRate.append(job_duration)

            job = J.Job(i, env.now, job_duration)
            #print(job.__str__())

            if ( len( self.server.Jobs) <  self.server.maxBuffer) :
                self.server.Jobs.append(job)

                self.server.jobsWaiting.append(job.name)
                self.server.processInQueue.append(len( self.server.Jobs)) # number of process in queue
                self.server.timeprocessInQueue.append(env.now)

                print('%.2f Job %d : duration %.2f len %d'%(job.arrtime , job.name , job.duration, len( self.server.Jobs)))

            else:
                print('%.2f Job %d is rejected'%(env.now, i ))
                self.server.jobsRejected.append(i)
                self.server.jobsRejectedTime.append(env.now)


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
