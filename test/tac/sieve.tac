    instr 1: nop
    instr 2: entrypc
    instr 3: enter 8016
    instr 4: mul 0 8
    instr 5: add is_prime_base#-8016 FP
    instr 6: add (5) (4)
    instr 7: store 0 (6)
    instr 8: mul 1 8
    instr 9: add is_prime_base#-8016 FP
    instr 10: add (9) (8)
    instr 11: store 0 (10)
    instr 12: move 2 i@-8
    instr 13: cmplt i@-8 1000
    instr 14: blbc (13) [22]
    instr 15: mul i@-8 8
    instr 16: add is_prime_base#-8016 FP
    instr 17: add (16) (15)
    instr 18: store 1 (17)
    instr 19: add i@-8 1
    instr 20: move (19) i@-8
    instr 21: br [13]
    instr 22: move 2 i@-8
    instr 23: cmplt i@-8 1000
    instr 24: blbc (23) [46]
    instr 25: mul i@-8 8
    instr 26: add is_prime_base#-8016 FP
    instr 27: add (26) (25)
    instr 28: load (27)
    instr 29: cmpeq (28) 0
    instr 30: blbs (29) [43]
    instr 31: move 2 j@-16
    instr 32: mul i@-8 j@-16
    instr 33: cmplt (32) 1000
    instr 34: blbc (33) [43]
    instr 35: mul i@-8 j@-16
    instr 36: mul (35) 8
    instr 37: add is_prime_base#-8016 FP
    instr 38: add (37) (36)
    instr 39: store 0 (38)
    instr 40: add j@-16 1
    instr 41: move (40) j@-16
    instr 42: br [32]
    instr 43: add i@-8 1
    instr 44: move (43) i@-8
    instr 45: br [23]
    instr 46: move 2 i@-8
    instr 47: cmplt i@-8 1000
    instr 48: blbc (47) [59]
    instr 49: mul i@-8 8
    instr 50: add is_prime_base#-8016 FP
    instr 51: add (50) (49)
    instr 52: load (51)
    instr 53: cmpeq (52) 0
    instr 54: blbs (53) [56]
    instr 55: write i@-8
    instr 56: add i@-8 1
    instr 57: move (56) i@-8
    instr 58: br [47]
    instr 59: wrl
    instr 60: ret 0
    instr 61: nop
