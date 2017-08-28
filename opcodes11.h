case 0x00: /* mnea */
    status = (dataMemory[(x << 4)|y] != a) ? 1 : 0;break;
case 0x01: /* alem */
    status = (a <= dataMemory[(x << 4)|y]) ? 1 : 0;break;
case 0x02: /* ynea */
    status = sl = ((y != a) ? 1 : 0);break;
case 0x03: /* xma */
    _temp = dataMemory[(x << 4)|y];dataMemory[(x << 4)|y] = a;a = _temp;break;
case 0x04: /* dyn */
    status = (y >= 1) ? 1 : 0; y = (y - 1) & 0xF;break;
case 0x05: /* iyc */
    status = (y == 15) ? 1 : 0;y = (y + 1) & 0xF;break;
case 0x06: /* amaac */
    a = a + dataMemory[(x << 4)|y];status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x07: /* dman */
    status = (dataMemory[(x << 4)|y] >= 1) ? 1 : 0;a = (dataMemory[(x << 4)|y] - 1) & 0xF;break;
case 0x08: /* tka */
    a = hardwareIO(READ_KEYS,0,0);break;
case 0x09: /* comx */
    x = x ^ 4;break;
case 0x0a: /* tdo */
    o = ((sl != 0) ? 0x10: 0) | a;hardwareIO(WRITE_O,o,0);break;
case 0x0b: /* comc */
    cb = cb ^ 1;break;
case 0x0c: /* rstr */
    if (x <= 3 && y <= 10) { r[y] = 0; hardwareIO(WRITE_R,y,0); };break;
case 0x0d: /* setr */
    if (x <= 3 && y <= 10) { r[y] = 1; hardwareIO(WRITE_R,y,1); };break;
case 0x0e: /* knez */
    status = (hardwareIO(READ_KEYS,0,0) != 0) ? 1 : 0;break;
case 0x0f: /* retn */
    retnCommand();break;
case 0x10: /* ldp 0 */
    pb = 0x0;break;
case 0x11: /* ldp 8 */
    pb = 0x8;break;
case 0x12: /* ldp 4 */
    pb = 0x4;break;
case 0x13: /* ldp c */
    pb = 0xc;break;
case 0x14: /* ldp 2 */
    pb = 0x2;break;
case 0x15: /* ldp a */
    pb = 0xa;break;
case 0x16: /* ldp 6 */
    pb = 0x6;break;
case 0x17: /* ldp e */
    pb = 0xe;break;
case 0x18: /* ldp 1 */
    pb = 0x1;break;
case 0x19: /* ldp 9 */
    pb = 0x9;break;
case 0x1a: /* ldp 5 */
    pb = 0x5;break;
case 0x1b: /* ldp d */
    pb = 0xd;break;
case 0x1c: /* ldp 3 */
    pb = 0x3;break;
case 0x1d: /* ldp b */
    pb = 0xb;break;
case 0x1e: /* ldp 7 */
    pb = 0x7;break;
case 0x1f: /* ldp f */
    pb = 0xf;break;
case 0x20: /* tay */
    y = a;break;
case 0x21: /* tma */
    a = dataMemory[(x << 4)|y];break;
case 0x22: /* tmy */
    y = dataMemory[(x << 4)|y];break;
case 0x23: /* tya */
    a = y;break;
case 0x24: /* tamdyn */
    dataMemory[(x << 4)|y] = a;status = (y >= 1) ? 1 : 0;y = (y - 1) & 0xF;break;
case 0x25: /* tamiyc */
    dataMemory[(x << 4)|y] = a;status = (y == 15) ? 1 : 0; y = (y + 1) & 0xF;break;
case 0x26: /* tamza */
    dataMemory[(x << 4)|y] = a;a = 0;break;
case 0x27: /* tam */
    dataMemory[(x << 4)|y] = a;break;
case 0x28: /* ldx 0 */
    x = 0;break;
case 0x29: /* ldx 4 */
    x = 4;break;
