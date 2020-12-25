# SetSB: Mixer Setting Utility (v0.91b)
Copyright (C) 1998 Jeremiah Blanchard

## License
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

## About SetSB
SetSB is a mixer setting utility for your Sound Blaster or compatible sound
card. I don't know if it'll work on everything - it's beta. So far, the only
thing I know is that SetSB works on is my sound card, which is a Sound Blaster
Pro from Creative. It probably works with others, too. I'd appreciate it if
you'd let me know if it works with your sound card, and if it doesn't, what
happens.

The source should have come in this distribution. If not, you can download the
entire package from http://fruitbat.n3.net. This package comes with source for
DJGPP and for Borland C++. If the source was not included and you can't get it
from my homepage, try mailing me at squirminworm@geocities.com.

## Using this Program
A DPMI server is required to run the DJGPP version of this program. If you don't
have a DPMI server running, simply download CWSDPMI and place it somewhere that
it can be accessed by SetSB (for instance, in the directory from which it is run
or somewhere in the path)and it will run when SetSB is run and terminate
following execution. If you're in Windows, you've already got a DPMI server
running.

SetSB can be easily set up in your autoexec.bat file if you are running a DOS
compatible system. The following parameters are accepted by SetSB:

-?, -h  - display the help screen
-s      - setup

The syntax of SetSB for autoload from a file should be:

setsb <config file>

To set up a new file or edit an existing one, type:

setsb -s [<config file>]

The default file that comes with setsb, setsb.cfg, might be a good starting
point if you're unfamiliar with common settings.

## **DISCLAIMER**
USE THIS PROGRAM AT YOUR OWN RISK. I AM NOT RESPONSIBLE FOR ANYTHING
IT MAY DO TO YOUR COMPUTER. <And if you sue me, it won't help - I'm poor.
And ya know, you can't suck water out of a well that's dry.> THIS SOFTWARE
IS NOT COMPLETE AND COMES WITH NO GUARANTEES WHATSOEVER.

Enjoy.  

--Jeremiah Blanchard
