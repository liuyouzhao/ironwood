const tests = [
	"none",
	"1_net_framework_udp.js"
];

function main(argc, argv) {
	var num = argv[2];
	if(num == 'a') {
		for(var i = 1; i < tests.length; i ++) {
			require("./tests/" + tests[i])(argc, argv);
		}
	}
	else {
		require(tests[parseInt(num)])(argc, argv);
	}
}

main(process.argv.length, process.argv);
