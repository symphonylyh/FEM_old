#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr 22 03:52:19 2018

@author: luojiayi
"""

import numpy as np
E = np.zeros((2,3))
F = np.zeros((2,3))
E[:,0] = 50000
E[:,1] = 0.15
E[:,2] = 4
F[0,0] = 2
F[1,0] = 4
F[0,1] = 500
F[1,1] = 600
F[0,2] = 1
F[1,2] = 1
x0 = 0.0
x1 = 12.0
_bounded = 0
thermal_cond = "6.5e-6"
temperature_change = 100
B_F = (0,-0.0807)

# X,Y-direction mesh
x_num = 13
y_num = 13
err = 0.5
# Initialize
for i in range(int(E.shape[0]/2)):
    temp = E[i];
    E[i] = E[E.shape[0]-1-i]
    E[E.shape[0]-1-i] = temp 
X = [x0]
for i in range(F.shape[0]):
    X.append(F[i,0])
X.append(x1)
Y = [0]   
for i in range(E.shape[0]):
    Y.append(np.sum(E[0:i+1,2]))
x_len = X[-1] - X[0]
y_len = Y[-1] - Y[0]
x_sec = len(X)-1 # Number of x sections
y_sec = len(Y)-1 # Number of y sections

# Calculate y coordinates
# Last Layer (twice mesh desity && logspace)
# num_ycor1 = 2*np.abs(int(np.ceil((Y[1] - Y[0])/y_len*y_num)))
# y_cor1 = np.logspace(np.log2(Y[0]+err), np.log2(Y[1]+err), num=num_ycor1, endpoint = False, base=2.0) - err
# y_cor_list = list(y_cor1)
y_cor_list = []
# Remained Layers (linspace)
for i in range(y_sec - 1):
    num_ycor1 = np.abs(int(np.ceil((Y[i+1] - Y[i])/y_len*y_num)))
    y_cor_temp = np.linspace(Y[i], Y[i+1], num = num_ycor1, endpoint = False)
    y_cor_list +=(list(y_cor_temp))
num_ycor1 = 2*np.abs(int(np.ceil((Y[-1] - Y[-2])/y_len*y_num)))
y_cor1 = np.logspace(np.log2(Y[-2]+err), np.log2(Y[-1]+err), num=num_ycor1, base=2.0) - err
y_cor_list += list(Y[-2] + Y[-1] - y_cor1[::-1])
#y_cor_list.append(Y[-1])


# Calculate x coordinates
#x_cor_list = []
x_cor_inner = []
x_start =[]
x_end = []
if x_sec == 1:
    x_cor_list = np.linspace(X[0], X[-1], num = x_num)
elif x_sec >= 2:
    num_xcor_start = np.abs(int(np.ceil((X[1] - X[0])/x_len*x_num)))
    x_cor_temp_start = np.logspace(np.log2(X[0]+err), np.log2(X[1]+err), num=num_xcor_start+1, base=2.0) - err
    num_xcor_end = np.abs(int(np.ceil((X[-1] - X[-2])/x_len*x_num)))
    x_cor_temp_end = np.logspace(np.log2(X[-2]+err), np.log2(X[-1]+err), num=num_xcor_end ,base=2.0) - err
    x_start = list(X[0] + X[1] - x_cor_temp_start[::-1])
    del x_start[-1]
    x_end = list(x_cor_temp_end)
    for i in range(x_sec - 2):
        num_xcor = np.abs(int(np.ceil((X[i+2] - X[i+1])/(2*x_len)*x_num)))
        x_cor_temp1 = np.logspace(np.log2(X[i+1]+err), np.log2((X[i+2]+X[i+1])/2+err), num=num_xcor, endpoint = False, base=2.0) - err
        x_cor_temp2 = np.logspace(np.log2((X[i+2]+X[i+1])/2+err), np.log2(X[i+2]+err), num=num_xcor+1, base=2.0) - err
        x_cor_temp3 = list((X[i+2]+X[i+1])/2+X[i+2]-x_cor_temp2[::-1])
        del x_cor_temp3[-1]
        x_cor_inner = x_cor_inner + list(x_cor_temp1) + list(x_cor_temp3)
    x_cor_list = x_start + x_cor_inner + x_end

# Generate Mesh
x = np.array(x_cor_list)
y = np.array(y_cor_list)
num_elem = (x.shape[0]-1)*(y.shape[0]-1)
#num_node = (x.shape[0]*2-1)*y.shape[0] + x.shape[0]*(y.shape[0] - 1)
num_node = (x.shape[0]*2-1)*(y.shape[0]*2-1) - num_elem

# Node coordinates
node = {}
node_re = {}
#elem_cen = {}
elem_cen_re ={}
idx = 0
idx0 = 0
for j in range(2*y.shape[0]-1):
    for i in range(2*x.shape[0]-1):
        if (i%2 == 1 and j%2 != 1):
            x_temp = (x[int((i+1)/2)]+x[int((i-1)/2)])/2
            y_temp = y[int(j/2)]
            node[idx] = (x_temp, y_temp)
            node_re[x_temp, y_temp] = idx
            idx += 1
        elif (i%2 != 1 and j%2 == 1):
            y_temp = (y[int((j+1)/2)]+y[int((j-1)/2)])/2
            x_temp = x[int(i/2)]
            node[idx] = (x_temp, y_temp)
            node_re[x_temp, y_temp] = idx
            idx += 1
        elif (i%2 != 1 and j%2 != 1):
            x_temp = x[int(i/2)]
            y_temp = y[int(j/2)]
            node[idx] = (x_temp, y_temp)
            node_re[x_temp, y_temp] = idx
            idx += 1
        elif (i%2 == 1 and j%2 == 1):
            x_temp = (x[int((i+1)/2)]+x[int((i-1)/2)])/2            
            y_temp = (y[int((j+1)/2)]+y[int((j-1)/2)])/2 
            #elem_cen[idx0] = (x_temp, y_temp)
            elem_cen_re[x_temp, y_temp] = idx0
            idx0 += 1
elem = {}
for j in range(2*y.shape[0]-1):
    for i in range(2*x.shape[0]-1):
        temp = []
        if (i%2 == 1 and j%2 == 1):
            xtemp = x[int((i-1)/2)]
            ytemp = y[int((j-1)/2)]
            temp.append(node_re[xtemp,ytemp])
            
            xtemp = x[int((i+1)/2)]
            ytemp = y[int((j-1)/2)]
            temp.append(node_re[xtemp,ytemp])
            
            xtemp = x[int((i+1)/2)]
            ytemp = y[int((j+1)/2)]
            temp.append(node_re[xtemp,ytemp])
            
            xtemp = x[int((i-1)/2)]
            ytemp = y[int((j+1)/2)]
            temp.append(node_re[xtemp,ytemp])
            
            xtemp = (x[int((i+1)/2)]+x[int((i-1)/2)])/2
            ytemp = y[int((j-1)/2)]
            temp.append(node_re[xtemp,ytemp])
            
            xtemp = x[int((i+1)/2)]
            ytemp = (y[int((j+1)/2)]+y[int((j-1)/2)])/2

            temp.append(node_re[xtemp,ytemp])
            xtemp = (x[int((i+1)/2)]+x[int((i-1)/2)])/2
            ytemp = y[int((j+1)/2)]
            temp.append(node_re[xtemp,ytemp])
            xtemp = x[int((i-1)/2)]
            ytemp = (y[int((j+1)/2)]+y[int((j-1)/2)])/2
            temp.append(node_re[xtemp,ytemp])
            xtemp = (x[int((i+1)/2)]+x[int((i-1)/2)])/2
            ytemp = (y[int((j+1)/2)]+y[int((j-1)/2)])/2
            idx = elem_cen_re[xtemp, ytemp]
            elem[idx] = temp
            
diff_E = 1 # Need Modification
for i in range(E.shape[0]):
    for j in range(i):
        if (E[i,0]!= E[j,0]):
            diff_E += 1
x_full = []
y_full = []
for i in range(x.shape[0]-1):
    x_full.append(x[i])
    x_full.append((x[i]+x[i+1])/2)
x_full.append(x[-1])
x_full = np.array(x_full)

for i in range(y.shape[0]-1):
    y_full.append(y[i])
    y_full.append((y[i]+y[i+1])/2)
y_full.append(y[-1])
y_full = np.array(y_full)
       
f = np.zeros(x_full.shape[0]) #Initialize f vector
for i in range(F.shape[0]):
    f_start = F[i,0] - F[i,2] # Didn't consider the range
    f_end = F[i,0] + F[i,2]
    f_mean = F[i,1]/(2*F[i,2])
    for j in range(f.shape[0]):
        if x_full[j] >= f_start and x_full[j-1] < f_start:
            f_start_idx = j
        elif x_full[j] <= f_end and x_full[j+1] > f_end:
            f_end_idx = j
    f[f_start_idx] = ((x_full[f_start_idx]+x_full[f_start_idx+1])/2 - f_start)*f_mean
    for i in range(f_start_idx+1, f_end_idx):
        f[i] = (x_full[i+1]-x_full[i-1])/2*f_mean
    f[f_end_idx] = (-(x_full[f_end_idx-1]+x_full[f_end_idx])/2 + f_end)*f_mean

f_force_nonzero_idx = []
for i in range(f.shape[0]):
    if f[i] > 0:
        f_force_nonzero_idx.append(i)
        
 # Only include y direction
y_force = y_full[-1]
force_node = {}
for i in f_force_nonzero_idx:
    force_node[node_re[x_full[i], y_force]] = f[i]

y_load = Y[0]
pre_ydisp = {}
for i in range (x_full.shape[0]):
    pre_ydisp[node_re[x_full[i], y_load]] = 0

pre_xdisp = {}
if(_bounded):
    x_load1 = x_full[0]
    x_load2 = x_full[-1]
    for i in range (y_full.shape[0]):
        pre_xdisp[node_re[x_load1, y_full[i]]] = 0
        pre_xdisp[node_re[x_load2, y_full[i]]] = 0

#Output as an input.txt file

f = open("input_6.txt","w")
f.write("%d %d %d %d %d %d %d %d\r\n" %\
        (num_node, num_elem, diff_E, 0, len(force_node), 0, len(pre_xdisp), len(pre_ydisp)))
for i in range(num_node):
    f.write("%.4f %.4f\r\n" %(node[i][0], node[i][1]))
for i in range(num_elem):
    f.write("%d %d %d %d %d %d %d %d\r\n" %\
        (elem[i][0],elem[i][1],elem[i][2],elem[i][3],elem[i][4],elem[i][5],elem[i][6],elem[i][7]))
for i in range(num_elem - 1):
    f.write("%d "%i)
f.write("%d\r\n"% (num_elem - 1))
f.write("%.5f %.5f %.5f %.5f %s %.5f\r\n" %(E[0][0], E[0][1], B_F[0], B_F[1], thermal_cond, temperature_change))
for i in force_node:
    f.write("%d "%i) # One Space here
if (len(force_node)!=0):
    f.write("\r\n")
for i in force_node:
    f.write("%.5f "%force_node[i])
if (len(force_node)!=0):
    f.write("\r\n")
for i in pre_xdisp:
    f.write("%d "%i) # One Space here
if (len(pre_xdisp)!=0):
    f.write("\r\n")
for i in pre_xdisp:
    f.write("%.5f "%pre_xdisp[i])
if (len(pre_xdisp)!=0):
    f.write("\r\n")
for i in pre_ydisp:
    f.write("%d "%i) # One Space here
if (len(pre_ydisp)!=0):
    f.write("\r\n")
for i in pre_ydisp:
    f.write("%.5f "%pre_ydisp[i])
f.close()

    