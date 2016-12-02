/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.session;

import javax.ejb.Stateless;
import java.util.List;

import penjoySrc.entities.User;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import java.security.SecureRandom;
import java.math.BigInteger;

/**
 *
 * @author Domenico
 */
@Stateless
public class registrationBean {
    // Add business logic below. (Right-click in editor and choose
    // "Insert Code > Add Business Method")
    private static EntityManagerFactory m_userFactory;
    private static SecureRandom m_randomGenerator = new SecureRandom();
    
    public User registerUser(String username, String password, String email, String licenseNumber, String paymentInfo) {
        m_userFactory = Persistence.createEntityManagerFactory("User");
        EntityManager em = m_userFactory.createEntityManager();
        Query q = em.createQuery("SELECT u FROM User u WHERE u.UserName = :username");
        q.setParameter("username", username);
        if (q.getSingleResult() != null) {
            return null;
        } else {
            User u = new User();
            u.setUsername(username);
            u.setPassword(password);
            u.setEmail(email);
            u.setLicense(licenseNumber);
            u.setPaymentinfo(Integer.parseInt(paymentInfo));
            u.setLoggedin(false);

            em.getTransaction().begin();
            em.persist(u); //em.merge(u); for updates
            em.getTransaction().commit();
            em.close();
            //Test write users in console
            q = em.createQuery("SELECT u FROM User u");
            List<User> userList = q.getResultList();
            for (User user : userList) {
                System.out.println(user.getUsername());
            }
            System.out.println("Size: " + userList.size());
            
            return u;
        }
    }
    
    public String generatePassword() {
        return new BigInteger(80, m_randomGenerator).toString(32);
    }
}




