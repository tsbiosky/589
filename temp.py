# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import scipy
import xlrd
import math
import numpy as np
from scipy import stats
from numpy.linalg import inv
def format (arr):
    for i in range(0,len(arr)):
        for j in range(0,len(arr[0])):
            arr[i][j]=round(arr[i][j],3)
    return arr
def loglike(arr,mean,sigma):
    re=0
    for i in range(0,len(arr)):
        a=scipy.stats.norm.pdf(arr[i],mean,sigma)
        a=math.log(a)
        re=re+a
    return re
data=xlrd.open_workbook('./DataSet/university data.xlsx')
table = data.sheets()[0]
x1=[]
x2=[]
x3=[]
x4=[]
print'UBitName = junxuanh'
print 'personNumber = 50245589'
print'UBitName = sililiu'
print 'personNumber = 50132918'
for i in range(1,50):
    x1.append(table.cell(i,2).value)
    x2.append(table.cell(i,3).value)
    x3.append(table.cell(i,4).value)
    x4.append(table.cell(i,5).value)
x1=np.array(x1)
mu1=x1.mean()
var1=x1.var()
sigma1=x1.std()
x2=np.array(x2)
mu2=x2.mean()
var2=x2.var()
sigma2=x2.std()
x3=np.array(x3)
mu3=x3.mean()
var3=x3.var()
sigma3=x3.std()
x4=np.array(x4)
mu4=x4.mean()
var4=x4.var()
sigma4=x4.std()
print 'mu1=',mu1,'\nmu2=',mu2,'\nmu3=',mu3,'\nmu4=',mu4
print 'var1=',var1,'\nvar2=',var2,'\nvar3=',var3,'\nvar4=',var4
print 'sigma1=',sigma1,'\nsigma2=',sigma2,'\nsigma3=',sigma3,'\nsigma4=',sigma4
x=np.array((x1,x2,x3,x4))
covarianceMat=np.cov(x)
correlationMat=np.corrcoef(x)
correlationMat=format(correlationMat)
l1=loglike(x1,mu1,sigma1)
l2=loglike(x2,mu2,sigma2)
l3=loglike(x3,mu3,sigma3)
l4=loglike(x4,mu4,sigma4)
print 'covarianceMat=\n',covarianceMat 
print 'correlationMat=\n',correlationMat
print 'loglikelihoodx1=',l1
print 'loglikelihoodx2=',l2
print 'loglikelihoodx3=',l3
print 'loglikelihoodx4=',l4
print 'loglikelihood=',l1+l2+l3+l4
n = 4 # the number of variables 
multivariateLoglikelihood = 0;
mu = np.array([mu1, mu2, mu3, mu4])
muVector = mu.reshape(4,1)
for i in range(len(x1)):
    xVector = np.vstack([x1[i],x2[i], x3[i], x4[i]])
    single = 1/((2*math.pi)**(n/2) * (np.linalg.det(covarianceMat)**0.5)) * np.exp(-0.5 * np.dot(np.dot(np.transpose(np.subtract(xVector,muVector)), inv(covarianceMat)), np.subtract(xVector,muVector)))
    multivariateLoglikelihood += np.log(single)
print 'multivariateLoglikelihood=',multivariateLoglikelihood