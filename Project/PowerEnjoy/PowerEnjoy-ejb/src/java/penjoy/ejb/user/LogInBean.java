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
import javax.persistence.Persistence;
import javax.persistence.Query;
import javax.persistence.TypedQuery;
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
        m_userFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_userFactory.createEntityManager();
        Query q = em.createNamedQuery("User.findByUsernameAndPassword");
        q.setParameter("username", username);
        q.setParameter("password", password);
        boolean userExists = !q.getResultList().isEmpty();
        em.close();
        return userExists;
    }

    public Long getUserId(String username) {
        m_userFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_userFactory.createEntityManager();
        TypedQuery<User> q = em.createNamedQuery("User.findByUsername", User.class);
        q.setParameter("username", username);
        List<User> userList = q.getResultList();
        Long result = new Long("-1");
        if (!userList.isEmpty()) {
            result = userList.get(0).getId();
        }
        em.close();
        return result;
    }

    public boolean LogIn(Long id) {
        m_userFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_userFactory.createEntityManager();
        User loginUser = em.find(User.class, id);
        if (loginUser != null /*&& !loginUser.getLoggedin()*/) {
            //Log In User
            em.getTransaction().begin();
            loginUser.setLoggedin(true);
            em.getTransaction().commit();
            em.close();
            return true;
        }
        em.close();
        return false;
    }

    public boolean LogOut(Long id) {
        System.out.println(id);
        m_userFactory = Persistence.createEntityManagerFactory("PUnit");
        EntityManager em = m_userFactory.createEntityManager();
        User logoutUser = em.find(User.class, id);
        if (logoutUser != null /*&& logoutUser.getLoggedin()*/) {
            //Log Out User
            em.getTransaction().begin();
            logoutUser.setLoggedin(false);
            em.getTransaction().commit();
            em.close();
            return true;
        }
        em.close();
        return false;
    }
}
