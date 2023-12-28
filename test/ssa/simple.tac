    instr 1: nop
    instr 2: entrypc
    instr 11: nop

    instr 3: enter 64
    instr 4: move 0 i@-8
	instr 5: cmplt i@-8 10
	instr 6: blbc (5) [10]
	instr 7: add i@-8 1
	instr 8: move (7) i@-8
	instr 9: br [5]
	instr 10: ret 0
    
