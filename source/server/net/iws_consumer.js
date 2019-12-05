class IWSConsumer {

	constructor() {
		this.id = "";
	}

	consume(session) {
		return false;
	}
}

module.exports = IWSConsumer;
