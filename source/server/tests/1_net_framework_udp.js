const IWSGwUdp = require("../net/iws_gw_udp.js");
const IWSHub = require("../net/iws_hub.js");
const IWSRobotRouter = require("../core/iws_robot_router.js");

function main(argc, argv) {
	
	var gw = new IWSGwUdp(8089);
	var hub = new IWSHub(gw);
	
	gw.bindHub(hub);
	
	var rr = new IWSRobotRouter();
	
	hub.addConsumer(rr);
	
	gw.serverStart();
}

module.exports = main;
