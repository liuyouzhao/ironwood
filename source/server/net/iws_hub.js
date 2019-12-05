const IWSNetVerificator = require("./iws_net_verificator.js");
const IWSNetSession = require("./iws_net_session.js");

class IWSHub {

	constructor(gw) {
		this.genUUID = require("uuid/v1");
		this.consumers = [];
		this.sessions = {};
		this.gw = gw;
		this.DeviceType = this.gw.getDeviceClass();
	}
	
	addConsumer(consumer) {
		this.consumers.push(consumer);
	}
	
  	postProcess(requestObject) {
  		var key = requestObject.markFrom;
  		if(this.sessions[key] == null) {
  			var device = new this.DeviceType(requestObject);
  			this.sessions[key] = new IWSNetSession(device.getId(), device, this._doResponse);
  		}
  		
  		var session = this.sessions[key];
  		session.onRequest(requestObject);
  	
  		for(var i = 0; i < this.consumers.length; i ++) {
  			this.consumers[i].consume(session);
  		}
  	}
  	
/* Private functions */
	_doResponse(session) {
		var device = session.device;
		var responseObject = session.responseObject;
		this.gw.response(device, responseObject);
	}
}

module.exports = IWSHub;
