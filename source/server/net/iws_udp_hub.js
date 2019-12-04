const IWSUdpSecClient = require("iws_udp_sec_client.js");

class IWSUdpHub {

	constructor() {
		this.uuidv1 = require("uuid/v1");
		this.ippUuidMap = {};
		this.uuidIppMap = {};
	}

  	register(ip, port) {
	  	var uuid = uuidv1();
	  	var ipp = ip + ":" + port;
	  	this.ippUuidMap[ipp] = uuid;
	  	this.uuidIpMap[uuid] = ipp;
	}

	unregister(ip, port) {
		var uuid = this.ipUuidMap[ip];
		var ipp = ip + ":" + port;
		this.ipUuidMap[ipp] = null;
		this.uuidIpMap[uuid] = null;
	}
  	
  	isInHub(ip, port) {
  		var ipp = ip + ":" + port;
  		return this.ipUuidMap[ipp] != null;
  	}
}