case 0x2a: /* ldx 2 */
    x = 2;break;
case 0x2b: /* ldx 6 */
    x = 6;break;
case 0x2c: /* ldx 1 */
    x = 1;break;
case 0x2d: /* ldx 5 */
    x = 5;break;
case 0x2e: /* ldx 3 */
    x = 3;break;
case 0x2f: /* ldx 7 */
    x = 7;break;
case 0x30: /* sbit 0 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] | (1 << 0x0);break;
case 0x31: /* sbit 2 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] | (1 << 0x2);break;
case 0x32: /* sbit 1 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] | (1 << 0x1);break;
case 0x33: /* sbit 3 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] | (1 << 0x3);break;
case 0x34: /* rbit 0 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] & ((1 << 0x0) ^ 0xF);break;
case 0x35: /* rbit 2 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] & ((1 << 0x2) ^ 0xF);break;
case 0x36: /* rbit 1 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] & ((1 << 0x1) ^ 0xF);break;
case 0x37: /* rbit 3 */
    dataMemory[(x << 4)|y] = dataMemory[(x << 4)|y] & ((1 << 0x3) ^ 0xF);break;
case 0x38: /* tbit1 0 */
    status = ((dataMemory[(x << 4)|y] & (1 << 0x0)) != 0) ? 1 : 0;break;
case 0x39: /* tbit1 2 */
    status = ((dataMemory[(x << 4)|y] & (1 << 0x2)) != 0) ? 1 : 0;break;
case 0x3a: /* tbit1 1 */
    status = ((dataMemory[(x << 4)|y] & (1 << 0x1)) != 0) ? 1 : 0;break;
case 0x3b: /* tbit1 3 */
    status = ((dataMemory[(x << 4)|y] & (1 << 0x3)) != 0) ? 1 : 0;break;
case 0x3c: /* saman */
    status = (a <= dataMemory[(x << 4)|y]) ? 1 : 0;a = (a ^ 0xF) + 1 + dataMemory[(x << 4)|y];a = a & 0xF;break;
case 0x3d: /* cpaiz */
    status = (a == 0) ? 1 : 0;a = ((a ^ 0xF) + 1) & 0xF;break;
case 0x3e: /* imac */
    status = (dataMemory[(x << 4)|y] == 15) ? 1 : 0;a = (dataMemory[(x << 4)|y]+1) & 0xF;break;
case 0x3f: /* mnez */
    status = (dataMemory[(x << 4)|y] != 0) ? 1 : 0;break;
case 0x40: /* tcy 0 */
    y = 0x0;break;
case 0x41: /* tcy 8 */
    y = 0x8;break;
case 0x42: /* tcy 4 */
    y = 0x4;break;
case 0x43: /* tcy c */
    y = 0xc;break;
case 0x44: /* tcy 2 */
    y = 0x2;break;
case 0x45: /* tcy a */
    y = 0xa;break;
case 0x46: /* tcy 6 */
    y = 0x6;break;
case 0x47: /* tcy e */
    y = 0xe;break;
case 0x48: /* tcy 1 */
    y = 0x1;break;
case 0x49: /* tcy 9 */
    y = 0x9;break;
case 0x4a: /* tcy 5 */
    y = 0x5;break;
case 0x4b: /* tcy d */
    y = 0xd;break;
case 0x4c: /* tcy 3 */
    y = 0x3;break;
case 0x4d: /* tcy b */
    y = 0xb;break;
case 0x4e: /* tcy 7 */
    y = 0x7;break;
case 0x4f: /* tcy f */
    y = 0xf;break;
case 0x50: /* ynec 0 */
    status = (y != 0x0) ? 1 : 0;break;
case 0x51: /* ynec 8 */
    status = (y != 0x8) ? 1 : 0;break;
case 0x52: /* ynec 4 */
    status = (y != 0x4) ? 1 : 0;break;
case 0x53: /* ynec c */
    status = (y != 0xc) ? 1 : 0;break;
