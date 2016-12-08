/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.car;

import java.util.List;
import java.util.Random;
import javax.ejb.LocalBean;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import javax.persistence.TypedQuery;

/**
 *
 * @author Domenico
 */
@Stateless
@LocalBean
public class CarSearchBean {

    private EntityManagerFactory m_carManagerFactory;

    public List<Car> getAvailableCars() {
        m_carManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_carManagerFactory.createEntityManager();
        TypedQuery<Car> q = em.createNamedQuery("Car.findByStatus", Car.class);
        q.setParameter("carStatus", Car.CarStatus.Available);
        return q.getResultList();
    }

    //Test Function, create a Set of Cars in the DB
    public void createCars() {
        m_carManagerFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_carManagerFactory.createEntityManager();

        float milanLat = 45.391f;
        float milanLon = 9.077f;
        Random rnd = new Random();
        for (int i = 0; i < 10; i++) {
            //Create new User Entity
            float lat = milanLat + (0.15f * rnd.nextFloat());
            float lon = milanLon + (0.20f * rnd.nextFloat());
            Car newCar = new Car();
            newCar.setLocation(lat, lon);
            newCar.setCarStatus(Car.CarStatus.Available);
            newCar.setBatteryStatus(100);
            newCar.setNumberOfPassengers(0);
            newCar.setNumberOfSeats(4);
            newCar.setIsPluggedIn(true);
            newCar.setCarType(Car.CarType.FiatUno);
            //Register new User in the DB
            em.getTransaction().begin();
            newCar.showFields();
            em.persist(newCar);
            em.getTransaction().commit();
        }
        em.close();
    }
}
