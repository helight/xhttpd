#!/bin/sh
echo "Content-type: text/html;  charset=UTF-8\r\n\r\n"
echo ""
echo ""
#echo "Set-Cookie:user=helight.xu"

echo "<html><head><title>The content you want </title>"
echo "<meta http-equiv="Content-Style-Type"content="text/css";charset="gb2312">"
echo "</head><body  background='./img/boo.jpg'; bgcolor='#FFFFFF' >"

echo "<h1 align=center style="border-style:outset" > The imformation you need is</h1>"
echo "<div style='margin:4 8em'></div>"
echo "<hr>"

echo $QUERY_STRING
if [ -z $XY_path ]
	then
	XY_path=xu_zhenwen
fi
echo "<span style='width:40px;height:50px;'><a href='get.cgi?XY_path=$XY_path$XY_path'>$XY_path</a></span>"

echo "<hr>"
echo "<a href='get.cgi?XY_path=home&XY_name=zhenwen&XY_www=zhwen.org&XY_dim4=dim4.cn&XY_gmail=helight&XY_xux=xux'>test</a>"

echo "<hr>"
echo $XY_path
echo $XY_name
echo $XY_www
echo $XY_dim4
echo $XY_gmail
echo $XY_xux

echo "<hr></body></html>"

