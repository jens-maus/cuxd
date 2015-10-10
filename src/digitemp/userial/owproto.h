/* Prototypes for userial driver functions */

/* From other low level userial files */
SMALLINT owAccess(int);

#ifndef OWUSB
SMALLINT owAcquire(int,char *);
#else
SMALLINT owAcquire(int,char *, char *);
#endif /* OWUSB */

void     owSerialNum(int,uchar *,SMALLINT);
SMALLINT owWriteBytePower(int,SMALLINT);
SMALLINT owWriteByte(int,SMALLINT);
SMALLINT owReadByte(int);
SMALLINT owLevel(int,SMALLINT);
void     msDelay(int);
SMALLINT owBlock(int,SMALLINT,uchar *,SMALLINT);
SMALLINT owTouchReset(int);

#ifndef OWUSB
void     owRelease(int);
#else
void     owRelease(int, char *);
#endif /* OWUSB */

SMALLINT owFirst(int,SMALLINT,SMALLINT);
SMALLINT owNext(int,SMALLINT,SMALLINT);


/* From owerr.c */
int owGetErrorNum(void);
void owClearError(void);
int owHasErrors(void);
#ifdef DEBUG
   void owRaiseError(int,int,char*);
#else
   void owRaiseError(int);
#endif
#ifndef SMALL_MEMORY_TARGET
   void owPrintErrorMsg(FILE *);
   void owPrintErrorMsgStd();
   char *owGetErrorMsg(int);
#endif
               


/* From ioutil.c */
int EnterString(char *, char *, int, int);
int EnterNum(char *, int, long *, long, long);
int EnterHex(char *, int, ulong *);
int ToHex(char ch);
int getkeystroke(void);
int key_abort(void);
void ExitProg(char *, int);
int getData(uchar*, int, SMALLINT);
void PrintHex(uchar*, int);
void PrintChars(uchar*, int);
void PrintSerialNum(uchar*);


/* From crcutil.c */
void setcrc16(int,ushort);
ushort docrc16(int,ushort);
void setcrc8(int,uchar);
uchar docrc8(int,uchar);


/* From swt1f.c */
int SetSwitch1F(int,uchar *,int,int,uchar *,int);
int SwitchStateToString1F(int, char *);
int FindBranchDevice(int,uchar *,uchar BranchSN[][8],int,int);
int owBranchFirst(int,uchar *,int,int);
int owBranchNext(int,uchar *,int,int);

/* From cnt1d.c */
SMALLINT ReadCounter(int,int,ulong *);

/* From ad26.c */
double Get_Temperature(int portnum);
float Volt_Reading(int portnum, int vdd, int *cad);
int PIO_Reading(int portnum, int pionum /* TS ignored so far */ );

/* From XXXlnk.c */
SMALLINT owTouchBit(int,SMALLINT);
SMALLINT owSpeed(int,SMALLINT);
SMALLINT owTouchByte(int portnum, SMALLINT sendbyte);
SMALLINT owProgramPulse(int portnum);

