const Dgram = require("dgram");
const Log = require("../utils/iw_log.js");

class IWSUdpGW {

	constructor() {
		this.eventHub = null;
		this.port = 0;	
		this.started = false;
		this.server = null;
		this.T = "IWSUdpGW";
	}
  	
  	bindEventHub(eventHub) {
  		this.eventHub = eventHub;
  	}
  	
  	onServerStarted() {
  		var address = server.address();
  		Log.OK(this.T, "server listening " + address.address + ":" + address.port);
  	}
  	
  	onServerError(err) {
  		Log.FAILED(this.T, "Server started failed with error");
  		Log.E(this.T, err.stack);
		this.server.close();
  	}
  	
  	onUdpMessage(msg, routeInfo) {
  		Log.D(this.T, routeInfo.address + ":" + routeInfo.port + " - " + msg);
  		
  		if(this.eventHub != null) {
  			this.eventHub.onEvent(msg, routeInfo);	
  		}
		else {
			Log.W(this.T, "Nothing will be done. bindEventHub should be called to set an eventHub.");
		}
  	}
  	
  	sendMessage(msg, addr, port) {
  		this.server.send(msg, port, addr);
  	}
  	
  	serverStart(port) {
  		this.port = port;
  		this.server = dgram.createSocket("udp4"); 
  		this.server.on("listening", onServerStarted);
		this.server.on("error", onServerError);
		this.server.on("message", onUdpMessage);
		this.server.bind(this.port);
  	}
}

module.export = IWSUdpReceiver;
