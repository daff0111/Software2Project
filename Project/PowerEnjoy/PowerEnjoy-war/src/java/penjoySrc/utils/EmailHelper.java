/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package penjoySrc.utils;

import java.util.Date;
import java.util.Properties;
 
import javax.mail.Authenticator;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.SendFailedException;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

/**
 *
 * A utility class for sending e-mail messages
 * @author Domenico
 */
public class EmailHelper {
        public static void sendRegistrationEmail(String host, String port,
            final String userName, final String password, String newUserName, 
            String toAddress, String newUserPassword) throws AddressException,
            MessagingException {
 
        // sets SMTP server properties
        Properties properties = new Properties();
        properties.put("mail.smtp.host", host);
        properties.put("mail.smtp.port", port);
        properties.put("mail.smtp.auth", "true");
        properties.put("mail.smtp.starttls.enable", "true");
 
        // creates a new session with an authenticator
        Authenticator auth = new Authenticator() {
            public PasswordAuthentication getPasswordAuthentication() {
                return new PasswordAuthentication(userName, password);
            }
        };
 
        Session session = Session.getInstance(properties, auth);
 
        // creates a new e-mail message
        Message msg = new MimeMessage(session);
 
        msg.setFrom(new InternetAddress(userName));
        InternetAddress[] toAddresses = { new InternetAddress(toAddress) };
        msg.setRecipients(Message.RecipientType.TO, toAddresses);
        msg.setSubject("[PowerEnjoy] Thank you for Registering to PowerEnjoy");
        msg.setSentDate(new Date());
        msg.setText("Your username is "+newUserName+"\nYour password is "+newUserPassword);
 
        // sends the e-mail
        try {
		Transport.send(msg);
	} catch (SendFailedException ex) {
                System.err.println("Password Sent Failed: Email Not Valid");
		ex.printStackTrace();
	}
    }
}