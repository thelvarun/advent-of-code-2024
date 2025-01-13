from collections import deque


def solve2(coords: list[tuple[int, int]]):
  # Create our 71 by 71 grid of characters
  grid_size = 71

  # grid[i][j] tells us if grid[i][j] is corrupted. EX: grid[0][5] = True tells
  # us that coordinate 0, 5 is corrupted
  # char_grid = [['.' for _ in range(grid_size)] for _ in range(grid_size)]
  grid = [[False for _ in range(grid_size)] for _ in range(grid_size)]

  def is_corrupted(i: int, j: int) -> bool:
    nonlocal grid
    return grid[i][j]

  def inb(i: int, j: int) -> bool:
    nonlocal grid_size
    return 0 <= i < grid_size and 0 <= j < grid_size
  
  # instead of marking the corrupted spots and then running a BFS, we'll mark
  # a spot and then run a bfs. Since there are ~4900 spots in the grid, the 
  # total time this will take will be around 5000 * 5000 = 25,000,000, which is
  # long but not terribly long. 
  # 
  # Actually, it'll take a little less since we know the first 1024 spots still lead to an exit.
  # We can essentially skip the first 1024 (after marking them of course)

  # Mark corrupted spots 
  num_coords_that_still_we_know_still_lead_to_exit = 1024
  for i in range(num_coords_that_still_we_know_still_lead_to_exit):
    coord = coords[i]
    grid[coord[0]][coord[1]] = True
  
  ans = (-1, -1)
  for i in range(len(coords)):
    # Now mark the next corrupted byte
    cur_corrupted_coord = coords[i]
    # print("marking", cur_corrupted_coord, "as corrupted")
    grid[cur_corrupted_coord[0]][cur_corrupted_coord[1]] = True
    # print("marking char grid corrupted coord", cur_corrupted_coord, "as corrupted")
    # char_grid[cur_corrupted_coord[0]][cur_corrupted_coord[1]] = '#'

    # # Create a copy of the grid for the bfs
    # char_grid_copy = [[x for x in row]for row in char_grid]
    # def print_char_grid():
    #   nonlocal char_grid_copy
    #   for row in char_grid_copy:
    #     print(row)

    # Visited 2D array for our BFS
    visited = [[False for _ in range(grid_size)] for _ in range(grid_size)]

    # Queue of coords as first two ints, number of steps as last int
    queue: deque[tuple[int, int, int]] = deque()
    queue.append((0, 0, 0))
    visited[0][0] = True
    can_reach_exit = False
    # char_grid_copy[0][0] = 'O'
    # print("---beginning---")
    while queue:
      cur = queue.popleft()
      # print("cur in queue", cur)
      coord = cur[0], cur[1]
      # Mark where we are
      # char_grid_copy[coord[0]][coord[1]] = 'O'
      # print_char_grid()
      # input()
      if (coord == (grid_size-1, grid_size-1)):
        can_reach_exit = True
        break
      # go through our neighbors, make sure their inbounds, then
      for delta in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
        next_pos = (coord[0] + delta[0], coord[1] + delta[1])
        if inb(*next_pos) and not is_corrupted(*next_pos) and not visited[next_pos[0]][next_pos[1]]:
          queue.append((next_pos[0], next_pos[1], cur[2] + 1))
          visited[next_pos[0]][next_pos[1]] = True
    
    if not can_reach_exit:
      ans = cur_corrupted_coord
      break
  print(ans)
    

def solve(coords: list[tuple[int, int]]):
  # Create our 71 by 71 grid of characters
  grid_size = 71

  # grid[i][j] tells us if grid[i][j] is corrupted. EX: grid[0][5] = True tells
  # us that coordinate 0, 5 is corrupted
  grid = [[False for _ in range(grid_size)] for _ in range(grid_size)]

  def is_corrupted(i: int, j: int) -> bool:
    nonlocal grid
    return grid[i][j]

  def inb(i: int, j: int) -> bool:
    nonlocal grid_size
    return 0 <= i < grid_size and 0 <= j < grid_size

  # Mark corrupted spots 
  num_corrupted_bytes = 1024
  for i in range(num_corrupted_bytes):
    coord = coords[i]
    grid[coord[0]][coord[1]] = True
  
  # Visited 2D array for our BFS
  visited = [[False for _ in range(grid_size)] for _ in range(grid_size)]

  # Queue of coords as first two ints, number of steps as last int
  queue: deque[tuple[int, int, int]] = deque()
  queue.append((0, 0, 0))
  visited[0][0] = True
  ans = -1
  while queue:
    cur = queue.popleft()
    coord = cur[0], cur[1]
    if (coord == (grid_size-1, grid_size-1)):
      ans = cur[2]
      break
    # go through our neighbors, make sure their inbounds, then
    for delta in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
      next_pos = (coord[0] + delta[0], coord[1] + delta[1])
      if inb(*next_pos) and not is_corrupted(*next_pos) and not visited[next_pos[0]][next_pos[1]]:
        queue.append((next_pos[0], next_pos[1], cur[2] + 1))
        visited[next_pos[0]][next_pos[1]] = True
  print(ans)

  

coords: list[tuple[int, int]] = []
with open("day18.in") as f:
  lines = f.read().splitlines()
  for line in lines:
    coord = tuple(int(x) for x in line.split(','))
    coord = coord[1], coord[0]
    coords.append(coord)
solve2(coords)