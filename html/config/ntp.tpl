<html>
	<head><title>NTP settings</title>
	<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
	<link rel="stylesheet" type="text/css" href="../style.css">

<script type="text/javascript">

window.onload=function(e) {
	sc('ntp-enable',%ntp-enable%);
	di();
};

function sc(l,v) {
document.getElementById(l).checked=v;}

function sd(l,v) {
if(document.getElementById(l)) document.getElementById(l).disabled=v;}

function di(){
var v=true;
if (document.getElementById('ntp-enable').checked) v=false;
sd('ntp-tz',v);
}

</script>

</head>
	<body>
		<p><b>NTP Settings</b></p>
		<form name="ntpform" action="ntp.cgi" method="post">
			<table>
				<tr><td>NTP client enabled?:</td><td><input type="checkbox" name="ntp-enable" id="ntp-enable" onclick="di();"/></td></tr>
				<tr><td>GMT offset:</td><td><input type="text" name="ntp-tz" id="ntp-tz" value="%ntp-tz%"/></td></tr>
				<tr><td>Server #1:</td><td><input type="text" name="ntp-server-1" id="ntp-server-1" value="%ntp-server-1%"/></td></tr>
				<tr><td>Server #2:</td><td><input type="text" name="ntp-server-2" id="ntp-server-2" value="%ntp-server-2%"/></td></tr>
				<tr><td>Server #3:</td><td><input type="text" name="ntp-server-3" id="ntp-server-3" value="%ntp-server-3%"/></td></tr>
			</table>
			</br>
			<button type="button" onClick="parent.location='/'">Back</button><input type="submit" name="save" value="Save">
		</form>
		<p>%NTP%</p>
	</body>
</html>