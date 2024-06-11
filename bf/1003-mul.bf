,> // Read the first digit
,  // Read the second digit

>++++++[<-------->-]   // Set cell 2 to -48 (convert ASCII digit to integer)
<<[->+>-[>+>>]>[+[-<+>]>+>>]<<<<<<]  // Multiply cell 0 and cell 1
>[-]  // Clear cell 1
<[->+<]  // Move the result to cell 0
>++++++++++.  // Add 48 to convert the integer back to ASCII and print

.  // Print newline character (ASCII 10 is line feed)
