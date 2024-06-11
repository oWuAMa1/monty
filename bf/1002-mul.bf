,> ,<       // Read two digits and store them in cell 0 and cell 1
[->+>+<<]   // Move the value of cell 0 to cell 2 and cell 3, and clear cell 0
>>[-<<+>>]  // Move the value of cell 2 back to cell 0 and clear cell 2
<           // Move to cell 1
[           // Loop cell 1 times
  ->        // Decrement cell 1
  <[->+<]   // Add cell 0 to cell 2
  >>        // Move to cell 3
  -         // Decrement cell 3
  <[->+<]   // Add cell 0 to cell 2
  <         // Move to cell 1
]           // End of loop
>[-<+>]     // Move the value of cell 2 to cell 1 and clear cell 2
[-]         // Clear cell 1
<           // Move to cell 0
.           // Print the result from cell 0
