# (Not so) OISC

The **(Not so) OISC**, or **NOISC**, is a computer that operates primarily using the subleq instruction. However, unlike a typical OISC, this computer has five instructions. Therefore, it could also be called a FISC (Five Instruction Set Computer).

## Specifications

- Signed 8-bit data and 16-bit address width
- 5 instructions (subleq, set, in, out, hlt)

Each instruction has a variable length, making this computer similar to a CISC architecture. But the only instruction that performs actual computation is subleq, while the other instructions are used to write/read values to/from specific memory addresses or perform operations that would be difficult with just subleq.

### Instructions of the NOISC Computer
1. `subleq a, b, c`
<br>perform `b = b - a`, and if `b` <= 0, branch to `c`. `a` and `b`, `c` are addressees. `c` can either be a label or a 16-bit address, but it is recommended to use labels. 7 Bytes long.

2. `set addr, value`
<br>Sets the value at memory address `addr` to `value`. 4 bytes long.

3. `in addr`
<br>Reads input from the user and stores it at memory address `addr`. 3 bytes long.

4. `out addr`
<br>Prints the value stored at memory address `addr`. 3 bytes long.

5. `hlt`
<br>Halts the program. 1 byte long.

## How to use
This code has only been tested in a Linux environment with GCC. However, since the codes are simple, it is likely to work on other compilers or operating systems as well.
### On Linux

If you are using Debian/Ubuntu,
```
sudo apt update && sudo apt upgrade
sudo apt install python3 gcc git
git clone https://github.com/Aurorasphere/not-so-oisc/
cd not-so-oisc
make
```
If everything is done correctly, the computer executable will be located in the bin directory.

#### Assembler
1. Write some assembly code and save.
2. type `python src/assembler.py [YOUR CODE'S NAME].s [OUT FILE'S NAME].out` in your terminal.
#### Computer
1. type `src/oiscomputer [OUT FILE'S NAME].out` in your terminal.
 
## Example Program

Here is an example program that compares two numbers and prints the result:

```asm
// Compare A and B, 
// If A > B, Print 0.
// If A <= B, Print 1.

in      0x00F0                  // Read A, Address of A is 0x00F0
in      0x00F1                  // Read B, Address of B is 0x00F1
set     0x00FF 0x00             // Set Bool = 0, Address of Bool value is 0x00FF
subleq  0x00F1 0x00F0 RETURN    // If A - B <=0, go to RETURN 
out     0x00FF                  // If not, Print 0 
hlt                             // Stop the Program

RETURN:                         
    set 0x00FF 0x01             // Set Bool = 1
    out 0x00FF                  // Print 1
    hlt                         // Stop the program
```
