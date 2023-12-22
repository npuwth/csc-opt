    instr 1: nop
    instr 2: entrypc
    instr 3: enter 48
    instr 4: move 123 a@-8
    instr 5: read
    instr 6: move (5) b@-16
    instr 7: add a@-8 b@-16
    instr 8: move (7) c@-24
    instr 9: add c@-24 1
    instr 10: move (9) d@-32
    instr 11: mul d@-32 2
    instr 12: move (11) e@-40
    instr 13: sub e@-40 d@-32
    instr 14: move (13) f@-48
    instr 15: ret 0
    instr 16: nop
