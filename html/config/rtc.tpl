<html>
	<head><title>Real Time Clock Settings</title>
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
		<meta http-equiv="Content-Type" content="text/html;charset=utf-8">
		<link rel="stylesheet" type="text/css" href="style.css">
	</head>
	<body>
		<div id="main">
			<h1>Adjust Date & Time</h1>
			<form method="post" action="rtc.cgi">
				Time <input type="time" value="%rtc_time%"><br/>
				Date <input type="date" value="%rtc_date%"> 
				<input type="submit" name="submit" value="Submit New Settings">
			</form>
		</div>
	</body>
</html>