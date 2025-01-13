from __future__ import annotations
from typing import Dict
import math

WALL = "#"
EXIT_SPOT = "E"
START_SPOT = "S"
OPEN_SPOT = "."

class Edge:
  def __init__(self, dest: Node, cost: int):
    self.dest = dest
    self.cost = cost
  
  def __str__(self):
    return f"dest={self.dest.id},cost={self.cost}"

class Node:
  def __init__(self, id: tuple[int, int], neighbors: list[Edge] = None, visited: bool = False, dist: int = math.inf):
    self.id = id
    if neighbors is None:
      self.neighbors: list[Edge] = []
    self.visited = visited
    self.dist = dist
    self.parents: list[Node] = []
  
  def __str__(self):
    return f"id={self.id},visited={self.visited},dist={self.dist},\nneighbors={[str(edge) for edge in self.neighbors]},\nparents={[node.id for node in self.parents]}"

class Spot:
  def __init__(self, id: tuple[int, int]):
    # each node, when you get to it, also implicitly represents what direction
    # you're facing i.e. when you get into the left_node of any node, you'd be
    # facing right
    self.left_node = Node(id)
    self.right_node = Node(id)
    self.top_node = Node(id)
    self.bot_node = Node(id)
    self.id = id
  
  def get_dist(self):
    return min(self.left_node.dist if self.left_node.dist else -1, self.right_node.dist if self.right_node.dist else -1, self.bot_node.dist if self.bot_node.dist else -1, self.top_node.dist if self.top_node.dist else -1)
    
  
  def __str__(self):
    return f"------\nid={self.id},\n++++++\nleft_node={self.left_node},\n++++++\nright_node={self.right_node},\n++++++\ntop_node={self.top_node},\n++++++\nbot_node={self.bot_node}"

grid = []
with open("day16.in") as f:
  grid = f.read().splitlines()

start_spot: tuple
exit_spot: tuple
spots: Dict[tuple[int, int], Spot] = {}
for i, line in enumerate(grid):
  for j, char in enumerate(line):
    if char != WALL: 
      spot = Spot((i, j))
      spots[(i, j)] = spot
    if char == START_SPOT:
      start_spot = (i, j)
    if char == EXIT_SPOT:
      exit_spot = (i, j)

for cur_spot in spots.values():
  # check left right up and down
  cur_pos = cur_spot.id
  left = cur_pos[0],cur_pos[1]-1
  right = cur_pos[0], cur_pos[1]+1
  up = cur_pos[0]-1, cur_pos[1]
  down = cur_pos[0]+1, cur_pos[1]

  if left in spots:
    # there exists a spot to our left, connect all our internal nodes to the left
    # spot
    left_spot = spots[left]
    cur_spot.top_node.neighbors.append(Edge(left_spot.right_node, 1001))
    cur_spot.right_node.neighbors.append(Edge(left_spot.right_node, 1))
    cur_spot.bot_node.neighbors.append(Edge(left_spot.right_node, 1001))
  
  if right in spots:
    right_spot = spots[right]
    cur_spot.top_node.neighbors.append(Edge(right_spot.left_node, 1001))
    cur_spot.left_node.neighbors.append(Edge(right_spot.left_node, 1))
    cur_spot.bot_node.neighbors.append(Edge(right_spot.left_node, 1001))
  
  if up in spots:
    up_spot = spots[up]
    cur_spot.left_node.neighbors.append(Edge(up_spot.bot_node, 1001))
    cur_spot.right_node.neighbors.append(Edge(up_spot.bot_node, 1001))
    cur_spot.bot_node.neighbors.append(Edge(up_spot.bot_node, 1))
  
  if down in spots:
    down_spot = spots[down]
    cur_spot.top_node.neighbors.append(Edge(down_spot.top_node, 1))
    cur_spot.right_node.neighbors.append(Edge(down_spot.top_node, 1001))
    cur_spot.left_node.neighbors.append(Edge(down_spot.top_node, 1001))


# now let's put all the nodes into one list so that we have something that we can iterate over nicely
nodes: list[Node] = []
for spot in spots.values():
  nodes.append(spot.left_node)
  nodes.append(spot.right_node)
  nodes.append(spot.top_node)
  nodes.append(spot.bot_node)

# now we run dijkstra's, starting from the start positions "left" node since we
# apparently start out facing east.
spots[start_spot].left_node.dist = 0
spots[start_spot].left_node.parents = []

while (True):
  # Find the smallest unvisited node in the list, passing by nodes whos dist are inf
  cur_node = None
  min_dist = None
  for node in nodes:
    if not node.visited and node.dist != math.inf:
      if min_dist is None:
        min_dist = node.dist
        cur_node = node
      else:
        if node.dist < min_dist:
          min_dist = node.dist
          cur_node = node
  # there are no other nodes to process
  if cur_node is None:
    break;

  # otherwise, we found a node, visit it, update the neighbors
  cur_node.visited = True
  for edge in cur_node.neighbors:
    if cur_node.dist + edge.cost < edge.dest.dist:
      edge.dest.dist = cur_node.dist + edge.cost
      edge.dest.parents = [cur_node]
    elif cur_node.dist + edge.cost == edge.dest.dist:
      edge.dest.parents.append(cur_node)

min_dist = spots[exit_spot].get_dist()
print(min_dist)

# now start tracing the parents using a dfs, find the minimum(s) from the exit spot and go backwards.
best_path_tuples = set()
stack: list[Node] = []

# first find all the best paths from the end, then we can start the dfs
if spots[exit_spot].bot_node is not None and spots[exit_spot].bot_node.dist == min_dist:
  stack.append(spots[exit_spot].bot_node)

if spots[exit_spot].top_node is not None and spots[exit_spot].top_node.dist == min_dist:
  stack.append(spots[exit_spot].top_node)

if spots[exit_spot].left_node is not None and spots[exit_spot].left_node.dist == min_dist:
  stack.append(spots[exit_spot].left_node)

if spots[exit_spot].right_node is not None and spots[exit_spot].right_node.dist == min_dist:
  stack.append(spots[exit_spot].right_node)

# now that we have the minimums, we can do our little dfs and figure out the spots
while stack:
    node = stack.pop()
    best_path_tuples.add(node.id)
    for parent in node.parents:
        stack.append(parent)

print(len(best_path_tuples))