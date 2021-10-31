import numpy as np
import matplotlib.pyplot as plt

def loadDataset(infile):
    f = open(infile, "r")
    line = f.readline()
    line = line[:-1]
    lines = []
    lines2 = []
    while line:  # 直到读取完文件
        line = f.readline()  # 读取一行文件，包括换行符
        line = line[:-1]  # 去掉换行符，也可以不去
        lines.append(line)
    f.close()  # 关闭文件
    lines = lines[:-1]
    for x in lines:
        a,b = x.split(" ")
        a = float(a)
        b = float(b)
        lines2.append([a,b])
    t = np.array(lines2)
    return t

class KMeansCluster():
    def __init__ (self, k=3,initCent = 'random', max_iter = 500):
        self._k = k
        self._initCent = initCent
        self._max_iter = max_iter
        self._clusterAssment = None
        self._labels = None

    def _calEDist(self, arrA, arrB):
        return np.math.sqrt(sum(np.power(arrA - arrB, 2)))

    def _randCent(self, data_X,k):
        n = data_X.shape[1]  # 获取特征的维数
        centroids = np.empty((k, n))  # 使用numpy生成-一个k*n的矩阵，用于存储质心
        for j in range(n):
            minJ= min(data_X[:,j])
            rangeJ = float(max(data_X[:, j]-minJ))
        # 使用flatten拉平嵌套列表(nested list)
            centroids[:, j] = (minJ + rangeJ * np.random.rand(k,1)).flatten()
        return centroids

    def fit(self, data_X):
        m = data_X.shape[0]  # 获取样本的个数
        # 一个m*2的维矩阵 ，矩阵第- -列存储样本点所 富的族的素引值，
        # 第二列存储该点与所属族的质心的平方误差
        self._clusterAssment = np.zeros((m, 2))
        if self._initCent == "random" :
            self._centroids = self._randCent(data_X,self._k)

        clusterChanged = True
        for _ in range(self._max_iter):
            clusterChanged = False
            for i in range(m):  # 将每个样本点分配到离它最近的质心所属的族
                minDist = np.inf  # 首先将minDist置为- 个无穷大的数
                minIndex= - 1  # 将最近质心的下标置为-1
                for j in range(self._k):  # 次迭代用于寻找最近的质心
                    arrA = self._centroids[j, :]
                    arrB = data_X[i, :]
                    distJI = self._calEDist(arrA, arrB)  # 计算误差值
                    if distJI < minDist:
                        minDist = distJI
                        minIndex = j
                if self._clusterAssment[i, 0]!= minIndex or self._clusterAssment[i, 1] > minDist ** 2:
                    clusterChanged = True
                    self._clusterAssment[i, :] = minIndex, minDist**2
            if not clusterChanged:  # 若所有样本点所属的族都不改变,则已收敛,结束迭代
                break
            for i in range(self._k):  # 更新质心，将每个族中的点的均值作为质心
                index_all = self._clusterAssment[:,0]  # 取出样本所属簇的索引值
                value = np.nonzero(index_all == i)  # 取出所有属于第i个簇的索引值
                ptsInClust = data_X[value[0]]  # 取出属于第1个族的所有样本点
                self._centroids[i,:] = np.mean(ptsInClust, axis = 0)
        self._labels = self._clusterAssment[:,0]

        return self._centroids,self._labels



if __name__ == "__main__":

    data_X = loadDataset("data.txt")
    k = 3
    clf = KMeansCluster(k)
    cents,labels = clf.fit(data_X)
    colors = ['b', 'g', 'r', 'k', 'C','m', 'y',' #e24fff',' #524C90',' #845868']
    for i in range(k):
        index = np.nonzero(labels == i)[0]
        x0 = data_X[index, 0]
        x1 = data_X[index,1]
        y_i = i
        for j in range(len(x0)):
            plt.text(x0[j], x1[j],str(y_i),color = colors[i], fontdict = {'weight': 'bold',  'size': 6})
        plt.scatter(cents[i, 0],cents[i, 1], marker = 'x', color = colors[i], linewidths = 7)
    plt.axis([-50, 150, -50, 150])
    plt.show()

