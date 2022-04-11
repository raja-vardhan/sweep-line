import matplotlib.pyplot as plt

n = int(input("Enter the number of points: "))
pts = []

for i in range(0, n):
    a = [float(x) for x in input().split()]
    pts.append(a)

for i in range(0, n):
    a = pts[i]
    plt.plot([a[0], a[2]], [a[1], a[3]])

plt.show()