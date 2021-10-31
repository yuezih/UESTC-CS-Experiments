import math
import re

dataset = []
dataLen = []
testDataSet = []

with open("iris.2D.train.arff") as fin:
    for line in fin.readlines():
        data = re.split(",", line.strip())
        dataLen = len(data)
        dataset.append(data)

def computeDis(x, y):
    return math.sqrt(math.pow(float(x[0])-float(y[0]), 2) + math.pow(float(x[1])-float(y[1]), 2))

def findNN(testData, k):
    distances = []
    for data in dataset:
        distances.append(computeDis(testData, data))
    return (sorted(range(len(distances)), key=lambda m:distances[m]))[:k]

def vote(indexs):
    votes = {}
    for i in indexs:
        label = dataset[i][dataLen-1]
        if label not in votes.keys():
            votes[label] = 1
        else:
            votes[label] = votes[label]+1
    return (max(votes, key=votes.get))

if __name__ == "__main__":
    with open("iris.2D.test.arff") as ftest:
        for line in ftest.readlines():
            testData = re.split(",", line.strip())
            testData.append(vote(findNN([testData[0], testData[1]], 15)))
            testDataSet.append(testData)
    total = 0
    right = 0
    for each in testDataSet:
        total += 1
        if each[2]==each[3]:
            right += 1

    print("%.2f"%(right/total*100),'%')
