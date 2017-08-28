
 static BYTE8 a,c,ac,r[64],timerEnabled,timerCount,timerOverflow,temp,sp;
 static WORD16 pctr,timerRegister;
 static int totalElapsedCycles;

 static void i8021reset() {
  timerCount = 0;totalElapsedCycles = 0;
  pctr = sp = 0;
 }

 static int fetch() {
  int opcode = codeMemory[pctr++];
  pctr = pctr & 0x7FF;
  timerCount--;
  return opcode;
 }

 static void add(int n,int cIn) {
  ac = ((a & 0x0F)+(n & 0x0F) + (cIn ? 1 : 0)) > 15;
  a = a + n + (cIn ? 1 : 0);
  c = (a > 0xFF) ? 1 : 0;
  a = a & 0xFF;
 }

 static void push(int address) {
  int addr = sp * 2 + 8;
  sp = (sp + 1) & 7;
  r[addr] = address & 0xFF;
  r[addr+1] = address >> 8;
 }

 static int pull() {
  sp = (sp - 1) & 7;
  int addr = sp * 2 + 8;
  return (r[addr] | (r[addr+1] << 8)) & 0x7FF;
 }

 static void decimalAdjust() {
  if (ac || (a & 0x0F) > 9) {
   a = a + 6;
  }
  if (c || (a & 0xF0) > 0x90) {
   a = a + 0x60;
   c = 1;
  }
 }

 static void exchangeNibble(int address) {
  int oldReg = r[address];
  r[address] = (r[address] & 0xF0) | (a & 0x0F);
  a = (a & 0xF0) | (oldReg & 0x0F);
 }

 static void rotateLeft() {
  a = a << 1;
  if (a > 0xFF) a = (a & 0xFF) | 1;
 }

 static void rotateLeftThroughc() {
  a = (a << 1) | (c ? 1 : 0);
  c = (a > 0xFF);
  a = a & 0xFF;
 }

 static void rotateRight() {
  int lsb = a & 1;
  a = (a >> 1)  | (lsb << 7);
 }

 static void rotateRightThroughc() {
  int newc = (a & 1) != 0;
  a = (a >> 1) | (c ? 0x80:0x00);
  c = newc;
 }

