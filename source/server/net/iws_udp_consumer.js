class IWSUdpConsumer {

	constructor() {
		
	}

  	register(ip) {
	  	var uuid = uuidv1();
	  	this.ipUuidMap[ip] = uuid;
	  	this.uuidIpMap[uuid] = ip;
	}

	unregister(ip) {
		var uuid = this.ipUuidMap[ip];
		this.ipUuidMap[ip] = null;
		this.uuidIpMap[uuid] = null;
	}
  	
  	isInHub(ip) {
  		return this.ipUuidMap[ip] != null;
  	}
}
