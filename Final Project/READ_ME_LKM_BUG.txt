The panic() function was used to halt (terminate) the running system. Allowing
me to use GDB to single step through, catch the bug, and display the stack trace.  

The message format is a printk style format string. 

The message is printed to the console and the location panicstr 
is set to the address of the message text for retrieval	from the OS core dump.

Since I have the kernel debugger installed on the guest OS the control is passed to it. 

There is not Return Value from the panic() function.

The ManPage is located @ https://manpages.debian.org/wheezy/linux-manual-3.2/panic.9.en.html
