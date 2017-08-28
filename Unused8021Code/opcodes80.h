case 0x03: /* add a,#! */
    add(fetch(),0);break;
case 0x04: /* jmp 0! */
    pctr = fetch();break;
case 0x07: /* dec a */
    a = (a - 1) & 0xFF;break;
case 0x08: /* in a,p0 */
    a = hardwareIO('R',0,0);timerCount--;break;
case 0x09: /* in a,p1 */
    a = hardwareIO('R',1,0);timerCount--;break;
case 0x0a: /* in a,p2 */
    a = hardwareIO('R',2,0);timerCount--;break;
case 0x10: /* inc @r0 */
    temp = r[0] & 0x3F;r[temp] = (r[temp]+1) & 0xFF;break;
case 0x11: /* inc @r1 */
    temp = r[1] & 0x3F;r[temp] = (r[temp]+1) & 0xFF;break;
case 0x13: /* addc a,#! */
    add(fetch(),c);break;
case 0x14: /* call 0! */
    temp = fetch();push(pctr);pctr = temp;break;
case 0x16: /* jtf ! */
    temp = fetch();if (timerOverflow) pctr = (pctr & 0xFF00) | temp; timerOverflow = 0;break;
case 0x17: /* inc a */
    a = (a + 1) & 0xFF;break;
case 0x18: /* inc r0 */
    r[0] = (r[0] + 1) & 0xFF;break;
case 0x19: /* inc r1 */
    r[1] = (r[1] + 1) & 0xFF;break;
case 0x1a: /* inc r2 */
    r[2] = (r[2] + 1) & 0xFF;break;
case 0x1b: /* inc r3 */
    r[3] = (r[3] + 1) & 0xFF;break;
case 0x1c: /* inc r4 */
    r[4] = (r[4] + 1) & 0xFF;break;
case 0x1d: /* inc r5 */
    r[5] = (r[5] + 1) & 0xFF;break;
case 0x1e: /* inc r6 */
    r[6] = (r[6] + 1) & 0xFF;break;
case 0x1f: /* inc r7 */
    r[7] = (r[7] + 1) & 0xFF;break;
case 0x20: /* xch a,@r0 */
    temp = r[r[0] & 0x3F];r[r[0] & 0x3F] = a;a = temp;break;
case 0x21: /* xch a,@r1 */
    temp = r[r[1] & 0x3F];r[r[1] & 0x3F] = a;a = temp;break;
case 0x23: /* mov a,#! */
    a = fetch();break;
case 0x24: /* jmp 1! */
    pctr = fetch()+0x100;break;
case 0x27: /* clr a */
    a = 0;break;
case 0x28: /* xch a,r0 */
    temp = r[0];r[0] = a;a = temp;break;
case 0x29: /* xch a,r1 */
    temp = r[1];r[1] = a;a = temp;break;
case 0x2a: /* xch a,r2 */
    temp = r[2];r[2] = a;a = temp;break;
case 0x2b: /* xch a,r3 */
    temp = r[3];r[3] = a;a = temp;break;
case 0x2c: /* xch a,r4 */
    temp = r[4];r[4] = a;a = temp;break;
case 0x2d: /* xch a,r5 */
    temp = r[5];r[5] = a;a = temp;break;
case 0x2e: /* xch a,r6 */
    temp = r[6];r[6] = a;a = temp;break;
case 0x2f: /* xch a,r7 */
    temp = r[7];r[7] = a;a = temp;break;
case 0x30: /* xchd a,@r0 */
    exchangeNibble(r[0] & 0x3F);break;
case 0x31: /* xchd a,@r1 */
    exchangeNibble(r[1] & 0x3F);break;
case 0x34: /* call 1! */
    temp = fetch();push(pctr);pctr = temp + 0x100;break;
case 0x37: /* cpl a */
    a = a ^ 0xFF;break;
case 0x39: /* outl p1,a */
    hardwareIO('W',1,a);timerCount--;break;
case 0x3a: /* outl p2,a */
    hardwareIO('W',2,a);timerCount--;break;
case 0x40: /* orl a,@r0 */
    a = a | r[r[0] & 0x3F];break;
case 0x41: /* orl a,@r1 */
    a = a | r[r[1] & 0x3F];break;
case 0x42: /* mov a,t */
    a = timerRegister;break;
case 0x43: /* orl a,#! */
    a = a | fetch();break;
case 0x44: /* jmp 2! */
    pctr = fetch()+0x200;break;
case 0x47: /* swap a */
    a = ((a >> 4) | (a << 4)) & 0xFF;break;
case 0x48: /* orl a,r0 */
    a = a | r[0];break;
case 0x49: /* orl a,r1 */
    a = a | r[1];break;
case 0x4a: /* orl a,r2 */
    a = a | r[2];break;
case 0x4b: /* orl a,r3 */
    a = a | r[3];break;
