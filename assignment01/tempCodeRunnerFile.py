you=open('myresult')
m=open('result')

a=[]
b=[]
c=0

for i in you.readlines():
    a.append(i)
for i in m.readlines():
    b.append(i)

for i in range(len(a)):
    if a[i]!=b[i]:
        print('error\t',i)
        break
  