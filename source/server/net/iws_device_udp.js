const IWSDevice = require("./iws_device.js");

class IWSDeviceUdp extends IWSDevice {
	constructor(rinfo) {
		super();
		this.ip = rinfo.address;
		this.port = rinfo.port;
		this.key = this.ip + ":" + this.port;
	}
	
	getId() {
		return this.key;
	}
}

module.exports = IWSDeviceUdp;
