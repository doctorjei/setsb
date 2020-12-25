/* SetSB, by Jeremiah Blanchard
   To the reader: This code is beta. It's written mainly
   because there was a piece of shareware that was annoying the living
   hell out of me. I'm really sick of seeing shareware that shouldn't be
   shareware, but free software, so I wrote this mixer setting program
   in the hopes that some of you will find it easier to set your mixer up
   upon the startup of your computer without getting any annoying messages
   or beeps. This program does not reside in memory. And remember,
   information wants to be free... and so does software!

   P.S. Sorry that there aren't many comments in the code. I'm kinda new at
   sound programming. In actuality, I had a helluva time figuring out how it
   works. BTW, remember that this code is under the GPL - that means that any
   software derived from it must also be covered by the GPL. However, you can
   modify it and distribute it, or pull code right out of it, under the terms
   of the GPL.

   P.P.S.  If you have any questions or bug reports, feel free to email me. */

#include <ctype.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>

#define DSP		0xAA	/* byte DSP returns when ready */
#define MASTERVOL	0x22
#define VOICEVOL	0x04
#define MIDIVOL		0x26
#define CDVOL		0x28
#define MICVOL		0x0A
#define ADCSELECT	0x0C

struct
  {                                          // Structure for file
   unsigned char VoiceVol, MasterVol, MicVol, CDVol, MIDIVol, unused;
   unsigned int sbaddr;
  } vol;

void mode(int modenum)
{
 _AX=modenum;
 geninterrupt(0x10);
}

char takechar()
{
 _AX = 0;
 geninterrupt(0x16);
 return(_AL);
}

char *num_to_hex(int number, char *whereto)
{
 for (char index = 0; index < 4; index++)
 {
  switch ((number >> ((index^3) << 2)) & 0xf)
  {
   case 0x0: whereto[index] = '0'; break;
   case 0x1: whereto[index] = '1'; break;
   case 0x2: whereto[index] = '2'; break;
   case 0x3: whereto[index] = '3'; break;
   case 0x4: whereto[index] = '4'; break;
   case 0x5: whereto[index] = '5'; break;
   case 0x6: whereto[index] = '6'; break;
   case 0x7: whereto[index] = '7'; break;
   case 0x8: whereto[index] = '8'; break;
   case 0x9: whereto[index] = '9'; break;
   case 0xA: whereto[index] = 'A'; break;
   case 0xB: whereto[index] = 'B'; break;
   case 0xC: whereto[index] = 'C'; break;
   case 0xD: whereto[index] = 'D'; break;
   case 0xE: whereto[index] = 'E'; break;
   case 0xF: whereto[index] = 'F'; break;
  }
 }
 whereto[4] = 0;
return(whereto);
}

long hex_to_num(char *hexstr)
{
 long hexnum = 0;

 for(int index = 0; index < strlen(hexstr); index++)
 {
  hexnum <<= 4;
  switch (toupper(hexstr[index]))
  {
   case '0': hexnum += 0x0; break;
   case '1': hexnum += 0x1; break;
   case '2': hexnum += 0x2; break;
   case '3': hexnum += 0x3; break;
   case '4': hexnum += 0x4; break;
   case '5': hexnum += 0x5; break;
   case '6': hexnum += 0x6; break;
   case '7': hexnum += 0x7; break;
   case '8': hexnum += 0x8; break;
   case '9': hexnum += 0x9; break;
   case 'A': hexnum += 0xA; break;
   case 'B': hexnum += 0xB; break;
   case 'C': hexnum += 0xC; break;
   case 'D': hexnum += 0xD; break;
   case 'E': hexnum += 0xE; break;
   case 'F': hexnum += 0xF; break;
  }
 }
return(hexnum);
}


unsigned int get_dsp()
/* Get info from dsp. Wait until bit is on and read from the data port.*/
{
 int data_read=vol.sbaddr+0xA;    /* reading address (DSP) */
 int datastatus=vol.sbaddr+0xE;   /* address to check for data status */

 while (!(inp(datastatus) & 0x80));
 return(inp(data_read));
}

void send_dsp(unsigned int val)
/* Outputs a byte to the dsp.  Waits for write buffer status bit to be 0,
   then outputs the value to the write buffer. */
{
 int writebuffer=vol.sbaddr+0xC;    /* DSP write buffer address */

 while (inp(writebuffer) & 0x80);
 outp(writebuffer, val);
}

