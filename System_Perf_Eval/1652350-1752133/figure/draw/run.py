import os
import simpy
import subprocess
import numpy.random as random
import matplotlib.pyplot as plt

import src.JobGen as G
import src.ServerClass as S
import src.JobAttribute as J


MAXSIMTIME = 100 # max idle time
VERBOSE = False

def survey(njob, l, m, BUFFER_SIZE):
    POPULATION = njob
    LAMBDA = l
    MU = m

    env = simpy.Environment()
    servant = simpy.Resource(env, capacity=1)

    MyServer = S.Server( env,  BUFFER_SIZE, servant, POPULATION, 'FIFO')

    MyJobGenerator = G.JobGenerator(env, MyServer, POPULATION, LAMBDA, MU)

    env.run()

    MyServer.timeprocessInQueue.append( MyServer.CPUtime + 100)
    #print( MyServer.CPUtime + 1000) # trick
    MyServer.processInQueue.append(0)

    print('FINAL RESULT:')
    print('Jobs processed : %d / %d'% (MyServer.jobsDone, POPULATION))
    print('Total waiting time : %.2f'% MyServer.waitingTime)
    print('Total serve time: %.2f'%MyServer.totalServeTime)

    print('Idle time : %.2f'% MyServer.idleTime)
    print('CPU time = idle + runTime: %.2f'%  MyServer.CPUtime)

    print('Average waiting time : %.2f'% ( MyServer.waitingTime / MyServer.jobsDone))

    print('Average arrival rate : %.2f'% (sum(MyServer.listArrivalRate) / POPULATION))
    print('Average serive rate: %.2f'% (sum(MyServer.listServiceRate) / POPULATION))


    Untilization =  1- (MyServer.idleTime / MyServer.CPUtime)
    print('Untilization: %.2f'% Untilization)

    plt.figure(figsize=(9, 3))

    a = MyServer.jobsWaiting
    b = MyServer.jobsWaitingTime

    plt.subplot(231)
    plt.plot(a, b, label ='id over waiting time')
    plt.xlabel ('job id')
    plt.ylabel ('waiting time')
    plt.title ('waiting time of each job')
    plt.legend()
    #plt.show()

    c = MyServer.jobsWaiting
    d = MyServer.jobsWaitingTime

    plt.subplot(232)
    plt.plot(c, d, label ='reponse time / id')
    plt.xlabel ('job id')
    plt.ylabel ('reponse time')
    plt.title ('reponse time of each job')
    plt.legend()
    #plt.show()


    e =  MyServer.jobsRejected
    f = MyServer.jobsRejectedTime

    plt.subplot(233)
    plt.scatter(e, f, label= "job", color= "green")
    plt.xlabel ('Reject job id')
    plt.ylabel ('time')
    #plt.title ('reponse time of each job')
    plt.legend()
    #plt.show()

    plt.subplot(223)
    #  a b c d e f [g h k]
    # plot serve and arrival distribution
    g = MyServer.listArrivalRate
    h = MyServer.listServiceRate
    k = list(range(1, POPULATION + 1))
    # plot arrival_rate
    plt.bar(k, g)
    plt.xlabel ('job')
    plt.ylabel ('time')
    plt.title ('distribution of arrival rate')
    #plt.show()

    plt.subplot(224)
    # plot service_rate
    h = MyServer.listServiceRate
    # plot arrival_rate
    plt.bar(k, h)
    plt.xlabel ('job')
    plt.ylabel ('time')
    plt.title ('distribution of service rate')
    plt.show()

    #  [a b] [c d] [e f] [g h k ]                                           [x y]


    #  [a b] [c d] [e f] [g h k]                                           [x y]
    x = MyServer.timeprocessInQueue
    y = MyServer.processInQueue

    plt.subplot(131)
    plt.plot(x, y, label ='size buffer over time ')
    plt.xlabel ('time')
    plt.ylabel ('number of jobs in queue')
    plt.title ('processes in queue over time ')
    #plt.show()

    # defining labels
    activities = ['idle time',  'wait time', 'serve time']

    # portion covered by each label
    slices =[
              MyServer.idleTime,
              MyServer.waitingTime,
              MyServer.totalServeTime,
             ]

    # color for each label
    colors = ['r', 'y', 'g']
    plt.subplot(133)
    # plotting the pie chart
    plt.pie(slices, labels = activities, colors=colors,
            startangle=90, shadow = True, explode = (0, 0, 0),
            radius = 1.2, autopct = '%1.1f%%')

    # plotting legend
    plt.legend()
    # showing the plot
    plt.show()


BUFFER_SIZE = input('Enter Buffer size: ')
Jobs = input('Enter Number of Jobs: ')
lam = input('Mean(poisson) for  inter-arrival-time :')
mu = input('Mean(exponential) for  service-time :')


survey(Jobs, lam, mu, BUFFER_SIZE)
os.system('./clean.sh')
