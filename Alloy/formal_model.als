// Signatures

// raw types 
sig Data{}
sig Date{}
sig PayInfo{}

// will work as an interface for the various modules, making the bridge between the funcionalities and the user
sig System{
	listOfCars: set Car, // all existing cars and reservations 
	listOfReservs: set Reservation 
}

// User will be considered as a first time user, so it has no attibutes 
sig User{}

// we will treat account as being a registered User
sig Account{
	credentials: one Data,
	bankData: one PayInfo,
	password: one Int
}

// detais of the reservation 
sig Reservation{
	user: one Account, 
	car: one Car,
	time: one Date, // so we can know when to charge the "not pick up the car" fee
	nearby: one Int, // when this is 1, the instance of this reservation is deleted and we start the usage of the car 
}

// after the user sinalises he's nearby, the car is unlocked and as soon as the engine ignites, it starts charging
// after the car is dropped off or an hour has passed and the user did not show up the invoice will be made
sig FareCalc{
	timeofPickUp: one Date,
	timeofDropOff: one Date, // change for 1 or 0 
	NotPickUp: one Int,  // review if this attribute needs to be here later
	PassengerDisc: one Int,
	batteryDisc: one Int,
	specialParkingDisc: one Int,
	HarshConditionsFee: one Int // more then 3km or less the 20% battery
}

sig Car{
	isAvailable: one Int,
	loc: one Location,
	engineOn: one Int,
	numOfPassengers: one Int,
	battery: one Int, 

}

sig Location{}

sig SafePlace extends Location{
//	addresses: set //to be done 
}

sig SpecialParking extends SafePlace{
//	addresses: set , //to be done
	availablePlugs: one Int
}
