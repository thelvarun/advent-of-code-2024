from operator import itemgetter
import time

INPUT_FILE = "day23.in"

def solve(lines: list[str]):
  network = []
  total = 0
  for line in lines:
    elems = line.split('-')
    network.append(elems if elems[0] < elems[1] else elems[::-1])
  network.sort(key=itemgetter(0,1))
  graph: dict[str, set] = {}
  for elem in network:
    first, second = elem
    if first not in graph:
      graph[first] = set()
    graph[first].add(second)

    if second not in graph:
      graph[second] = set()
    graph[second].add(first)
    
    
    # now we check if the second has any neighbors that connect with first
    for neighbor in graph[second]:
      if first in graph[neighbor] and (neighbor[0] == 't' or first[0] == 't' or second[0] == 't'):
        total += 1
  print(total)

def solve2(lines: list[str]):
  network = []
  total = 0
  for line in lines:
    elems = line.split('-')
    network.append(elems)
    network.append(elems[::-1])
  graph: dict[str, set] = {}
  for elem in network:
    first, second = elem
    if first not in graph:
      graph[first] = set()
    graph[first].add(first)
    graph[first].add(second)

    if second not in graph:
      graph[second] = set()
    graph[second].add(second)
    graph[second].add(first)

    
  best_set = set()
  for key in sorted(graph):
    neighbors = graph[key]
    for neighbor in neighbors:
      if neighbor != key:
        intersection = neighbors.intersection(graph[neighbor])
        final_intersection = intersection
        for intersection_neighbor in intersection:
          if intersection_neighbor != key:
            final_intersection = final_intersection.intersection(neighbors.intersection(graph[intersection_neighbor]))
        if len(final_intersection) > len(best_set):
          best_set = final_intersection
  print(','.join(sorted(best_set)))


with open(INPUT_FILE) as f:
  lines = f.read().splitlines()
  t0 = time.time()
  solve2(lines)
  t1 = time.time()
  time_taken = t1-t0
  print(time_taken)