void set_mixer(void)
{
 char hexidecimal[5];
 int mixeraddr=vol.sbaddr+0x4,    /* mixer select port */
     mixerdata=vol.sbaddr+0x5,    /* mixer data port */
     resetport=vol.sbaddr+0x6;    /* reset port address */
 mode(0x03);
 cout << "SetSB Sound Blaster Mixer Setting Utility\n"
      << "Copyright (C) 1998 Jeremiah Blanchard - FEEL FREE TO DISTRIBUTE!\n"
      << "This program is distributed under the terms of the\n"
      << "GNU General Public License.  See GPL.DOC for more information.\n\n";
 outp(resetport,1);       /* reset DSP */
 delay(1);
 outp(resetport,0);
 if (get_dsp()!=DSP) { cout << "SBPRO not ready.\n\n"; exit(1); }
 outp(mixeraddr,MICVOL);
 outp(mixerdata,vol.MicVol); /* set microphone */
 outp(mixeraddr,VOICEVOL);
 outp(mixerdata,vol.VoiceVol); /* set voice */
 outp(mixeraddr,MASTERVOL);
 outp(mixerdata,vol.MasterVol);/* set master */
 outp(mixeraddr,CDVOL);
 outp(mixerdata,vol.CDVol);       /* set CD */
 outp(mixeraddr,MIDIVOL);
 outp(mixerdata,vol.MIDIVol);  /* set MIDI */
 send_dsp(0xD1);               /* turn speaker on */

 cout << "Initialized with the following values:\n\n"
      << "Channel -         Left       Right\n"
      << "Voice Volume    " << setw(6) << ((vol.VoiceVol & 0xf0) >> 4)
      << setw(12) << (vol.VoiceVol & 0x0f) << endl
      << "Master Volume   " << setw(6) << ((vol.MasterVol & 0xf0) >> 4)
      << setw(12) << (vol.MasterVol & 0x0f) << endl
      << "CD Player Volume" << setw(6) << ((vol.CDVol & 0xf0) >> 4)
      << setw(12) << (vol.CDVol & 0x0f) << endl
      << "MIDI Volume     " << setw(6) << ((vol.MIDIVol & 0xf0) >> 4)
      << setw(12) << (vol.MIDIVol & 0x0f) << endl << endl
      << "Mic Volume      " << setw(6) << (vol.MicVol >> 0) << "   Hex: x"
      << num_to_hex(vol.sbaddr, hexidecimal)+1 << endl << endl
      << "Support free software! USE IT!\n\n";
}

char openfile(char *filename)
{
   ifstream configfile;

   configfile.open(filename, ios::nocreate);
   if (!configfile) return(1);
   if (!configfile.read((char *) &vol, sizeof(vol)))
       {configfile.close(); return(1);}
   configfile.close();
   return(0);
}

char parseparam(char *param) /* parameter parser */
{
 int index;

 if (param[0]!='-' && param[0]!='/') /* if not parameter, is it a filename? */
   {
    for (index=0; index <=strlen(param); index++)
      {
       if (param[index]=='\0') break; /* if end of string, end loop */
       if (iscntrl(param[index]))
	 {
	  cout << "Invalid filename. For help, try setsb -h\n\n";
	  exit(1); /* If invalid, exit with error. */
	 }
      }
    return(1);
   }

 else if (param[0]=='-' || param[0]=='/') /* If is a parameter, parse. */
   {
    if (toupper(param[1])=='H' || param[1]=='?') /* Help? If yes, display. */
      {
       cout << "Usage:\n"
	    << "setsb [-parameter]\n"
	    << "or\n"
	    << "setsb [filename]\n\n"
	    << "Parameters:\n"
	    << "h, ?  -  this screen\n"
	    << "s     -  invokes setup\n\n"
	    << "[filename] is the name of the configuration file.\n\n";
       exit(0); /* exit without error. */
      }
    else if (toupper(param[1])=='S') return(2); /* Setup? If yes, return 2. */
    else
      {
       cout <<"Invalid parameter. For help, try setsb -h\n\n";
       exit(1); /* exit with error. */
      }
   }
 else
   {
    cout << "Invalid command line. For help, try setsb -h\n\n";
    exit(1); /* if none of the above, exit with error. */
   }
return(0);
}

