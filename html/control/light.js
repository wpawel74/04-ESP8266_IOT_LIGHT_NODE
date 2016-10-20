var visibleFlag = 1;
var statusMsg = false;
var connected = false;
var doingsave = false;

var state = {
	light: 0,
};

function update() {
	if (state.light == 1) {
		$("#light").html("ON");
		$("#light").css("background-color", "#ff9600");
	} else {
		$("#light").html("OFF");
		$("#light").css("background-color", "#555");
	}
}

$("#light").click(function () {
	state.light++;
	if (state.light >  1) state.light = 0;
	if (state.light == 1) {
		$(this).html("ON");
		$(this).css("background-color", "#ff9600");
	} else {
		$(this).html("OFF");
		$(this).css("background-color", "#555");
	}
	save("light", state.light);
});

// function for checking if the page is visible or not
// (if not visible it will stop updating data)
function checkVisibility() {
	$(window).bind("focus", function(event) {
		visibleFlag = 1;
	});
	$(window).bind("blur", function(event) {
		visibleFlag = 0;
	});
}

// show msg on status bar
function setStatus(msg,dur,pri){
	if(statusMsg == true)
		return;
	statusMsg = true;
	if(pri > 0){
		$("#statusView").toggleClass("statusViewAlert",true);
		$("#statusView").toggleClass("statusView",false);
	} else {
		$("#statusView").toggleClass("statusView",true);
		$("#statusView").toggleClass("statusViewAlert",false);
	}
	$("#statusView").show();
	$("#statusView").html(msg);
	dur = dur*1000;
	if(dur > 0){
		setTimeout(function(){$("#statusView").hide(200);$("#statusView").html(""); statusMsg= false},dur)
	}
}

function save(param, payload) {
	doingsave=true;
	$.ajax({
		type: 'GET',
		url: "light.cgi?" + param + "=" + payload,
		async: true,
		timeout: 3000,
		tryCount : 0,
		retryLimit : 3,
		success: function (data) {
			statusMsg = false;
			if(!connected) setStatus("Connected",2,0); 
			connected = true;
			doingsave = false;
		},
		error : function(xhr, textStatus, errorThrown ) {
			if (textStatus == 'timeout') {
				this.tryCount++;
				if (this.tryCount <= this.retryLimit) {
					//try again
					$.ajax(this);
					return;
				}
				return;
			}
			if(connected) setStatus("No connection to server!",0,1);
			connected=false;
			doingsave=false;
		}
	});
}

function server_get() {
	var output = {};
	checkVisibility();
	if (visibleFlag) {
		$.ajax({
			url: "light.cgi",
			dataType: 'json',
			async: true,
			timeout: 3000,
			tryCount : 0,
			retryLimit : 3,
			success: function (data) {
				if (data.length !== 0) {
					statusMsg = false;
					if(!connected) setStatus("Connected",2,0); 
					connected=true;
					if(!doingsave) {
						state = data;
						update();
					}
				}
			},
			error : function(xhr, textStatus, errorThrown ) {
				if (textStatus == 'timeout') {
					this.tryCount++;
					if (this.tryCount <= this.retryLimit) {
						//try again
						$.ajax(this);
						return;
					}
					return;
				}
				if(connected) setStatus("No connection to server!",0,1);
					connected = false;
			}
		});
	}
	return;
}

$(document).ready(function() {
	server_get();
	update();
	setInterval(server_get, 5000);
	checkVisibility();
});