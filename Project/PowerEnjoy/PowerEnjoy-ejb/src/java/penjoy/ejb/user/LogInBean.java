/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.user;

import javax.ejb.Stateless;
import javax.ejb.LocalBean;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.NoResultException;
import javax.persistence.NonUniqueResultException;
import javax.persistence.Persistence;
import javax.persistence.Query;
import java.util.List;

/**
 *
 * @author Domenico
 */
@Stateless
@LocalBean
public class LogInBean {

    private EntityManagerFactory m_userFactory;

    public boolean checkLogin(String username, String password) {
        m_userFactory = Persistence.createEntityManagerFactory("User");
        EntityManager em = m_userFactory.createEntityManager();
        Query q = em.createNamedQuery("User.findByUsernameAndPassword");
        q.setParameter("username", username);
        q.setParameter("password", password);
        return !q.getResultList().isEmpty();
    }

    public boolean LogIn(String username) {
        m_userFactory = Persistence.createEntityManagerFactory("User");
        EntityManager em = m_userFactory.createEntityManager();
        Query q = em.createNamedQuery("User.findByUsername", User.class);
        q.setParameter("username", username);
        try {
            //User logInUser = (User) q.getSingleResult();
            //if (!logInUser.getLoggedin()) 
            {
                //Log In User
                /*em.getTransaction().begin();
                logInUser.setLoggedin(true);
                em.getTransaction().commit();*/
                em.close();
                return true;
            }
            //return false;
        } catch (NoResultException ex) {
            return false;
        } catch (NonUniqueResultException ex) {
            return false;
        }
    }
}
