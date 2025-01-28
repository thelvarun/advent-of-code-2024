FILE_NAME = "day21.in"

# length_cache stores the minimum length to build the path.
length_cache: dict[tuple[str, int], int] = {}
keypad_paths: dict[tuple[str, str], set[str]] = {}

# this builds the keypad sequence that me (a human) would need to press in order to control a robot that needs to press keypad_presses
def get_sequence(key_presses: str, key_press_index: int, prev_key: str, current_path: str, output: list[str]):
    global keypad_paths, length_cache
    if key_press_index == len(key_presses):
        output.append(current_path)
        return
    for path in keypad_paths[(prev_key, key_presses[key_press_index])]: # we need to use the prev key in our output since we have to start with an "A"
        get_sequence(key_presses, key_press_index + 1, key_presses[key_press_index], current_path + path + 'A', output)

def get_shortest_sequence(key_presses: str, cur_depth: int):
    global keypad_paths, length_cache
    # print(key_presses, cur_depth)
    if cur_depth == 0:
        return len(key_presses)
    if (key_presses, cur_depth) in length_cache:
        return length_cache[(key_presses, cur_depth)]
    # we can split the key presses at every A since we have to press A to input a button
    split_by_a = key_presses.split('A')
    
    sub_key_presses = split_by_a[:-1] if len(split_by_a) > 1 else split_by_a
    for i in range(len(sub_key_presses)):
        sub_key_presses[i] += "A"

    total = 0
    for sub_key_press in sub_key_presses:
        # build the key presses we'd need to do in order to make this subkey
        sub_seqs = []
        get_sequence(sub_key_press, 0, "A", "", sub_seqs)
        min_length = None
        for sub_seq in sub_seqs:
            seq_len = get_shortest_sequence(sub_seq, cur_depth-1)
            min_length = min(min_length, seq_len) if min_length else seq_len
        total += min_length
    length_cache[(key_presses, cur_depth)] = total
    return total


    

def solve2(codes: list[str]):
    global keypad_paths, length_cache
    num_pad = [["7", "8", "9"], ["4", "5", "6"], ["1", "2", "3"], ["", "0", "A"]]
    empty_space_pos_num_pad = (3, 0)
    directional_pad = [["", "^", "A"], ["<", "v", ">"]]
    empty_space_pos_directional_pad = (0, 0)

    def generate_paths(rise_run: tuple[int, int]):
        paths = set()

        def foo(rise: int, run: int, cur: str):
            nonlocal paths
            if rise < 0:
                foo(rise + 1, run, cur + "^")
            if rise > 0:
                foo(rise - 1, run, cur + "v")
            if run < 0:
                foo(rise, run + 1, cur + "<")
            if run > 0:
                foo(rise, run - 1, cur + ">")
            if not rise and not run:
                paths.add(cur)

        foo(rise_run[0], rise_run[1], "")
        return paths

    def would_pass_over_empty_space(
        path: str, start_pos: tuple[int, int], empty_space_pos: tuple[int, int]
    ):
        arrow_to_delta = {"<": (0, -1), ">": (0, 1), "^": (-1, 0), "v": (1, 0)}
        cur_pos = start_pos
        if start_pos == empty_space_pos:
            return True
        for char in path:
            delta = arrow_to_delta[char]
            cur_pos = cur_pos[0] + delta[0], cur_pos[1] + delta[1]
            if cur_pos == empty_space_pos:
                return True
        return False

    def generate_paths_for_pad(pad: list[list[str]], empty_space_pos: tuple[int, int]):
        paths_for_pad = {}
        for start_char_i, start_char_row in enumerate(pad):
            for start_char_j, start_char in enumerate(start_char_row):
                # skip the empty space since we should never start there
                if not start_char:
                    continue
                for end_char_i, end_char_row in enumerate(pad):
                    for end_char_j, end_char in enumerate(end_char_row):
                        # we should never end at the empty space either
                        if not end_char:
                            continue
                        dist = end_char_i - start_char_i, end_char_j - start_char_j

                        paths = set(
                            [
                                x
                                for x in generate_paths(dist)
                                if not would_pass_over_empty_space(
                                    x,
                                    (start_char_i, start_char_j),
                                    empty_space_pos,
                                )
                            ]
                        )
                        paths_for_pad[(start_char, end_char)] = paths
        return paths_for_pad

    paths_between_nums: dict[tuple[str, str] : set[str]] = generate_paths_for_pad(
        num_pad, empty_space_pos_num_pad
    )

    keypad_paths = generate_paths_for_pad(
        directional_pad, empty_space_pos_directional_pad
    )
    
    ans = 0
    # find me
    for code in codes:
        cur_robot_pos = "A"
        total = 0
        for letter in code:
            start_end = cur_robot_pos, letter
            cur_robot_pos = letter
            num_pad_paths = paths_between_nums[(start_end)]
            best_path_length = None
            for num_pad_path in num_pad_paths:
                seq_len = get_shortest_sequence(num_pad_path + "A", 25)
                best_path_length = min(best_path_length, seq_len) if best_path_length else seq_len
            total += best_path_length

        numeric_part = int(code[:3])
        total *= numeric_part
        ans += total
    print(ans)








