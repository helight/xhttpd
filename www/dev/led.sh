#!/bin/sh
echo "<html><head><title>Led控制</title><meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>"

echo "<body><div width=900px><div align=center><h1>Led控制</h1></div><hr> <div>"

echo $XY_led
led=1
if [ -nz $XY_led ]; then
	if [ $XY_led -eq $led ]; then
		echo "led off"
#	elif [ $XY_led == 0 ]; then
	else
		echo "led on"
	fi
fi
echo "<form action=led.cgi method=post>"
#echo <input type="checkbox" name="XY_on" value="0"/>
#echo <input type="checkbox" name="XY_off" id="checkbox" value="1" />
echo "<input name='XY_led' value='1' checked='checked' type='radio'><label >ON &nbsp;</label>"
echo "<input name='XY_led' value='0' type='radio'><label >OFF &nbsp;</label>"

echo "<br><input type=submit value=配置 ><input type=reset value=重置 ></form> </div><hr></div></body></html>"


