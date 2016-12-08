/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.servlets;

import java.io.IOException;
import javax.ejb.EJB;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import penjoy.ejb.user.LogInBean;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "logoutServlet", urlPatterns = {"/logoutServlet"})
public class LogoutServlet extends BaseServlet {

    @EJB
    private LogInBean m_loginBean;

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processLogout(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        //Get User To Log Out
        String username = request.getSession().getAttribute("username").toString();
        
        Long id = m_loginBean.getUserId(username);
        System.out.println(username + " "+ id);
        //LogOut User
        if (m_loginBean.LogOut(id)) {
            HttpSession session = request.getSession(false);
            session.invalidate();
            request.logout();
            //Redirect to Login Page
            getServletContext().getRequestDispatcher("/Login/userLoginPage.jsp").forward(request, response);
        } else {
            getServletContext().getRequestDispatcher("/errorPage.html").forward(request, response);
        }
    }

// <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">
    /**
     * Handles the HTTP <code>POST</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        processLogout(request, response);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "LogIn Servlet";
    }// </editor-fold>

}
