
graph = {
    'A': ['B', 'C'],
    'B': ['D', 'E'],
    'C': ['F'],
    'D': [],
    'E': [],
    'F': []
}

# Breadth First Search
def bfs(start):
    visited = []
    queue = [start]

    while queue:
        node = queue.pop(0)

        if node not in visited:
            print(node, end=" ")
            visited.append(node)

            for neighbour in graph[node]:
                queue.append(neighbour)


# Depth First Search
def dfs(start):
    visited = []
    stack = [start]

    while stack:
        node = stack.pop()

        if node not in visited:
            print(node, end=" ")
            visited.append(node)

            for neighbour in reversed(graph[node]):
                stack.append(neighbour)


print("BFS Traversal:")
bfs('A')

print("\nDFS Traversal:")
dfs('A')
