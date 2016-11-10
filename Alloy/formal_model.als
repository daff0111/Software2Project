// Signatures

sig Date{}
sig PayInfo{}

sig Location{
	latitude: Int,
	Longitude: Int
}

// system will work as an interface for the various modules, 
//making the bridge between the funcionalities and the user
sig System{
	listOfRentedCars: set Car, // all reserved cars  
	listOfAvailableCars: set Car, // cars that are available for pick up 
	listOfReservs: set Reservation, // all active reservations 
	listOfAccounts: set Account, // accounts createad 
	listofInvoice: set Ride, // Archive of old rides and reservations 
	listOfReports: set UserReport //CRM operations 
}

// User will be considered as a first time user, so it has no attibutes 
sig User{}

// we will treat account as being a registered User as there is no difference for the System
sig Account{
	id: one Int,
	credentials: one User, //altought there is an ID, no user can have the same credentials 
	bankData: one PayInfo,
	password: one Int
}

// detais of the reservation 
sig Reservation{
	id: one Int,
	madeBy: one Account,
	selectedCar: one Car, 
	time: one Date, // so we can know when to charge the "not pick up the car" fee
}

sig Car{
	id: one Int,
	loc: one Location,
	engineOn: one Int, // we gonna keep?
	numOfPassengers: one Int,
	battery: one Int, 
}{
//	0 <= isAvailable <= 1
	numOfPassengers < 5
	battery <= 100
	battery >= 0	
}

// after the user sinalises he's nearby, the car is unlocked and as soon as the engine ignites, it starts charging
// after the car is dropped off or an hour has passed and the user did not show up the invoice will be made
sig Ride{
	reservInQuestion : one Reservation,
	timeofPickUp: lone Date,
	timeofDropOff: lone Date,  
	NotPickUp: one Int, // if will be charged fee for not picking up the car
	PassengerDisc: one Int,
	batteryDisc: one Int,
	ReChargingStationDisc: one Int,
	HarshConditionsFee: one Int // more then 3km or less the 20% battery
}


sig SafeParkingArea{
	zone: set Location 
}
 
sig ReChargingStation extends SafeParkingArea{
	spots: set Location -> one Int, //this Int is the number of available plugs
}

sig Crm{
	name: String,
	email: String
}

sig UserReport{
	crmName: Crm,
	car: Car,
	userName: Account,
	reportFee: Int
}


//FACTS

//every car, reservation and account is managed by the system
fact systemManagesAll{
	all c:Car | one s: System | c in (s.listOfRentedCars + s.listOfAvailableCars)
	all r:Reservation | one s: System | r in s.listOfReservs
	all a:Account | one s:System | a in s.listOfAccounts
}

fact noCarIntwoLists{
	all s:System | 
	s.listOfRentedCars & s.listOfAvailableCars = none 
}

fact noFareIn2List{
	all s:System | 
	s.listOfReservs & s.listofInvoice.reservInQuestion = none 	
}

fact OneFareOneRide{
	no disj f1,f2:Ride, r:Reservation |  f1.reservInQuestion = r and  f2.reservInQuestion = r
}

fact noDuplicateId{
	no disj a1, a2: Account | a1.id = a2.id
	no disj r1, r2: Reservation | r1.id = r2.id
	no disj c1, c2: Car | c1.id = c2.id
}

// **************************
// CREATION OF ACCOUNT *
// **************************

// the same user cannot create two accounts 
fact noUserHas2Accounts{
	no disj a1, a2:Account, u: User | a1 != a2 and a1.credentials = u and a2.credentials = u
}

// returns a list of accounts that has "a" in it. will be used to assure no duplicates
fun lookup [a: Account, s: System] : set Account {
	a & s.listOfAccounts // & = intersection 
}

// the list cannot have more then one item
fact noDuplicateUser{
	all a: Account, s:System | 
	#lookup[a, s] < 2
}

// Creating a new account
pred addAccountToSystem(a:Account, s, s':System){
	(s'.listOfAccounts = s.listOfAccounts + a)
}
run addAccountToSystem 

// if two accounts have the same data, they are equal  
assert addEqualAccount{
	all a, a':Account, s, s':System |
		addAccountToSystem[a,s,s'] and lookup[a, s'] = lookup[a', s'] 
		implies 
		a = a'  		
}
check addEqualAccount for 4 but 1 System


// **************************
// MAKING A CAR RESERVATION *
// **************************


fact carOnlyInOneReserv{
	all c:Car | no disj r1, r2: Reservation |
		r1.selectedCar = c and r2.selectedCar = c
}

fact UserOnlyInOneReserv{
	all a:Account | no disj r1, r2: Reservation |
		r1.madeBy = a and r2.madeBy = a
	}

// just a fail proof for our system, 
fact noReservedCars{
	all c:Car, s:System |
	c in s.listOfReservs.selectedCar
	implies
	c in s.listOfRentedCars
}

fact noReservedCars2{
	all c:Car, s:System |
	c not in s.listOfReservs.selectedCar
	implies
	c in s.listOfAvailableCars
}

// the system will display a map and suggest one car for the user to pick up,
// but the calculation of the best car 
// WILL NOT be made in our software. It's an external package 

pred createReservation[a: Account, c:Car, r,r': Reservation]{
	r'.id = r.id + 1
	r'.madeBy = a 
	r'.selectedCar = c 
	r'.time = Date
}
run createReservation

// the user will select his car and the data of the will be passed 
pred makeReservation[a: Account, c:Car, s,s',s'': System, r, r':Reservation]{
	// create a reservation
	createReservation[a, c, r,r'] 
	// add reservation the the list	
	s'.listOfReservs = s.listOfReservs + r' 
	// change car list 
	s'.listOfAvailableCars = s.listOfAvailableCars - c
	s''.listOfRentedCars = s'.listOfRentedCars + c
}
run makeReservation for 5 but 3 Reservation, 1 System 

assert sameCar2Reserv{
	no disj a1,a2:Account, s1,s2,s3:System, c:Car, r1,r2, r3:Reservation |
		makeReservation[a1, c, s1,s2,s3, r1,r2] and makeReservation[a2, c, s1,s2,s3, r2,r3]
}
check sameCar2Reserv

assert sameUser2Reserv{
	no disj c1,c2:Car, a:Account, s1,s2,s3:System, r1,r2, r3:Reservation |
		makeReservation[a, c1, s1,s2,s3, r1,r2] and makeReservation[a, c2, s1,s2,s3, r2,r3]
}
check sameUser2Reserv




