/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoy.ejb.user;

import penjoy.ejb.AbstractFacade;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
//import javax.persistence.PersistenceContext;

/**
 *
 * @author Domenico
 */
@Stateless
public class UserFacade extends AbstractFacade<User> {

    //@PersistenceContext(unitName = "User")
    private EntityManager em;

    @Override
    protected EntityManager getEntityManager() {
        return em;
    }

    public UserFacade() {
        super(User.class);
    }
    
}