case 0x54: /* ynec 2 */
    status = (y != 0x2) ? 1 : 0;break;
case 0x55: /* ynec a */
    status = (y != 0xa) ? 1 : 0;break;
case 0x56: /* ynec 6 */
    status = (y != 0x6) ? 1 : 0;break;
case 0x57: /* ynec e */
    status = (y != 0xe) ? 1 : 0;break;
case 0x58: /* ynec 1 */
    status = (y != 0x1) ? 1 : 0;break;
case 0x59: /* ynec 9 */
    status = (y != 0x9) ? 1 : 0;break;
case 0x5a: /* ynec 5 */
    status = (y != 0x5) ? 1 : 0;break;
case 0x5b: /* ynec d */
    status = (y != 0xd) ? 1 : 0;break;
case 0x5c: /* ynec 3 */
    status = (y != 0x3) ? 1 : 0;break;
case 0x5d: /* ynec b */
    status = (y != 0xb) ? 1 : 0;break;
case 0x5e: /* ynec 7 */
    status = (y != 0x7) ? 1 : 0;break;
case 0x5f: /* ynec f */
    status = (y != 0xf) ? 1 : 0;break;
case 0x60: /* tcmiy 0 */
    dataMemory[(x << 4)|y] = 0x0;y = (y + 1) & 0xF;break;
case 0x61: /* tcmiy 8 */
    dataMemory[(x << 4)|y] = 0x8;y = (y + 1) & 0xF;break;
case 0x62: /* tcmiy 4 */
    dataMemory[(x << 4)|y] = 0x4;y = (y + 1) & 0xF;break;
case 0x63: /* tcmiy c */
    dataMemory[(x << 4)|y] = 0xc;y = (y + 1) & 0xF;break;
case 0x64: /* tcmiy 2 */
    dataMemory[(x << 4)|y] = 0x2;y = (y + 1) & 0xF;break;
case 0x65: /* tcmiy a */
    dataMemory[(x << 4)|y] = 0xa;y = (y + 1) & 0xF;break;
case 0x66: /* tcmiy 6 */
    dataMemory[(x << 4)|y] = 0x6;y = (y + 1) & 0xF;break;
case 0x67: /* tcmiy e */
    dataMemory[(x << 4)|y] = 0xe;y = (y + 1) & 0xF;break;
case 0x68: /* tcmiy 1 */
    dataMemory[(x << 4)|y] = 0x1;y = (y + 1) & 0xF;break;
case 0x69: /* tcmiy 9 */
    dataMemory[(x << 4)|y] = 0x9;y = (y + 1) & 0xF;break;
case 0x6a: /* tcmiy 5 */
    dataMemory[(x << 4)|y] = 0x5;y = (y + 1) & 0xF;break;
case 0x6b: /* tcmiy d */
    dataMemory[(x << 4)|y] = 0xd;y = (y + 1) & 0xF;break;
case 0x6c: /* tcmiy 3 */
    dataMemory[(x << 4)|y] = 0x3;y = (y + 1) & 0xF;break;
case 0x6d: /* tcmiy b */
    dataMemory[(x << 4)|y] = 0xb;y = (y + 1) & 0xF;break;
case 0x6e: /* tcmiy 7 */
    dataMemory[(x << 4)|y] = 0x7;y = (y + 1) & 0xF;break;
case 0x6f: /* tcmiy f */
    dataMemory[(x << 4)|y] = 0xf;y = (y + 1) & 0xF;break;
case 0x70: /* iac */
    a = a + 1;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x71: /* a9aac */
    a = a + 9;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x72: /* a5aac */
    a = a + 5;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x73: /* a13aac */
    a = a + 13;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x74: /* a3aac */
    a = a + 3;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x75: /* a11aac */
    a = a + 11;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x76: /* a7aac */
    a = a + 7;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x77: /* dan */
    a = a + 15;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x78: /* a2aac */
    a = a + 2;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x79: /* a10aac */
    a = a + 10;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7a: /* a6aac */
    a = a + 6;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7b: /* a14aac */
    a = a + 14;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7c: /* a4aac */
    a = a + 4;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7d: /* a12aac */
    a = a + 12;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7e: /* a8aac */
    a = a + 8;status = (a > 15) ? 1 : 0;a = a & 0xF;break;
