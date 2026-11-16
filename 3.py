graph = {
    'A':[('B',1),('C',4)],
    'B':[('D',3)],
    'C':[('D',2)],
    'D':[]
}

h = {'A':3,'B':3,'C':2,'D':0}

def search(start,goal,astar):
    q=[(h[start],0,[start])]
    
    while q:
        q.sort()
        _,cost,path = q.pop(0)
        node = path[-1]

        if node == goal:
            return cost,path
        for n,w in graph[node]:
            if n not in path:
                c= cost +w
                score = c + h[n] if astar else h[n]
                q.append((score,c,path+[n]))
    
print("Greedy:", search('A', 'D', False))
print("A*:", search('A', 'D', True))
