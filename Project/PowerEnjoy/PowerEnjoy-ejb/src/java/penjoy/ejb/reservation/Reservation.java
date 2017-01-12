/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.reservation;

import java.io.Serializable;
import java.sql.Date;
import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.NamedQueries;
import javax.persistence.NamedQuery;
import javax.persistence.Table;
import javax.validation.constraints.NotNull;
import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author Domenico
 */
@Entity
@Table(name = "Reservation")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Reservation.findAll", query = "SELECT r FROM Reservation r")
    , @NamedQuery(name = "Reservation.findById", query = "SELECT r FROM Reservation r WHERE r.id = :id")
    , @NamedQuery(name = "Reservation.findByStatus", query = "SELECT r FROM Reservation r WHERE r.status = :reservationStatus")
    , @NamedQuery(name = "Reservation.findByCarID", query = "SELECT r FROM Reservation r WHERE r.carID = :carID")
    , @NamedQuery(name = "Reservation.findByUserReservationID", query = "SELECT r FROM Reservation r WHERE r.userID = :userID")
    , @NamedQuery(name = "Reservation.findByUserAndStatus", query = "SELECT r FROM Reservation r WHERE r.status = :reservationStatus AND r.userID = :userID")
    })
public class Reservation implements Serializable {

    public enum ReservationStatus {
        Active,
        Confirmed,
        Canceled,
        Finished
    }
        
    private static final long serialVersionUID = 1L;
    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Basic(optional = false)
    @Column(name = "ID", nullable = false)
    private Long id;

    @Basic(optional = false)
    @NotNull
    @Column(name = "STATUS")
    private ReservationStatus status;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "CarID")
    private Long carID;
        
    @Basic(optional = false)
    @NotNull
    @Column(name = "USERID")
    private Long userID;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "RESERVATIONTIME")
    private String reservationTime;
    
    @Basic(optional = false)
    @NotNull
    @Column(name = "PICKUPTIME")
    private String pickUpTime;
    
// <editor-fold defaultstate="collapsed" desc="Get/Set methods. Click on the + sign on the left to edit the code.">
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public ReservationStatus getReservationStatus() {
        return status;
    }

    public void setReservationStatus(ReservationStatus status) {
        this.status = status;
    }

    public Long getCarId() {
        return carID;
    }

    public void setCarId(Long id) {
        this.carID = id;
    }

    public Long getUserId() {
        return userID;
    }

    public void setUserId(Long id) {
        this.userID = id;
    }

    public String getReservationTime() {
        return reservationTime;
    }

    public void setReservationTime(String reservationTime) {
        this.reservationTime = reservationTime;
    }
    
    public String getPickUpTime() {
        return reservationTime;
    }

    public void setPickUpTime(String reservationTime) {
        this.reservationTime = reservationTime;
    }
// </editor-fold>

    @Override
    public int hashCode() {
        int hash = 0;
        hash += (id != null ? id.hashCode() : 0);
        return hash;
    }

    @Override
    public boolean equals(Object object) {
        // TODO: Warning - this method won't work in the case the id fields are not set
        if (!(object instanceof Reservation)) {
            return false;
        }
        Reservation other = (Reservation) object;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "penjoy.ejb.reservation.Reservation[ id=" + id + " ]";
    }
    
    public void showReservation() {
        // Print all the User Fields
        System.out.println("User ID:" + this.getUserId());
        System.out.println("Car ID:" + this.getCarId());
        System.out.println("Status:" + this.getReservationStatus());
        System.out.println("Reservation Time:" + this.getReservationTime());
        System.out.println("Pickup Time:" + this.getPickUpTime());
    }
    
}