case 0x4c: /* orl a,r4 */
    a = a | r[4];break;
case 0x4d: /* orl a,r5 */
    a = a | r[5];break;
case 0x4e: /* orl a,r6 */
    a = a | r[6];break;
case 0x4f: /* orl a,r7 */
    a = a | r[7];break;
case 0x50: /* anl a,@r0 */
    a = a & r[r[0] & 0x3F];break;
case 0x51: /* anl a,@r1 */
    a = a & r[r[1] & 0x3F];break;
case 0x53: /* anl a,#! */
    a = a & fetch();break;
case 0x54: /* call 2! */
    temp = fetch();push(pctr);pctr = temp + 0x200;break;
case 0x55: /* strt t */
    timerEnabled = 1;break;
case 0x57: /* da a */
    decimalAdjust();break;
case 0x58: /* anl a,r0 */
    a = a & r[0];break;
case 0x59: /* anl a,r1 */
    a = a & r[1];break;
case 0x5a: /* anl a,r2 */
    a = a & r[2];break;
case 0x5b: /* anl a,r3 */
    a = a & r[3];break;
case 0x5c: /* anl a,r4 */
    a = a & r[4];break;
case 0x5d: /* anl a,r5 */
    a = a & r[5];break;
case 0x5e: /* anl a,r6 */
    a = a & r[6];break;
case 0x5f: /* anl a,r7 */
    a = a & r[7];break;
case 0x62: /* mov t,a */
    timerRegister = a;break;
case 0x64: /* jmp 3! */
    pctr = fetch()+0x300;break;
case 0x65: /* stop tcnt */
    timerEnabled = 0;break;
case 0x67: /* rrc a */
    rotateRightThroughc();break;
case 0x70: /* addc a,@r0 */
    add(r[r[0] & 0x3F],c);break;
case 0x71: /* addc a,@r1 */
    add(r[r[1] & 0x3F],c);break;
case 0x74: /* call 3! */
    temp = fetch();push(pctr);pctr = temp + 0x300;break;
case 0x77: /* rr a */
    rotateRight();break;
case 0x78: /* addc a,r0 */
    add(r[0],c);break;
case 0x79: /* addc a,r1 */
    add(r[1],c);break;
case 0x7a: /* addc a,r2 */
    add(r[2],c);break;
case 0x7b: /* addc a,r3 */
    add(r[3],c);break;
case 0x7c: /* addc a,r4 */
    add(r[4],c);break;
case 0x7d: /* addc a,r5 */
    add(r[5],c);break;
case 0x7e: /* addc a,r6 */
    add(r[6],c);break;
case 0x7f: /* addc a,r7 */
    add(r[7],c);break;
case 0x80: /* add a,@r0 */
    add(r[r[0] & 0x3F],0);break;
case 0x81: /* add a,@r1 */
    add(r[r[1] & 0x3F],0);break;
case 0x83: /* ret */
    pctr = pull();timerCount--;break;
case 0x84: /* jmp 4! */
    pctr = fetch()+0x400;break;
case 0x88: /* add a,r0 */
    add(r[0],0);break;
case 0x89: /* add a,r1 */
    add(r[1],0);break;
case 0x8a: /* add a,r2 */
    add(r[2],0);break;
case 0x8b: /* add a,r3 */
    add(r[3],0);break;
case 0x8c: /* add a,r4 */
    add(r[4],0);break;
case 0x8d: /* add a,r5 */
    add(r[5],0);break;
case 0x8e: /* add a,r6 */
    add(r[6],0);break;
case 0x8f: /* add a,r7 */
    add(r[7],0);break;
case 0x90: /* outl p0,a */
    hardwareIO('W',0,a);timerCount--;break;
case 0x94: /* call 4! */
    temp = fetch();push(pctr);pctr = temp + 0x400;break;
