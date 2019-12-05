class IWSNetSession {
	constructor(sid, device, doResponse) {
		this.id = sid;
		this.device = device;
		this.doResponse = doResponse;
		this.lastReqTime = 0;
		this.requestObject = null;
		this.responseObject = null;
	}
	
	onRequest(requestObject) {
		this.lastReqTime = Date.now();
		this.requestObject = requestObject;
	}
	
	getCurrentRequestObject() {
		return this.requestObject;
	}
	
	response(responseObject) {
		this.responseObject = responseObject;
		this.doResponse(this);
	}
}

module.exports = IWSNetSession;
