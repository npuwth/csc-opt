	instr 1: nop  
	instr 17: entrypc  
	instr 51: nop  

	instr 2: enter 8 
	instr 52: phi c$0 c$2 
	instr 55: move (52) c$1
	instr 53: phi a$0 a$2 
	instr 56: move (53) a$1
	instr 54: phi b$0 b$2 
	instr 57: move (54) b$1
	instr 3: cmpeq b$1 0
	instr 4: blbs (3) [14]
	instr 5: move a$1 c$2
	instr 6: move b$1 a$2
	instr 7: mod c$2 b$1
	instr 8: move (7) b$2
	instr 9: write c$2 
	instr 10: write a$2 
	instr 11: write b$2 
	instr 12: wrl  
	instr 13: br [52] 
	instr 14: add res_base#32744 GP
	instr 15: store a$1 (14)
	instr 16: ret 16 

	instr 18: enter 0 
	instr 19: add a_base#32760 GP
	instr 20: store 25733 (19)
	instr 21: add b_base#32752 GP
	instr 22: store 48611 (21)
	instr 23: add a_base#32760 GP
	instr 24: load (23) 
	instr 25: param (24) 
	instr 26: add b_base#32752 GP
	instr 27: load (26) 
	instr 28: param (27) 
	instr 29: call [2] 
	instr 30: add res_base#32744 GP
	instr 31: load (30) 
	instr 32: write (31) 
	instr 33: wrl  
	instr 34: wrl  
	instr 35: add a_base#32760 GP
	instr 36: store 7485671 (35)
	instr 37: add b_base#32752 GP
	instr 38: store 7480189 (37)
	instr 39: add a_base#32760 GP
	instr 40: load (39) 
	instr 41: param (40) 
	instr 42: add b_base#32752 GP
	instr 43: load (42) 
	instr 44: param (43) 
	instr 45: call [2] 
	instr 46: add res_base#32744 GP
	instr 47: load (46) 
	instr 48: write (47) 
	instr 49: wrl  
	instr 50: ret 0 

