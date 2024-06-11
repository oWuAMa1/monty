,>              // Read first digit and move to next cell
,               // Read second digit
<               // Move back to the first cell
[               // While cell 0 is not 0
  ->+>+<<       // Move its value to cells 1 and 2
]
>>              // Move to the third cell
[-<+>]          // Transfer value from cell 2 to cell 1
<               // Move to cell 1
[               // While cell 1 is not 0
  ->[<+>-]      // Transfer its value to cell 0 one by one and move the result to cell 2
  <             // Move back to cell 0
  [->+<]        // Add cell 0 value to cell 2
  >>            // Move to cell 2
  -             // Decrement cell 2
  <[->+<]       // Add cell 0 value to cell 2 again
  <             // Move back to cell 1
]               // End loop
>[-<+>]         // Transfer value from cell 2 to cell 1
[-]             // Clear cell 1
<               // Move back to cell 0
.               // Print result
