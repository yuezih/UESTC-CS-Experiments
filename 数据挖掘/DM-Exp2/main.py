import collections
import itertools

# traDatas = [ ' abe ' , 'ae' , ' abc' , ' ade ',]


class Apriori:
    # transition set
    traDatas = []
    # transition set's length
    traLen = 0
    # frequent k set, start with 1
    k = 1
    # counting the number of transition set
    traCount = {}
    # store frequent transition
    freTran = {}
    # support
    sup = 0
    # confidence
    conf = 0
    freAllTran = {}

    def __init__(self, traDatas, sup, conf):
        self.traDatas = traDatas
        self.traLen = len(traDatas)
        self.sup = sup
        self.conf = conf

    def scanFirDatas(self): # count frequency for each element
        tmpList = ','.join(traDatas).split(',')
        tmpSetList = [(each,) for each in tmpList]
        self.traCount = dict(collections.Counter(tmpSetList))
        return self.traCount

    def getFreSet(self): # find event with higher support. and get frequent k set
        self.freTran = {}
        for tra in self.traCount.keys():
            if self.traCount[tra] >= self.sup and len(tra) == self.k:
                self.freTran[tra] = self.traCount[tra] # store frequent set
                self.freAllTran[tra] = self.traCount[tra]

    # compare if k-l elements is equal
    def cmpTwoSet(self, setA, setB) :
        setA = set(setA)
        setB = set(setB)
        if len(setA-setB) == 1 and len(setB-setA) == 1:
            return True
        else:
            return False

    def selfConn(self): # connecting events. Only an element is added.
        self.traCount = {}
        for item in itertools.combinations(self.freTran.keys(),2):# connecting event between any two event.
            if self.cmpTwoSet(item[0], item[1]) == True: # only an element is added.
                key = item[0] + item[1]
                if self.cutBranch(key) != False:
                    self.traCount[key] = 0

    def cutBranch(self, key): # if subKey of the event is not frequent，return false
        for subKey in list(itertools.combinations(key, self.k)):
            if subKey not in self.freTran.keys():
                return False

    def scanDatas(self):# count support
        self.k = self.k + 1
        for key in self.traCount.keys():
            for tra in traDatas:
                if set(key).issubset(tuple(tra.split(','))):
                    self.traCount[key] += 1

    # def findChars(self, str, chars):
    #     for char in list(chars):
    #         if char not in str:
    #             return False
    #     return 1

    def permutation2(self, string, pre_str, container):
        if len(string) == 1:
            container.append(pre_str + string)
        for idx, str in enumerate(string):
            new_str = string[:idx] + string[idx+1:]
            new_pre_str = pre_str + str
            self.permutation(new_str, new_pre_str, container)

    def permutation(self, tup, pre_tup, container):
        if len(tup) == 1:
            container.append(pre_tup + tup)
        for idx, elem in enumerate(tup):
            new_tup = tup[:idx] + tup[idx+1:]
            new_pre_tup = pre_tup + (elem,)
            self.permutation(new_tup, new_pre_tup, container)

    def genAssRule(self):
        container = []
        ruleSet = set()
        for item in self.freTran.keys():
            self.permutation(item, (), container)
        for item in container:
            for i in range(1, len(item)):
                ruleSet.add((item[:i],item[i:]))
        for rule in ruleSet:
            if self.calcConfi(rule[0], rule[1]) > self.conf:
                print (rule[0],end="_--->>>")
                print(rule[1])

    def calcConfi(self, first, last): #computing confidence
        if first+last not in self.freAllTran.keys():
            return self.freAllTran[last+first]/self.freAllTran[first]
        return self.freAllTran[first + last] / self.freAllTran[first]

    def algorithm (self):
        self.scanFirDatas()# count frequency for each element
        while self.traCount != {}:
            self.getFreSet() # find event with higher support. and get frequent k set
            self.selfConn() # connecting events. Only an element is added.cut branch
            self.scanDatas() # count support
        print(self.freAllTran)
        print(self.freTran)
        self.genAssRule()# mining rules.

with open('contact-lenses.arff') as f:
    traDatas = []
    perline = f.readline()[:-1]
    while perline:
        traDatas.append(perline)
        perline = f.readline()[:-1]
    # print(traDatas)
    apriori = Apriori(traDatas, 2, 0.7)
    apriori.algorithm()