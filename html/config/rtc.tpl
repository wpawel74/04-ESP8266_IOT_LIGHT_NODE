<html>
	<head><title>Real Time Clock Settings</title>
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8">
		<link rel="stylesheet" type="text/css" href="../style.css">
	</head>
	<body>
		<div id="main">
			<h1>Adjust Date & Time</h1>
			<form name="rtcform" method="post" action="rtc.cgi">
				Time <input type="time" name="time" id="time" value="%rtc_time%"><br/>
				Date <input type="date" name="date" id="date" value="%rtc_date%">
				<button type="button" onClick="parent.location='/'">Back</button>
				<input type="submit" name="save" value="Save">
			</form>
		</div>
	</body>
</html>