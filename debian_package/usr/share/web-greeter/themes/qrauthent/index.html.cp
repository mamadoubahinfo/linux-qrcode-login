<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN">
<html>
<head>
	<style>
		html, body {
			width: 100%;
			height: 100%;
			overflow: hidden;
			opacity: 1;
			margin: 0;
			background-color: #000;
			transition: opacity 2s, background-color 2s, background-position 2s;
			transition-timing-function: ease-in;
		}

		body {
			background: linear-gradient(transparent, SlateGray);
			background-color: #ffffff;
			font-family: 'sans-serif';
			font-size: 24px;
		}

		html.session_starting {
			opacity: 0;
		}

		.session_starting body {
			opacity: 0;
			background-color: rgba(0, 0, 0, 0);
			background-position-y: -100%;
		}

		.container {
			width: 100%;
			height: 100%;
			display: -webkit-flex;
			display: flex;
			-webkit-flex-direction: column;
			flex-direction: column;
		}

		.topBox, .inputBox, .messageBox {
			width: 100%;
			height: 33.33%;
			text-align: center;
		}

		.inputBox {
			position: relative;
		}

		.wrapper {
			height: 85px;
			width: 350px;
			position: absolute;
			left: 0;
			right: 0;
			top: 0;
			bottom: 0;
			margin: auto;
		}

		.topBox {
			text-align: right;
			padding: 15px;
			box-sizing: border-box;
		}

		.messageBox {
			text-align: center;
			visibility: hidden;
		}

		input#entry {
			outline: none;
			background: rgba(0, 0, 0, 0.05);
			box-shadow: none;
			box-sizing: border-box;
			line-height: normal;
			transition: .3s ease;
			border: none !important;
			padding: 14px 4% !important;
			font-size: 16px;
			color: #666;
			background-color: #fff;
			width: 100%;
			font-weight: 400;
			border-radius: 3px !important;
		}

		input#entry:focus {
			background: rgba(0, 0, 0, 0.1);
			color: rgba(0, 0, 0, 0.75);
		}

		input#entry:focus::placeholder {
			color: rgba(255, 255, 255, 0.3);
		}
	</style>
</head>

<body id="body">
<div class="container">
	<div class="topBox">
		<img onclick="javascript:lightdm.shutdown();" src="power_button.png"/>
	</div>
	<div class="inputBox">
		<div class="wrapper">
			<div id="prompt"></div>
			<form action="javascript:handle_input();">
				<input id="entry" />
			</form>
		</div>
	</div>
	<div class="messageBox" id="messages"></div>
</div>
<script>
	/*********************************************************/
	/*               Callbacks for the greeter               */
	/*********************************************************/

	/**
	 * show_prompt callback.
	 */
	window.show_prompt = function(text, type) {
		// type is either "text" or "password"
		let prompt = document.getElementById("prompt"),
			entry = document.getElementById("entry");

		entry.placeholder = text.charAt(0).toUpperCase() + text.slice(1, -1);

		// clear entry
		entry.value = "";
		entry.type = type == 0 ? "text" : "password";
	};

	/**
	 * show_message callback.
	 */
	window.show_message = function(text, type) {
		if (0 === text.length) {
			return;
		}
		let messages = document.getElementById('messages');
		messages.style.visibility = "visible";
		// type is either "info" or "error"
		if (type == 'error') {
			text = `<p style="color:red;">${text}</p>`;
		}
		messages.innerHTML = `${messages.innerHTML}${text}`;
	};

	/**
	 * authentication_complete callback.
	 */
	window.authentication_complete = function() {
		if (window.lightdm.is_authenticated) {
			// Start default session
			// let body = document.getElementById('body');
			let session = window.lightdm.default_session
			document.documentElement.addEventListener('transitionend', () => {window.lightdm.start_session(session)});
			document.documentElement.className = 'session_starting';
		} else {
			show_message("Authentication Failed", "error");
			setTimeout(start_authentication, 2000);
		}
	};

	/**
	 * autologin_timer_expired callback.
	 */
	function autologin_timer_expired(username) {
		/* Stub.  Does nothing. */
	}

	/*******************************************************************/
	/*                Functions local to this theme                    */
	/*******************************************************************/

	/**
	 * clear_messages
	 */
	function clear_messages() {
		let messages = document.getElementById("messages");
		messages.innerHTML = "";
		messages.style.visibility = "hidden";
	}

	/**
	 * Kickoff the authentication process
	 */
	window.start_authentication = function() {
		clear_messages();
		// start without providing "user" to make the greeter prompt for "user"
		window.lightdm.authenticate(null);
	};

	/**
	 * handle the input from the entry field.
	 */
	window.handle_input = function() {
		let entry = document.getElementById("entry");
		window.lightdm.respond(entry.value);
	};

	window.addEventListener("GreeterReady", () => {
		window.lightdm.show_message.connect(show_message)
		window.lightdm.show_prompt.connect(show_prompt)
		window.lightdm.authentication_complete.connect(authentication_complete)
		window.lightdm.autologin_timer_expired.connect(autologin_timer_expired)

		start_authentication()
	});

</script>
<!--<script src="../_vendor/js/mock.js"></script>-->
</body>
</html>
