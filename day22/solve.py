INPUT_FILE = "day22.in"

def mix(secret_num: int, mix_value: int):
  return secret_num ^ mix_value

def prune(secret_num: int):
  return secret_num % 16777216

def solve2(secret_nums: list[int]):
  most_bananas_sold = 0
  best_banana_per_price_change: dict[tuple[int, int, int, int], int] = {}
  for secret_num in secret_nums:
    last_four_changes = []
    last_ones_digit = secret_num % 10
    cur_secret_num = secret_num
    packing_num = 0
    seen_changes = set()
    for _ in range(2000):
      # 1. mix with secret_num * 64 then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num * 64))
      # 2. mix secret_num / 32, then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num // 32))
      # 3. mix with secret_num * 64 then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num * 2048))
      cur_ones_digit = cur_secret_num % 10
      if len(last_four_changes) < 4:
        last_four_changes.append(cur_ones_digit - last_ones_digit)
      else:
        last_four_changes[packing_num] = cur_ones_digit - last_ones_digit
        packing_num += 1
        packing_num %= 4

      # todo: where do we switch the nums?
      if len(last_four_changes) == 4:
        tup = None
        if packing_num == 0:
          tup = (last_four_changes[0], last_four_changes[1], last_four_changes[2], last_four_changes[3])
        elif packing_num == 1:
          tup = (last_four_changes[1], last_four_changes[2], last_four_changes[3], last_four_changes[0])
        elif packing_num == 2:
          tup = (last_four_changes[2], last_four_changes[3], last_four_changes[0], last_four_changes[1])
        elif packing_num == 3:
          tup = (last_four_changes[3], last_four_changes[0], last_four_changes[1], last_four_changes[2])
        else:
          assert('wtf')
        if tup not in seen_changes:
          if tup in best_banana_per_price_change:
            best_banana_per_price_change[tup] += cur_ones_digit
          else:
            best_banana_per_price_change[tup] = cur_ones_digit
          seen_changes.add(tup)
          most_bananas_sold = max(most_bananas_sold, best_banana_per_price_change[tup])
      last_ones_digit = cur_ones_digit
  print(most_bananas_sold)

def solve(secret_nums: list[int]):
  total = 0
  for secret_num in secret_nums:
    cur_secret_num = secret_num
    for _ in range(2000):
      # 1. mix with secret_num * 64 then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num * 64))
      # 2. mix secret_num / 32, then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num // 32))
      # 3. mix with secret_num * 64 then prune
      cur_secret_num = prune(mix(cur_secret_num, cur_secret_num * 2048))
    total += cur_secret_num
  print(total)
    

    

with open(INPUT_FILE) as f:
  lines = [int(x) for x in f.read().splitlines()]
  solve2(lines)