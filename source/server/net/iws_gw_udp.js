const Dgram = require("dgram");

const Log = require("../utils/iw_log.js");
const IWSGw = require("./iws_gw.js");
const IWSRequest = require("./iws_request.js");
const IWSDeviceUdp = require("./iws_device_udp.js");

class IWSUdpGW extends IWSGw {

	constructor(port) {
		super(port)
		this.started = false;
		this.server = null;
		this.T = "IWSUdpGW";
	}
  	
/** Override functions */
  	response(responseObject) {
  		var device = responseObject.device;
  		var msg = responseObject.msg;
  		this.server.send(msg, device.port, device.ip);
  	}

	onRequest(requestObject) {
		var routeInfo = requestObject.head;
		var msg = requestObject.body;
		
		Log.D(this.T, routeInfo.address + ":" + routeInfo.port + " - " + msg);
  		
  		if(this.hub != null) {
  			this.hub.postProcess(requestObject);	
  		}
		else {
			Log.W(this.T, "Nothing will be done because hub is not given.");
		}
	}
	
  	getDeviceClass() {
  		return IWSDeviceUdp;
  	}
  	
  	serverStart() {
  		const thiz = this;
  		this.server = Dgram.createSocket("udp4"); 
  		this.server.on("listening", () => {
  			thiz._onServerStarted();
  		});
		this.server.on("error", () => {
			thiz._onServerError();
		});
		this.server.on("message", (msg, rinfo) => {
			thiz._onUdpMessage(msg, rinfo);
		});
		this.server.bind(this.port);
  	}
  	
/* Private functions */
  	_onServerStarted() {
  		var address = this.server.address();
  		Log.OK(this.T, "server listening " + address.address + ":" + address.port);
  	}
  	
  	_onServerError(err) {
  		Log.FAILED(this.T, "Server started failed with error");
  		Log.E(this.T, err.stack);
		this.server.close();
  	}
  	
  	_onUdpMessage(__msg, __routeInfo) {
  		var requestObject = new IWSRequest();
  		requestObject.head = __routeInfo;
  		requestObject.body = __msg;
  		requestObject.markFrom = __routeInfo.address + ":" + __routeInfo.port;
  		this.onRequest(requestObject);
  	}
}

module.exports = IWSUdpGW;
