    instr 1: nop
    instr 2: entrypc
    instr 3: enter 64
    instr 4: move 13 n@-56
    instr 5: move 0 x@-64
    instr 6: move 0 a@-8
    instr 7: cmplt a@-8 n@-56
    instr 8: blbc (7) [45]
    instr 9: move 0 b@-16
    instr 10: move 0 x@-64
    instr 11: cmplt b@-16 n@-56
    instr 12: blbc (11) [42]
    instr 13: move 0 c@-24
    instr 14: cmplt c@-24 n@-56
    instr 15: blbc (14) [39]
    instr 16: move 0 d@-32
    instr 17: cmplt d@-32 n@-56
    instr 18: blbc (17) [36]
    instr 19: move 0 e@-40
    instr 20: cmplt e@-40 n@-56
    instr 21: blbc (20) [33]
    instr 22: move 0 f@-48
    instr 23: cmplt f@-48 n@-56
    instr 24: blbc (23) [30]
    instr 25: add x@-64 1
    instr 26: move (25) x@-64
    instr 27: add f@-48 1
    instr 28: move (27) f@-48
    instr 29: br [23]
    instr 30: add e@-40 1
    instr 31: move (30) e@-40
    instr 32: br [20]
    instr 33: add d@-32 1
    instr 34: move (33) d@-32
    instr 35: br [17]
    instr 36: add c@-24 1
    instr 37: move (36) c@-24
    instr 38: br [14]
    instr 39: add b@-16 1
    instr 40: move (39) b@-16
    instr 41: br [11]
    instr 42: add a@-8 1
    instr 43: move (42) a@-8
    instr 44: br [7]
    instr 45: write x@-64
    instr 46: wrl
    instr 47: ret 0
    instr 48: nop
