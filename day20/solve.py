from collections import deque

FILE_NAME = "day20.in"
START = "S"
END = "E"
WALL = "#"
OPEN = "."


def solve2(grid: list[list[str]], start_pos: tuple[int, int], end_pos: tuple[int, int]):
    visited = [
        [False] * len(grid[0]) for _ in range(len(grid))
    ]  # we techincally don't need this since we can use distance_to_end as a visited array as well
    distance_to_end = [[-1 for _ in range(len(row))] for row in grid]
    queue: deque[tuple[int, int, int]] = deque()
    queue.append((*end_pos, 0))
    visited[end_pos[0]][end_pos[1]] = True

    def inb(i: int, j: int):
        nonlocal grid
        return 0 <= i < len(grid) and 0 <= j < len(grid[i])

    def is_wall(i: int, j: int):
        return grid[i][j] == WALL

    # We need to find the full deltas. We know cheating
    full_cheat_deltas: list[list[tuple[int, int]]] = [[] for _ in range(21)]
    for lim in range(2, 21):
        delta = []
        for x in range(-lim, lim):
            other = lim - abs(x)
            delta.append((other, x))
            delta.append((-other, -x))
        full_cheat_deltas[lim] = delta

    while queue:
        *coords, dist = queue.popleft()
        distance_to_end[coords[0]][coords[1]] = dist
        for delta in [(0, -1), (0, 1), (-1, 0), (1, 0)]:
            new_pos = coords[0] + delta[0], coords[1] + delta[1]
            if (
                inb(*new_pos)
                and not visited[new_pos[0]][new_pos[1]]
                and not is_wall(*new_pos)
            ):
                queue.append((*new_pos, dist + 1))
                visited[new_pos[0]][new_pos[1]] = True

    # Now that we've set the distances from the start, we run a BFS again but this
    # time, at each location, we can "cheat" (AKA go to any spot 2 units away).

    # Reset visited array and queue (techincally queue doesn't need to be reset).
    visited = [[False] * len(grid[0]) for _ in range(len(grid))]
    queue: deque[tuple[int, int, int]] = deque()
    time_saves: dict[int : set[tuple[tuple[int, int], tuple[int, int]]]] = {}

    queue.append((start_pos))
    visited[start_pos[0]][start_pos[1]] = True
    while queue:
        coords = queue.popleft()

        # Now calculate the "cheats" - up to 20 seconds inclusive
        for cheat_time in range(2, 21):
            for cheat_delta in full_cheat_deltas[cheat_time]:
                cheat_pos = coords[0] + cheat_delta[0], coords[1] + cheat_delta[1]
                if inb(*cheat_pos) and not is_wall(*cheat_pos):
                    # Calculate the distance saved as cur - new - 2 (since each cheat takes 2 seconds)
                    time_saved = (
                        distance_to_end[coords[0]][coords[1]]
                        - distance_to_end[cheat_pos[0]][cheat_pos[1]]
                        - cheat_time
                    )
                    if time_saved > 0:
                        if time_saved in time_saves:
                            time_saves[time_saved].add((coords, cheat_pos))
                        else:
                            time_saves[time_saved] = set()
                            time_saves[time_saved].add((coords, cheat_pos))

        for delta in [(0, -1), (0, 1), (-1, 0), (1, 0)]:
            new_pos = coords[0] + delta[0], coords[1] + delta[1]
            if (
                inb(*new_pos)
                and not visited[new_pos[0]][new_pos[1]]
                and not is_wall(*new_pos)
            ):
                queue.append(new_pos)
                visited[new_pos[0]][new_pos[1]] = True

    total = 0
    for time_save in time_saves:
        if time_save >= 100:
            total += len(time_saves[time_save])
    print(total)
    return


def solve(grid: list[list[str]], start_pos: tuple[int, int], end_pos: tuple[int, int]):
    visited = [
        [False] * len(grid[0]) for _ in range(len(grid))
    ]  # we techincally don't need this since we can use distance_to_end as a visited array as well
    distance_to_end = [[-1 for _ in range(len(row))] for row in grid]
    queue: deque[tuple[int, int, int]] = deque()
    queue.append((*end_pos, 0))
    visited[end_pos[0]][end_pos[1]] = True

    def inb(i: int, j: int):
        nonlocal grid
        return 0 <= i < len(grid) and 0 <= j < len(grid[i])

    def is_wall(i: int, j: int):
        return grid[i][j] == WALL

    while queue:
        *coords, dist = queue.popleft()
        distance_to_end[coords[0]][coords[1]] = dist
        for delta in [(0, -1), (0, 1), (-1, 0), (1, 0)]:
            new_pos = coords[0] + delta[0], coords[1] + delta[1]
            if (
                inb(*new_pos)
                and not visited[new_pos[0]][new_pos[1]]
                and not is_wall(*new_pos)
            ):
                queue.append((*new_pos, dist + 1))
                visited[new_pos[0]][new_pos[1]] = True

    # Now that we've set the distances from the start, we run a BFS again but this
    # time, at each location, we can "cheat" (AKA go to any spot 2 units away).

    # Reset visited array and queue (techincally queue doesn't need to be reset).
    visited = [[False] * len(grid[0]) for _ in range(len(grid))]
    queue: deque[tuple[int, int, int]] = deque()
    time_saves: dict[int : list[tuple[tuple[int, int], tuple[int, int]]]] = {}

    queue.append((start_pos))
    visited[start_pos[0]][start_pos[1]] = True
    while queue:
        coords = queue.popleft()
        udlr = [(0, -1), (0, 1), (-1, 0), (1, 0)]

        # Now calculate the "cheats"
        for cd1 in udlr:
            for cd2 in udlr:
                total_delta = cd1[0] + cd2[0], cd1[1] + cd2[1]
                cheat_pos = coords[0] + total_delta[0], coords[1] + total_delta[1]
                if inb(*cheat_pos) and not is_wall(*cheat_pos):
                    # Calculate the distance saved as cur - new - 2 (since each cheat takes 2 seconds)
                    time_saved = (
                        distance_to_end[coords[0]][coords[1]]
                        - distance_to_end[cheat_pos[0]][cheat_pos[1]]
                        - 2
                    )
                    if time_saved > 0:
                        if time_saved in time_saves:
                            time_saves[time_saved].append((coords, cheat_pos))
                        else:
                            time_saves[time_saved] = [(coords, cheat_pos)]

        for delta in udlr:
            new_pos = coords[0] + delta[0], coords[1] + delta[1]
            if (
                inb(*new_pos)
                and not visited[new_pos[0]][new_pos[1]]
                and not is_wall(*new_pos)
            ):
                queue.append(new_pos)
                visited[new_pos[0]][new_pos[1]] = True

    total = 0
    for time_save in time_saves:
        if time_save >= 100:
            total += len(time_saves[time_save])
    print(total)
    return


with open(FILE_NAME) as f:
    grid = f.read().splitlines()
    # find the start and end position
    start_pos = None
    end_pos = None
    for i in range(len(grid)):
        for j in range(len(grid[i])):
            if grid[i][j] == START:
                start_pos = (i, j)
            elif grid[i][j] == END:
                end_pos = (i, j)
    solve2(grid, start_pos, end_pos)
