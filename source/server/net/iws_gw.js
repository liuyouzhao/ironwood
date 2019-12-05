class IWSGw {
	constructor(port) {
		this.port = port;
		this.hub = null;
	}
	
	bindHub(hub) {
		this.hub = hub;
	}
	
	serverStart() {
	}
	
	response(responseObject) {
	}
	
	onRequest(requestObject) {
	}
	
	getDeviceClass() {
		return null;
	}
}

module.exports = IWSGw;
