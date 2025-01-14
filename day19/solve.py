FILE_NAME = "day19.in"

def solve2(towels: list[str], designs: list[str]):
  total = 0
  for design in designs:
    can_get_to = [0] * (len(design) + 1)
    # can_get_to[i] means we can make a substring of length i

    # We can always make the empty string.
    can_get_to[0] = 1
    for i in range(1, len(can_get_to)):
      # Now go through each towel to see if we can make it
      can_make_substr_count = 0
      for towel in towels:
        if len(towel) > i:
          # this means the towel is longer than our current substring.
          continue
        if not can_get_to[i - len(towel)]:
          # this means we couldn't get to the substring that ends before this 
          # substring would start, so there's no point checking.
          continue
        if towel == design[i - len(towel):i]:
          can_make_substr_count += can_get_to[i - len(towel)]
      can_get_to[i] = can_make_substr_count

    total += can_get_to[len(design)]
  print(total)
  return

def solve(towels: list[str], designs: list[str]):
  total = 0
  for design in designs:
    can_get_to = [False] * (len(design) + 1)
    # can_get_to[i] means we can make a substring of length i

    # We can always make the empty string.
    can_get_to[0] = True
    for i in range(1, len(can_get_to)):
      # Now go through each towel to see if we can make it
      can_make_substr = False
      for towel in towels:
        if len(towel) > i:
          # this means the towel is longer than our current substring.
          continue
        if not can_get_to[i - len(towel)]:
          # this means we couldn't get to the substring that ends before this 
          # substring would start, so there's no point checking.
          continue
        if towel == design[i - len(towel): i]:
          can_make_substr = True
          break
      can_get_to[i] = can_make_substr

    if can_get_to[len(design)]:
      total += 1
  print(total)
  return

with open(FILE_NAME) as f:
  lines = f.read().splitlines()
  towels = [x.strip() for x in lines[0].split(',')]
  designs = lines[2:]
  solve2(towels, designs)
