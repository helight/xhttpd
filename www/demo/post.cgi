#!/bin/sh
#echo "Content-type:text/html\r\n\r\n"
#echo "Set-Cookie:user=helight.xu"
echo "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>"
echo "<html>"
echo "<head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8' /><title>XiYou-OS-Monitor</title></head>"
echo "<body><div style='height: 50px;'></div>"
echo "<div align='center'><div>"
if [ "$REQUEST_METHOD" = "POST" ]; then
	QUERY_STRING=`cat -`
fi

echo $str

echo "<form name=f action=oo.cgi method=post>"
echo "<input type=text name=XY_wd  size=42 maxlength=100><br>"
echo "<input type=text name=XY_ww  size=42 maxlength=100><br>"
echo "<br><br><input type=submit value=配置 ><input type=reset value=重置 >"
echo "</form></div></div></body></html>"
