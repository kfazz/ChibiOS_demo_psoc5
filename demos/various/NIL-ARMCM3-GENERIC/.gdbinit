target remote | openocd -c "gdb_port pipe; log_output openocd.log" -f interface/kitprog.cfg -f target/psoc5lp.cfg
file build/ch.elf
load build/ch.elf
break main
continue
