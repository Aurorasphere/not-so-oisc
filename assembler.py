import sys

MEMORY_SIZE = 65536

class Assembler:
    def __init__(self):
        self.memory = [0] * MEMORY_SIZE
        self.pc = 0
        self.labels = {}  # Label
        self.pending_labels = []  # Undefined Label
        self.instructions = []

    def parse_address(self, token):
        if token.startswith("0x"):
            return int(token, 16)  # Base16 address
        elif token.isdigit():
            return int(token)  # Base10 address
        return token  # Label

    def add_instruction(self, instruction, *operands):
        """명령어를 메모리에 추가"""
        if instruction == "subleq":
            a, b, next_addr = operands
            self.memory[self.pc] = 0x00 
            self.memory[self.pc + 1] = (a >> 8) & 0xFF
            self.memory[self.pc + 2] = a & 0xFF
            self.memory[self.pc + 3] = (b >> 8) & 0xFF
            self.memory[self.pc + 4] = b & 0xFF

            # 라벨일 경우 나중에 해결
            if isinstance(next_addr, str):
                self.pending_labels.append((self.pc + 5, next_addr))
                self.memory[self.pc + 5] = 0x00 # Set the temp value
                self.memory[self.pc + 6] = 0x00
            else:
                self.memory[self.pc + 5] = (next_addr >> 8) & 0xFF
                self.memory[self.pc + 6] = next_addr & 0xFF

            self.pc += 7

        elif instruction == "set":
            addr, value = operands
            self.memory[self.pc] = 0x01 
            self.memory[self.pc + 1] = (addr >> 8) & 0xFF
            self.memory[self.pc + 2] = addr & 0xFF
            self.memory[self.pc + 3] = value
            self.pc += 4

        elif instruction == "in":
            addr = operands[0]
            self.memory[self.pc] = 0x02
            self.memory[self.pc + 1] = (addr >> 8) & 0xFF
            self.memory[self.pc + 2] = addr & 0xFF
            self.pc += 3

        elif instruction == "out":
            addr = operands[0]
            
            if isinstance(addr, str):
                self.pending_labels.append((self.pc + 1, addr))
                self.memory[self.pc + 1] = 0x00 
                self.memory[self.pc + 2] = 0x00
            else:
                self.memory[self.pc + 1] = (addr >> 8) & 0xFF
                self.memory[self.pc + 2] = addr & 0xFF

            self.memory[self.pc] = 0x03 
            self.pc += 3

        elif instruction == "hlt":
            self.memory[self.pc] = 0x04 
            self.pc += 1

    def parse_instruction(self, line):
        tokens = line.split()
        if len(tokens) == 0 or tokens[0].startswith(';'):
            return

        command = tokens[0]

        if command == "set":
            addr = self.parse_address(tokens[1])
            value = int(tokens[2], 16)
            self.add_instruction("set", addr, value)

        elif command == "subleq":
            a = self.parse_address(tokens[1])
            b = self.parse_address(tokens[2])
            next_addr = self.parse_address(tokens[3])
            self.add_instruction("subleq", a, b, next_addr)

        elif command == "in":
            addr = self.parse_address(tokens[1])
            self.add_instruction("in", addr)

        elif command == "out":
            addr = self.parse_address(tokens[1])
            self.add_instruction("out", addr)

        elif command == "hlt":
            self.add_instruction("hlt")

        elif command.endswith(":"):
            label = command[:-1]
            self.labels[label] = self.pc

    def assemble_file(self, input_file):
        with open(input_file, 'r') as f:
            lines = f.readlines()

        for line in lines:
            self.parse_instruction(line)

        for pc, label in self.pending_labels:
            if label in self.labels:
                address = self.labels[label]
                self.memory[pc] = (address >> 8) & 0xFF
                self.memory[pc + 1] = address & 0xFF
            else:
                raise ValueError(f"Undefined label: {label}")

    def save_to_file(self, output_file):
        with open(output_file, 'wb') as f:
            f.write(bytearray(self.memory[:self.pc]))

def main():
    if len(sys.argv) != 3:
        print("Usage: python assembler.py <input.asm> <output.bin>")
        return

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    assembler = Assembler()
    assembler.assemble_file(input_file)
    assembler.save_to_file(output_file)
    print(f"Assembly complete. Output written to {output_file}")

if __name__ == "__main__":
    main()