def solve(codes: list[str]):
    num_pad = [["7", "8", "9"], ["4", "5", "6"], ["1", "2", "3"], ["", "0", "A"]]
    empty_space_pos_num_pad = (3, 0)
    directional_pad = [["", "^", "A"], ["<", "v", ">"]]
    empty_space_pos_directional_pad = (0, 0)

    def generate_paths(rise_run: tuple[int, int]):
        paths = set()

        def foo(rise: int, run: int, cur: str):
            nonlocal paths
            if rise < 0:
                foo(rise + 1, run, cur + "^")
            if rise > 0:
                foo(rise - 1, run, cur + "v")
            if run < 0:
                foo(rise, run + 1, cur + "<")
            if run > 0:
                foo(rise, run - 1, cur + ">")
            if not rise and not run:
                paths.add(cur)

        foo(rise_run[0], rise_run[1], "")
        return paths

    def would_pass_over_empty_space(
        path: str, start_pos: tuple[int, int], empty_space_pos: tuple[int, int]
    ):
        arrow_to_delta = {"<": (0, -1), ">": (0, 1), "^": (-1, 0), "v": (1, 0)}
        cur_pos = start_pos
        if start_pos == empty_space_pos:
            return True
        for char in path:
            delta = arrow_to_delta[char]
            cur_pos = cur_pos[0] + delta[0], cur_pos[1] + delta[1]
            if cur_pos == empty_space_pos:
                return True
        return False

    def generate_paths_for_pad(pad: list[list[str]], empty_space_pos: tuple[int, int]):
        paths_for_pad = {}
        for start_char_i, start_char_row in enumerate(pad):
            for start_char_j, start_char in enumerate(start_char_row):
                # skip the empty space since we should never start there
                if not start_char:
                    continue
                for end_char_i, end_char_row in enumerate(pad):
                    for end_char_j, end_char in enumerate(end_char_row):
                        # we should never end at the empty space either
                        if not end_char:
                            continue
                        dist = end_char_i - start_char_i, end_char_j - start_char_j

                        paths = set(
                            [
                                x
                                for x in generate_paths(dist)
                                if not would_pass_over_empty_space(
                                    x,
                                    (start_char_i, start_char_j),
                                    empty_space_pos,
                                )
                            ]
                        )
                        paths_for_pad[(start_char, end_char)] = paths
        return paths_for_pad

    paths_between_nums: dict[tuple[str, str] : set[str]] = generate_paths_for_pad(
        num_pad, empty_space_pos_num_pad
    )
    paths_between_directions: dict[tuple[str, str] : set[str]] = generate_paths_for_pad(
        directional_pad, empty_space_pos_directional_pad
    )

    # this might be the worst code i've ever written in my entire lifewh
    total = 0
    for code in codes:
        path_for_code = ""
        cur_r1_pos = "A"
        for letter in code:
            start_end = cur_r1_pos, letter
            cur_r1_pos = letter
            num_pad_paths = paths_between_nums[(start_end)]
            shortest_translated_num_pad_path = ""

            # go through each of the different paths we can take on the num pad, translate them, then choose the shortest of the translated paths.
            for num_pad_path in num_pad_paths:
                cur_r2_pos = "A"
                num_pad_path += "A"  # we need to press A at the end of the path to actually press the character.
                translated_path = ""
                for dir_char in num_pad_path:
                    dir_char_start_end = cur_r2_pos, dir_char
                    cur_r2_pos = dir_char
                    shortest_dir_path = ""
                    for dir_pad_path in paths_between_directions[(dir_char_start_end)]:
                        dir_pad_path += "A"
                        cur_r3_pos = "A"
                        shortest_secondary_translation = ""
                        for secondary_dir_char in dir_pad_path:
                            secondary_dir_char_start_end = cur_r3_pos, secondary_dir_char
                            cur_r3_pos = secondary_dir_char
                            shortest_secondary_dir_path = ""
                            for dir_pad_path2 in paths_between_directions[(secondary_dir_char_start_end)]:
                                if not shortest_secondary_dir_path or len(dir_pad_path2) <= len(shortest_secondary_dir_path):
                                    shortest_secondary_dir_path = dir_pad_path2
                            shortest_secondary_translation += shortest_secondary_dir_path + "A"
                        if not shortest_dir_path or len(shortest_secondary_translation) <= len(shortest_dir_path):
                            shortest_dir_path = shortest_secondary_translation
                    translated_path += shortest_dir_path
                if not shortest_translated_num_pad_path or len(translated_path) <= len(shortest_translated_num_pad_path):
                    shortest_translated_num_pad_path = translated_path
            path_for_code += shortest_translated_num_pad_path
                        
            
        print("path for", code, ":", path_for_code)
        numeric_part = int(code[:3])
        total += numeric_part * len(path_for_code)
    print(total)


with open(FILE_NAME) as f:
    lines = f.read().splitlines()
    print(lines)
    solve2(lines)