case 0x7f: /* cla */
    a = 0;break;
case 0x80: /* br 0 */
    if (lastStatus) brCommand(0x0);break;
case 0x81: /* br 1 */
    if (lastStatus) brCommand(0x1);break;
case 0x82: /* br 2 */
    if (lastStatus) brCommand(0x2);break;
case 0x83: /* br 3 */
    if (lastStatus) brCommand(0x3);break;
case 0x84: /* br 4 */
    if (lastStatus) brCommand(0x4);break;
case 0x85: /* br 5 */
    if (lastStatus) brCommand(0x5);break;
case 0x86: /* br 6 */
    if (lastStatus) brCommand(0x6);break;
case 0x87: /* br 7 */
    if (lastStatus) brCommand(0x7);break;
case 0x88: /* br 8 */
    if (lastStatus) brCommand(0x8);break;
case 0x89: /* br 9 */
    if (lastStatus) brCommand(0x9);break;
case 0x8a: /* br a */
    if (lastStatus) brCommand(0xa);break;
case 0x8b: /* br b */
    if (lastStatus) brCommand(0xb);break;
case 0x8c: /* br c */
    if (lastStatus) brCommand(0xc);break;
case 0x8d: /* br d */
    if (lastStatus) brCommand(0xd);break;
case 0x8e: /* br e */
    if (lastStatus) brCommand(0xe);break;
case 0x8f: /* br f */
    if (lastStatus) brCommand(0xf);break;
case 0x90: /* br 10 */
    if (lastStatus) brCommand(0x10);break;
case 0x91: /* br 11 */
    if (lastStatus) brCommand(0x11);break;
case 0x92: /* br 12 */
    if (lastStatus) brCommand(0x12);break;
case 0x93: /* br 13 */
    if (lastStatus) brCommand(0x13);break;
case 0x94: /* br 14 */
    if (lastStatus) brCommand(0x14);break;
case 0x95: /* br 15 */
    if (lastStatus) brCommand(0x15);break;
case 0x96: /* br 16 */
    if (lastStatus) brCommand(0x16);break;
case 0x97: /* br 17 */
    if (lastStatus) brCommand(0x17);break;
case 0x98: /* br 18 */
    if (lastStatus) brCommand(0x18);break;
case 0x99: /* br 19 */
    if (lastStatus) brCommand(0x19);break;
case 0x9a: /* br 1a */
    if (lastStatus) brCommand(0x1a);break;
case 0x9b: /* br 1b */
    if (lastStatus) brCommand(0x1b);break;
case 0x9c: /* br 1c */
    if (lastStatus) brCommand(0x1c);break;
case 0x9d: /* br 1d */
    if (lastStatus) brCommand(0x1d);break;
case 0x9e: /* br 1e */
    if (lastStatus) brCommand(0x1e);break;
case 0x9f: /* br 1f */
    if (lastStatus) brCommand(0x1f);break;
case 0xa0: /* br 20 */
    if (lastStatus) brCommand(0x20);break;
case 0xa1: /* br 21 */
    if (lastStatus) brCommand(0x21);break;
case 0xa2: /* br 22 */
    if (lastStatus) brCommand(0x22);break;
case 0xa3: /* br 23 */
    if (lastStatus) brCommand(0x23);break;
case 0xa4: /* br 24 */
    if (lastStatus) brCommand(0x24);break;
case 0xa5: /* br 25 */
    if (lastStatus) brCommand(0x25);break;
case 0xa6: /* br 26 */
    if (lastStatus) brCommand(0x26);break;
case 0xa7: /* br 27 */
    if (lastStatus) brCommand(0x27);break;
case 0xa8: /* br 28 */
    if (lastStatus) brCommand(0x28);break;
