/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.servlets;

import javax.ejb.EJB;

import java.io.IOException;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import penjoy.ejb.user.RegistrationBean;
import penjoySrc.utils.EmailHelper;
import penjoySrc.utils.StringValidator;
import penjoy.utils.PasswordHelper;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "registrationServlet", urlPatterns = {"/registrationServlet"})
public class RegistrationServlet extends BaseServlet {

    private String m_host;
    private String m_port;
    private String m_user;
    private String m_pass;
    @EJB
    private RegistrationBean m_registrationBean;

    public void init() {
        // reads SMTP server setting from web.xml file
        ServletContext context = getServletContext();
        m_host = context.getInitParameter("host");
        m_port = context.getInitParameter("port");
        m_user = context.getInitParameter("emailUser");
        m_pass = context.getInitParameter("emailPass");
    }

    /**
     * Processes requests for both HTTP <code>GET</code> and <code>POST</code>
     * methods.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    protected void processRegistration(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String username = request.getParameter("username");
        String email = request.getParameter("email");
        String repeatEmail = request.getParameter("repeatEmail");
        String licenseNumber = request.getParameter("licenseNumber");
        String paymentInfo = request.getParameter("paymentInfo");

        request.setAttribute("username", username);
        request.setAttribute("email", email);
        request.setAttribute("repeatEmail", repeatEmail);
        request.setAttribute("licenseNumber", licenseNumber);
        request.setAttribute("paymentInfo", paymentInfo);

        //Validate Fields (Server side)
        if (validateFields(request, username, email, licenseNumber, paymentInfo)) {

            String password = PasswordHelper.generatePassword();
            //Registrate User
            if (m_registrationBean.registerUser(username, password, email, licenseNumber, paymentInfo) != null) {
                //Send Password
                try {
                    EmailHelper.sendRegistrationEmail(m_host, m_port, m_user, m_pass, username, email, password);
                } catch (Exception ex) {
                    ex.printStackTrace();
                } finally {
                    System.err.println("User ["+username+"] Registered");
                    getServletContext().getRequestDispatcher("/Login/registrationSuccess.jsp").forward(request, response);
                }
            } else {
                //Error User Not Registered
                System.err.println("Could not Register User");
                getServletContext().getRequestDispatcher("/errorPage.html").forward(request, response);
            }
        } else {
            //Error Fields not Valid
            getServletContext().getRequestDispatcher("/Login/registerPage.jsp").forward(request, response);
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
        request.getSession(true);
        processRegistration(request, response);
    }

    /**
     * Returns a short description of the servlet.
     *
     * @return a String containing servlet description
     */
    @Override
    public String getServletInfo() {
        return "Servlet for User Registration";
    }// </editor-fold>

    private boolean validateFields(HttpServletRequest request, String username, String email, String licenseNumber, String paymentInfo) {
        StringValidator validator = new StringValidator();
        if (!validator.validateUsername(username)) {
            request.setAttribute("usernameVal", "Username must be 4-32 characters long and use [a-z_0-9]");
            return false;
        }
        if (!m_registrationBean.isUserNameFree(username)) {
            request.setAttribute("usernameVal", "Username already in use");
            return false;
        }
        if (!validator.validateEmail(email)) {
            request.setAttribute("emailVal", "Email not Valid. Use the format name@example.com");
            return false;
        }
        if (!validator.validateLicense(licenseNumber)) {
            request.setAttribute("licenseVal", "License format not Valid. Must be RM1234567Y");
            return false;
        }
        if (!validator.validatePayment(paymentInfo)) {
            request.setAttribute("paymentVal", "Credit Card number is not Valid. At least 15 numbers");
            return false;
        }
        return true;
    }
}
