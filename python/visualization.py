import os
 
data_path = os.path.abspath(__file__ + '/../data' )

with open(data_path + '/all_points.txt', 'r') as f:
    data = f.readlines()

    for line in data:
        odom = line.split()        #将单个数据分隔开存好  
        numbers_float = list(map(float, odom)) #转化为浮点数  
        x.append( numbers_float[0] )
        y.append( numbers_float[1] )
        z.append( numbers_float[2] )


