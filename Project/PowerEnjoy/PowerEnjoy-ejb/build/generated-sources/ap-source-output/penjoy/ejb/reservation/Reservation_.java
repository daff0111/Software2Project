package penjoy.ejb.reservation;

import javax.annotation.Generated;
import javax.persistence.metamodel.SingularAttribute;
import javax.persistence.metamodel.StaticMetamodel;
import penjoy.ejb.reservation.Reservation.ReservationStatus;

@Generated(value="EclipseLink-2.5.2.v20140319-rNA", date="2017-01-12T20:24:54")
@StaticMetamodel(Reservation.class)
public class Reservation_ { 

    public static volatile SingularAttribute<Reservation, String> reservationTime;
    public static volatile SingularAttribute<Reservation, String> pickUpTime;
    public static volatile SingularAttribute<Reservation, Long> id;
    public static volatile SingularAttribute<Reservation, Long> userID;
    public static volatile SingularAttribute<Reservation, ReservationStatus> status;
    public static volatile SingularAttribute<Reservation, Long> carID;

}