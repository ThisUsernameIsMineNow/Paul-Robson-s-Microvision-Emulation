static BYTE8 a,x,y,status,lastStatus,pa,pb,pc,ca,cb,cs,sr,cl,sl,o,_temp;
static BYTE8 dataMemory[128],r[11];
#define READ_KEYS ('K')
#define WRITE_O ('O')
#define WRITE_R ('R')
static void tms1100reset() {
 ca = cb = cs = 0;
 pa = pb = 0xF;pc = 0;
 o = 0;hardwareIO(WRITE_O,o,0);
 int i;
 for (i = 0;i <= 10;i++) {
  r[i] = 0;hardwareIO(WRITE_R,i,0);
 }
 a = a & 0xF;x = x & 7;y = y & 0xF;sr = sr & 0x3F;
 cl = cl & 1;sl = sl & 1;lastStatus = lastStatus & 1;
 status = status & 1;
 for (i = 0;i <= 0x80;i++)
    dataMemory[i] = dataMemory[i] & 0xF;
}
static void brCommand(int address)  {
  if (cl == 0){
  ca = cb;pa = pb;pc = address;
  } else {
  ca = cb;pc = address;
  }
}
static void callCommand(int address)  {

  if (cl == 0){
  cs = ca;ca = cb;
  int temp = pa;pa = pb;pb = temp;
  sr = pc;pc = address;
  cl = 1;
  } else {
  ca = cb;pb = pa;pc = address;
  }
}
static void retnCommand()  {

  if (cl != 0){
  pc = sr;pa = pb;ca = cs;
  cl = 0;
 } else {
  pa = pb;
  }
}