case 0xa9: /* br 29 */
    if (lastStatus) brCommand(0x29);break;
case 0xaa: /* br 2a */
    if (lastStatus) brCommand(0x2a);break;
case 0xab: /* br 2b */
    if (lastStatus) brCommand(0x2b);break;
case 0xac: /* br 2c */
    if (lastStatus) brCommand(0x2c);break;
case 0xad: /* br 2d */
    if (lastStatus) brCommand(0x2d);break;
case 0xae: /* br 2e */
    if (lastStatus) brCommand(0x2e);break;
case 0xaf: /* br 2f */
    if (lastStatus) brCommand(0x2f);break;
case 0xb0: /* br 30 */
    if (lastStatus) brCommand(0x30);break;
case 0xb1: /* br 31 */
    if (lastStatus) brCommand(0x31);break;
case 0xb2: /* br 32 */
    if (lastStatus) brCommand(0x32);break;
case 0xb3: /* br 33 */
    if (lastStatus) brCommand(0x33);break;
case 0xb4: /* br 34 */
    if (lastStatus) brCommand(0x34);break;
case 0xb5: /* br 35 */
    if (lastStatus) brCommand(0x35);break;
case 0xb6: /* br 36 */
    if (lastStatus) brCommand(0x36);break;
case 0xb7: /* br 37 */
    if (lastStatus) brCommand(0x37);break;
case 0xb8: /* br 38 */
    if (lastStatus) brCommand(0x38);break;
case 0xb9: /* br 39 */
    if (lastStatus) brCommand(0x39);break;
case 0xba: /* br 3a */
    if (lastStatus) brCommand(0x3a);break;
case 0xbb: /* br 3b */
    if (lastStatus) brCommand(0x3b);break;
case 0xbc: /* br 3c */
    if (lastStatus) brCommand(0x3c);break;
case 0xbd: /* br 3d */
    if (lastStatus) brCommand(0x3d);break;
case 0xbe: /* br 3e */
    if (lastStatus) brCommand(0x3e);break;
case 0xbf: /* br 3f */
    if (lastStatus) brCommand(0x3f);break;
case 0xc0: /* call 0 */
    if (lastStatus) callCommand(0x0);break;
case 0xc1: /* call 1 */
    if (lastStatus) callCommand(0x1);break;
case 0xc2: /* call 2 */
    if (lastStatus) callCommand(0x2);break;
case 0xc3: /* call 3 */
    if (lastStatus) callCommand(0x3);break;
case 0xc4: /* call 4 */
    if (lastStatus) callCommand(0x4);break;
case 0xc5: /* call 5 */
    if (lastStatus) callCommand(0x5);break;
case 0xc6: /* call 6 */
    if (lastStatus) callCommand(0x6);break;
case 0xc7: /* call 7 */
    if (lastStatus) callCommand(0x7);break;
case 0xc8: /* call 8 */
    if (lastStatus) callCommand(0x8);break;
case 0xc9: /* call 9 */
    if (lastStatus) callCommand(0x9);break;
case 0xca: /* call a */
    if (lastStatus) callCommand(0xa);break;
case 0xcb: /* call b */
    if (lastStatus) callCommand(0xb);break;
case 0xcc: /* call c */
    if (lastStatus) callCommand(0xc);break;
case 0xcd: /* call d */
    if (lastStatus) callCommand(0xd);break;
case 0xce: /* call e */
    if (lastStatus) callCommand(0xe);break;
case 0xcf: /* call f */
    if (lastStatus) callCommand(0xf);break;
case 0xd0: /* call 10 */
    if (lastStatus) callCommand(0x10);break;
case 0xd1: /* call 11 */
    if (lastStatus) callCommand(0x11);break;
case 0xd2: /* call 12 */
    if (lastStatus) callCommand(0x12);break;
case 0xd3: /* call 13 */
    if (lastStatus) callCommand(0x13);break;
case 0xd4: /* call 14 */
    if (lastStatus) callCommand(0x14);break;
