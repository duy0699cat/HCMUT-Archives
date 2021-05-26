class Job:
    def __init__(self, name, arrtime, duration):

        self.name = name
        self.arrtime = arrtime
        self.duration = duration
        self.responseTime = -1
        self.beginService = -1

    def __str__(self):
        return '%s at %.2f, duration %.2f' %(self.name, self.arrtime, self.duration)
