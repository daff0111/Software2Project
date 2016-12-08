/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.car;

import java.io.Serializable;
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
import javax.validation.constraints.Size;
import javax.xml.bind.annotation.XmlRootElement;

/**
 *
 * @author Domenico
 */
@Entity
@Table(name = "Car")
@XmlRootElement
@NamedQueries({
    @NamedQuery(name = "Car.findAll", query = "SELECT c FROM Car c")
    , @NamedQuery(name = "Car.findById", query = "SELECT c FROM Car c WHERE c.id = :id")
    , @NamedQuery(name = "Car.findByStatus", query = "SELECT c FROM Car c WHERE c.status = :carStatus")
    , @NamedQuery(name = "Car.findByLocation", query = "SELECT c FROM Car c WHERE c.location = :location")
    , @NamedQuery(name = "Car.findByBatteryStatus", query = "SELECT c FROM Car c WHERE c.batteryStatus = :batteryStatus")
    , @NamedQuery(name = "Car.findBatteryStatusMoreThan", query = "SELECT c FROM Car c WHERE c.batteryStatus >= :batteryStatus")
    , @NamedQuery(name = "Car.findBatteryStatusLessThan", query = "SELECT c FROM Car c WHERE c.batteryStatus <= :batteryStatus")
    , @NamedQuery(name = "Car.findByPluggedIn", query = "SELECT c FROM Car c WHERE c.pluggedIn = :pluggedIn")
    , @NamedQuery(name = "Car.findByNumberOfPassengers", query = "SELECT c FROM Car c WHERE c.numPassengers = :numPassengers")
    , @NamedQuery(name = "Car.findByNumberOfSeats", query = "SELECT c FROM Car c WHERE c.numSeats = :numSeats")
    , @NamedQuery(name = "Car.findByCarType", query = "SELECT c FROM Car c WHERE c.carType = :carType")})
public class Car implements Serializable {

    public enum CarStatus {
        Available,
        Unavailable,
        Reserved,
        Ready,
        InUse
    }

    public enum CarType {
        FiatUno,
        FiatPanda
    }

    private static final long serialVersionUID = 1L;
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Id
    @Basic(optional = false)
    @Column(name = "ID", nullable = false)
    private Long id;

    @Basic(optional = false)
    @NotNull
    @Column(name = "STATUS")
    private CarStatus status;

    @Basic(optional = false)
    @NotNull
    @Size(min = 1, max = 32)
    @Column(name = "LOCATION")
    private String location;

    @Basic(optional = false)
    @NotNull
    @Column(name = "BATTERYSTATUS")
    private int batteryStatus;

    @Basic(optional = false)
    @NotNull
    @Column(name = "PLUGGEDIN")
    private boolean pluggedIn;

    @Basic(optional = false)
    @NotNull
    @Column(name = "NUMPASSENGERS")
    private int numPassengers;

    @Basic(optional = false)
    @NotNull
    @Column(name = "NUMSEATS")
    private int numSeats;

    @Basic(optional = false)
    @NotNull
    @Column(name = "CARTYPE")
    private CarType carType;

    public Car() {
    }

    public Car(Long id) {
        this.id = id;
    }

// <editor-fold defaultstate="collapsed" desc="Get/Set methods. Click on the + sign on the left to edit the code.">
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public CarStatus getCarStatus() {
        return status;
    }

    public void setCarStatus(CarStatus status) {
        this.status = status;
    }

    public int getBatteryStatus() {
        return batteryStatus;
    }

    public void setBatteryStatus(int batteryStatus) {
        this.batteryStatus = batteryStatus;
    }

    public String getLocation() {
        return location;
    }

    public float getLatitude() {
        return Float.parseFloat(location.substring(0, location.indexOf(",")));
    }

    public float getLongitude() {
        return Float.parseFloat(location.substring(location.indexOf(",")+1));
    }

    public void setLocation(float lat, float lon) {
        this.location = lat + "," + lon;
    }

    public boolean getIsPluggedIn() {
        return pluggedIn;
    }

    public void setIsPluggedIn(boolean pluggedIn) {
        this.pluggedIn = pluggedIn;
    }

    public int getNumberOfPassengers() {
        return numPassengers;
    }

    public void setNumberOfPassengers(int numPassengers) {
        this.numPassengers = numPassengers;
    }

    public int getNumberOfSeats() {
        return numSeats;
    }

    public void setNumberOfSeats(int numSeats) {
        this.numSeats = numSeats;
    }

    public CarType getCarType() {
        return carType;
    }

    public void setCarType(CarType carType) {
        this.carType = carType;
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
        if (!(object instanceof Car)) {
            return false;
        }
        Car other = (Car) object;
        if ((this.id == null && other.id != null) || (this.id != null && !this.id.equals(other.id))) {
            return false;
        }
        return true;
    }

    @Override
    public String toString() {
        return "penjoy.ejb.car.Car[ id=" + id + " ]";
    }

    public void showFields() {
        // Print all the User Fields
        System.out.println("Car ID:" + this.getId());
        System.out.println("Car Status:" + this.getCarStatus());
        System.out.println("Car Location:" + this.getLocation());
        System.out.println("Car Battery Status:" + this.getBatteryStatus());
        System.out.println("Car Number of Passengers:" + this.getNumberOfPassengers());
        System.out.println("Car Is Plugged In:" + this.getIsPluggedIn());
        System.out.println("Car Number of Seats:" + this.getNumberOfSeats());
        System.out.println("Car Type:" + this.getCarType());
    }
}