case 0x96: /* jnz ! */
    temp = fetch();if (a != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0x97: /* clr c */
    c = 0x0;break;
case 0xa0: /* mov @r0,a */
    r[r[0] & 0x3F] = a;break;
case 0xa1: /* mov @r1,a */
    r[r[1] & 0x3F] = a;break;
case 0xa3: /* movp a,@a */
    a=codeMemory[(pctr & 0xFF00)|a];timerCount--;break;
case 0xa4: /* jmp 5! */
    pctr = fetch()+0x500;break;
case 0xa7: /* cpl c */
    c = (c == 0) ? 1 : 0;break;
case 0xa8: /* mov r0,a */
    r[0] = a;break;
case 0xa9: /* mov r1,a */
    r[1] = a;break;
case 0xaa: /* mov r2,a */
    r[2] = a;break;
case 0xab: /* mov r3,a */
    r[3] = a;break;
case 0xac: /* mov r4,a */
    r[4] = a;break;
case 0xad: /* mov r5,a */
    r[5] = a;break;
case 0xae: /* mov r6,a */
    r[6] = a;break;
case 0xaf: /* mov r7,a */
    r[7] = a;break;
case 0xb0: /* mov @r0,#! */
    r[r[0] & 0x3F] = fetch();break;
case 0xb1: /* mov @r1,#! */
    r[r[1] & 0x3F] = fetch();break;
case 0xb3: /* jmpp @a */
    pctr &= 0xFF00; pctr |= codeMemory[pctr+a];break;
case 0xb4: /* call 5! */
    temp = fetch();push(pctr);pctr = temp + 0x500;break;
case 0xb8: /* mov r0,#! */
    r[0] = fetch();break;
case 0xb9: /* mov r1,#! */
    r[1] = fetch();break;
case 0xba: /* mov r2,#! */
    r[2] = fetch();break;
case 0xbb: /* mov r3,#! */
    r[3] = fetch();break;
case 0xbc: /* mov r4,#! */
    r[4] = fetch();break;
case 0xbd: /* mov r5,#! */
    r[5] = fetch();break;
case 0xbe: /* mov r6,#! */
    r[6] = fetch();break;
case 0xbf: /* mov r7,#! */
    r[7] = fetch();break;
case 0xc4: /* jmp 6! */
    pctr = fetch()+0x600;break;
case 0xc6: /* jz ! */
    temp = fetch();if (a == 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xd0: /* xrl a,@r0 */
    a = a ^ r[r[0] & 0x3F];break;
case 0xd1: /* xrl a,@r1 */
    a = a ^ r[r[1] & 0x3F];break;
case 0xd3: /* xrl a,#! */
    a = a ^ fetch();break;
case 0xd4: /* call 6! */
    temp = fetch();push(pctr);pctr = temp + 0x600;break;
case 0xd8: /* xrl a,r0 */
    a = a ^ r[0];break;
case 0xd9: /* xrl a,r1 */
    a = a ^ r[1];break;
case 0xda: /* xrl a,r2 */
    a = a ^ r[2];break;
case 0xdb: /* xrl a,r3 */
    a = a ^ r[3];break;
case 0xdc: /* xrl a,r4 */
    a = a ^ r[4];break;
case 0xdd: /* xrl a,r5 */
    a = a ^ r[5];break;
case 0xde: /* xrl a,r6 */
    a = a ^ r[6];break;
case 0xdf: /* xrl a,r7 */
    a = a ^ r[7];break;
case 0xe4: /* jmp 7! */
    pctr = fetch()+0x700;break;
case 0xe6: /* jnc ! */
    temp = fetch();if (!c) pctr = (pctr & 0xFF00) | temp;break;
case 0xe7: /* rl a */
    rotateLeft();break;
case 0xe8: /* djnz r0,! */
    temp = fetch();r[0] = (r[0]-1) & 0xFF;if (r[0] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xe9: /* djnz r1,! */
    temp = fetch();r[1] = (r[1]-1) & 0xFF;if (r[1] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xea: /* djnz r2,! */
    temp = fetch();r[2] = (r[2]-1) & 0xFF;if (r[2] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xeb: /* djnz r3,! */
    temp = fetch();r[3] = (r[3]-1) & 0xFF;if (r[3] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xec: /* djnz r4,! */
    temp = fetch();r[4] = (r[4]-1) & 0xFF;if (r[4] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xed: /* djnz r5,! */
    temp = fetch();r[5] = (r[5]-1) & 0xFF;if (r[5] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xee: /* djnz r6,! */
    temp = fetch();r[6] = (r[6]-1) & 0xFF;if (r[6] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xef: /* djnz r7,! */
    temp = fetch();r[7] = (r[7]-1) & 0xFF;if (r[7] != 0) pctr = (pctr & 0xFF00) | temp;break;
case 0xf0: /* mov a,@r0 */
    a = r[r[0] & 0x3F];break;
case 0xf1: /* mov a,@r1 */
    a = r[r[1] & 0x3F];break;
case 0xf4: /* call 7! */
    temp = fetch();push(pctr);pctr = temp + 0x700;break;
case 0xf6: /* jc ! */
    temp = fetch();if (c) pctr = (pctr & 0xFF00) | temp;break;
case 0xf7: /* rlc a */
    rotateLeftThroughc();break;
case 0xf8: /* mov a,r0 */
    a = r[0];break;
case 0xf9: /* mov a,r1 */
    a = r[1];break;
case 0xfa: /* mov a,r2 */
    a = r[2];break;
case 0xfb: /* mov a,r3 */
    a = r[3];break;
case 0xfc: /* mov a,r4 */
    a = r[4];break;
case 0xfd: /* mov a,r5 */
    a = r[5];break;
case 0xfe: /* mov a,r6 */
    a = r[6];break;
case 0xff: /* mov a,r7 */
    a = r[7];break;
