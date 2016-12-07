/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.utils;

import javax.ejb.Singleton;
import javax.ejb.LocalBean;
import javax.servlet.annotation.WebListener;
import javax.servlet.http.HttpSessionEvent;
import javax.servlet.http.HttpSessionListener;

/**
 *
 * @author Domenico
 * Class to Manage Current Active Sessions
 */
@Singleton
@LocalBean
@WebListener
public class WebSessionManager implements HttpSessionListener{
    private static int m_sessionCount = 0;
    
    @Override
    public void sessionCreated(HttpSessionEvent se) {
        m_sessionCount++;
    }

    @Override
    public void sessionDestroyed(HttpSessionEvent se) {
        m_sessionCount--;
    }
}
