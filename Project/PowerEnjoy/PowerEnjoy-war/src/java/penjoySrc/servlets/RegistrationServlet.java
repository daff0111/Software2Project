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

import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import penjoy.ejb.user.RegistrationBean;
import penjoy.ejb.user.UserFacade;
import penjoySrc.utils.EmailHelper;
import penjoy.utils.PasswordHelper;

import java.io.PrintWriter;

/**
 *
 * @author Domenico
 */
@WebServlet(name = "registrationServlet", urlPatterns = {"/registrationServlet"})
public class RegistrationServlet extends HttpServlet {

    private String m_host;
    private String m_port;
    private String m_user;
    private String m_pass;
    @EJB
    private RegistrationBean m_registrationBean;
    //private UserFacade m_userfacade;

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
            String password = PasswordHelper.generatePassword();
            /*int i = -1;
            if(m_userfacade.findAll() != null)
            {
                i = m_userfacade.findAll().size();
            }*/
            //Registrate User
            if (m_registrationBean.registerUser(username, password, email, licenseNumber, paymentInfo) != null) {
                //Send Password
                try {
                    EmailHelper.sendRegistrationEmail(m_host, m_port, m_user, m_pass, username, email, password);
                } catch (Exception ex) {
                    ex.printStackTrace();
                } finally {
                    getServletContext().getRequestDispatcher("/jsp/registrationResult.jsp").forward(request, response);
                }
            }
            PrintWriter out = response.getWriter();
            out.println(
                    "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" +"
                    + "http://www.w3.org/TR/html4/loose.dtd\">\n"
                    + "<html> \n"
                    + "<head> \n"
                    + "<title> Crunchify.com JSP Servlet Example  </title> \n"
                    + "</head> \n"
                    + "<body> <div align='center'> \n"
                    + "<style= \"font-size=\"12px\" color='black'\"" + "\">"
                    + "Users: " + password + " <br> "
                    + "</font></body> \n"
                    + "</html>"
            );
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
        //request.getSession(true);
        //processRegistration(request, response);
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

}
