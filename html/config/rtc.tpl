<html>
	<head><title>Real Time Clock Settings</title>
		<link rel="stylesheet" type="text/css" href="style.css">
	</head>
	<body>
		<div id="main">
			<h1>Adjust Date & Time (format: YYYY/MM/DD HH:mm)</h1>
			<form method="post" action="rtc.cgi">
				<input type="text" name="year" value="%year%">/<input type="text" name="month" value="%month%">/<input type="text" name="day" value="%day%">  <input type="text" name="hours" value="%hours%">:<input type="text" name="minutes" value="%minutes%">
				<input type="submit" name="submit" value="Submit New Settings">
			</form>
		</div>
	</body>
</html>