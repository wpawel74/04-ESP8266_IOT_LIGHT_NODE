<html>
	<head>
		<title>Light Comfort settings</title>
		<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
		<link rel="stylesheet" type="text/css" href="../style.css">
	</head>
	<body>
		<div id="main"><p><b>Light Comfort Settings</b></p>
		<form name="sensorform" action="light.cgi" method="post">
			<table>
				<tr><td>Light driver enabled?:</td><td><input type="checkbox" name="light_driver_enable" id="light_driver_enable" %light_driver_enable% /></td></tr>
				<tr><td>Power Supply Configuration:</td><td>
				<select name="power_supply_configuration" id="power_supply_configuration">
					<option value="0" %selected_relay1% >%name_relay1%</option>
					<option value="1" %selected_relay2% >%name_relay2%</option>
					<option value="2" %selected_dontcare% >Don't care</option>
				</select>
				<tr><td>No of leds in chain:</td><td><input type="text" name="chain_size"  id="chain_size"  value="%chain_size%"/> ms</td></tr>
				<tr><td>Delay Power ON  SEQ:</td><td><input type="text" name="delay_power_on"  id="delay_power_on"  value="%delay_power_on%"/> ms</td></tr>
				<tr><td>Delay Power OFF SEQ:</td><td><input type="text" name="delay_power_off" id="delay_power_off" value="%delay_power_off%"/> ms</td></tr>
			</table>
			<br/>
			<table>
				<tr><td>Simple style enabled (1th) ?:</td><td><input type="checkbox" name="light_simple_1_enable" id="light_simple_1_enable" %light_simple_1_enable% /></td></tr>
				<tr><td>Start led no:</td><td><input type="text" name="simple_1_start_no"  id="simple_1_start_no"  value="%simple_1_start_no%"/></td></tr>
				<tr><td>Stop  led no:</td><td><input type="text" name="simple_1_stop_no"   id="simple_1_stop_no"   value="%simple_1_stop_no%"/></td></tr>
			</table>
			<table>
				<tr><td>Simple style enabled (2th) ?:</td><td><input type="checkbox" name="light_simple_2_enable" id="light_simple_2_enable" %light_simple_2_enable% /></td></tr>
				<tr><td>Start led no:</td><td><input type="text" name="simple_2_start_no"  id="simple_2_start_no"  value="%simple_2_start_no%"/></td></tr>
				<tr><td>Stop  led no:</td><td><input type="text" name="simple_2_stop_no"   id="simple_2_stop_no"   value="%simple_2_stop_no%"/></td></tr>
			</table>
			<table>
				<tr><td>Flame style enabled (1th) ?:</td><td><input type="checkbox" name="light_flame_1_enable" id="light_flame_1_enable" %light_flame_1_enable% /></td></tr>
				<tr><td>Start led no:</td><td><input type="text" name="flame_1_start_no"  id="flame_1_start_no"  value="%flame_1_start_no%"/></td></tr>
				<tr><td>Stop  led no:</td><td><input type="text" name="flame_1_stop_no"   id="flame_1_stop_no"   value="%flame_1_stop_no%"/></td></tr>
			</table>
			<button type="button" onClick="parent.location='/'">Back</button>
			<input type="submit" name="save" value="Save">
			</p>
		</form>
	</body>
</html>