const IWSConsumer = require("../net/iws_consumer.js");
const Log = require("../utils/iw_log.js");

class IWSRobotRouter extends IWSConsumer {

	constructor() {
		super();
		this.id = "";
		this.T = "IWSRobotRouter";
	}

	consume(session) {
		var requestObject = session.getCurrentRequestObject();
		Log.D(this.T, requestObject);
		return false;
	}
}

module.exports = IWSRobotRouter;