case 0xd5: /* call 15 */
    if (lastStatus) callCommand(0x15);break;
case 0xd6: /* call 16 */
    if (lastStatus) callCommand(0x16);break;
case 0xd7: /* call 17 */
    if (lastStatus) callCommand(0x17);break;
case 0xd8: /* call 18 */
    if (lastStatus) callCommand(0x18);break;
case 0xd9: /* call 19 */
    if (lastStatus) callCommand(0x19);break;
case 0xda: /* call 1a */
    if (lastStatus) callCommand(0x1a);break;
case 0xdb: /* call 1b */
    if (lastStatus) callCommand(0x1b);break;
case 0xdc: /* call 1c */
    if (lastStatus) callCommand(0x1c);break;
case 0xdd: /* call 1d */
    if (lastStatus) callCommand(0x1d);break;
case 0xde: /* call 1e */
    if (lastStatus) callCommand(0x1e);break;
case 0xdf: /* call 1f */
    if (lastStatus) callCommand(0x1f);break;
case 0xe0: /* call 20 */
    if (lastStatus) callCommand(0x20);break;
case 0xe1: /* call 21 */
    if (lastStatus) callCommand(0x21);break;
case 0xe2: /* call 22 */
    if (lastStatus) callCommand(0x22);break;
case 0xe3: /* call 23 */
    if (lastStatus) callCommand(0x23);break;
case 0xe4: /* call 24 */
    if (lastStatus) callCommand(0x24);break;
case 0xe5: /* call 25 */
    if (lastStatus) callCommand(0x25);break;
case 0xe6: /* call 26 */
    if (lastStatus) callCommand(0x26);break;
case 0xe7: /* call 27 */
    if (lastStatus) callCommand(0x27);break;
case 0xe8: /* call 28 */
    if (lastStatus) callCommand(0x28);break;
case 0xe9: /* call 29 */
    if (lastStatus) callCommand(0x29);break;
case 0xea: /* call 2a */
    if (lastStatus) callCommand(0x2a);break;
case 0xeb: /* call 2b */
    if (lastStatus) callCommand(0x2b);break;
case 0xec: /* call 2c */
    if (lastStatus) callCommand(0x2c);break;
case 0xed: /* call 2d */
    if (lastStatus) callCommand(0x2d);break;
case 0xee: /* call 2e */
    if (lastStatus) callCommand(0x2e);break;
case 0xef: /* call 2f */
    if (lastStatus) callCommand(0x2f);break;
case 0xf0: /* call 30 */
    if (lastStatus) callCommand(0x30);break;
case 0xf1: /* call 31 */
    if (lastStatus) callCommand(0x31);break;
case 0xf2: /* call 32 */
    if (lastStatus) callCommand(0x32);break;
case 0xf3: /* call 33 */
    if (lastStatus) callCommand(0x33);break;
case 0xf4: /* call 34 */
    if (lastStatus) callCommand(0x34);break;
case 0xf5: /* call 35 */
    if (lastStatus) callCommand(0x35);break;
case 0xf6: /* call 36 */
    if (lastStatus) callCommand(0x36);break;
case 0xf7: /* call 37 */
    if (lastStatus) callCommand(0x37);break;
case 0xf8: /* call 38 */
    if (lastStatus) callCommand(0x38);break;
case 0xf9: /* call 39 */
    if (lastStatus) callCommand(0x39);break;
case 0xfa: /* call 3a */
    if (lastStatus) callCommand(0x3a);break;
case 0xfb: /* call 3b */
    if (lastStatus) callCommand(0x3b);break;
case 0xfc: /* call 3c */
    if (lastStatus) callCommand(0x3c);break;
case 0xfd: /* call 3d */
    if (lastStatus) callCommand(0x3d);break;
case 0xfe: /* call 3e */
    if (lastStatus) callCommand(0x3e);break;
case 0xff: /* call 3f */
    if (lastStatus) callCommand(0x3f);break;
