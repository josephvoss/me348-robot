import urllib
import matplotlib.pyplot as plt
import numpy as np
import time

map_matrix = np.ones([6,6]) * 255

# 8 4 2 1
# N S E W

# 0 1 2 3
# N S E W


#map_matrix = [[9, 8, 8, 8, 8, 10], [1, 0, 0, 4, 0, 2], [1, 0, 0, 0, 0, 2], [1,
#    0, 0, 0, 0 ,2], [1, 0, 0, 0, 0, 2], [5, 4, 4, 4, 4, 6]]

map_matrix = [[13, 9, 8, 8, 8, 12], [1, 2, 4, 3, 0, 4], [1, 8, 2, 12, 1, 4],[1,
    4, 9, 0, 0, 4],[1, 0, 0, 0, 0, 4], [3, 2, 2, 2, 2, 6]]

map_matrix = np.asarray(map_matrix)

link = "http://192.168.4.1/map.html"

fig = plt.figure()
ax = plt.axes()
while True:

    #bParse webpage
    f = urllib.urlopen(link)
#    f = open("./map_test.html")
    
    line_array = []
    for line in f:
        x = ((line.rstrip('\n')).rstrip('\r')).split('\t')
        x = filter(None, x)
        x = map(int, x)
        line_array.append(np.asarray(x))
    
    map_matrix = np.asarray(line_array)

    for i, item in enumerate(map_matrix.flat):
	x = i % 6
	y = i / 6
	x_bot = x - 0.5
        y_bot = -1*(y - 0.5)
        x_top = x_bot+1
        y_top = y_bot-1

        if item & 2 > 0 and item != 255:
            ax.add_line(plt.Line2D([x_bot, x_top], [y_top, y_top]))
        if item & 8 > 0 and item != 255:
            ax.add_line(plt.Line2D([x_bot, x_top], [y_bot, y_bot]))
        if item & 4 > 0 and item != 255:
            ax.add_line(plt.Line2D([x_top, x_top], [y_bot, y_top]))
        if item & 1 > 0 and item != 255:
            ax.add_line(plt.Line2D([x_bot, x_bot], [y_bot, y_top]))
        
        ax.plot()

    ax.set_ylim([-6,1])
    ax.set_xlim([-1,6])

    plt.pause(4)
    print time.time()
    time.sleep(1)

    
