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
import penjoy.utils.PasswordHelper;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "loginServlet", urlPatterns = {"/loginServlet"})
public class LoginServlet extends BaseServlet {

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
    protected void processLogin(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String username = request.getParameter("username");
        String password = request.getParameter("password");

        String hashedPassword = PasswordHelper.hashPassword(password);

        request.setAttribute("username", username);
        request.setAttribute("password", hashedPassword);

        if (m_loginBean.checkLogin(username, hashedPassword)) {
            Long Id = m_loginBean.getUserId(username);
            //Initiate Session
            if (m_loginBean.LogIn(Id)) {
                request.login("User", "penjoy_userpwd");
                createSession(Id, username, request, response); 
                System.out.println("Loggin In ["+username+"]...");
                response.sendRedirect("/PowerEnjoy-war/MainPage");
                //getServletContext().getRequestDispatcher("/PowerEnjoy-war/MainPageServlet").forward(request, response);
            } else {
                System.err.println("Could not Log In User");
                getServletContext().getRequestDispatcher("/errorPage.html").forward(request, response);
            }
        } else {
            request.setAttribute("usernameVal", "Username or Password Invalid");
            getServletContext().getRequestDispatcher("/Login/userLoginPage.jsp").forward(request, response);
        }
    }

    private void createSession(Long Id, String username, HttpServletRequest request, HttpServletResponse response) {
        //create Session
        HttpSession session = request.getSession(true);
        // store the boolean value to the session
        session.setAttribute("authentication", true);
        session.setAttribute("username", username);
        session.setAttribute("id", Id);
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
        if (!sessionCheck(request, response)) {
            processLogin(request, response);
        }
        else
        {
            response.sendRedirect("/PowerEnjoy-war/MainPage");
            //getServletContext().getRequestDispatcher("/PowerEnjoy-war/MainPageServlet").forward(request, response);
        }
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