char disp_menu()
{
 union { unsigned int decm; unsigned char character:4, micbits:2; } val;
 unsigned char *ptr, dist, result;	/* pointer to variable to be set */
 char choiceval, channel[]="VoiceVol\0MasterVol\0CDVol\0MIDIVol",
      rl[]="Left\0Right", hex[5], argm[3];


 mode(0x03);
 cout << "Here's the current data:\n"      /* print the current data */
      << "(A)Left VoiceVol:   " << setw(10) << ((vol.VoiceVol & 0xf0) >> 4)
      << " (F)Left CDVol:     " << setw(10) << ((vol.CDVol & 0xf0) >> 4) << endl
      << "(B)Right VoiceVol:  " << setw(10) << (vol.VoiceVol & 0x0f)
      << " (G)Right CDVol:    " << setw(10) << (vol.CDVol & 0x0f) << endl
      << "(C)Left MasterVol:  " << setw(10) << ((vol.MasterVol & 0xf0) >> 4)
      << " (H)Left MIDIVol:   " << setw(10) << ((vol.MIDIVol & 0xf0) >> 4) << endl
      << "(D)Right MasterVol: " << setw(10) << (vol.MasterVol & 0x0f)
      << " (I)Right MIDIVol:  " << setw(10) << (vol.MIDIVol & 0x0f) << endl
      << "(E)MicVol:          " << setw(10) << (vol.MicVol >> 0) << " (J)SB Hex Address:     0x"
      << num_to_hex(vol.sbaddr, hex) << endl << endl
      << "(Q)uit\n\n"
      << "Please select one: ";
 choiceval = takechar();
 switch (toupper(choiceval))
 {  /* set string and pointer accordingly for generic values. */
  case 'A': dist = 0x00; ptr=&vol.VoiceVol; break;
  case 'B': dist = 0x80; ptr=&vol.VoiceVol; break;
  case 'C': dist = 0x09; ptr=&vol.MasterVol; break;
  case 'D': dist = 0x89; ptr=&vol.MasterVol; break;
  case 'E':
  cout << "\n\nPlease enter a value for MicVol (0-3): ";
  cin.get(argm, 2, '\n');
  cin.ignore(80, '\n');
  vol.MicVol = atoi(argm);
  if (vol.MicVol > 3) vol.MicVol = 3;
  return(0);
  case 'F': dist = 0x13; ptr=&vol.CDVol; break;
  case 'G': dist = 0x93; ptr=&vol.CDVol; break;
  case 'H': dist = 0x19; ptr=&vol.MIDIVol; break;
  case 'I': dist = 0x99; ptr=&vol.MIDIVol; break;
  case 'J':  /* if it's setting the SB hex value, */
  cout << "\n\nPlease enter a new hex value: 0x"; /* prompt for hex. */
  cin.get(hex, 5, '\n');
  cin.ignore(80, '\n');
  vol.sbaddr = hex_to_num(hex);
  if (vol.sbaddr > 0x240) vol.sbaddr = 0x240;
  return(0);     /* return, continue. */
  case 'Q': return(1); /* return, quit. */
  default: cout << "\n\nNot a choice."; takechar(); return(0);
 } /* if none of the above, print "Not a choice" and return, continue. */
 cout << "\n\nPlease enter a value for " << &rl[((dist & 0x80) >> 7) * 5]
      << " " << &channel[dist & 0x7f] << " (0-15): ";
 cin.get(argm, 3, '\n');
 cin.ignore(80, '\n');
 result = atoi(argm);
 if (result > 15) result = 15;
 if (dist & 0x80) *ptr = (*ptr & 0xf0) | result;
 else *ptr = (*ptr & 0x0f) | (result << 4);
 return(0); /* return, continue. */
}

char setupmixer(char *filename)  /* main setup function */
{
 char yn;
 ofstream outfile; /* file object */

 if (openfile(filename)) /* if the file doesn't open, */
   {
    cout << "File doesn't seem to exist, or can't be accessed. Starting\n"
	 << "a new file with filename.\n";
    takechar();
    vol.MicVol=0;      /* set values to 0. */
    vol.VoiceVol=0;
    vol.MasterVol=0;
    vol.CDVol=0;
    vol.MIDIVol=0;
    vol.sbaddr=0x220;        /* set sbaddr to default. */
   }
 while (!disp_menu());    /* while the return value is 0, DispMenu.*/
 mode(0x03);
 cout << "Saving....";
 outfile.open(filename);
 if (!outfile) cout << "error saving file.\n\n";
 else
   {
    if (!outfile.write((char *) &vol, sizeof(vol)))
	cout << "Error saving file.\n\n";
    outfile.close();
    cout << "done!\n\n";
   }
 cout << "\nWrite values to SB registers[y/N]? "; /* Write to registers? */
 yn = takechar();
 if (toupper(yn) == 'Y') return(1); else return(0);
}


void main(int argc, char *argv[])
{
 char *filename, parseindex, parseval, exists = 0, runsetup = 0, setupr, err;

 /* If there are no arguments, display message and exit with error.  Then
    parse the arguments, setting values accordingly. */

 if (!(argc - 1))
    {cout << "No parameters. For help, try setsb -h\n"; exit(1);}

 for(parseindex = argc - 1; parseindex > 0; parseindex--)
   {
    parseval = parseparam(argv[parseindex]);
    if (parseval == 1) { filename = argv[parseindex]; exists = 1; }
    if (parseval == 2) runsetup = 1;
   }

 /* If there was no filename entered, get a filename from the user. */
 if (!exists)
   {
    if ((filename = (char *) malloc(256)) == NULL)
      {
       cout << "Mem Error.\n";
       exit(1);
      }
    cout << "Please enter a filename: ";
    cin.get(filename, 30, '\n');
    cin.ignore(80, '\n');
   }

 /* If the setup argument was used, run the setup. */
 if (runsetup)
   {
    setupr=setupmixer(filename);
    /* If the user chose not to write the values to the register, exit. */
    if (!setupr)
      {
       cout << "\nThis program is distributed under the terms of the GNU\n"
	    << "General Public License.  See GPL.DOC for more information.\n";
       exit(0);
      }

   }

 /* Open the file. If there's an error, exit with error. */
 if ((err = openfile(filename)) > 0)
   {
    cout << "Error " << (int) err << " opening " << filename << ".\n";
    exit(1);
   }

 /* Set the registers. */
 set_mixer(); /* Run SetMixer function. */
}
