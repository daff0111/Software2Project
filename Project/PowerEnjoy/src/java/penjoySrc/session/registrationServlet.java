/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.session;

import java.io.IOException;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;

import javax.ejb.EJB;

import penjoySrc.session.utils.EmailUtility;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "registrationServlet", urlPatterns = {"/registrationServlet"})
public class registrationServlet extends HttpServlet {

    private String m_host;
    private String m_port;
    private String m_user;
    private String m_pass;
    @EJB
    private registrationBean m_registrationBean;

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
        request.setAttribute("licenseNumber", licenseNumber);
        request.setAttribute("paymentInfo", paymentInfo);

        if (true) { //Check fields JSP
            String password = m_registrationBean.generatePassword();
            //Registrate User
            //if (m_registrationBean.registerUser(username, password, email, licenseNumber, paymentInfo) != null) {
                //Send Password
                try {
                    EmailUtility.sendRegistrationEmail(m_host, m_port, m_user, m_pass, username, email, password);
                } catch (Exception ex) {
                    ex.printStackTrace();
                } finally {
                    getServletContext().getRequestDispatcher("/jsp/registrationResult.jsp").forward(request, response);
                }
            //}
        }
    }

// <editor-fold defaultstate="collapsed" desc="HttpServlet methods. Click on the + sign on the left to edit the code.">

    /**
     * Handles the HTTP <code>GET</code> method.
     *
     * @param request servlet request
     * @param response servlet response
     * @throws ServletException if a servlet-specific error occurs
     * @throws IOException if an I/O error occurs
     */
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        //processRequest(request, response);
    }

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

}
