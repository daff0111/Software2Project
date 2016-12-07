/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.user;

import javax.ejb.Stateless;
import java.util.List;
import java.util.Set;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import javax.validation.ConstraintViolation;
import javax.validation.Validation;
import javax.validation.Validator;
import javax.validation.ValidatorFactory;

import penjoy.utils.PasswordHelper;

/**
 *
 * @author Domenico
 */
@Stateless
public class RegistrationBean {
    // Add business logic below. (Right-click in editor and choose
    // "Insert Code > Add Business Method")

    private EntityManagerFactory m_userFactory;

    public User registerUser(String username, String password, String email, String licenseNumber, String paymentInfo) {
        m_userFactory = Persistence.createEntityManagerFactory("User");
        EntityManager em = m_userFactory.createEntityManager();
        /*Query q = em.createQuery("SELECT newUser FROM User newUser WHERE newUser.username = :username");
        q.setParameter("username", username);
        if (q.getSingleResult() != null) {
            return null;
        } else {*/
        String hashedPassword = PasswordHelper.hashPassword(password);
        User newUser = new User();
        newUser.setUsername(username);
        newUser.setPassword(hashedPassword);
        newUser.setEmail(email);
        newUser.setLicense(licenseNumber);
        newUser.setPaymentinfo(paymentInfo);
        newUser.setLoggedin(false);
        
                    //Test write users in console
            Query q = em.createNamedQuery("User.findAll");
            List<User> userList = q.getResultList();
            for (User user : userList) {
                user.showFields();
            }
            System.out.println("Size: " + userList.size());
            System.out.println(PasswordHelper.hashPassword("admin"));
        //Database Validation Check
        if (validateUser(newUser)) {
            //Register new User in the DB
            em.getTransaction().begin();
            em.persist(newUser);
            em.getTransaction().commit();            
            em.close();
            return newUser;
        }
        return null;
    }
    
    private boolean validateUser(User newUser)
    {
        ValidatorFactory factory = Validation.buildDefaultValidatorFactory();
        Validator validator = factory.getValidator();
        Set<ConstraintViolation<User>> constraintViolations = validator.validate(newUser);
        if (constraintViolations.size() > 0) {
            System.out.println("Constraint Violations occurred..");
            for (ConstraintViolation<User> contraints : constraintViolations) {
                System.out.println(contraints.getRootBeanClass().getSimpleName()
                        + "." + contraints.getPropertyPath() + " " + contraints.getMessage());
            }
            newUser.showFields();
            return false;
        }
        
        return true;
    }
}

