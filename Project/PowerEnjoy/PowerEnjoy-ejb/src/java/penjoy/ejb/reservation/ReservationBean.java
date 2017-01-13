/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.reservation;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import javax.ejb.LocalBean;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.TypedQuery;

/**
 *
 * @author Domenico
 */
@Stateless
@LocalBean
public class ReservationBean {

    private EntityManagerFactory m_ReservationManagerFactory;

    public List<Reservation> getReservationByCarID(Long carID) {
        m_ReservationManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_ReservationManagerFactory.createEntityManager();
        TypedQuery<Reservation> q = em.createNamedQuery("Reservation.findByCarID", Reservation.class);
        q.setParameter("carID", carID);
        return q.getResultList();
    }
    
    public List<Reservation> getReservationByUserID(Long userID) {
        m_ReservationManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_ReservationManagerFactory.createEntityManager();
        TypedQuery<Reservation> q = em.createNamedQuery("Reservation.findByUserID", Reservation.class);
        q.setParameter("userID", userID);
        return q.getResultList();
    }
    
    public Reservation getActiveReservationByUserID(Long userID) {
        m_ReservationManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_ReservationManagerFactory.createEntityManager();
        TypedQuery<Reservation> q = em.createNamedQuery("Reservation.findByUserAndStatus", Reservation.class);
        q.setParameter("reservationStatus", Reservation.ReservationStatus.Active);
        q.setParameter("userID", userID);
        if(q.getResultList().size() > 0)
        {
            return q.getResultList().get(0);
        }
        return null;
    }

    //Test Function, create a Set of Reservations in the DB
    public Reservation createReservation(Long userID, Long carID) {
        m_ReservationManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_ReservationManagerFactory.createEntityManager();
        
        //Create new Reservation Entity
        Reservation newReservation = new Reservation();
        newReservation.setCarId(carID);
        newReservation.setUserId(userID);
        newReservation.setReservationStatus(Reservation.ReservationStatus.Active);
        
        DateFormat dateFormat = new SimpleDateFormat("dd/MM/yyyy HH:mm:ss");
        Date reservationTime = new Date();
        Date pickupTime = new Date();
        pickupTime.setTime(pickupTime.getTime()+(60*60000)); //Set Max Pickup time to 1hr after Reservation
        newReservation.setReservationTime(dateFormat.format(reservationTime));
        newReservation.setPickUpTime(dateFormat.format(pickupTime));

        //Register new User in the DB
        em.getTransaction().begin();
        newReservation.showReservation();
        em.persist(newReservation);
        em.getTransaction().commit();
        
        em.close();
        
        return newReservation;
    }
}
