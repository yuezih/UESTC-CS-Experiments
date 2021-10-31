import numpy as np
import pandas as pd
from collections import Counter

def loadLabor(address):
    spf = pd.read_csv(address,sep=',',index_col=False,header=None)
    column=['duration','wage-increase-first-year','wage-increase-second-year', 'wage-increase-third-year','cost-of-living-adjustment','working-hours', 'pension', 'standby-pay', 'shift-differential','education-allowance','statuory-holidays','vaction','longterm-disability-assistance', 'contribution-to-dental-plan', 'bereavement-assistance','contribution-to-health-plan','class']
    spf.columes = column
    # label data
    str_typeName = ['cost-of-living-adjustment' , 'pension ', 'education-allowance' ,' vacation ', 'longterm-disability-assistance' ,'contribution-to-dental-plan',' bereavement-assistance', 'contribution-to-health-plan ', 'class ']
    str2numeric = {}
    str2numeric['?'] = '-1'
    spf = spf.replace(str2numeric)
    return spf, str2numeric, str_typeName

def fillMissData(spf, str2numeric):
    row, col = spf.shape
    columns = spf.columns
    for column_name in columns:
        if column_name not in str2numeric:
        # number,"first stretegy
            tmp = spf[column_name].apply(float)
            ave = np.average(tmp[tmp != -1])
            tmp[tmp== -1]= ave
            spf[column_name] = tmp
        else:
        # label, second stretegy
            v = spf[column_name].values
            vl = v[v != '-1']
            c = Counter(vl)
            cc = c.most_common(1)
            v[v=='-1'] =cc[0][0]
        return spf

if __name__ == '__main__':
    filepath = 'labor.arff'
    fillFilepath = 'laborMissing_handle.txt'
    spf, str2numeric, str2numeric = loadLabor(filepath)  # load dataspf = fillMissData(spf， str2numeric) # fill missing data
    spf = fillMissData(spf,str2numeric)
    spf.to_csv(fillFilepath, index=False,header = False)  # save data
