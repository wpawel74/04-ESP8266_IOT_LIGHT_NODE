<html>
	<head>
		<title>Alarm settings</title>
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
		<link rel="stylesheet" type="text/css" href="../style.css">
	</head>
	<body>
		<p><b>Alarm Settings</b></p>
		<form name="alarmform" action="alarm.cgi" method="post">
			<table>
				<select name="power_supply_configuration" id="power_supply_configuration">
					<option value="0" %selected_relay1% >%name_relay1%</option>
					<option value="1" %selected_relay2% >%name_relay2%</option>
					<option value="2" %selected_dontcare% >Don't care</option>
				</select>
				<tr><td>Alarm duration:</td><td><input type="text" name="alarm_duration"  id="alarm_duration"  value="%alarm_duration%"/> ms</td></tr>
			</table>
			<br/>
			<table>
				<tr>
					<td>Enable</td>
					<td>Time</td>
					<td>Sun</td>
					<td>Mon</td>
					<td>Tue</td>
					<td>Wen</td>
					<td>Thr</td>
					<td>Fri</td>
					<td>Sat</td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a0-enable" id="a0-enable" %a0-enable% /></td>
					<td><input type="time" name="a0-time" id="a0-time" value="%a0-time%" /></td>
					<td><input type="checkbox" name="a0-day0" id="a0-day0" %a0-day0%/></td>
					<td><input type="checkbox" name="a0-day1" id="a0-day1" %a0-day1%/></td>
					<td><input type="checkbox" name="a0-day2" id="a0-day2" %a0-day2%/></td>
					<td><input type="checkbox" name="a0-day3" id="a0-day3" %a0-day3%/></td>
					<td><input type="checkbox" name="a0-day4" id="a0-day4" %a0-day4%/></td>
					<td><input type="checkbox" name="a0-day5" id="a0-day5" %a0-day5%/></td>
					<td><input type="checkbox" name="a0-day6" id="a0-day6" %a0-day6%/></td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a1-enable" id="a1-enable" %a1-enable% /></td>
					<td><input type="time" name="a1-time" id="a1-time" value="%a1-time%" /></td>
					<td><input type="checkbox" name="a1-day0" id="a1-day0" %a1-day0%/></td>
					<td><input type="checkbox" name="a1-day1" id="a1-day1" %a1-day1%/></td>
					<td><input type="checkbox" name="a1-day2" id="a1-day2" %a1-day2%/></td>
					<td><input type="checkbox" name="a1-day3" id="a1-day3" %a1-day3%/></td>
					<td><input type="checkbox" name="a1-day4" id="a1-day4" %a1-day4%/></td>
					<td><input type="checkbox" name="a1-day5" id="a1-day5" %a1-day5%/></td>
					<td><input type="checkbox" name="a1-day6" id="a1-day6" %a1-day6%/></td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a2-enable" id="a2-enable" %a2-enable% /></td>
					<td><input type="time" name="a2-time" id="a2-time" value="%a2-time%" /></td>
					<td><input type="checkbox" name="a2-day0" id="a2-day0" %a2-day0%/></td>
					<td><input type="checkbox" name="a2-day1" id="a2-day1" %a2-day1%/></td>
					<td><input type="checkbox" name="a2-day2" id="a2-day2" %a2-day2%/></td>
					<td><input type="checkbox" name="a2-day3" id="a2-day3" %a2-day3%/></td>
					<td><input type="checkbox" name="a2-day4" id="a2-day4" %a2-day4%/></td>
					<td><input type="checkbox" name="a2-day5" id="a2-day5" %a2-day5%/></td>
					<td><input type="checkbox" name="a2-day6" id="a2-day6" %a2-day6%/></td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a3-enable" id="a3-enable" %a3-enable% /></td>
					<td><input type="time" name="a3-time" id="a3-time" value="%a3-time%" /></td>
					<td><input type="checkbox" name="a3-day0" id="a3-day0" %a3-day0%/></td>
					<td><input type="checkbox" name="a3-day1" id="a3-day1" %a3-day1%/></td>
					<td><input type="checkbox" name="a3-day2" id="a3-day2" %a3-day2%/></td>
					<td><input type="checkbox" name="a3-day3" id="a3-day3" %a3-day3%/></td>
					<td><input type="checkbox" name="a3-day4" id="a3-day4" %a3-day4%/></td>
					<td><input type="checkbox" name="a3-day5" id="a3-day5" %a3-day5%/></td>
					<td><input type="checkbox" name="a3-day6" id="a3-day6" %a3-day6%/></td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a4-enable" id="a4-enable" %a4-enable% /></td>
					<td><input type="time" name="a4-time" id="a4-time" value="%a4-time%" /></td>
					<td><input type="checkbox" name="a4-day0" id="a4-day0" %a4-day0%/></td>
					<td><input type="checkbox" name="a4-day1" id="a4-day1" %a4-day1%/></td>
					<td><input type="checkbox" name="a4-day2" id="a4-day2" %a4-day2%/></td>
					<td><input type="checkbox" name="a4-day3" id="a4-day3" %a4-day3%/></td>
					<td><input type="checkbox" name="a4-day4" id="a4-day4" %a4-day4%/></td>
					<td><input type="checkbox" name="a4-day5" id="a4-day5" %a4-day5%/></td>
					<td><input type="checkbox" name="a4-day6" id="a4-day6" %a4-day6%/></td>
				</tr>
				<tr>
					<td><input type="checkbox" name="a5-enable" id="a5-enable" %a5-enable% /></td>
					<td><input type="time" name="a5-time" id="a5-time" value="%a5-time%" /></td>
					<td><input type="checkbox" name="a5-day0" id="a5-day0" %a5-day0%/></td>
					<td><input type="checkbox" name="a5-day1" id="a5-day1" %a5-day1%/></td>
					<td><input type="checkbox" name="a5-day2" id="a5-day2" %a5-day2%/></td>
					<td><input type="checkbox" name="a5-day3" id="a5-day3" %a5-day3%/></td>
					<td><input type="checkbox" name="a5-day4" id="a5-day4" %a5-day4%/></td>
					<td><input type="checkbox" name="a5-day5" id="a5-day5" %a5-day5%/></td>
					<td><input type="checkbox" name="a5-day6" id="a5-day6" %a5-day6%/></td>
				</tr>
			</table>
			</br>
			<button type="button" onClick="parent.location='/'">Back</button><input type="submit" name="save" value="Save">
		</form>
	</body>
</html>