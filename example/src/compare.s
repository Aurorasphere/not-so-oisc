// Compare A and B, 
// If A > B, Print 0.
// If A <= B, Print 1.

in      0x00F0                  // Read A
in      0x00F1                  // Read B
set     0x00FF 0x00             // Set Bool = 0
subleq  0x00F1 0x00F0 RETURN    // If A - B <=0, go to RETURN 
out     0x00FF                  // If not, Print 0 
hlt                             // Stop the Program

RETURN:                         
    set 0x00FF 0x01             // Set Bool = 1
    out 0x00FF                  // Print 1
    hlt                         // Stop the program
