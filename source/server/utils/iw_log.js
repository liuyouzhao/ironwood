require('colors');

class IWLog {
	constructor() {
	}

	E(tag, text) {
		console.log(new Date().toISOString(), '[E]'.red, tag.red, text.red);
	}
	
	W(tag, text) {
		console.log(new Date().toISOString(), '[W]'.yellow, tag.yellow, text.yellow);
	}
	
	D(tag, text) {
		console.log(new Date().toISOString(), '[D]'.gray, tag.gray, text.gray);
	}

	I(tag, text) {
		console.log(new Date().toISOString(), '[I]'.white, tag.white, text.white);
	}
	
	OK(tag, text) {
		console.log(new Date().toISOString(), '[OK]'.green, tag.green, text.green);
	}
	
	FAILED(tag, text) {
		console.log(new Date().toISOString(), '[FAILED]'.magenta, tag.magenta, text.magenta);	
	}
}

module.exports = new IWLog();
