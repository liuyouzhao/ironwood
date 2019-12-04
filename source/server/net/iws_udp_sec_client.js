class IWSUdpSecClient {

	constructor(uuid) {
		this.lastReqTime = null;
		this.uuid = null;
		this.truth = null;
		
		/* states */
		initiative = 0; /// uuid hasn't verified
		notbound = 1;   /// uuid doesn't exist
		
		
		this.stateMathine = [
			
		];
	}
	
	trigger() {
		
	}
}

module.export = IWSUdpSecClient;
