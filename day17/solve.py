class Computer:
  def __init__(self, reg_a:int = 0, reg_b:int = 0, reg_c: int =0, instructions: list[int] = []):
    self.instruction_pointer = 0
    self.reg_a = reg_a
    self.reg_b = reg_b
    self.reg_c = reg_c
    self.instructions = instructions
    self.output = []
  
  def combo_operand(self, operand: int):
    if 0 <= operand <= 3:
      return operand
    elif operand == 4:
      return self.reg_a
    elif operand == 5:
      return self.reg_b
    elif operand == 6:
      return self.reg_c
    print('wtf! combo operand:', operand)
    assert(False)
  #   The adv instruction (opcode 0) performs division. The numerator is the value in the A register. The denominator is found by raising 2 to the power of the instruction's combo operand. (So, an operand of 2 would divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result of the division operation is truncated to an integer and then written to the A register.
  def adv(self, operand: int):
    self.reg_a //= (2 ** self.combo_operand(operand))
    self.instruction_pointer += 2

 
# The bxl instruction (opcode 1) calculates the bitwise XOR of register B and the instruction's literal operand, then stores the result in register B.
  def bxl(self, operand: int):
    self.reg_b = self.reg_b ^ operand
    self.instruction_pointer += 2


# The bst instruction (opcode 2) calculates the value of its combo operand modulo 8 (thereby keeping only its lowest 3 bits), then writes that value to the B register.
  def bst(self, operand: int):
    self.reg_b = self.combo_operand(operand) % 8
    self.instruction_pointer += 2

# 2,4,1,3,7,5,4,7,0,3,1,5,5,5,3,0
# (2, 4) take the last 3 bits of a and move them into b
# (1, 3) b = b ^ 3
# (7, 5) c = a // 2**b
# (4, 7) b = b ^ c
# (0, 3) a = a // 2**3
# (1, 5) b = b ^ 5
# (5, 5) output last 3 bits of b register
# (3, 0) jump to instruction 0 if register a is not 0.
# The jnz instruction (opcode 3) does nothing if the A register is 0. However, if the A register is not zero, it jumps by setting the instruction pointer to the value of its literal operand; if this instruction jumps, the instruction pointer is not increased by 2 after this instruction.
  def jnz(self, operand: int):
    if self.reg_a:
      self.instruction_pointer = operand
    else:
      self.instruction_pointer += 2

# The bxc instruction (opcode 4) calculates the bitwise XOR of register B and register C, then stores the result in register B. (For legacy reasons, this instruction reads an operand but ignores it.)
  def bxc(self, operand: int):
    self.reg_b = self.reg_b ^ self.reg_c
    self.instruction_pointer += 2

# The out instruction (opcode 5) calculates the value of its combo operand modulo 8, then outputs that value. (If a program outputs multiple values, they are separated by commas.)
  def out(self, operand: int):
    # print('outputting', self.combo_operand(operand) % 8)
    self.output.append(self.combo_operand(operand) % 8)
    self.instruction_pointer += 2

# The bdv instruction (opcode 6) works exactly like the adv instruction except that the result is stored in the B register. (The numerator is still read from the A register.)
  def bdv(self, operand: int):
    self.reg_b = self.reg_a // (2 ** self.combo_operand(operand))
    self.instruction_pointer += 2

# The cdv instruction (opcode 7) works exactly like the adv instruction except that the result is stored in the C register. (The numerator is still read from the A register.)
  def cdv(self, operand: int):
    self.reg_c = self.reg_a // (2 ** self.combo_operand(operand))
    self.instruction_pointer += 2
  
  def process_instructions(self):
    operations = [self.adv, self.bxl, self.bst, self.jnz, self.bxc, self.out, self.bdv, self.cdv]
    while self.instruction_pointer < len(self.instructions):
      # print('reg a', self.reg_a, 'reg b', self.reg_b, 'reg c', self.reg_c)
      opcode = self.instructions[self.instruction_pointer]
      operand = self.instructions[self.instruction_pointer + 1]
      # print('op code', opcode, operand, operand)
      operations[opcode](operand)


def convert_to_num(a_digits: list[int]) -> int:
  s = 0
  for dig in a_digits:
    s <<= 3
    s |= dig
  return s

def foo(cur_instruction_index, instructions, a_digits: list[int]):
  if (cur_instruction_index == -1):
    return a_digits
  cur_instruction = instructions[cur_instruction_index]
  for i in range(8):
    a_val = (convert_to_num(a_digits) << 3) | i
    computer = Computer(a_val, 0, 0, instructions[:-2]) # ignore the jump instruction
    computer.process_instructions()
    assert(len(computer.output) == 1)
    if (computer.output[0] == cur_instruction):
      new_a_digits = a_digits[:]
      new_a_digits.append(i)
      output = foo(cur_instruction_index-1, instructions, new_a_digits)
      if output:
        return output

with open("day17.in") as f:
  lines = f.read().splitlines()
  reg_a = int(lines[0].split()[-1])
  reg_b = int(lines[1].split()[-1])
  reg_c = int(lines[2].split()[-1])
  instructions = [int(x) for x in lines[4].split()[1].split(',')]
  computer = Computer(reg_a, reg_b, reg_c, instructions)
  computer.process_instructions()
  print("find me", computer.output)
  # figure out the bits, 3 bits at a time.
  output = foo(len(instructions)-1, instructions, [])
  print("output", output)
  print(convert_to_num(output))
