import numpy as np
import pandas as pd

def loadIris(address):
    spf = pd.read_csv(address,sep=',',index_col=False,header=None)
    strs = spf[4]
    spf.drop([4],axis=1,inplace=True)
    return spf.values,strs

def normalization(data_matrix):
    e = 1e-5
    for c in range(4):
        maxNum = np.max(data_matrix[:,c])
        minNum = np.min(data_matrix[:,c])
        data_matrix[:,c] = (data_matrix[:,c] - minNum + e)/(maxNum - minNum +e)
    return data_matrix

if __name__ == '__main__':
    filepath = 'iris.arff'
    writepath = 'iris_normal.txt'
    # read data
    data_matrix,str_name = loadIris(filepath)
    # normalization
    data_matrix = normalization(data_matrix)

    spf = pd.DataFrame(data_matrix)
    strs = str_name.values
    spf.insert(4,4,strs)
    spf.to_csv(writepath,index=False,header=False)