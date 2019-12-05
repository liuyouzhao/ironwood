var dgram = require("dgram");

var client = dgram.createSocket("udp4");
 

setInterval(() => {
	var message = new Buffer("hello node js");
	client.send(message, 0, message.length, 8089, "127.0.0.1", function(err, bytes){
		console.log(err);
		//client.close();
	});
}, 100);

