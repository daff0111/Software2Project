package penjoy.ejb.car;

import javax.annotation.Generated;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;
import penjoy.ejb.car.Car.CarStatus;
import penjoy.ejb.car.Car.CarType;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2017-01-12T20:24:54")
@StaticMetamodel(Car.class)
public class Car_ { 

    public static volatile SingularAttribute<Car, Integer> numSeats;
    public static volatile SingularAttribute<Car, CarType> carType;
    public static volatile SingularAttribute<Car, Integer> batteryStatus;
    public static volatile SingularAttribute<Car, Integer> numPassengers;
    public static volatile SingularAttribute<Car, String> location;
    public static volatile SingularAttribute<Car, Long> id;
    public static volatile SingularAttribute<Car, Boolean> pluggedIn;
    public static volatile SingularAttribute<Car, CarStatus> status;

}